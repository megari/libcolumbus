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

#include "Document.hh"
#include "Word.hh"
#include "WordList.hh"
#include <map>
#include <stdexcept>

using namespace std;

struct DocumentPrivate {
    Word id;
    map<Word, WordList> texts;
};

typedef map<Word, WordList>::iterator TextIter;

Document::Document(const Word &id_) {
    p = new DocumentPrivate();
    p->id = id_;
}

Document::Document(const Document& d) {
    p = new DocumentPrivate();
    p->id = d.p->id;
    p->texts = d.p->texts;
}

Document::~Document() {
    delete p;
}

void Document::addText(const Word &field, const WordList &words) {
    p->texts[field] = words;
}

const WordList& Document::getText(const Word &field) const {
    TextIter res = p->texts.find(field);
    if(res == p->texts.end()) {
        throw invalid_argument("Tried to access nonexisting text field in Document.");
    }
    return res->second;
}

size_t Document::fieldCount() const {
    return p->texts.size();
}

const Word& Document::getID() const {
    return p->id;
}

void Document::getFieldNames(WordList &list) const {
    for(TextIter it=p->texts.begin(); it != p->texts.end(); it++) {
        list.addWord(it->first);
    }
}

const Document& Document::operator=(const Document&d) {
    if(this == &d)
        return *this;
    p->id = d.p->id;
    p->texts = d.p->texts;
    return *this;
}
