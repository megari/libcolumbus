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
 * Tests the custom word class. Valgrind strongly recommended.
 */

#include <cassert>
#include "Word.hh"

void testEmpty() {
    Word w1;
    assert(w1.length() == 0);
}

void testIndexing() {
    Word w("abc");
    bool gotException = false;

    assert(w[0] == 'a');
    assert(w[1] == 'b');
    assert(w[2] == 'c');

    try {
        w[3];
    } catch(const char *str) {
        gotException = true;
    }
    assert(gotException);

}

void shouldThrow(const char *str) {
    bool gotException;
    try {
        Word w(str);
        gotException = false;
    } catch(const char *s) {
        gotException = true;
    }
    assert(gotException);
}

void testWhitespace() {
    shouldThrow(" ");
    shouldThrow(" a");
    shouldThrow("a ");
    shouldThrow("a a");

    shouldThrow("\t");
    shouldThrow("a\t");
    shouldThrow("\ta");
    shouldThrow("a\ta");

    shouldThrow("\n");
    shouldThrow("a\n");
    shouldThrow("\na");
    shouldThrow("a\na");
}

int main(int argc, char **argv) {
    testEmpty();
    testIndexing();
    testWhitespace();
}

