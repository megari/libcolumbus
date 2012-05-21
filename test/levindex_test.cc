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

/*
 * This file tests the error tolerant matching of the Levenshtein index.
 */

#include <cassert>
#include "LevenshteinIndex.hh"

using namespace std;

void test_trivial() {
    LevenshteinIndex ind;
    IndexMatches matches;

    ind.find_words("a", 100*LevenshteinIndex::getDefaultError(), matches);
    assert(matches.numMatches() == 0);
}

void test_simple() {
    LevenshteinIndex ind;
    IndexMatches matches;
    string w1 = "abc";
    string w2 = "def";

    ind.insert_word(w1);
    ind.insert_word(w2);

    ind.find_words(w1, LevenshteinIndex::getDefaultError(), matches);
    assert(matches.numMatches() == 1);
    assert(matches.getMatch(0) == w1);
    assert(matches.getMatchError(0) == 0);

    matches.clear();

    ind.find_words(w2, LevenshteinIndex::getDefaultError(), matches);
    assert(matches.numMatches() == 1);
    assert(matches.getMatch(0) == w1);
    assert(matches.getMatchError(0) == 0);
}

int main(int argc, char **argv) {
    test_trivial();
    test_simple();
    return 0;
}
