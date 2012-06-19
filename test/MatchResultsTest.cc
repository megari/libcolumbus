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

#include "MatchResults.hh"
#include "Word.hh"
#include <cassert>

void testMatchResult() {
    MatchResults r;
    DocumentID w1 = 0;
    double r1 = 1;
    DocumentID w2 = 1;
    double r2 = 2;
    DocumentID w3 = 2;
    double r3 = 0.5;

    assert(r.size() == 0);
    r.addResult(w1, r1);
    assert(r.size() == 1);
    assert(r.getRelevancy(0) == r1);

    r.addResult(w2, r2);
    assert(r.size() == 2);
    assert(r.getRelevancy(0) == r2);

    r.addResult(w3, r3);
    assert(r.size() == 3);
    assert(r.getRelevancy(0) == r2);
}

int main(int argc, char **argv) {
    testMatchResult();
    return 0;
}
