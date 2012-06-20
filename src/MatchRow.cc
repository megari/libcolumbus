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

#include <stdio.h>
#include <cassert>
#include <limits.h>
#include "MatchRow.hh"

COL_NAMESPACE_START

MatchRow::MatchRow(size_t rowsize_, int error) : parent(0), rowsize(rowsize_) {
    values = new int[rowsize];
    for(size_t i=0; i<rowsize; i++)
        values[i] = i*error;
}
MatchRow::MatchRow(MatchRow *parent_, int deletion_error) : parent(parent_), rowsize(parent->rowsize){
    values = new int[rowsize];
    values[0] = parent->values[0] + deletion_error;
    for(size_t i=1; i<rowsize; i++) {
        values[i] = INT_MAX;
    }
}

MatchRow::~MatchRow() {
    delete []values;
}

void MatchRow::setValue(size_t i, int new_value) {
    assert(i >= 0);
    assert(i < rowsize);
    values[i] = new_value;
}

int MatchRow::minError() const {
    int result = values[0];
    for(size_t i=1; i<rowsize; i++)
        if(values[i] < result)
            result = values[i];
    return result;
}

void MatchRow::print() const {
    for(size_t i=0; i<rowsize; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");
}

COL_NAMESPACE_END
