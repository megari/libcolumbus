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

#ifndef MATCHER_HH_
#define MATCHER_HH_

#include "ColumbusCore.hh"

class Corpus;
struct MatcherPrivate;
class Word;
class Document;
class WordList;
class MatchResults;
class ErrorValues;
class IndexWeights;

class Matcher {
private:
    MatcherPrivate *p;

    void buildIndexes(const Corpus &c);
    void addToIndex(const Word &word, WordID wordID, const Word &indexName);
    void addToReverseIndex(const WordID wordID, const Word &indexName, const Document *d);
    void matchWithRelevancy(const WordList &query, const bool dynamicError, MatchResults &matchedDocuments);

public:
    Matcher(); // Matcher will delete[] the Corpus object.
    ~Matcher();

    void match(const WordList &query, MatchResults &matchedDocuments);
    void match(const char *queryAsUtf8, MatchResults &matchedDocuments);
    void index(const Corpus &c);
    ErrorValues& getErrorValues();
    IndexWeights& getIndexWeights();
};

#endif /* MATCHER_HH_ */
