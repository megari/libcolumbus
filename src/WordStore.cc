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

#include "WordStore.hh"
#include "Word.hh"
#include <map>
#include <stdexcept>

using namespace std;

struct WordStorePrivate {
    map<Word, WordID> wordList;
    map<WordID, Word> wordIndex; // The Word object is duplicated here. It should be fixed.
    WordID nextID; // A running counter.
};

WordStore::WordStore() {
    p = new WordStorePrivate();
    p->nextID = 0;

}

WordStore::~WordStore() {
    delete p;
}

WordID WordStore::getID(const Word &w) {
    map<Word, WordID>::const_iterator it = p->wordList.find(w);
    if(it != p->wordList.end())
        return it->second;
    WordID result = p->nextID;
    p->wordList[w] = p->nextID;
    p->wordIndex[p->nextID] = w;
    p->nextID++;
    return result;
}

const Word& WordStore::getWord(const WordID id) const {
    map<WordID, Word>::const_iterator it = p->wordIndex.find(id);
    if(it == p->wordIndex.end()) {
        throw out_of_range("Tried to access non-existing WordID in WordStore.");
    }
    return it->second;
}
