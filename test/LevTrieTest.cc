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
 * This file tests the trie portion of the Levenshtein index.
 * That is, exact matches.
 */

#include <cassert>
#include "LevenshteinIndex.hh"
#include "Word.hh"

void basicTest() {
    LevenshteinIndex ind;
    Word word1("word");
    Word word2("another");

    assert(!ind.hasWord(word1));
    assert(!ind.hasWord(word2));

    ind.insertWord(word1);
    assert(ind.hasWord(word1));
    assert(!ind.hasWord(word2));

    ind.insertWord(word2);
    assert(ind.hasWord(word1));
    assert(ind.hasWord(word2));
}

void shortTest() {
    LevenshteinIndex ind;
    Word word("a");

    assert(!ind.hasWord(word));

    ind.insertWord(word);
    assert(ind.hasWord(word));
}

void prefixTest() {
    LevenshteinIndex ind;
    Word word("ab");
    Word prefix("a");

    assert(!ind.hasWord(word));
    assert(!ind.hasWord(prefix));

    ind.insertWord(word);
    assert(ind.hasWord(word));
    assert(!ind.hasWord(prefix));

    ind.insertWord(prefix);
    assert(ind.hasWord(word));
    assert(ind.hasWord(prefix));
}

void suffixTest() {
    LevenshteinIndex ind;
    Word word("abc");
    Word word2("abcd");

    assert(!ind.hasWord(word));
    assert(!ind.hasWord(word2));

    ind.insertWord(word);
    assert(ind.hasWord(word));
    assert(!ind.hasWord(word2));

    ind.insertWord(word2);
    assert(ind.hasWord(word));
    assert(ind.hasWord(word2));
}

void branchTest() {
    LevenshteinIndex ind;
    Word word("abc");
    Word word2("abcd");
    Word word3("abce");

    assert(!ind.hasWord(word));
    assert(!ind.hasWord(word2));
    assert(!ind.hasWord(word3));

    ind.insertWord(word);
    assert(ind.hasWord(word));
    assert(!ind.hasWord(word2));
    assert(!ind.hasWord(word3));

    ind.insertWord(word2);
    assert(ind.hasWord(word));
    assert(ind.hasWord(word2));
    assert(!ind.hasWord(word3));

    ind.insertWord(word3);
    assert(ind.hasWord(word));
    assert(ind.hasWord(word2));
    assert(ind.hasWord(word3));
}

void countTest() {
    LevenshteinIndex ind;
    Word w1("abc");
    Word w2("def");
    Word w3("abce");

    assert(ind.wordCount(w1) == 0);
    assert(ind.wordCount(w2) == 0);
    assert(ind.wordCount(w3) == 0);

    ind.insertWord(w1);
    assert(ind.wordCount(w1) == 1);
    assert(ind.wordCount(w2) == 0);
    assert(ind.wordCount(w3) == 0);

    ind.insertWord(w2);
    assert(ind.wordCount(w1) == 1);
    assert(ind.wordCount(w2) == 1);
    assert(ind.wordCount(w3) == 0);

    ind.insertWord(w1);
    assert(ind.wordCount(w1) == 2);
    assert(ind.wordCount(w2) == 1);
    assert(ind.wordCount(w3) == 0);
}

int main(int argc, char **argv) {
    basicTest();
    shortTest();
    prefixTest();
    suffixTest();
    branchTest();
    countTest();
    return 0;
}
