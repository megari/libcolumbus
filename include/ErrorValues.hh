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

#ifndef ERRORVALUES_HH_
#define ERRORVALUES_HH_

#include "columbuscore.h"

class ErrorValues {
private:
    static const int DEFAULT_ERROR = 100;

    int insertion_error;
    int deletion_error;
    int substitute_error;
    int transpose_error;

public:

    ErrorValues();

    int getInsertionError() const { return insertion_error; }
    int getDeletionError() const { return deletion_error; }
    int getTransposeError() const { return transpose_error; }
    int getSubstituteError(Letter l1, Letter l2) const { return l1 == l2 ? 0 : substitute_error; }

    static int getDefaultError() { return ErrorValues::DEFAULT_ERROR; }
};

#endif /* ERRORVALUES_HH_ */