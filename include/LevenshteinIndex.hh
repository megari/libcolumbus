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

struct TrieNode;
class MatchRow;

struct Match {
    std::string word;
    int error;
};

class LevenshteinIndex {
private:
    TrieNode *root;
    static const int DEFAULT_ERROR = 100;
    int insertion_error;
    int deletion_error;
    int transpose_error;
    int substitute_error;

    int get_insertion_error() const { return insertion_error; }
    int get_deletion_error() const { return deletion_error; }
    int get_transpose_error() const { return transpose_error; }
    int get_substitute_error(Letter l1, Letter l2) const { return l1 == l2 ? 0 : substitute_error; }

    void search_recursive(const std::string &word, TrieNode *node, int depth, MatchRow *previous_row, std::vector<Match> &matches, int max_error);

public:
    LevenshteinIndex();
    ~LevenshteinIndex();

    void insert_word(const std::string &word);
    bool has_word(const std::string &word) const;

    void find_words(const std::string &word, const int max_error, std::vector<Match> &matches);
};

#endif
