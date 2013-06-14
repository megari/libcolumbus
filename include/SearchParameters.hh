/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEARCHPARAMETERS_H_
#define SEARCHPARAMETERS_H_

#include "ColumbusCore.hh"

COL_NAMESPACE_START

struct SearchParametersPrivate;

class COL_PUBLIC SearchParameters final {
private:
    SearchParametersPrivate *p;

public:
    SearchParameters();
    ~SearchParameters();
    SearchParameters & operator=(const SearchParameters &other) = delete;

    bool getDynamicError() const;
    void setDynamicError(bool dyn);
};

COL_NAMESPACE_END
#endif
