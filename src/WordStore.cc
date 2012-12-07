/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "WordStore.hh"
#include "Word.hh"
#include <map>
#include <vector>
#include <stdexcept>

COL_NAMESPACE_START
using namespace std;

struct WordStorePrivate {
    map<Word, WordID> wordList;
    vector<Word> wordIndex; // The Word object is duplicated here. It should be fixed.
};

WordStore::WordStore() {
    p = new WordStorePrivate();

}

WordStore::~WordStore() {
    delete p;
}

WordID WordStore::getID(const Word &w) {
    map<Word, WordID>::const_iterator it = p->wordList.find(w);
    if(it != p->wordList.end())
        return it->second;
    p->wordIndex.push_back(w);
    WordID result = p->wordIndex.size()-1;
    p->wordList[w] = result;
    return result;
}

const Word& WordStore::getWord(const WordID id) const {
    if(!hasWord(id)) {
        throw out_of_range("Tried to access non-existing WordID in WordStore.");
    }
    return p->wordIndex[id];
}

bool WordStore::hasWord(const WordID id) const {
    return id < p->wordIndex.size();
}

COL_NAMESPACE_END
