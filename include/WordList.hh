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

#ifndef WORDLIST_HH_
#define WORDLIST_HH_

#include <cstddef>
struct WordListPrivate;
class Word;

class WordList {
private:
    WordListPrivate *p;

public:
    WordList();
    WordList(const WordList &wl);
    ~WordList();

    size_t size() const;
    const Word& operator[](const size_t i) const;
    const WordList& operator=(const WordList &l);
    bool operator==(const WordList &l) const;
    bool operator!=(const WordList &l) const;
    void addWord(const Word &w); // This is more of an implementation detail and should not be exposed in a base class or interface.

    // Add proper iterators here.
};

#endif /* WORDLIST_HH_ */
