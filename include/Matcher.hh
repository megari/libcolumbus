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

#ifndef MATCHER_HH_
#define MATCHER_HH_

#include "ColumbusCore.hh"

COL_NAMESPACE_START

class Corpus;
struct MatcherPrivate;
class Word;
class Document;
class WordList;
class MatchResults;
class ErrorValues;
class IndexWeights;
class ResultFilter;
class SearchParameters;

class COL_PUBLIC Matcher final {
private:
    MatcherPrivate *p;

    void buildIndexes(const Corpus &c);
    void addToIndex(const Word &word, const WordID wordID, const WordID indexID);
    void matchWithRelevancy(const WordList &query, const SearchParameters &params, const int extraError, MatchResults &matchedDocuments);

public:
    Matcher();
    ~Matcher();
    Matcher& operator=(const Matcher &m) = delete;

    // The simple API
    void match(const char *queryAsUtf8, MatchResults &matchedDocuments);
    void match(const WordList &query, MatchResults &matchedDocuments);

    // When you want to specify search parameters exactly.
    void match(const char *queryAsUtf8, MatchResults &matchedDocuments, const SearchParameters &params);
    void match(const WordList &query, const SearchParameters &params, MatchResults &matchedDocuments);
    void index(const Corpus &c);
    ErrorValues& getErrorValues();
    IndexWeights& getIndexWeights();
};

COL_NAMESPACE_END

#endif /* MATCHER_HH_ */
