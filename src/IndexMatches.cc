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
#include <algorithm>
#include <cassert>

using namespace std;

struct MatchData {
    string text;
    int error;

    bool operator<(const MatchData &other) const {
        return error < other.error;
    };
};

struct IndexMatchesPrivate {
    vector<MatchData> matches;
};

IndexMatches::IndexMatches() {
    p = new IndexMatchesPrivate();
}

IndexMatches::~IndexMatches() {
    delete p;
}

void IndexMatches::addMatch(const std::string &str, int error) {
    MatchData m;
    m.text = str;
    m.error = error;
    p->matches.push_back(m);
}

size_t IndexMatches::size() const {
    return p->matches.size();
}

const std::string& IndexMatches::getMatch(size_t num) const {
    if(num >= p->matches.size()) {
        throw "Tried to access nonexisting match.";
    }
    return p->matches[num].text;
}

int IndexMatches::getMatchError(size_t num) const {
    if(num >= p->matches.size()) {
        throw "Tried to access nonexisting match error.";
    }
    return p->matches[num].error;
}

void IndexMatches::clear() {
    p->matches.clear();
}

void IndexMatches::sort() {
    std::sort(p->matches.begin(), p->matches.end());
}
