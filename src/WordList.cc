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

#include "WordList.hh"
#include "Word.hh"
#include <vector>
#include <stdexcept>

using namespace std;

struct WordListPrivate {
    vector<Word> words;
};

WordList::WordList() {
    p = new WordListPrivate();
}

WordList::WordList(const WordList &wl) {
    p = new WordListPrivate();
    p->words = wl.p->words;
}


WordList::~WordList() {
    delete p;
}

size_t WordList::size() const {
    return p->words.size();
}
const Word& WordList::operator[](const size_t i) const {
    if(i >= p->words.size())
        throw out_of_range("Out of bounds access in WordList.");
    return p->words[i];
}

void WordList::addWord(const Word &w) {
    p->words.push_back(w);
}

const WordList& WordList::operator=(const WordList &l) {
    if(this == &l)
        return *this;
    p->words = l.p->words;
    return *this;
}
