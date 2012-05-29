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
    void addText(const Word &field, const WordList &words);
    const WordList& getText(const Word &field) const;
    size_t fieldCount() const;
    const Word& getID() const;
    void getFieldNames(WordList &list) const;

    // Needs iterators to browse through text names.
};

#endif /* DOCUMENT_HH_ */