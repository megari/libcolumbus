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

using namespace Columbus;
using namespace std;

void testFiltering() {
    Word textField("text");
    const char *txt = "something";
    Word filterField1("field1");
    const char *val1str = "one";
    const char *val2str = "two";
    Word val1(val1str);
    Word val2(val2str);
    Document d1(1);
    Document d2(2);
    Corpus c;
    Matcher m;
    ResultFilter emtpyFilter;
    ResultFilter onlyTakeOne;

    d1.addText(textField, txt);
    d1.addText(filterField1, val1str);
    c.addDocument(d1);
    d2.addText(textField, txt);
    d2.addText(filterField1, val2str);
    c.addDocument(d2);

    m.index(c);
    MatchResults r1;
    m.match(txt, r1, emtpyFilter);
    assert(r1.size() == 2);

    onlyTakeOne.addNewSubTerm(filterField1, val1);
    MatchResults r2;
    m.match(txt, r2, onlyTakeOne);
    assert(r1.size() == 1);
    assert(r1.getDocumentID(0) == 1);
}

int main(int argc, char **argv) {
    testFiltering();
    return 0;
}

