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
#include<cstring>

Word::Word() : text(0), len(0) {

}

Word::Word(const char *utf8_word) {
    // FIXME, add iconv to 32-bit codepoints rather than just blindly copying.
    int newlen = strlen(utf8_word);
    len = newlen;
    text = new Letter[len+1];
    for(unsigned int i=0; i< len; i++) {
        text[i] = Letter(utf8_word[i]);
    }
    text[len] = 0; // Null terminated, just in case.
    if(hasWhitespace())
        throw "Tried to create a word with whitespace in it.";
}

Word::~Word() {
    delete []text;
}

Letter Word::operator[](unsigned int i) const {
    if(i >= len)
        throw "Tried to access past the end of Word array.";
    return text[i];
}

Word& Word::operator=(const Word &s) {
    if(this == &s) {
        return *this;
    }
    delete []text;
    len = s.len;
    if(len == 0) {
        text = 0;
    } else {
        text = new Letter[len+1];
        memcpy(text, s.text, (len+1)*sizeof(Letter));
    }
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
