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

#ifndef ERRORVALUES_HH_
#define ERRORVALUES_HH_

#include "ColumbusCore.hh"

COL_NAMESPACE_START

struct ErrorValuesPrivate;
class Word;

class ErrorValues {
private:
    static const int DEFAULT_ERROR = 100;
    static const int DEFAULT_GROUP_ERROR = 30;
    static const int DEFAULT_TYPO_ERROR = 70;

    int insertionError;
    int deletionError;
    int substituteError;
    int transposeError;

    ErrorValuesPrivate *p;

    void clearLUT();
    void addToLUT(Letter l1, Letter l2, int value);
    int getSubstituteErrorSlow(Letter l1, Letter l2) const;

public:

    ErrorValues();
    ~ErrorValues();

    int getInsertionError() const { return insertionError; }
    int getDeletionError() const { return deletionError; }
    int getTransposeError() const { return transposeError; }
    int getSubstituteError(Letter l1, Letter l2) const;

    static int getDefaultError() { return ErrorValues::DEFAULT_ERROR; }
    static int getDefaultGroupError() { return ErrorValues::DEFAULT_GROUP_ERROR; }
    static int getDefaultTypoError() { return ErrorValues::DEFAULT_TYPO_ERROR; }

    void setError(Letter l1, Letter l2, int error);
    void setGroupError(const Word &groupLetters, int error);
    void addLatinAccents();
    void addKeyboardErrors();
    void addStandardErrors() { addLatinAccents(); addKeyboardErrors(); }
    bool isInGroup(Letter l);
    void clearErrors();
};

COL_NAMESPACE_END

#endif /* ERRORVALUES_HH_ */
