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

#ifndef MATCHRESULTS_HH_
#define MATCHRESULTS_HH_

#include <cstddef>

struct MatchResultsPrivate;
class Word;

class MatchResults {
    MatchResultsPrivate *p;

    void sortIfRequired() const;

public:
    MatchResults();
    ~MatchResults();

    void addResult(const char *documentID, double relevancy);
    size_t size() const;
    const char* getDocumentID(size_t i) const;
    double getRelevancy(size_t i) const;
};

#endif /* MATCHRESULTS_HH_ */
