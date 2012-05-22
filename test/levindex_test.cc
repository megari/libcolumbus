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

void testTrivial() {
    LevenshteinIndex ind;
    IndexMatches matches;

    ind.findWords("a", 100*LevenshteinIndex::getDefaultError(), matches);
    assert(matches.size() == 0);
}

void testSimple() {
    LevenshteinIndex ind;
    IndexMatches matches;
    string w1 = "abc";
    string w2 = "def";

    ind.insertWord(w1);
    ind.insertWord(w2);

    ind.findWords(w1, LevenshteinIndex::getDefaultError(), matches);
    assert(matches.size() == 1);
    assert(matches.getMatch(0) == w1);
    assert(matches.getMatchError(0) == 0);

    matches.clear();

    ind.findWords(w2, LevenshteinIndex::getDefaultError(), matches);
    assert(matches.size() == 1);
    assert(matches.getMatch(0) == w2);
    assert(matches.getMatchError(0) == 0);
}

void testOrder() {
    LevenshteinIndex ind;
    IndexMatches matches;
    const int defaultError = LevenshteinIndex::getDefaultError();

    string w1 = "abcde";
    string w2 = "abxye";
    string w3 = "abche";
    string w4 = "abxhe";
    string veryFarFromEveryOtherString = "supercalifragilisticexpialidocious";

    ind.insertWord(w1);
    ind.insertWord(w2);
    ind.insertWord(veryFarFromEveryOtherString);

    ind.findWords(w3, defaultError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatch(0) == w1);
    assert(matches.getMatchError(0) == defaultError);

    matches.clear();

    ind.findWords(w4, defaultError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatch(0) == w2);
    assert(matches.getMatchError(0) == defaultError);

    matches.clear();

    ind.findWords(w3, 2*defaultError, matches);
    assert(matches.size() == 2);
    assert(matches.getMatch(0) == w1);
    assert(matches.getMatchError(0) == defaultError);
    assert(matches.getMatchError(1) == 2*defaultError);

    matches.clear();

    ind.findWords(w4, 2*defaultError, matches);
    assert(matches.size() == 2);
    assert(matches.getMatch(0) == w2);
    assert(matches.getMatchError(0) == defaultError);
    assert(matches.getMatchError(1) == 2*defaultError);
}

void testEdges() {
    LevenshteinIndex ind;
    IndexMatches matches;
    const int defaultError = LevenshteinIndex::getDefaultError();
    const int bigError = 100*defaultError;
    string w1 = "abc";
    string w2 = "bbc";
    string w3 = "acc";
    string w4 = "abb";

    ind.insertWord(w1);

    ind.findWords(w2, bigError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();

    ind.findWords(w3, bigError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();

    ind.findWords(w4, bigError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();
}

int main(int argc, char **argv) {
    testTrivial();
    testSimple();
    testOrder();
    testEdges();
    return 0;
}
