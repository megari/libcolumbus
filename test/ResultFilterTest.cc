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

#include "ResultFilter.hh"
#include "Word.hh"
#include "Document.hh"
#include "Corpus.hh"
#include "MatchResults.hh"
#include "Matcher.hh"
#include <cassert>
#include <cstdio>

using namespace Columbus;
using namespace std;

void testFiltering() {
    Word textField("text");
    const char *txt = "something";
    Word filterField1("field1");
    Word filterField2("field2");
    const char *val1str = "one";
    const char *val2str = "two";
    const char *val3str = "three";
    Word val1(val1str);
    Word val2(val2str);
    Word val3(val3str);
    Document d1(1);
    Document d2(2);
    Corpus c;
    Matcher m;
    ResultFilter emptyFilter;
    ResultFilter onlyTakeFirst, onlyTakeSecond, orTest, andTest;

    d1.addText(textField, txt);
    d1.addText(filterField1, val1str);
    d1.addText(filterField2, val3str);
    c.addDocument(d1);
    d2.addText(textField, txt);
    d2.addText(filterField1, val2str);
    d2.addText(filterField2, val3str);
    c.addDocument(d2);

    m.index(c);
    MatchResults r1;
    m.match(txt, r1, emptyFilter);
    assert(r1.size() == 2);

    onlyTakeFirst.addNewSubTerm(filterField1, val1);
    MatchResults r2;
    m.match(txt, r2, onlyTakeFirst);
    assert(r2.size() == 1);
    assert(r2.getDocumentID(0) == 1);

    onlyTakeSecond.addNewSubTerm(filterField1, val2);
    MatchResults r3;
    m.match(txt, r3, onlyTakeSecond);
    assert(r3.size() == 1);
    assert(r3.getDocumentID(0) == 2);

    orTest.addNewSubTerm(filterField1, val1);
    orTest.addNewTerm();
    orTest.addNewSubTerm(filterField1, val2);
    MatchResults orResults;
    m.match(txt, orResults, orTest);
    assert(orResults.size() == 2);

    andTest.addNewSubTerm(filterField2, val2);
    andTest.addNewSubTerm(filterField1, val1);
    MatchResults andResults;
    m.match(txt, andResults, andTest);
    assert(andResults.size() == 0);
}

int main(int argc, char **argv) {
    testFiltering();
    return 0;
}

