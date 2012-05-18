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

#include "MatchRow.hh"

MatchRow::MatchRow(int rowsize_, int error) : parent(0), rowsize(rowsize_) {
    values = new int[rowsize];
    for(int i=0; i<rowsize; i++)
        values[i] = i*error;
}
MatchRow::MatchRow(MatchRow *parent_) : parent(parent_), rowsize(parent->rowsize){
    values = new int[rowsize];
    for(int i=0; i<rowsize; i++) {
        values[i] = parent->values[i];
    }
}

MatchRow::~MatchRow() {
    delete []values;
}

