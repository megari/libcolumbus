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

#include "columbus.h"
#include <assert.h>
#include <string.h>

void testWord() {
    ColWord w = col_word_new("abc");
    assert(w);
    assert(col_word_length(w) == 3);
    assert(strcmp("abc", col_word_as_utf8(w)) == 0);
    col_word_delete(w);
}

void testDocument() {
    ColDocument d = col_document_new(55);
    ColWord w = col_word_new("abc");

    assert(d);
    assert(col_document_get_id(d) == 55);

    col_document_add_text(d, w, "this is just some text");

    col_word_delete(w);
    col_document_delete(d);
}

void testMatcher() {
    ColMatcher m = col_matcher_new();
    assert(m);
    col_matcher_delete(m);
}

void testMatchResults() {
    ColMatchResults mr = col_match_results_new();
    assert(mr);
    col_match_results_delete(mr);
}

int main(int argc, char **argv) {
    testWord();
    testDocument();
    testMatcher();
    testMatchResults();
    return 0;
}
