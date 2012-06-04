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

#include "MatchResults.hh"
#include "Word.hh"
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

struct MatchResultsPrivate {
    vector<pair<double, const char*> > results; // Having ID as char* means that this result is only valid for as long as the original document exists.
    bool sorted;
};

MatchResults::MatchResults() {
    p = new MatchResultsPrivate();
    p->sorted = true;;
}

MatchResults::~MatchResults() {
    delete p;
}

void MatchResults::addResult(const char *documentID, double relevancy) {
    pair<double, const char*> n;
    n.first = -relevancy; // To make std::sort put the result in descending order.
    n.second = documentID;
    p->results.push_back(n);
    p->sorted = false;
}

size_t MatchResults::size() const {
    return p->results.size();
}

void MatchResults::sortIfRequired() const {
    if(p->sorted)
        return;
    MatchResults *me = const_cast<MatchResults*>(this);
    sort(me->p->results.begin(), me->p->results.end());
    me->p->sorted = true;
}

const char* MatchResults::getDocumentID(size_t i) const {
    if(i>=p->results.size()) {
        throw out_of_range("Access out of bounds in MatchResults::getDocumentID.");
    }
    sortIfRequired();
    return p->results[i].second;
}

double MatchResults::getRelevancy(size_t i) const {
    if(i>=p->results.size()) {
        throw out_of_range("Access out of bounds in MatchResults::getDocumentID.");
    }
    sortIfRequired();
    return -p->results[i].first;
}
