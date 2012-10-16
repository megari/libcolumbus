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

#include "Word.hh"
#include <cstring>
#include <stdexcept>
#include <cassert>
#include <cstring>
#include "ColumbusHelpers.hh"


COL_NAMESPACE_START

Word::Word() : text(0), len(0), utf8Repr(0) {

}

Word::Word(const char *utf8Word) : text(0), len(0), utf8Repr(0) {
    convertString(utf8Word);
}

Word::Word(const Word &w) : text(0), len(0), utf8Repr(0) {
    duplicateFrom(w);
}

Word::Word(Word &&w) :
    text(w.text),
    len(w.len),
    utf8Repr(w.utf8Repr) {
    w.len = 0;
    w.text = 0;
    w.utf8Repr = 0;
}

Word::~Word() {
    delete []text;
    delete []utf8Repr;
}

void Word::convertString(const char *utf8Word) {
    text = utf8ToInternal(utf8Word, len);
    if(hasWhitespace()) {
        delete []text;
        text = nullptr;
        std::string err("Tried to create a word with whitespace in it: ");
        err += (const char*)utf8Word;
        throw std::invalid_argument(err);
    }
}

void Word::duplicateFrom(const Word &w) {
    if(this == &w) {
        return;
    }
    delete []text;
    delete []utf8Repr;
    utf8Repr = 0;
    len = w.len;
    if(len == 0) {
        text = 0;
    } else {
        text = new Letter[len+1];
        memcpy(text, w.text, (len+1)*sizeof(Letter));
    }
}

Letter Word::operator[](unsigned int i) const {
    if(i >= len) {
        std::string msg("Tried to access letter ");
        msg += i;
        msg += " in a word of size ";
        msg += len;
        msg += ".";
        throw std::out_of_range(msg);
    }
    return text[i];
}

Word& Word::operator=(const Word &w) {
    duplicateFrom(w);
    return *this;
}

Word& Word::operator=(Word &&w) {
    delete []text;
    delete []utf8Repr;

    text = w.text;
    len = w.len;
    utf8Repr = w.utf8Repr;

    w.text = 0;
    w.len = 0;
    w.utf8Repr = 0;
    return *this;
}

/**
 * A word is not supposed to have any whitespace in it. Verify that we don't.
 */
bool Word::hasWhitespace() {
    for(unsigned int i=0; i<len; i++) {
        Letter cur = text[i];
        if(isWhitespace(cur))
            return true;
    }
    return false;
}

bool Word::operator==(const Word &w) const {
    if(this == &w)
        return true;
    if(len != w.len)
        return false;
    for(unsigned int i=0; i<len; i++)
        if(text[i] != w.text[i])
            return false;
    return true;
}

bool Word::operator==(const char *utf8Word) const {
    return strcmp(asUtf8(), utf8Word) == 0;
}


bool Word::operator!=(const Word &w) const {
    return !(*this == w);
}

bool Word::operator!=(const char *utf8Word) const {
    return !(*this == utf8Word);
}

bool Word::operator<(const Word &w) const {
    if(this == &w)
        return false;
    size_t minLen = len < w.len ? len : w.len;
    for(size_t i=0; i<minLen; i++) {
        if(text[i] < w.text[i])
            return true;
        if(text[i] > w.text[i])
            return false;
    }
    if(w.len > len)
        return true;
    return false;
}

void Word::toUtf8(char *buf, unsigned int bufSize) const {
    internalToUtf8(text, len, buf, bufSize);
}

const char* Word::asUtf8() const {
    if(utf8Repr)
        return utf8Repr;
    size_t strSize = 4*(len+1); // One codepoint is max 4 bytes in UTF-8.
    char *u8 = new char[strSize];
    toUtf8(u8, strSize);
    const_cast<Word*>(this)->utf8Repr = u8;
    return utf8Repr;
}

Word Word::join(const Word &w) const {
    Word result;
    size_t newLen = length() + w.length();
    result.len = newLen;
    result.text = new Letter[newLen+1];
    memcpy(result.text, text, len*sizeof(Letter));
    memcpy(result.text + len, w.text, w.len*sizeof(Letter));
    result.text[newLen] = '\0';
    return result;
}

Word& Word::operator=(const char *utf8Word) {
    delete []text;
    delete []utf8Repr;
    text = nullptr;
    utf8Repr = nullptr;
    len = 0;
    convertString(utf8Word);
    return *this;
}

COL_NAMESPACE_END
