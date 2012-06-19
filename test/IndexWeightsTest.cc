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

#include "IndexWeights.hh"
#include "Word.hh"
#include <cassert>

void testWeights() {
    IndexWeights w;
    Word w1("abc");

    assert(w.getWeight(w1) == 1.0);
    w.setWeight(w1, 2.0);
    assert(w.getWeight(w1) == 2.0);
}

int main(int argc, char **argv) {
    testWeights();
    return 0;
}
