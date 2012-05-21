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

void basicTest() {
    LevenshteinIndex ind;
    std::string word1("word");
    std::string word2("another");

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
    std::string word("a");

    assert(!ind.hasWord(word));

    ind.insertWord(word);
    assert(ind.hasWord(word));
}

void prefixTest() {
    LevenshteinIndex ind;
    std::string word("ab");
    std::string prefix("a");

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
    std::string word("abc");
    std::string word2("abcd");

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
    std::string word("abc");
    std::string word2("abcd");
    std::string word3("abce");

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

int main(int argc, char **argv) {
    basicTest();
    shortTest();
    prefixTest();
    suffixTest();
    branchTest();
    return 0;
}
