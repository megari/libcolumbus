/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "columbus.h"
#include "Word.hh"
#include "Document.hh"
#include "Matcher.hh"
#include "MatchResults.hh"
#include "Corpus.hh"
#include "ErrorValues.hh"
#include "IndexWeights.hh"
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
    }
    return 0;
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

void col_matcher_index(ColMatcher m, ColCorpus c) {
    try {
        Matcher *matcher = reinterpret_cast<Matcher*>(m);
        Corpus *corp = reinterpret_cast<Corpus*>(c);
        matcher->index(*corp);
    } catch(exception &e) {
        fprintf(stderr, "Exception when indexing: %s\n", e.what());
    }
}

void col_matcher_match(ColMatcher m, const char *query_as_utf8, ColMatchResults mr) {
    try {
        Matcher *matcher = reinterpret_cast<Matcher*>(m);
        MatchResults *results = reinterpret_cast<MatchResults*>(mr);
        matcher->match(query_as_utf8, *results);
    } catch(exception &e) {
        fprintf(stderr, "Exception when matching: %s\n", e.what());
    }
}

ColErrorValues col_matcher_get_error_values(ColMatcher m) {
    Matcher *matcher = reinterpret_cast<Matcher*>(m);
    return reinterpret_cast<ColErrorValues>(&matcher->getErrorValues());
}

ColIndexWeights col_matcher_get_index_weights(ColMatcher m) {
    Matcher *matcher = reinterpret_cast<Matcher*>(m);
    return reinterpret_cast<ColIndexWeights>(&matcher->getIndexWeights());
}

ColMatchResults col_match_results_new() {
    return reinterpret_cast<ColMatchResults>(new MatchResults());
}
void col_match_results_delete(ColMatchResults mr) {
    delete reinterpret_cast<MatchResults*>(mr);
}

size_t col_match_results_size(ColMatchResults mr) {
    return reinterpret_cast<MatchResults*>(mr)->size();
}

DocumentID col_match_results_get_id(ColMatchResults mr, size_t i) {
    try {
        MatchResults *results = reinterpret_cast<MatchResults*>(mr);
        return results->getDocumentID(i);
    } catch(exception &e) {
        fprintf(stderr, "Exception when getting result document ID: %s\n", e.what());
    }
    return INVALID_DOCID;
}

double col_match_results_get_relevancy(ColMatchResults mr, size_t i) {
    try {
        MatchResults *results = reinterpret_cast<MatchResults*>(mr);
        return results->getDocumentID(i);
    } catch(exception &e) {
        fprintf(stderr, "Exception when getting result document ID: %s\n", e.what());
    }
    return -1.0;
}

ColCorpus col_corpus_new() {
    return reinterpret_cast<ColCorpus>(new Corpus());
}
void col_corpus_delete(ColCorpus c) {
    delete reinterpret_cast<Corpus*>(c);
}

void col_corpus_add_document(ColCorpus c, ColDocument d) {
    Corpus *corp = reinterpret_cast<Corpus*>(c);
    Document *doc = reinterpret_cast<Document*>(d);
    corp->addDocument(*doc);
}

void col_error_values_add_standard_errors(ColErrorValues ev) {
    ErrorValues *results = reinterpret_cast<ErrorValues*>(ev);
    results->addStandardErrors();
}


void col_error_values_set_substring_mode(ColErrorValues ev) {
    ErrorValues *results = reinterpret_cast<ErrorValues*>(ev);
    results->setSubstringMode();
}

void col_index_weights_set_weight(ColIndexWeights weights, const ColWord field, const double new_weight) {
    IndexWeights *cweight = reinterpret_cast<IndexWeights*>(weights);
    Word *w = reinterpret_cast<Word*>(field);
    cweight->setWeight(*w, new_weight);
}

double col_index_weights_get_weight(ColIndexWeights weights, const ColWord field) {
    IndexWeights *cweight = reinterpret_cast<IndexWeights*>(weights);
    Word *w = reinterpret_cast<Word*>(field);
    return cweight->getWeight(*w);
}


#ifdef __cplusplus
}
#endif
