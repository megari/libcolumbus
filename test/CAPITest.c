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

void testWord() {
    ColWord w = col_word_new("abc");
    assert(w);
    col_word_delete(w);
}

void testDocument() {
    ColDocument d = col_document_new(0);
    assert(d);
    col_document_delete(d);
}

int main(int argc, char **argv) {
    testWord();
    testDocument();
    return 0;
}
