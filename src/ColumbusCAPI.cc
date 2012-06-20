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
#include "Word.hh"
#include "Document.hh"
#include "Matcher.hh"
#include <stdexcept>
#include <cstdio>

using namespace Columbus;
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

ColWord col_word_new(const char *utf8_word) {
    try {
        Word *w = new Word(utf8_word);
        return reinterpret_cast<ColWord>(w);
    } catch(exception &e) {
        fprintf(stderr, "Error creating Word: %s\n", e.what());
        return 0;
    }
}

void col_word_delete(ColWord w) {
    delete reinterpret_cast<Word*>(w);
}

size_t col_word_length(ColWord w) {
    return reinterpret_cast<Word*>(w)->length();
}

const char* col_word_as_utf8(ColWord w) {
    return reinterpret_cast<Word*>(w)->asUtf8();
}

ColDocument col_document_new(DocumentID id) {
    return reinterpret_cast<ColDocument>(new Document(id));
}
void col_document_delete(ColDocument doc) {
    delete reinterpret_cast<Document*>(doc);
}

DocumentID col_document_get_id(ColDocument doc) {
    return reinterpret_cast<Document*>(doc)->getID();
}
void col_document_add_text(ColDocument doc, ColWord field_name, const char *text_as_utf8) {
    Document *d = reinterpret_cast<Document*>(doc);
    Word *w = reinterpret_cast<Word*>(field_name);
    d->addText(*w, text_as_utf8);
}

ColMatcher col_matcher_new() {
    return reinterpret_cast<ColMatcher>(new Matcher());
}

void col_matcher_delete(ColMatcher m) {
    delete reinterpret_cast<Matcher*>(m);
}

#ifdef __cplusplus
}
#endif
