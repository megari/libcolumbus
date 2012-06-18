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
#include <fstream>
#include "ErrorValues.hh"
#include "Word.hh"
#include "ColumbusSlow.hh"

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
        if(isInGroup(curLetter)) {
            if(p->groupMap.find(curLetter)->second != newGroupID)
                throw runtime_error("Tried to add letter to two different error groups.");
        } else {
            p->groupMap[curLetter] = newGroupID;
        }
    }
    debugMessage("Added error group: %s\n", groupLetters.asUtf8());
}

bool ErrorValues::isInGroup(Letter l) {
    return p->groupMap.find(l) != p->groupMap.end();
}

void ErrorValues::addLatinAccents() {
    const char *baseName = "latinAccentedLetterGroups.txt";
    string dataFile = findDataFile(baseName);
    string line;
    if(dataFile.length() == 0) {
        string s = "Could not find file ";
        s += baseName;
        throw runtime_error(s);
    }
    ifstream ifile(dataFile.c_str());
    if(ifile.fail()) {
        string s = "Could not open data file ";
        s += dataFile;
        throw runtime_error(s);
    }
    while(getline(ifile, line)) {
        Word group(line.c_str());
        if(group.length() == 0)
            continue;
        setGroupError(group, getDefaultGroupError());
    }
}
