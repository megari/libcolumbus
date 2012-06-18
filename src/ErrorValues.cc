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

#include <map>
#include <vector>
#include <stdexcept>
#include "ErrorValues.hh"
#include "Word.hh"

using namespace std;

struct ErrorValuesPrivate {
    map<pair<Letter, Letter>, int> singleErrors;
    map<Letter, size_t> groupMap;
    vector<unsigned int> groupErrors;
};

ErrorValues::ErrorValues() :
    insertion_error(DEFAULT_ERROR),
    deletion_error(DEFAULT_ERROR),
    substitute_error(DEFAULT_ERROR),
    transpose_error(DEFAULT_ERROR) {
    p = new ErrorValuesPrivate;
}

ErrorValues::~ErrorValues() {
    delete p;
}

void ErrorValues::setError(Letter l1, Letter l2, int error) {
    if(l1 > l2) {
        Letter tmp = l1;
        l1 = l2;
        l2 = tmp;
    }
    pair<Letter, Letter> in(l1, l2);
    p->singleErrors[in] = error;
}

int ErrorValues::getSubstituteError(Letter l1, Letter l2) const {
    if(l1 == l2)
        return 0;
    if(l1 > l2) {
        Letter tmp = l1;
        l1 = l2;
        l2 = tmp;
    }
    pair<Letter, Letter> in(l1, l2);
    std::map<pair<Letter, Letter>, int>::iterator f = p->singleErrors.find(in);
    if(f != p->singleErrors.end())
        return f->second;

    // Are the letters in the same error group?
    map<Letter, size_t>::const_iterator g1 = p->groupMap.find(l1);
    if(g1 != p->groupMap.end()) {
        map<Letter, size_t>::const_iterator g2 = p->groupMap.find(l2);
        if(g2 != p->groupMap.end()) {
            if(g1->second == g2->second) {
                return p->groupErrors[g1->second];
            }
        }
    }
    return substitute_error;
}

void ErrorValues::clearErrors() {
    p->singleErrors.clear();
}

void ErrorValues::setGroupError(const Word &groupLetters, int error) {
    size_t newGroupID = p->groupErrors.size();
    p->groupErrors.push_back(error);
    for(size_t i = 0; i < groupLetters.length(); i++) {
        Letter curLetter = groupLetters[i];
        if(p->groupMap.find(curLetter) != p->groupMap.end()) {
            throw runtime_error("Tried to add letter to two different error groups.");
        }
        p->groupMap[curLetter] = newGroupID;
    }
}

void ErrorValues::addLatinAccents() {
    const unsigned char esses[] =
    {0x65, 0xc3, 0xa9, 0xc3, 0xa8, 0xc4, 0x95, 0xc3, 0xaa, 0xe1, 0xba, 0xbf, 0xe1, 0xbb, 0x81, 0xe1,
     0xbb, 0x85, 0xe1, 0xbb, 0x83, 0xc4, 0x9b, 0xc3, 0xab, 0xe1, 0xba, 0xbd, 0xc4, 0x97, 0xc8, 0xa9,
     0xe1, 0xb8, 0x9d, 0xc4, 0x99, 0xc4, 0x93, 0xe1, 0xb8, 0x97, 0xe1, 0xb8, 0x95, 0xe1, 0xba, 0xbb,
     0xc8, 0x85, 0xc8, 0x87, 0xe1, 0xba, 0xb9, 0xe1, 0xbb, 0x87, 0xe1, 0xb8, 0x99, 0xe1, 0xb8, 0x9b,
     0xc9, 0x87, 0
    };
    Word esWord((const char*) esses);
    setGroupError(esWord, getDefaultGroupError());
}
