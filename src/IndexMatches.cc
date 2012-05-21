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

#include "IndexMatches.hh"

#include <vector>
#include <cassert>

using namespace std;

struct IndexMatchesPrivate {
    vector<string> matches;
    vector<int> errors;
};

IndexMatches::IndexMatches() {
    p = new IndexMatchesPrivate();
}

IndexMatches::~IndexMatches() {
    delete p;
}

void IndexMatches::addMatch(const std::string &str, int error) {
    assert(p->errors.size() == p->matches.size());
    p->matches.push_back(str);
    p->errors.push_back(error);
}

size_t IndexMatches::numMatches() {
    return p->matches.size();
}

const std::string& IndexMatches::getMatch(size_t num) {
    if(num >= p->matches.size()) {
        throw "Tried to access nonexisting match.";
    }
    return p->matches[num];
}

int IndexMatches::getMatchError(size_t num) {
    if(num >= p->errors.size()) {
        throw "Tried to access nonexisting match.";
    }
    return p->errors[num];
}
