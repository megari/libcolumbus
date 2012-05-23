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
#include "Word.hh"

using namespace std;

void testTrivial() {
    LevenshteinIndex ind;
    IndexMatches matches;
    Word w("a");

    ind.findWords(w, 100*LevenshteinIndex::getDefaultError(), matches);
    assert(matches.size() == 0);
}

void testSimple() {
    LevenshteinIndex ind;
    IndexMatches matches;
    Word w1("abc");
    Word w2("def");

    ind.insertWord(w1);
    ind.insertWord(w2);

    ind.findWords(w1, LevenshteinIndex::getDefaultError(), matches);
    assert(matches.size() == 1);
    assert(w1 == matches.getMatch(0));
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

    Word w1("abcde");
    Word w2("abxye");
    Word w3("abche");
    Word w4("abxhe");
    Word veryFarFromEveryOtherString("supercalifragilisticexpialidocious");

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
    Word w1("abc");
    Word w2("bbc");
    Word w3("acc");
    Word w4("abb");

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

void testEmptyQuery() {
    LevenshteinIndex ind;
    IndexMatches matches;
    const int defaultError = LevenshteinIndex::getDefaultError();

    Word w1("a");
    Word w2("b");
    Word w3("abc");
    Word empty("");

    ind.insertWord(w1);
    ind.insertWord(w2);
    ind.insertWord(w3);

    ind.findWords(empty, defaultError, matches);
    assert(matches.size() == 2);
    assert(matches.getMatchError(0) == defaultError);
    assert(matches.getMatchError(1) == defaultError);
}

void testExact() {
    LevenshteinIndex ind;
    IndexMatches matches;

    Word w1("abcd");
    Word w2("abce");

    ind.insertWord(w1);

    ind.findWords(w2, 0, matches);
    assert(matches.size() == 0);

    ind.findWords(w1, 0, matches);
    assert(matches.size() == 1);
    assert(matches.getMatch(0) == w1);
    assert(matches.getMatchError(0) == 0);
}

void testTranspose() {
    LevenshteinIndex ind;
    IndexMatches matches;
    const int defaultError = LevenshteinIndex::getDefaultError();

    Word w1("abcd");
    Word w2("acbd");
    Word w3("bacd");
    Word w4("abdc");

    ind.insertWord(w1);

    ind.findWords(w2, defaultError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();

    ind.findWords(w3, defaultError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();

    ind.findWords(w4, defaultError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();
}


int main(int argc, char **argv) {
    testTrivial();
    testSimple();
    testOrder();
    testEdges();
    testEmptyQuery();
    testExact();
    testTranspose();
    return 0;
}
