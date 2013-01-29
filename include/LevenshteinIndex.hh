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

#ifndef LEVENSHTEININDEX_HH
#define LEVENSHTEININDEX_HH

#include <vector>
#include "ColumbusCore.hh"
#include "IndexMatches.hh"

COL_NAMESPACE_START

struct LevenshteinIndexPrivate;
struct TrieNode;
class ErrorMatrix;
class Word;
class ErrorValues;

typedef uint32_t trieOffset; // FIXME, move this.

class COL_PUBLIC LevenshteinIndex {
private:
    LevenshteinIndexPrivate *p;

    void searchRecursive(const Word &query, trieOffset node, const ErrorValues &e,
            const Letter letter, const Letter previousLetter, const size_t depth, ErrorMatrix &em,
            IndexMatches &matches, const int max_error) const;
    void trieInsert(TrieNode *node, const Word &word, const WordID wordID);

    // Disable copy and move.
    LevenshteinIndex(const LevenshteinIndex &other);
    LevenshteinIndex& operator=(const LevenshteinIndex &other);
    int findOptimalError(const Letter letter, const Letter previousLetter, const Word &query,
            const size_t i, const size_t depth, const ErrorMatrix &em, const ErrorValues &e) const;

public:
    LevenshteinIndex();
    ~LevenshteinIndex();

    static int getDefaultError();

    void insertWord(const Word &word, const WordID wordID);
    bool hasWord(const Word &word) const;

    void findWords(const Word &query, const ErrorValues &e, const int maxError, IndexMatches &matches) const;
    size_t wordCount(const WordID queryID) const;
    size_t maxCount() const;
    size_t numNodes() const;
    size_t numWords() const;
};

COL_NAMESPACE_END

#endif
