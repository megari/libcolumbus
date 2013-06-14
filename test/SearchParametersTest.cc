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

#include"SearchParameters.hh"
#include<cassert>

using namespace Columbus;

void testDynamic() {
    SearchParameters sp;
    assert(sp.isDynamic());

    sp.setDynamic(false);
    assert(!sp.isDynamic());

    sp.setDynamic(true);
    assert(sp.isDynamic());
}

int main(int /*argc*/, char **/*argv*/) {
    testDynamic();
}
