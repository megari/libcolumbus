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

#include "Matcher.hh"
#include "Corpus.hh"
#include "Word.hh"
#include "WordList.hh"
#include "Document.hh"

void testMatcher() {
    Corpus *c = new Corpus();
    Word w1("abc");
    Word w2("def");
    Word w3("abe");
    Word w4("test");
    Word name1("doc1");
    Word name2("doc2");
    Word textName("title");

    WordList wl1, wl2;
    wl1.addWord(w1);
    wl1.addWord(w2);
    wl2.addWord(w3);
    wl2.addWord(w4);

    Document d1(name1);
    d1.addText(textName, wl1);
    Document d2(name2);
    d2.addText(textName, wl2);
    c->addDocument(d1);
    c->addDocument(d2);

    Matcher m(c);
}

int main(int argc, char **argv) {
    testMatcher();
    return 0;
}
