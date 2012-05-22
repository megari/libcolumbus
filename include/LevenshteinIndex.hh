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

#include <string>
#include <vector>
#include "columbuscore.h"
#include "IndexMatches.hh"

struct TrieNode;
class MatchRow;
class MemoryCleaner;
class ErrorValues;

class LevenshteinIndex {
private:
    TrieNode *root;
    ErrorValues *e;

    void searchRecursive(const std::string &word, TrieNode *node, Letter letter, Letter previousLetter, MatchRow *previous_row, IndexMatches &matches, const int max_error, MemoryCleaner &cleaner) const;

public:
    LevenshteinIndex();
    ~LevenshteinIndex();

    static int getDefaultError();

    void insertWord(const std::string &word);
    bool hasWord(const std::string &word) const;

    void findWords(const std::string &word, const int max_error, IndexMatches &matches) const;
    ErrorValues * getErrorValues() { return e; }
};

#endif
