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

#ifndef LEVENSHTEININDEX_HH
#define LEVENSHTEININDEX_HH

#include <vector>
#include "ColumbusCore.hh"
#include "IndexMatches.hh"

struct LevenshteinIndexPrivate;
struct TrieNode;
class MatchRow;
class MemoryCleaner;
class Word;
class ErrorValues;

class LevenshteinIndex {
private:
    LevenshteinIndexPrivate *p;

    void searchRecursive(const Word &query, TrieNode *node, const ErrorValues &e, Letter letter, Letter previousLetter, MatchRow *previous_row, IndexMatches &matches, const int max_error, MemoryCleaner &cleaner) const;
    void trieInsert(TrieNode *node, const Word &word);

    // Disable copy and move.
    LevenshteinIndex(const LevenshteinIndex &other);
    LevenshteinIndex& operator=(const LevenshteinIndex &other);

public:
    LevenshteinIndex();
    ~LevenshteinIndex();

    static int getDefaultError();

    void insertWord(const Word &word);
    bool hasWord(const Word &word) const;

    void findWords(const Word &query, const ErrorValues &e, const int max_error, IndexMatches &matches) const;
    size_t wordCount(const Word &query) const;
    size_t maxCount() const;
    size_t numNodes() const;
    size_t numWords() const;
};

#endif
