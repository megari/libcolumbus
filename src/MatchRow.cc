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

#include <stdio.h>
#include <cassert>
#include "MatchRow.hh"

MatchRow::MatchRow(int rowsize_, int error) : parent(0), rowsize(rowsize_) {
    values = new int[rowsize];
    for(int i=0; i<rowsize; i++)
        values[i] = i*error;
}
MatchRow::MatchRow(MatchRow *parent_, int deletion_error) : parent(parent_), rowsize(parent->rowsize){
    values = new int[rowsize];
    for(int i=1; i<rowsize; i++) {
        values[i] = parent->values[i] + deletion_error;
    }
}

MatchRow::~MatchRow() {
    delete []values;
}

void MatchRow::set_value(int i, int new_value) {
    assert(i >= 0);
    assert(i < rowsize);
    values[i] = new_value;
}

int MatchRow::min_error() const {
    int result = values[0];
    for(int i=1; i<rowsize; i++)
        if(values[i] < result)
            result = values[i];
    return result;
}

void MatchRow::print() const {
    for(int i=0; i<rowsize; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");
}
