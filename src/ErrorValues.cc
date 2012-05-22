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
#include "ErrorValues.hh"

using namespace std;

struct ErrorValuesPrivate {
    std::map<pair<Letter, Letter>, int> errors;
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
    p->errors[in] = error;
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
    std::map<pair<Letter, Letter>, int>::iterator f = p->errors.find(in);
    if(f == p->errors.end())
        return substitute_error;
    return f->second;
}

void ErrorValues::clearErrors() {
    p->errors.clear();
}
