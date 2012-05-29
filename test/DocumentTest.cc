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

#include "Document.hh"
#include "Word.hh"
#include "WordList.hh"
#include <cassert>
#include <stdexcept>

void testDoc() {
    Word docId("tester");
    Document d(docId);

    Word w1("abc");
    Word w2("def");
    Word textName("title");
    WordList *l = new WordList();
    l->addWord(w1);
    l->addWord(w2);

    d.addText(textName, *l);
    const WordList &l2 = d.getText(textName);
    assert(l2.size() == 2);
    assert(l2[0] == w1);
    assert(l2[1] == w2);

    delete l;
    const WordList &l3 = d.getText(textName);
    assert(l3.size() == 2);
    assert(l3[0] == w1);
    assert(l3[1] == w2);
}

void testIndexNames() {
    Word docId("tester");
    Document d(docId);

    Word w1("abc");
    Word w2("def");
    Word text1Name("text1");
    Word text2Name("text2");
    WordList wl1;
    WordList wl2;
    WordList textNames;

    wl1.addWord(w1);
    wl2.addWord(w2);

    d.addText(text1Name, wl1);
    d.addText(text2Name, wl2);

    d.getFieldNames(textNames);
    for(size_t i=0; i<textNames.size(); i++) {
        bool gotException;
        try {
            d.getText(textNames[i]);
            gotException = false;
        } catch(std::invalid_argument &e) {
            gotException = true;
        }
        assert(!gotException);
    }

    bool gotException;
    try {
        Word nonexisting("foooooo");
        d.getText(nonexisting);
        gotException = false;
    } catch(std::invalid_argument &e) {
        gotException = true;
    }
    assert(gotException);

}

int main(int argc, char **argv) {
    testDoc();
    testIndexNames();
    return 0;
}
