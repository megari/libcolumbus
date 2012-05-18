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

int main(int argc, char **argv) {
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
    return 0;
}
