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
    int rowsize;

public:
    MatchRow(int rowsize_, int error);
    MatchRow(MatchRow *parent_);
    ~MatchRow();

    int value(int i) { return values[i]; }
};

#endif /* MATCHROW_H_ */
