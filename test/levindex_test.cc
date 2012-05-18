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

#include <cassert>
#include "LevenshteinIndex.hh"

void basic_test() {
    LevenshteinIndex ind;
    std::string word1("word");
    std::string word2("another");

    assert(!ind.has_word(word1));
    assert(!ind.has_word(word2));

    ind.insert_word(word1);
    assert(ind.has_word(word1));
    assert(!ind.has_word(word2));

    ind.insert_word(word2);
    assert(ind.has_word(word1));
    assert(ind.has_word(word2));
}

void short_test() {
    LevenshteinIndex ind;
    std::string word("a");

    assert(!ind.has_word(word));

    ind.insert_word(word);
    assert(ind.has_word(word));
}

void prefix_test() {
    LevenshteinIndex ind;
    std::string word("ab");
    std::string prefix("a");

    assert(!ind.has_word(word));
    assert(!ind.has_word(prefix));

    ind.insert_word(word);
    assert(ind.has_word(word));
    assert(!ind.has_word(prefix));

    ind.insert_word(prefix);
    assert(ind.has_word(word));
    assert(ind.has_word(prefix));
}

void suffix_test() {
    LevenshteinIndex ind;
    std::string word("abc");
    std::string word2("abcd");

    assert(!ind.has_word(word));
    assert(!ind.has_word(word2));

    ind.insert_word(word);
    assert(ind.has_word(word));
    assert(!ind.has_word(word2));

    ind.insert_word(word2);
    assert(ind.has_word(word));
    assert(ind.has_word(word2));
}

void branch_test() {
    LevenshteinIndex ind;
    std::string word("abc");
    std::string word2("abcd");
    std::string word3("abce");

    assert(!ind.has_word(word));
    assert(!ind.has_word(word2));
    assert(!ind.has_word(word3));

    ind.insert_word(word);
    assert(ind.has_word(word));
    assert(!ind.has_word(word2));
    assert(!ind.has_word(word3));

    ind.insert_word(word2);
    assert(ind.has_word(word));
    assert(ind.has_word(word2));
    assert(!ind.has_word(word3));

    ind.insert_word(word3);
    assert(ind.has_word(word));
    assert(ind.has_word(word2));
    assert(ind.has_word(word3));
}

int main(int argc, char **argv) {
    basic_test();
    short_test();
    prefix_test();
    suffix_test();
    branch_test();
    return 0;
}
