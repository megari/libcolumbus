/*
 * Copyright (C) 2013 Canonical, Ltd.
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

#ifndef TRIE_HH
#define TRIE_HH

#include "ColumbusCore.hh"

COL_NAMESPACE_START

struct TriePrivate;
class Word;

class Trie {
private:
    TriePrivate *p;
    void expand();
    TrieOffset append(const char *data, const int size);
    TrieOffset addNewSibling(const TrieOffset sibling, Letter l);
    TrieOffset addNewNode();

public:
    Trie();
    ~Trie();

    bool hasWord(const Word &word) const;
    void insertWord(const Word &word, const WordID wordID);
    TrieOffset getRoot() const;
    TrieOffset getSiblingList(TrieOffset node) const;
    TrieOffset getNextSibling(TrieOffset sibling) const;
    Letter getLetter(TrieOffset sibling) const;
    TrieOffset getChild(TrieOffset sibling) const;
    WordID getWordID(TrieOffset node) const;
    bool hasSibling(TrieOffset sibling) const;

    size_t numWords() const;
    size_t numNodes() const;
};

COL_NAMESPACE_END

#endif /*  */
