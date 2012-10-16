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

#ifdef USE_PCH
#include "columbus_pch.hh"
#else
#include <vector>
#endif

#include "MatchResults.hh"
#include "Word.hh"
#include <algorithm>
#include <stdexcept>

COL_NAMESPACE_START
using namespace std;

struct MatchResultsPrivate {
    vector<pair<double, DocumentID> > results;
    bool sorted;
};

MatchResults::MatchResults() {
    p = new MatchResultsPrivate();
    p->sorted = true;;
}

MatchResults::~MatchResults() {
    delete p;
}

void MatchResults::addResult(DocumentID id, double relevancy) {
    pair<double, DocumentID> n;
    n.first = relevancy;
    n.second = id;
    p->results.push_back(n);
    p->sorted = false;
}

void MatchResults::addResults(const MatchResults &r) {
    p->results.insert(p->results.end(), r.p->results.begin(), r.p->results.end());
    p->sorted = false;
}


size_t MatchResults::size() const {
    return p->results.size();
}

void MatchResults::copyResult(const MatchResults &other, const size_t i) {
    if(i >= other.p->results.size()) {
        throw out_of_range("Tried to copy an out-of-range result.");
    }
    p->results.push_back(other.p->results[i]);
    p->sorted = false;
}

void MatchResults::sortIfRequired() const {
    if(p->sorted)
        return;
    MatchResults *me = const_cast<MatchResults*>(this);
    sort(me->p->results.rbegin(), me->p->results.rend());
    me->p->sorted = true;
}

DocumentID MatchResults::getDocumentID(size_t i) const {
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
    return p->results[i].first;
}

COL_NAMESPACE_END
