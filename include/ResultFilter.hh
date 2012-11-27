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

#ifndef RESULTFILTER_HH_
#define RESULTFILTER_HH_

#include "ColumbusCore.hh"

COL_NAMESPACE_START

struct ResultFilterPrivate;
class Word;

class COL_PUBLIC ResultFilter {
private:

    ResultFilterPrivate *p;

public:
    ResultFilter();
    ~ResultFilter();

    void addNewTerm();
    void addNewSubTerm(const Word &field, const Word &word);
    size_t numTerms() const;
    size_t numSubTerms(const size_t term) const;

    const Word& getField(const size_t term, const size_t subTerm) const;
    const Word& getWord(const size_t term, const size_t subTerm) const;
};

COL_NAMESPACE_END

#endif /* RESULTFILTER_HH_ */
