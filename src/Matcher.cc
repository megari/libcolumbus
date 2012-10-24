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
#include "ResultFilter.hh"
#include <cassert>
#include <stdexcept>
#include <set>
#include <map>

COL_NAMESPACE_START
using namespace std;

typedef set<DocumentID> DocumentSet;
typedef map<WordID, LevenshteinIndex*> IndexMap;
typedef map<WordID, map<WordID, DocumentSet > > ReverseIndex; // Index name, word, documents.

struct MatcherPrivate {
    IndexMap indexes;
    ReverseIndex reverseIndex;
    ErrorValues e;
    IndexWeights weights;
    MatcherStatistics stats;
    WordStore store;
};

typedef IndexMap::iterator IndIterator;
typedef ReverseIndex::iterator RevIndIterator;
typedef map<WordID, DocumentSet >::iterator RevIterator;

typedef map<WordID, int> MatchErrorMap;

typedef map<WordID, MatchErrorMap> BestIndexMatches;

typedef map<WordID, MatchErrorMap>::iterator MatchIndIterator;
typedef map<WordID, int>::iterator MatchIterator;

/*
 * These are helper functions for Matcher. They are not member functions to avoid polluting the header
 * with STL includes.
 */

static bool documentHasTerm(MatcherPrivate *p, DocumentID id, const Word &indexName, const Word &value) {
    try {
        WordID indexID = p->store.getID(indexName);
        WordID valueID = p->store.getID(value);
        RevIndIterator rind = p->reverseIndex.find(indexID);
        if(rind == p->reverseIndex.end())
            return false;
        RevIterator rev = rind->second.find(valueID);
        if(rev == rind->second.end())
            return false;
        return rev->second.find(id) != rev->second.end();
    } catch(out_of_range &e) {
        // The return command would be here, but it had to be moved
        // below outside this block to shut the compiler up about
        // this function not having a return statement.
    }
    return false;
}

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

static void findDocuments(MatcherPrivate *p, const WordID wordID, const WordID fieldID, std::vector<DocumentID> &result) {
    IndexMatches im;
    RevIndIterator it = p->reverseIndex.find(fieldID);
    if(it == p->reverseIndex.end())
        return;
    map<WordID, DocumentSet > &rind = it->second;
    RevIterator s = rind.find(wordID);
    if(s == rind.end())
        return;
    DocumentSet &docSet = s->second;
    for(auto docIter = docSet.begin(); docIter != docSet.end(); docIter++) {
        result.push_back(*docIter);
    }
}

static void matchIndexes(MatcherPrivate *p, const WordList &query, const bool dynamicError, const int extraError, BestIndexMatches &bestIndexMatches) {
    for(size_t i=0; i<query.size(); i++) {
        const Word &w = query[i];
        int maxError;
        if(dynamicError)
            maxError = getDynamicError(w);
        else
            maxError = 2*LevenshteinIndex::getDefaultError();
        maxError += extraError;

        for(IndIterator it = p->indexes.begin(); it != p->indexes.end(); it++) {
            IndexMatches m;
            it->second->findWords(w, p->e, maxError, m);
            addMatches(p, bestIndexMatches, w, it->first, m);
            debugMessage("Matched word %s in index %s with error %d and got %lu matches.\n",
                    w.asUtf8(), p->store.getWord(it->first).asUtf8(), maxError, (unsigned long) m.size());
        }
    }
}

static void gatherMatchedDocuments(MatcherPrivate *p,  map<WordID, MatchErrorMap> &bestIndexMatches, map<DocumentID, double> &matchedDocuments) {
    for(MatchIndIterator it = bestIndexMatches.begin(); it != bestIndexMatches.end(); it++) {
        for(MatchIterator mIt = it->second.begin(); mIt != it->second.end(); mIt++) {
            vector<DocumentID> tmp;
            findDocuments(p, mIt->first, it->first, tmp);
            debugMessage("Exact searched \"%s\" in field \"%s\", which was found in %lu documents.\n",
                    p->store.getWord(mIt->first).asUtf8(),
                    p->store.getWord(it->first).asUtf8(), (unsigned long)tmp.size());
            for(size_t i=0; i<tmp.size(); i++) {
                DocumentID curDoc = tmp[i];
                // At this point we know the matched word, and which index and field
                // it matched in. Now we can just increment the relevancy of said document.
                double relevancy = calculateRelevancy(p, mIt->first, it->first, mIt->second);
                auto doc = matchedDocuments.find(curDoc);
                if(doc == matchedDocuments.end())
                    matchedDocuments[curDoc] = relevancy;
                else
                    matchedDocuments[curDoc] += relevancy;
            }
        }
    }
}

static void expandQuery(const WordList &query, WordList &expandedQuery) {
    for(size_t i=0; i<query.size(); i++)
        expandedQuery.addWord(query[i]);

    for(size_t i=0; i<query.size()-1; i++) {
        expandedQuery.addWord(query[i].join(query[i+1]));
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
    debugMessage("Added %lu documents to matcher. It now has %lu indexes. Index population took %.2f seconds.\n",
            (unsigned long) c.size(), (unsigned long) p->indexes.size(), buildEnd - buildStart);
    for(IndIterator it = p->indexes.begin(); it != p->indexes.end(); it++) {
        debugMessage("Index \"%s\" has %lu words and %lu nodes.\n",
                p->store.getWord(it->first).asUtf8(), (unsigned long) it->second->numWords(),
                (unsigned long) it->second->numNodes());
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
        map<WordID, DocumentSet > tmp;
        p->reverseIndex[indexID] = tmp;
        rit = p->reverseIndex.find(indexID);
    }
    map<WordID, DocumentSet > &indexRind = rit->second;
    RevIterator revIt = indexRind.find(wordID);
    if(revIt == indexRind.end()) {
        DocumentSet tmp;
        tmp.insert(d->getID());
        indexRind[wordID] = tmp;
    } else {
        DocumentID tmp = d->getID();
        revIt->second.insert(tmp);
    }
}

void Matcher::matchWithRelevancy(const WordList &query, const bool dynamicError, const int extraError, MatchResults &matchedDocuments) {
    map<DocumentID, double> docs;
    BestIndexMatches bestIndexMatches;
    double start, indexMatchEnd, gatherEnd, finish;

    start = hiresTimestamp();
    matchIndexes(p, query, dynamicError, extraError, bestIndexMatches);
    indexMatchEnd = hiresTimestamp();
    // Now we know all matched words in all indexes. Gather up the corresponding documents.
    gatherMatchedDocuments(p, bestIndexMatches, docs);
    gatherEnd = hiresTimestamp();
    for(auto it=docs.begin(); it != docs.end(); it++) {
        matchedDocuments.addResult(it->first, it->second);
    }
    debugMessage("Found a total of %lu documents.\n", (unsigned long) matchedDocuments.size());
    finish = hiresTimestamp();
    debugMessage("Query finished. Index lookups took %.2fs, result gathering %.2fs, result building %.2fs.\n",
            indexMatchEnd - start, gatherEnd - indexMatchEnd, finish - gatherEnd);
}

void Matcher::match(const WordList &query, MatchResults &matchedDocuments) {
    const int maxIterations = 1;
    const int increment = LevenshteinIndex::getDefaultError();
    const size_t minMatches = 10;
    WordList expandedQuery;

    if(query.size() == 0)
        return;
    expandQuery(query, expandedQuery);
    // Try to search with ever growing error until we find enough matches.
    for(int i=0; i<maxIterations; i++) {
        MatchResults matches;
        matchWithRelevancy(expandedQuery, true, i*increment, matches);
        if(matches.size() >= minMatches || i == maxIterations-1) {
            matchedDocuments.addResults(matches);
            return;
        }
    }

}

void Matcher::match(const char *queryAsUtf8, MatchResults &matchedDocuments) {
    WordList l;
    splitToWords(queryAsUtf8, l);
    match(l, matchedDocuments);
}

ErrorValues& Matcher::getErrorValues() {
    return p->e;
}

static bool subtermsMatch(MatcherPrivate *p, const ResultFilter &filter, size_t term, DocumentID id) {
    for(size_t subTerm=0; subTerm < filter.numSubTerms(term); subTerm++) {
        const Word &filterName = filter.getField(term, subTerm);
        const Word &value = filter.getWord(term, subTerm);
        bool termFound = documentHasTerm(p, id, filterName, value);
        if(!termFound) {
            return false;
        }
    }
    return true;

}

void Matcher::match(const char *queryAsUtf8, MatchResults &matchedDocuments, const ResultFilter &filter) {
    MatchResults allMatches;
    match(queryAsUtf8, allMatches);
    for(size_t i=0; i<allMatches.size(); i++) {
        DocumentID id = allMatches.getDocumentID(i);
        for(size_t term=0; term < filter.numTerms(); term++) {
            if(subtermsMatch(p, filter, term, id)) {
                matchedDocuments.copyResult(allMatches, i);
                break;
            }
        }
    }
}

IndexWeights& Matcher::getIndexWeights() {
    return p->weights;
}

COL_NAMESPACE_END
