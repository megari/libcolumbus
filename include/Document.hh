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

#ifndef DOCUMENT_HH_
#define DOCUMENT_HH_

#include <cstddef>

class Word;
class WordList;

struct DocumentPrivate;

class Document {
private:
    DocumentPrivate *p;

public:
    Document(const Word &id_);
    Document(const Document &d);
    ~Document();

    const Document& operator=(const Document &d);
    void addText(const Word &name, const WordList &words);
    const WordList& getText(const Word &name) const;
    size_t textCount() const;
    const Word& getName() const;
};

#endif /* DOCUMENT_HH_ */
