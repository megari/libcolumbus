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

#ifndef MATCHROW_H_
#define MATCHROW_H_

class MatchRow {

private:
    MatchRow *parent;
    int *values;
    size_t rowsize;

public:
    MatchRow(size_t rowsize_, int error);
    MatchRow(MatchRow *parent_, int deletion_error);
    ~MatchRow();

    int getValue(size_t i) const { return values[i]; }
    void setValue(size_t i, int new_value);
    int totalError() const { return values[rowsize-1]; }
    int minError() const;

    void print() const;
};

#endif /* MATCHROW_H_ */
