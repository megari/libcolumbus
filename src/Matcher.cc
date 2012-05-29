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
#include "ResultGatherer.hh"
#include <map>
#include <set>

using namespace std;

struct MatcherPrivate {
    Corpus *c;
    map<Word, LevenshteinIndex*> indexes;
    map<Word, map<Word, set<const Document*> > > reverseIndex; // Index name, word, documents.
};

typedef map<Word, LevenshteinIndex*>::iterator IndIterator;
typedef map<Word, map<Word, set<const Document*> > >::iterator RevIndIterator;
typedef map<Word, set<const Document*> >::iterator RevIterator;

Matcher::Matcher(Corpus *corp) {
    p = new MatcherPrivate();
    p->c = corp;
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

void Matcher::match_with_relevancy(const WordList &query, const bool dynamicError) {
    ResultGatherer r;
    for(size_t i=0; i<query.size(); i++) {
        const Word &w = query[i];
        int maxError;
        if(dynamicError)
            maxError = getDynamicError(w);
        else
            maxError = 2*LevenshteinIndex::getDefaultError();
        for(IndIterator it = p->indexes.begin(); it != p->indexes.end(); it++) {
            IndexMatches m;
            it->second->findWords(w, maxError, m);
            r.addMatches(w, it->first, m);
        }
        // Code missing.
    }
}

int Matcher::getDynamicError(const Word &w) {
    size_t len = w.length();
    if(len <= 4)
        return LevenshteinIndex::getDefaultError();
    else
        return int(len/4.0*LevenshteinIndex::getDefaultError()); // Permit a typo for every fourth letter.
}

void Matcher::findDocuments(const Word &word, const Word &fieldName, std::vector<const Document*> result) {
    IndexMatches im;
    RevIndIterator it = p->reverseIndex.find(fieldName);
    if(it == p->reverseIndex.end())
        return;
    map<Word, set<const Document*> > &rind = it->second;
    for(size_t i=0; i<im.size(); i++) {
        const Word &matched = im.getMatch(i);
        RevIterator s = rind.find(matched);
        if(s == rind.end())
            continue;
        set<const Document*> &docSet = s->second;
        set<const Document*>::iterator foo;
        for(set<const Document*>::iterator docIter = docSet.begin(); docIter != docSet.end(); docIter++) {
            result.push_back(*docIter);
        }
    }
}
