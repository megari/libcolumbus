/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
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
#include <cstring>

using namespace std;

Corpus * testCorpus() {
    Corpus *c = new Corpus();
    Word w1("abc");
    Word w2("def");
    Word w3("abe");
    Word w4("test");
    Word w5("faraway");
    Word w6("donotmatchme");
    const char *name1 = "doc1";
    const char *name2 = "doc2";
    const char *name3 = "distantdoc";
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
    Matcher m;
    MatchResults matches;
    WordList queryList;
    Word w1("abc");
    const char *dFarName = "distantdoc";
    const char *name1("doc1");

    m.index(*c);
    delete(c);

    queryList.addWord(w1);
    m.match(queryList, matches);
    assert(matches.size() == 2);
    assert(strcmp(matches.getDocumentID(0), dFarName) != 0);
    assert(strcmp(matches.getDocumentID(1), dFarName) != 0);
    assert(strcmp(matches.getDocumentID(0), name1) == 0 ||
           strcmp(matches.getDocumentID(1),name1) == 0);
}

void testRelevancy() {
    Corpus *c = testCorpus();
    Matcher m;
    MatchResults matches;
    WordList queryList;
    Word w1("abc");
    Word dFarName("distantdoc");
    const char *name1 = "doc1";

    m.index(*c);
    delete c;

    queryList.addWord(w1);
    m.match(queryList, matches);
    assert(matches.size() == 2);
    // Document doc1 has an exact match, so it should be the best match.
    assert(matches.getRelevancy(0) > matches.getRelevancy(1));
    assert(strcmp(matches.getDocumentID(0), name1) == 0);
}

void testMultiWord() {
    Corpus c;
    const char *correct = "correct";
    const char *wrong = "wrong";
    Document d1(correct);
    Document d2(wrong);
    Word fieldName("name");
    Matcher m;
    MatchResults matches;

    d1.addText(fieldName, "Sarah Michelle Gellar");
    d2.addText(fieldName, "Sara Giller");

    c.addDocument(d1);
    c.addDocument(d2);
    m.index(c);

    m.match("Sara Michell Geller", matches);
    assert(strcmp(matches.getDocumentID(0), correct) == 0);
}

int main(int argc, char **argv) {
    testMatcher();
    testRelevancy();
    testMultiWord();
    return 0;
}
