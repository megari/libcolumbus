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

#ifndef ERRORMATRIX_HH_
#define ERRORMATRIX_HH_

#include"ColumbusCore.hh"

COL_NAMESPACE_START

class ErrorMatrix {
    size_t rows, columns;
    int **m;

public:
    ErrorMatrix(const size_t rows_, const size_t columns_, const int insertError);
    ~ErrorMatrix();

    void initRow(const size_t row, const int deletionError);

    void set(const size_t rowNum, const size_t colNum, const int error);
    int get(const size_t rowNum, const size_t colNum) const;
    int totalError(const size_t rowNum) const;
    int minError(const size_t rowNum) const;

};

COL_NAMESPACE_END

#endif /* ERRORMATRIX_HH_ */
