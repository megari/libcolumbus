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

#ifndef CORPUS_HH_
#define CORPUS_HH_

#include "ColumbusCore.hh"

COL_NAMESPACE_START

struct CorpusPrivate;
class Document;

class COL_PUBLIC Corpus {
private:
    CorpusPrivate *p;
    Corpus(const Corpus &c);
    const Corpus& operator=(const Corpus &c);

public:
    Corpus();
    ~Corpus();

    void addDocument(const Document &d);
    size_t size() const;
    const Document& getDocument(size_t i) const;

    // Add iterators here. This class should really only expose them.
};

COL_NAMESPACE_END

#endif /* CORPUS_HH_ */
