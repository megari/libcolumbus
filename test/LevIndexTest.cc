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
#include "ErrorValues.hh"

using namespace std;

void testTrivial() {
    LevenshteinIndex ind;
    IndexMatches matches;
    ErrorValues e;
    Word w("a");

    ind.findWords(w, e, 100*LevenshteinIndex::getDefaultError(), matches);
    assert(matches.size() == 0);
}

void testSimple() {
    LevenshteinIndex ind;
    IndexMatches matches;
    ErrorValues e;
    Word w1("abc");
    Word w2("def");
    WordID w1ID = 1;
    WordID w2ID = 2;

    ind.insertWord(w1, w1ID);
    ind.insertWord(w2, w2ID);

    ind.findWords(w1, e, LevenshteinIndex::getDefaultError(), matches);
    assert(matches.size() == 1);
    assert(w1ID == matches.getMatch(0));
    assert(matches.getMatchError(0) == 0);

    matches.clear();

    ind.findWords(w2, e, LevenshteinIndex::getDefaultError(), matches);
    assert(matches.size() == 1);
    assert(matches.getMatch(0) == w2ID);
    assert(matches.getMatchError(0) == 0);
}

void testOrder() {
    LevenshteinIndex ind;
    IndexMatches matches;
    ErrorValues e;
    const int defaultError = LevenshteinIndex::getDefaultError();

    Word w1("abcde");
    Word w2("abxye");
    Word w3("abche");
    Word w4("abxhe");
    WordID w1ID = 1;
    WordID w2ID = 2;
    //WordID w3ID = 3;
    //WordID w4ID = 4;

    Word veryFarFromEveryOtherString("supercalifragilisticexpialidocious");
    WordID veryFarID = 100;

    ind.insertWord(w1, w1ID);
    ind.insertWord(w2, w2ID);
    ind.insertWord(veryFarFromEveryOtherString, veryFarID);

    ind.findWords(w3, e, defaultError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatch(0) == w1ID);
    assert(matches.getMatchError(0) == defaultError);

    matches.clear();

    ind.findWords(w4, e, defaultError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatch(0) == w2ID);
    assert(matches.getMatchError(0) == defaultError);

    matches.clear();

    ind.findWords(w3, e, 2*defaultError, matches);
    assert(matches.size() == 2);
    assert(matches.getMatch(0) == w1ID);
    assert(matches.getMatchError(0) == defaultError);
    assert(matches.getMatchError(1) == 2*defaultError);

    matches.clear();

    ind.findWords(w4, e, 2*defaultError, matches);
    assert(matches.size() == 2);
    assert(matches.getMatch(0) == w2ID);
    assert(matches.getMatchError(0) == defaultError);
    assert(matches.getMatchError(1) == 2*defaultError);
}

void testEdges() {
    LevenshteinIndex ind;
    IndexMatches matches;
    ErrorValues e;
    const int defaultError = LevenshteinIndex::getDefaultError();
    const int bigError = 100*defaultError;
    Word w1("abc");
    Word w2("bbc");
    Word w3("acc");
    Word w4("abb");
    WordID w1ID = 1;

    ind.insertWord(w1, w1ID);

    ind.findWords(w2, e, bigError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();

    ind.findWords(w3, e, bigError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();

    ind.findWords(w4, e, bigError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();
}

void testEmptyQuery() {
    LevenshteinIndex ind;
    IndexMatches matches;
    ErrorValues e;
    const int defaultError = LevenshteinIndex::getDefaultError();

    Word w1("a");
    Word w2("b");
    Word w3("abc");
    Word empty("");
    WordID w1ID = 1;
    WordID w2ID = 2;
    WordID w3ID = 3;

    ind.insertWord(w1, w1ID);
    ind.insertWord(w2, w2ID);
    ind.insertWord(w3, w3ID);

    ind.findWords(empty, e, defaultError, matches);
    assert(matches.size() == 2);
    assert(matches.getMatchError(0) == defaultError);
    assert(matches.getMatchError(1) == defaultError);
}

void testExact() {
    LevenshteinIndex ind;
    IndexMatches matches;
    ErrorValues e;

    Word w1("abcd");
    Word w2("abce");
    WordID w1ID = 1;

    ind.insertWord(w1, w1ID);

    ind.findWords(w2, e, 0, matches);
    assert(matches.size() == 0);

    ind.findWords(w1, e, 0, matches);
    assert(matches.size() == 1);
    assert(matches.getMatch(0) == w1ID);
    assert(matches.getMatchError(0) == 0);
}

void testTranspose() {
    LevenshteinIndex ind;
    IndexMatches matches;
    ErrorValues e;
    const int defaultError = LevenshteinIndex::getDefaultError();

    Word w1("abcd");
    Word w2("acbd");
    Word w3("bacd");
    Word w4("abdc");
    WordID w1ID = 1;

    ind.insertWord(w1, w1ID);

    ind.findWords(w2, e, defaultError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();

    ind.findWords(w3, e, defaultError, matches);
    assert(matches.size() == 1);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();

    ind.findWords(w4, e, defaultError, matches);
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
