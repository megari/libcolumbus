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

#include "ColumbusHelpers.hh"
#include "Word.hh"
#include "WordList.hh"
#include <cassert>

bool splitCorrectly(const char *txt, const WordList &l) {
    WordList result;
    splitToWords(txt, result);
    return result == l;
}

void testSplitter() {
    Word w1("abc");
    Word w2("def");
    WordList l1;
    l1.addWord(w1);
    l1.addWord(w2);

    assert(splitCorrectly("abc def", l1));
    assert(splitCorrectly("abc\tdef", l1));
    assert(splitCorrectly("abc\ndef", l1));
    assert(splitCorrectly("abc\rdef", l1));
    assert(splitCorrectly(" abc def", l1));
    assert(splitCorrectly("abc def ", l1));
    assert(splitCorrectly(" abc def ", l1));

    WordList empty;
    assert(splitCorrectly("", empty));
    assert(splitCorrectly(" ", empty));
    assert(splitCorrectly("\t", empty));
    assert(splitCorrectly("\n", empty));
    assert(splitCorrectly("\r", empty));
    assert(splitCorrectly(" \t\n\r\n\t ", empty));
}

int main(int argc, char **argv) {
    testSplitter();
    return 0;
}
