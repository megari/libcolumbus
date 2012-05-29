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
#include <map>
#include <set>

using namespace std;

struct MatcherPrivate {
    Corpus *c;
    map<Word, LevenshteinIndex*> indexes;
    map<Word, set<const Document*> > wordReverseIndex;
};

typedef map<Word, LevenshteinIndex*>::iterator IndIterator;
typedef map<Word, set<const Document*> >::iterator RevIndIterator;

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
        d.getTextNames(textNames);
        for(size_t ti=0; ti < textNames.size(); ti++) {
            const Word &name = textNames[ti];
            const WordList &text = d.getText(name);
            for(size_t wi=0; wi<text.size(); wi++) {
                const Word &word = text[wi];
                addToIndex(word, name);
                addToReverseIndex(word, &d);
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

void Matcher::addToReverseIndex(const Word &word, const Document *d) {
    RevIndIterator rit = p->wordReverseIndex.find(word);
    if(rit == p->wordReverseIndex.end()) {
        set<const Document*> tmp;
        tmp.insert(d);
        p->wordReverseIndex[word] = tmp;
    } else {
        rit->second.insert(d);
    }
}
