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

#ifndef INDEXMATCHES_H_
#define INDEXMATCHES_H_

#include<string>

struct IndexMatchesPrivate;

class IndexMatches {
private:

    IndexMatchesPrivate *p;

public:
    IndexMatches();
    ~IndexMatches();

    void addMatch(const std::string &str, int error);
    size_t numMatches() const;
    const std::string& getMatch(size_t num) const;
    int getMatchError(size_t num) const;

    void clear();
};

#endif /* INDEXMATCHES_H_ */
