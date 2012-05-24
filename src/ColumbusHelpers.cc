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

/*
 * This file contains various library helper functions.
 */

#include "columbuscore.h"
#include "ColumbusHelpers.hh"
#include <iconv.h>
#include <cstdio>
#include <cerrno>
#include <cassert>
#include <stdexcept>
#include <cstring>

Letter* utf8ToInternal(const char *utf8Text, unsigned int &resultStringSize) {
    iconv_t ic = iconv_open(INTERNAL_ENCODING, "UTF-8");
    char *tmp;
    char *txt;
    char *inBuf;
    char *outBuf;
    size_t badConvertedCharacters;
    size_t inBytes, outBytes, outBytesOriginal;
    size_t bytesWritten;
    if (ic == (iconv_t)-1) {
        throw std::runtime_error("Could not create iconv converter.");
    }

    unsigned int inputLen = strlen((const char*)(utf8Text));
    txt = new char[(inputLen+1)*sizeof(Letter)];
    tmp = strdup((const char*)(utf8Text)); // Iconv should take a const pointer but does not. Protect against it screwing up.
    assert(tmp);
    inBytes = inputLen;
    outBytes = sizeof(Letter)*(inBytes+1);
    outBytesOriginal = outBytes;

    inBuf = tmp;
    outBuf = txt;
    badConvertedCharacters = iconv(ic, &inBuf, &inBytes, &outBuf, &outBytes);
    free(tmp);
    iconv_close(ic);
    if(badConvertedCharacters == (size_t)-1) {
        std::string err("Could not convert UTF8-string to internal representation: ");
        err += (const char*)(utf8Text);
        throw std::runtime_error(err);
    }
    bytesWritten = outBytesOriginal - outBytes;
    resultStringSize = bytesWritten/sizeof(Letter);
    if(bytesWritten < inputLen) {
        // Shrink allocated memory size to exactly the produced string.
        char *newtxt = new char[(bytesWritten + 1)*sizeof(Letter)];
        memcpy(newtxt, txt, bytesWritten*sizeof(Letter));
        delete []txt;
        txt = newtxt;
    }
    Letter* text = reinterpret_cast<Letter*>(txt);
    text[resultStringSize] = 0; // Null terminated, just in case.
    return text;
}

void internalToUtf8(const Letter* source, unsigned int characters, char *buf, unsigned int bufsize) {
    iconv_t ic = iconv_open("UTF-8", INTERNAL_ENCODING);
    char *inBuf = reinterpret_cast<char*>(const_cast<Letter*>(source));
    char *outBuf;
    size_t badConvertedCharacters;
    size_t inBytes, outBytes, outBytesOriginal, resultStringSize;
    if (ic == (iconv_t)-1) {
        throw std::runtime_error("Could not create iconv converter.");
    }

    inBytes = characters*sizeof(Letter);
    outBytes = bufsize;
    outBytesOriginal = outBytes;

    outBuf = buf;
    badConvertedCharacters = iconv(ic, &inBuf, &inBytes, &outBuf, &outBytes);
    iconv_close(ic);
    if(badConvertedCharacters == (size_t)-1) {
        throw std::runtime_error("Could not convert internal string to UTF-8.");
    }
    resultStringSize = outBytesOriginal - outBytes;
    buf[resultStringSize] = 0; // Null terminated, just in case.
}
