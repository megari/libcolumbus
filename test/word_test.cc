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

#include <cstring>
#include <cassert>
#include <stdexcept>
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
    } catch(std::out_of_range &e) {
        gotException = true;
    }
    assert(gotException);

}

void shouldThrow(const char *str) {
    bool gotException;
    try {
        Word w(str);
        gotException = false;
    } catch(std::invalid_argument &e) {
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

void testCreation() {
    Word w1("abc");
    Word w2(w1);
    Word empty1;
    Word empty2(empty1);

    assert(empty1.length() == 0);
    assert(empty2.length() == 0);

    assert(w2.length() == 3);
    assert(w2[0] == 'a');
    assert(w2[1] == 'b');
    assert(w2[2] == 'c');

    Word *w3 = new Word(w1);
    assert(w3->length() == 3);
    assert((*w3)[0] == 'a');
    assert((*w3)[1] == 'b');
    assert((*w3)[2] == 'c');
    delete w3;

    // Check that w1 did not get destroyed along with w3.
    assert(w1.length() == 3);
    assert(w1[0] == 'a');
    assert(w1[1] == 'b');
    assert(w1[2] == 'c');
}

void testComparison() {
    Word w1a;
    Word w1b;
    Word w2a("abc");
    Word w2b("abc");
    Word w2c("abd");
    Word different("different");

    assert(w1a == w1a);
    assert(w2a == w2a);
    assert(w2b == w2b);

    assert(!(w1a != w1b));
    assert(!(w2a != w2a));
    assert(!(w2b != w2b));

    assert(w1a != w2a);
    assert(w2a != w2c);
    assert(w1a != different);
    assert(w2a != different);

    assert(!(w1a == w2a));
    assert(!(w2a == w2c));
    assert(!(w1a == different));
    assert(!(w2a == different));
}

void testEncoding() {
    const unsigned char txt[5] = {0x61, 0xc3, 0xa4, 0x63, 0}; // "aäc" in UTF-8.
    char *text = (char*)txt;
    char returned[4];
    Word w1(text);

    assert(w1.length() == 3);
    assert(w1[0] == 'a');
    assert(w1[1] == 0xe4);
    assert(w1[2] == 'c');

    w1.toUtf8(returned, 4);
    assert(strcmp(text, returned) == 0);
}

int main(int argc, char **argv) {
    testEmpty();
    testIndexing();
    testWhitespace();
    testCreation();
    testComparison();
    testEncoding();
}

