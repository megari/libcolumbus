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

#ifndef INDEXMATCHES_H_
#define INDEXMATCHES_H_

#include <cstddef>
#include "ColumbusCore.hh"

COL_NAMESPACE_START

class LevenshteinIndex;
struct IndexMatchesPrivate;
class Word;

/**
 * A class that contains a list of index matches
 * in growing error order.
 *
 */
class COL_PUBLIC IndexMatches {
    friend class LevenshteinIndex;

private:

    IndexMatchesPrivate *p;

    void addMatch(const Word &queryWord, const WordID matchedWord, int error);
    void sort();

    // Disable copy and assignment.
    IndexMatches(const IndexMatches &other);
    const IndexMatches & operator=(const IndexMatches &other);

public:
    IndexMatches();
    ~IndexMatches();

    size_t size() const;
    const WordID& getMatch(size_t num) const;
    const Word& getQuery(size_t num) const;
    int getMatchError(size_t num) const;
    void clear();

};

COL_NAMESPACE_END

#endif /* INDEXMATCHES_H_ */
