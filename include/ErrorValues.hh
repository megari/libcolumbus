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
    static const int DEFAULT_TYPO_ERROR = 30;
    static const int DEFAULT_SUBSTRING_END_DELETION_ERROR = 15;
    static const int DEFAULT_SUBSTRING_START_INSERTION_ERROR = 15;
    static const size_t DEFAULT_SUBSTRING_START_LENGTH = 3;

    int insertionError;
    int deletionError;
    int endDeletionError;
    int startInsertionError;
    int substituteError;
    int transposeError;
    size_t substringStartLimit;

    ErrorValuesPrivate *p;

    void clearLUT();
    void addToLUT(Letter l1, Letter l2, int value);
    void addGroupErrorToLUT(const Word &groupLetters, const int error);
    int getSubstituteErrorSlow(Letter l1, Letter l2) const;

public:

    ErrorValues();
    ~ErrorValues();

    int getInsertionError() const { return insertionError; }
    int getDeletionError() const { return deletionError; }
    int getEndDeletionError() const { return endDeletionError; }
    int getStartInsertionError(const size_t queryTermLength) const {
        return queryTermLength >= substringStartLimit ? startInsertionError : insertionError; }
    int getTransposeError() const { return transposeError; }

    void setInsertionError(const int e) { insertionError = e; }
    void setDeletionError(const int e) { deletionError = e; }
    void setEndDeletionError(const int e) { endDeletionError = e; }
    void setStartInsertionError(const int e) { startInsertionError = e; }
    void setTransposeError(const int e) { transposeError = e; }
    void setSubstringStartLimit(const size_t e) { substringStartLimit = e; }

    int getSubstituteError(Letter l1, Letter l2) const;

    static int getDefaultError() { return ErrorValues::DEFAULT_ERROR; }
    static int getDefaultGroupError() { return ErrorValues::DEFAULT_GROUP_ERROR; }
    static int getDefaultTypoError() { return ErrorValues::DEFAULT_TYPO_ERROR; }
    static int getSubstringDefaultEndDeletionError() { return ErrorValues::DEFAULT_SUBSTRING_END_DELETION_ERROR; }
    static int getSubstringDefaultStartInsertionError() { return ErrorValues::DEFAULT_SUBSTRING_START_INSERTION_ERROR; }

    void setError(Letter l1, Letter l2, const int error);
    void setGroupError(const Word &groupLetters, const int error);
    void addLatinAccents();
    void addKeyboardErrors();
    void addNumberpadErrors();
    void addStandardErrors() { addLatinAccents(); addKeyboardErrors(); }
    bool isInGroup(Letter l);
    void clearErrors();
    void setSubstringMode();
};

COL_NAMESPACE_END

#endif /* ERRORVALUES_HH_ */
