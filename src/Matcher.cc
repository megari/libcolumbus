/*
 * Copyright (C) 2012 Canonical, Ltd.
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
#include <vector>
#include <map>
#include <set>

using namespace std;

struct MatcherPrivate {
    Corpus *c;
    map<Word, LevenshteinIndex*> indexes;
    map<Word, map<Word, set<const Document*> > > reverseIndex; // Index name, word, documents.
    ErrorValues e;
};

typedef map<Word, LevenshteinIndex*>::iterator IndIterator;
typedef map<Word, map<Word, set<const Document*> > >::iterator RevIndIterator;
typedef map<Word, set<const Document*> >::iterator RevIterator;

typedef map<Word, int> MatchErrorMap;

struct ResultGathererPrivate {
    // For each index list matched words and the smallest error with which they were matched.
    map<Word, MatchErrorMap> bestIndexMatches;
};

typedef map<Word, MatchErrorMap>::iterator MatchIndIterator;
typedef map<Word, int>::iterator MatchIterator;

/*
 * These are helper functions for Matcher. They are not member functions to avoid polluting the header
 * with STL includes.
 */

static void addMatches(map<Word, MatchErrorMap> &bestIndexMatches, const Word &queryWord, const Word &indexName, IndexMatches &matches) {
    MatchIndIterator it = bestIndexMatches.find(indexName);
    map<Word, int> *indexMatches;
    if(it == bestIndexMatches.end()) {
        map<Word, int> tmp;
        bestIndexMatches[indexName] = tmp;
        it = bestIndexMatches.find(indexName);
    }
    indexMatches = &(it->second);
    for(size_t i=0; i < matches.size(); i++) {
        const Word &matchWord = matches.getMatch(i);
        const int matchError = matches.getMatchError(i);
        MatchIterator mIt = indexMatches->find(matchWord);
        if(mIt == indexMatches->end()) {
            (*indexMatches)[matchWord] = matchError;
        } else {
            if(mIt->second > matchError)
                (*indexMatches)[matchWord] = matchError;
        }

    }
}

static double calculateRelevancy(const Word &w, int error) {
    return 100.0/(100.0+error);
}

static void findDocuments(MatcherPrivate *p, const Word &word, const Word &fieldName, std::vector<const Document*> &result) {
    IndexMatches im;
    RevIndIterator it = p->reverseIndex.find(fieldName);
    if(it == p->reverseIndex.end())
        return;
    map<Word, set<const Document*> > &rind = it->second;
    RevIterator s = rind.find(word);
    if(s == rind.end())
        return;
    set<const Document*> &docSet = s->second;
    set<const Document*>::iterator foo;
    for(set<const Document*>::iterator docIter = docSet.begin(); docIter != docSet.end(); docIter++) {
        result.push_back(*docIter);
    }
}

static void gatherMatchedDocuments(MatcherPrivate *p,  map<Word, MatchErrorMap> &bestIndexMatches, map<const Document*, double> &matchedDocuments) {
    for(MatchIndIterator it = bestIndexMatches.begin(); it != bestIndexMatches.end(); it++) {
        for(MatchIterator mIt = it->second.begin(); mIt != it->second.end(); mIt++) {
            vector<const Document*> tmp;
            findDocuments(p, mIt->first, it->first, tmp);
            debugMessage("Exact searched \"%s\" in field \"%s\", which was found in %ld documents.\n",
                    mIt->first.asUtf8(), it->first.asUtf8(), tmp.size());
            for(size_t i=0; i<tmp.size(); i++) {
                const Document *curDoc = tmp[i];
                // At this point we know the matched word, and which index and field
                // it matched in. Now we can just increment the relevancy of said document.
                double relevancy = calculateRelevancy(mIt->first, mIt->second);
                map<const Document*, double>::iterator doc = matchedDocuments.find(curDoc);
                if(doc == matchedDocuments.end())
                    matchedDocuments[curDoc] = relevancy;
                else
                    matchedDocuments[curDoc] += relevancy;
            }
        }
    }
}

Matcher::Matcher(Corpus *corp) {
    p = new MatcherPrivate();
    p->c = corp;
    buildIndexes();
    debugMessage("Created matcher with %ld documents.\n", corp->size());
}

Matcher::~Matcher() {
    delete p->c;
    for(IndIterator it = p->indexes.begin(); it != p->indexes.end(); it++) {
        delete it->second;
    }
    delete p;
}

void Matcher::buildIndexes() {
    for(size_t ci = 0; ci < p->c->size(); ci++) {
        const Document &d = p->c->getDocument(ci);
        WordList textNames;
        d.getFieldNames(textNames);
        for(size_t ti=0; ti < textNames.size(); ti++) {
            const Word &name = textNames[ti];
            const WordList &text = d.getText(name);
            for(size_t wi=0; wi<text.size(); wi++) {
                const Word &word = text[wi];
                addToIndex(word, name);
                addToReverseIndex(word, name, &d);
            }
        }
    }
}

void Matcher::addToIndex(const Word &word, const Word &indexName) {
    LevenshteinIndex *target;
    IndIterator it = p->indexes.find(indexName);
    if(it == p->indexes.end()) {
        target = new LevenshteinIndex();
        p->indexes[indexName] = target;
    } else {
        target = it->second;
    }
    target->insertWord(word);
}

void Matcher::addToReverseIndex(const Word &word, const Word &indexName, const Document *d) {
    RevIndIterator rit = p->reverseIndex.find(indexName);
    if(rit == p->reverseIndex.end()) {
        map<Word, set<const Document*> > tmp;
        p->reverseIndex[indexName] = tmp;
        rit = p->reverseIndex.find(indexName);
    }
    map<Word, set<const Document*> > &indexRind = rit->second;
    RevIterator revIt = indexRind.find(word);
    if(revIt == indexRind.end()) {
        set<const Document*> tmp;
        tmp.insert(d);
        indexRind[word] = tmp;
    } else {
        revIt->second.insert(d);
    }
}

void Matcher::matchWithRelevancy(const WordList &query, const bool dynamicError, MatchResults &matchedDocuments) {
    map<const Document*, double> docs;
    map<Word, MatchErrorMap> bestIndexMatches;
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
            addMatches(bestIndexMatches, w, it->first, m);
            debugMessage("Matched word %s in index %s with error %d and got %ld matches.\n",
                    w.asUtf8(), it->first.asUtf8(), maxError, m.size());
        }
    }
    // Now we know all matched words in all indexes. Gather up the corresponding documents.
    gatherMatchedDocuments(p, bestIndexMatches, docs);
    debugMessage("Found a total of %ld documents.\n", matchedDocuments.size());
    for(map<const Document*, double>::iterator it=docs.begin(); it != docs.end(); it++) {
        matchedDocuments.addResult(it->first->getID(), it->second);
    }
}

int Matcher::getDynamicError(const Word &w) {
    size_t len = w.length();
    if(len <= 4)
        return LevenshteinIndex::getDefaultError();
    else
        return int(len/4.0*LevenshteinIndex::getDefaultError()); // Permit a typo for every fourth letter.
}

void Matcher::match(const WordList &query, MatchResults &matchedDocuments) {
    matchWithRelevancy(query, true, matchedDocuments);
}

ErrorValues& Matcher::getErrorValues() {
    return p->e;
}
