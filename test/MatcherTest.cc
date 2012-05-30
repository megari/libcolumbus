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
#include "MatchResults.hh"
#include <cassert>

using namespace std;

Corpus * testCorpus() {
    Corpus *c = new Corpus();
    Word w1("abc");
    Word w2("def");
    Word w3("abe");
    Word w4("test");
    Word w5("faraway");
    Word w6("donotmatchme");
    Word name1("doc1");
    Word name2("doc2");
    Word name3("distantdoc");
    Word textName("title");

    WordList wl1, wl2, wlFar;
    wl1.addWord(w1);
    wl1.addWord(w2);
    wl2.addWord(w3);
    wl2.addWord(w4);
    wlFar.addWord(w5);
    wlFar.addWord(w6);

    Document d1(name1);
    d1.addText(textName, wl1);
    Document d2(name2);
    d2.addText(textName, wl2);
    Document dFar(name3);
    dFar.addText(textName, wlFar);
    c->addDocument(d1);
    c->addDocument(d2);
    c->addDocument(dFar);

    return c;
}

void testMatcher() {
    Corpus *c = testCorpus();
    Matcher m(c);
    MatchResults matches;
    WordList queryList;
    Word w1("abc");
    Word dFarName("distantdoc");
    Word name1("doc1");

    queryList.addWord(w1);
    m.match(queryList, matches);
    assert(matches.size() == 2);
    assert(matches.getDocumentID(0) != dFarName);
    assert(matches.getDocumentID(1) != dFarName);
    assert(matches.getDocumentID(0) == name1 || matches.getDocumentID(1) == name1);
}

int main(int argc, char **argv) {
    testMatcher();
    return 0;
}
