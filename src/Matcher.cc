/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Matcher.hh"
#include "Corpus.hh"
#include "LevenshteinIndex.hh"
#include "Word.hh"
#include "Document.hh"
#include "WordList.hh"
#include "IndexMatches.hh"
#include "MatchResults.hh"
#include "ErrorValues.hh"
#include "ColumbusHelpers.hh"
#include "IndexWeights.hh"
#include "MatcherStatistics.hh"
#include "WordStore.hh"
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <string>

using namespace std;

typedef map<WordID, map<WordID, set<string> > > ReverseIndex; // Index name, word, documents.

struct MatcherPrivate {
    map<WordID, LevenshteinIndex*> indexes;
    ReverseIndex reverseIndex;
    ErrorValues e;
    IndexWeights weights;
    MatcherStatistics stats;
    WordStore store;
};

typedef map<WordID, LevenshteinIndex*>::iterator IndIterator;
typedef ReverseIndex::iterator RevIndIterator;
typedef map<WordID, set<string> >::iterator RevIterator;

typedef map<WordID, int> MatchErrorMap;

typedef map<WordID, MatchErrorMap> BestIndexMatches;

typedef map<WordID, MatchErrorMap>::iterator MatchIndIterator;
typedef map<WordID, int>::iterator MatchIterator;

/*
 * These are helper functions for Matcher. They are not member functions to avoid polluting the header
 * with STL includes.
 */

static int getDynamicError(const Word &w) {
    size_t len = w.length();
    if(len <= 4)
        return LevenshteinIndex::getDefaultError();
    else
        return int(len/4.0*LevenshteinIndex::getDefaultError()); // Permit a typo for every fourth letter.
}

static void addMatches(MatcherPrivate *p, BestIndexMatches &bestIndexMatches, const Word &queryWord, const WordID indexID, IndexMatches &matches) {
    MatchIndIterator it = bestIndexMatches.find(indexID);
    map<WordID, int> *indexMatches;
    if(it == bestIndexMatches.end()) {
        map<WordID, int> tmp;
        bestIndexMatches[indexID] = tmp;
        it = bestIndexMatches.find(indexID);
    }
    indexMatches = &(it->second);
    for(size_t i=0; i < matches.size(); i++) {
        const WordID matchWordID = matches.getMatch(i);
        const int matchError = matches.getMatchError(i);
        MatchIterator mIt = indexMatches->find(matchWordID);
        if(mIt == indexMatches->end()) {
            (*indexMatches)[matchWordID] = matchError;
        } else {
            if(mIt->second > matchError)
                (*indexMatches)[matchWordID] = matchError;
        }

    }
}

/*
 * A simple relevancy calculator for matched word. Better ranking functions exist and should be examined:
 * http://en.wikipedia.org/wiki/TF_IDF
 * http://en.wikipedia.org/wiki/Okapi_BM25
 */
static double calculateRelevancy(MatcherPrivate *p, const WordID wID, const WordID indexID, int error) {
    const LevenshteinIndex * const ind = p->indexes[indexID];
    double errorMultiplier = 100.0/(100.0+error); // Should be adjusted for maxError or word length.
    size_t indexCount = ind->wordCount(wID);
    size_t indexMaxCount = ind->maxCount();
    assert(indexCount > 0);
    assert(indexMaxCount > 0);
    double frequencyMultiplier = 1.0 - double(indexCount)/(indexMaxCount+1);
    double indexWeightMultiplier = p->weights.getWeight(p->store.getWord(indexID));
    return errorMultiplier*frequencyMultiplier*indexWeightMultiplier;
}

static void findDocuments(MatcherPrivate *p, const WordID wordID, const WordID fieldID, std::vector<string> &result) {
    IndexMatches im;
    RevIndIterator it = p->reverseIndex.find(fieldID);
    if(it == p->reverseIndex.end())
        return;
    map<WordID, set<string> > &rind = it->second;
    RevIterator s = rind.find(wordID);
    if(s == rind.end())
        return;
    set<string> &docSet = s->second;
    set<string>::iterator foo;
    for(set<string>::iterator docIter = docSet.begin(); docIter != docSet.end(); docIter++) {
        result.push_back(*docIter);
    }
}

static void matchIndexes(MatcherPrivate *p, const WordList &query, const bool dynamicError, BestIndexMatches &bestIndexMatches) {
    for(size_t i=0; i<query.size(); i++) {
        const Word &w = query[i];
        int maxError;
        if(dynamicError)
            maxError = getDynamicError(w);
        else
            maxError = 2*LevenshteinIndex::getDefaultError();
        for(IndIterator it = p->indexes.begin(); it != p->indexes.end(); it++) {
            IndexMatches m;
            it->second->findWords(w, p->e, maxError, m);
            addMatches(p, bestIndexMatches, w, it->first, m);
            debugMessage("Matched word %s in index %s with error %d and got %ld matches.\n",
                    w.asUtf8(), p->store.getWord(it->first).asUtf8(), maxError, m.size());
        }
    }
}

static void gatherMatchedDocuments(MatcherPrivate *p,  map<WordID, MatchErrorMap> &bestIndexMatches, map<string, double> &matchedDocuments) {
    for(MatchIndIterator it = bestIndexMatches.begin(); it != bestIndexMatches.end(); it++) {
        for(MatchIterator mIt = it->second.begin(); mIt != it->second.end(); mIt++) {
            vector<string> tmp;
            findDocuments(p, mIt->first, it->first, tmp);
            debugMessage("Exact searched \"%s\" in field \"%s\", which was found in %ld documents.\n",
                    p->store.getWord(mIt->first).asUtf8(),
                    p->store.getWord(it->first).asUtf8(), tmp.size());
            for(size_t i=0; i<tmp.size(); i++) {
                string curDoc = tmp[i];
                // At this point we know the matched word, and which index and field
                // it matched in. Now we can just increment the relevancy of said document.
                double relevancy = calculateRelevancy(p, mIt->first, it->first, mIt->second);
                map<string, double>::iterator doc = matchedDocuments.find(curDoc);
                if(doc == matchedDocuments.end())
                    matchedDocuments[curDoc] = relevancy;
                else
                    matchedDocuments[curDoc] += relevancy;
            }
        }
    }
}

Matcher::Matcher() {
    p = new MatcherPrivate();
}

void Matcher::index(const Corpus &c) {
    double buildStart, buildEnd;
    buildStart = hiresTimestamp();
    buildIndexes(c);
    buildEnd = hiresTimestamp();
    debugMessage("Added %ld documents to matcher. It now has %ld indexes. Index population took %.2f seconds.\n",
            c.size(), p->indexes.size(), buildEnd - buildStart);
    for(IndIterator it = p->indexes.begin(); it != p->indexes.end(); it++) {
        debugMessage("Index \"%s\" has %ld words and %ld nodes.\n",
                p->store.getWord(it->first).asUtf8(), it->second->numWords(), it->second->numNodes());
    }
}

Matcher::~Matcher() {
    for(IndIterator it = p->indexes.begin(); it != p->indexes.end(); it++) {
        delete it->second;
    }
    delete p;
}

void Matcher::buildIndexes(const Corpus &c) {
    for(size_t ci = 0; ci < c.size(); ci++) {
        const Document &d = c.getDocument(ci);
        WordList textNames;
        d.getFieldNames(textNames);
        for(size_t ti=0; ti < textNames.size(); ti++) {
            const Word &fieldName = textNames[ti];
            const WordID fieldID = p->store.getID(fieldName);
            const WordList &text = d.getText(fieldName);
            for(size_t wi=0; wi<text.size(); wi++) {
                const Word &word = text[wi];
                const WordID wordID = p->store.getID(word);
                p->stats.wordProcessed(wordID);
                addToIndex(word, wordID, fieldID);
                p->stats.addedWordToIndex(wordID, fieldName);
                addToReverseIndex(wordID, fieldID, &d);
            }
        }
    }
}

void Matcher::addToIndex(const Word &word, const WordID wordID, const WordID indexID) {
    LevenshteinIndex *target;
    IndIterator it = p->indexes.find(indexID);
    if(it == p->indexes.end()) {
        target = new LevenshteinIndex();
        p->indexes[indexID] = target;
    } else {
        target = it->second;
    }
    target->insertWord(word, wordID);
}

void Matcher::addToReverseIndex(const WordID wordID, const WordID indexID, const Document *d) {
    RevIndIterator rit = p->reverseIndex.find(indexID);
    if(rit == p->reverseIndex.end()) {
        map<WordID, set<string> > tmp;
        p->reverseIndex[indexID] = tmp;
        rit = p->reverseIndex.find(indexID);
    }
    map<WordID, set<string> > &indexRind = rit->second;
    RevIterator revIt = indexRind.find(wordID);
    if(revIt == indexRind.end()) {
        set<string> tmp;
        tmp.insert(d->getID());
        indexRind[wordID] = tmp;
    } else {
        string tmp = d->getID();
        revIt->second.insert(tmp);
    }
}

void Matcher::matchWithRelevancy(const WordList &query, const bool dynamicError, MatchResults &matchedDocuments) {
    map<string, double> docs;
    BestIndexMatches bestIndexMatches;
    double start, indexMatchEnd, gatherEnd, finish;

    start = hiresTimestamp();
    matchIndexes(p, query, dynamicError, bestIndexMatches);
    indexMatchEnd = hiresTimestamp();
    // Now we know all matched words in all indexes. Gather up the corresponding documents.
    gatherMatchedDocuments(p, bestIndexMatches, docs);
    gatherEnd = hiresTimestamp();
    for(map<string, double>::iterator it=docs.begin(); it != docs.end(); it++) {
        matchedDocuments.addResult(it->first.c_str(), it->second);
    }
    debugMessage("Found a total of %ld documents.\n", matchedDocuments.size());
    finish = hiresTimestamp();
    debugMessage("Query finished. Index lookups took %.2fs, result gathering %.2fs, result building %.2fs.\n",
            indexMatchEnd - start, gatherEnd - indexMatchEnd, finish - gatherEnd);
}

void Matcher::match(const WordList &query, MatchResults &matchedDocuments) {
    matchWithRelevancy(query, true, matchedDocuments);
}

void Matcher::match(const char *queryAsUtf8, MatchResults &matchedDocuments) {
    WordList l;
    splitToWords(queryAsUtf8, l);
    match(l, matchedDocuments);
}

ErrorValues& Matcher::getErrorValues() {
    return p->e;
}

IndexWeights& Matcher::getIndexWeights() {
    return p->weights;
}
