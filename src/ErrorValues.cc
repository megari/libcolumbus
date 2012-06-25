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

/*
 * This class implements an error lookup system.
 * It is in the hottest of the hot paths in the entire system.
 * Use of crazy optimization techniques is approved.
 */

#include <map>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cassert>
#include "ErrorValues.hh"
#include "Word.hh"
#include "ColumbusSlow.hh"

COL_NAMESPACE_START
using namespace std;

/*
 * This is an experiment into using unordered_map instead
 * of regular map. It turned out to be massively slower.
 */
struct Hasher {
    size_t operator()(const pair<Letter, Letter> &p) const {
        const size_t shift = sizeof(size_t)*4;
        assert(shift == 32);
        // The simplest possible hash, just pack the elements together.
        return (size_t(p.first) << shift) | p.second;
    }
};

const int LUT_LETTERS = 512;
const int LUT_SIZE = (LUT_LETTERS*LUT_LETTERS);
const int LUT_SHIFT = (9);
#define LUT_OFFSET(l1, l2) ((l1) << LUT_SHIFT | (l2))

struct ErrorValuesPrivate {
    map<pair<Letter, Letter>, int> singleErrors;
    map<Letter, size_t> groupMap;
    vector<unsigned int> groupErrors;
    int *lut;
};

ErrorValues::ErrorValues() :
    insertionError(DEFAULT_ERROR),
    deletionError(DEFAULT_ERROR),
    substituteError(DEFAULT_ERROR),
    transposeError(DEFAULT_ERROR) {
    p = new ErrorValuesPrivate;
    p->lut = new int[LUT_SIZE];
    clearLUT();
}

ErrorValues::~ErrorValues() {
    delete []p->lut;
    delete p;
}

void ErrorValues::clearLUT() {
    for(int i=0; i<LUT_LETTERS; i++) {
        for(int j=0; j<LUT_LETTERS; j++) {
            p->lut[LUT_OFFSET(i, j)] = i == j ? 0 : substituteError;
        }
    }
}

void ErrorValues::setError(Letter l1, Letter l2, int error) {
    if(l1 > l2) {
        Letter tmp = l1;
        l1 = l2;
        l2 = tmp;
    }
    pair<Letter, Letter> in(l1, l2);
    p->singleErrors[in] = error;
    addToLUT(l1, l2, error);
}

int ErrorValues::getSubstituteError(Letter l1, Letter l2) const {
    if(l1 < LUT_LETTERS && l2 < LUT_LETTERS) {
        return p->lut[LUT_OFFSET(l1, l2)];
    }
    return getSubstituteErrorSlow(l1, l2);
}

int ErrorValues::getSubstituteErrorSlow(Letter l1, Letter l2) const {
    if(l1 == l2)
        return 0;
    if(l1 > l2) {
        Letter tmp = l1;
        l1 = l2;
        l2 = tmp;
    }
    pair<Letter, Letter> in(l1, l2);
    auto f = p->singleErrors.find(in);
    if(f != p->singleErrors.end())
        return f->second;

    // Are the letters in the same error group? Check the bigger
    // value first, because it is probably a more uncommon letter.
    auto g1 = p->groupMap.find(l2);
    if(g1 != p->groupMap.end()) {
        auto g2 = p->groupMap.find(l1);
        if(g2 != p->groupMap.end()) {
            if(g1->second == g2->second) {
                return p->groupErrors[g1->second];
            }
        }
    }
    return substituteError;
}

void ErrorValues::clearErrors() {
    p->singleErrors.clear();
    p->groupErrors.clear();
    p->groupMap.clear();
    clearLUT();
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
    for(size_t i=0; i<groupLetters.length(); i++) {
        for(size_t j=i; j<groupLetters.length(); j++) {
            Letter l1 = groupLetters[i];
            Letter l2 = groupLetters[j];
            if(l1 == l2)
                continue;
            addToLUT(l1, l2, error);
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

void ErrorValues::addKeyboardErrors() {
    int error = getDefaultTypoError();
    // Yes, this is a Finnish keyboard.
    const Letter line1[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+'};
    const Letter line2[] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0xe5};
    const Letter line3[] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0xf6, 0xe4, '\''};
    const Letter line4[] = {'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-'};

    const Letter *keyboard_layout[4] = {line1, line2, line3, line4};
    const size_t lineLens[] = {11, 11, 12, 11};
    for(size_t i = 0; i < 3; i++) {
        const Letter *cur_row = keyboard_layout[i];
        const Letter *next_row = keyboard_layout[i+1];
        for(size_t j1=0; j1 < lineLens[i]; j1++) {
            Letter l1 = cur_row[j1];
            if(j1 + 1 < lineLens[i])
                setError(l1, cur_row[j1+1], error);
            if(j1 > 0 && j1-1 < lineLens[i+1])
                setError(l1, next_row[j1-1], error);
            if(j1 < lineLens[i+1])
                setError(l1, next_row[j1], error);
        }
    }


}


void ErrorValues::addToLUT(Letter l1, Letter l2, int value) {
    if(l1 < LUT_LETTERS && l2 < LUT_LETTERS) {
        p->lut[LUT_OFFSET(l1, l2)] = value;
        p->lut[LUT_OFFSET(l2, l1)] = value;
    }
}

COL_NAMESPACE_END
