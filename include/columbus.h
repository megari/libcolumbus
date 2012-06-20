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

/*
 * This file defines the C API of Columbus. If you can, it is strongly
 * recommended to use the C++ API of columbus.hh instead.
 */

#ifndef COLUMBUS_H_
#define COLUMBUS_H_

#ifdef COLUMBUS_HH_
#error "Mixing C and C++ public header includes. You can only use one or the other."
#endif

#include "ColumbusCore.hh"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* ColWord;
typedef void* ColDocument;
typedef void* ColMatcher;
typedef void* ColMatchResults;

ColWord col_word_new(const char *utf8_word);
void col_word_delete(ColWord w);
size_t col_word_length(ColWord w);
const char* col_word_as_utf8(ColWord w);

ColDocument col_document_new(DocumentID id);
void col_document_delete(ColDocument doc);
DocumentID col_document_get_id(ColDocument doc);
void col_document_add_text(ColDocument doc, ColWord field_name, const char *text_as_utf8);

ColMatcher col_matcher_new();
void col_matcher_delete(ColMatcher m);

ColMatchResults col_match_results_new();
void col_match_results_delete(ColMatchResults mr);

#ifdef __cplusplus
}
#endif

#endif
