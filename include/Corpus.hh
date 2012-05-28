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

#ifndef CORPUS_HH_
#define CORPUS_HH_

#include <cstddef>

struct CorpusPrivate;
class Document;

class Corpus {
private:
    CorpusPrivate *p;
    Corpus(const Corpus &c);
    const Corpus& operator=(const Corpus &c);

public:
    Corpus();
    ~Corpus();

    void addDocument(const Document &d);
    size_t size() const;
    const Document& getDocument(size_t i);

    // Add iterators here. This class should really only expose them.
};

#endif /* CORPUS_HH_ */
