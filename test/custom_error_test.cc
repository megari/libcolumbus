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
#include "ErrorValues.hh"

using namespace std;

void testCustomError() {
    LevenshteinIndex ind;
    IndexMatches matches;
    const int defaultError = ErrorValues::getDefaultError();
    const int smallError = 1;
    const int biggerError = 2;
    assert(smallError < defaultError);
    assert(biggerError < defaultError);

    string w1 = "abc";
    string w2 = "adc";

    ind.insertWord(w1);

    ind.findWords(w2, defaultError, matches);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();

    ind.getErrorValues()->setError(Letter('b'), Letter('d'), smallError);
    ind.findWords(w2, defaultError, matches);
    assert(matches.getMatchError(0) == smallError);
    matches.clear();

    ind.getErrorValues()->setError(Letter('d'), Letter('b'), biggerError);
    ind.findWords(w2, defaultError, matches);
    assert(matches.getMatchError(0) == biggerError);
    matches.clear();

    ind.getErrorValues()->clearErrors();
    ind.findWords(w2, defaultError, matches);
    assert(matches.getMatchError(0) == defaultError);
    matches.clear();
}

int main(int argc, char **argv) {
    testCustomError();
    return 0;
}
