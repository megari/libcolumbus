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

#include "WordList.hh"
#include "Word.hh"
#include <cassert>
#include <stdexcept>

void testList() {
    WordList l;
    bool gotException;
    Word w1("abc");
    Word w2("def");
    Word *w2Shadow = new Word(w2);

    assert(l.size() == 0);
    try {
        Word wTmp = l[1];
        gotException = false;
    } catch(std::out_of_range &e) {
        gotException = true;
    }

    assert(gotException);

    l.addWord(w1);
    assert(l.size() == 1);
    assert(l[0] == w1);

    l.addWord(*w2Shadow);
    delete w2Shadow;
    assert(l.size() == 2);
    assert(l[1] == w2);
}

int main(int argc, char **argv) {
    testList();
    return 0;
}
