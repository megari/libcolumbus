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
#include <map>

using namespace std;

struct MatcherPrivate {
    Corpus *c;
    map<Word, LevenshteinIndex*> indexes;
};

typedef map<Word, LevenshteinIndex*>::iterator IndIterator;

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
        //for(size_t di=0; di < d.textCount(); di++)
    }
}
