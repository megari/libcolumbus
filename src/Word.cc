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

#include "Word.hh"
#include <cstring>
#include <stdexcept>
#include <cassert>
#include "ColumbusHelpers.hh"

Word::Word() : text(0), len(0) {

}

Word::Word(const char *utf8Word) {
    text = utf8ToInternal(utf8Word, len);
    if(hasWhitespace()) {
        delete []text;
        std::string err("Tried to create a word with whitespace in it: ");
        err += (const char*)utf8Word;
        throw std::invalid_argument(err);
    }
}
Word::Word(const Word &w) : text(0), len(0) {
    duplicateFrom(w);
}

Word::~Word() {
    delete []text;
}

void Word::duplicateFrom(const Word &w) {
    if(this == &w) {
        return;
    }
    delete []text;
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

/**
 * A word is not supposed to have any whitespace in it. Verify that we don't.
 */
bool Word::hasWhitespace() {
    Letter space = ' ';
    Letter tab = '\t';
    Letter linefeed = '\n';
    for(unsigned int i=0; i<len; i++) {
        Letter cur = text[i];
        if(cur == space || cur == tab || cur == linefeed)
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

bool Word::operator!=(const Word &w) const {
    return !(*this == w);
}

void Word::toUtf8(char *buf, unsigned int bufSize) const {
    // Again, fix to use iconv.
    unsigned int count = std::min(bufSize-1, len);

    for(unsigned int i=0; i<count; i++) {
        buf[i] = char(text[i]);
    }
    buf[count] = 0;
}
