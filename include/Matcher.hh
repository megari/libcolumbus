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

class Corpus;
struct MatcherPrivate;
class Word;
class Document;

class Matcher {
private:
    MatcherPrivate *p;

    void buildIndexes();
    void addToIndex(const Word &word, const Word &indexName);
    void addToReverseIndex(const Word &word, const Document *d);

public:
    Matcher(Corpus *corp); // Matcher will delete[] the Corpus object.
    ~Matcher();
};

#endif /* MATCHER_HH_ */
