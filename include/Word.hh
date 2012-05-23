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

#ifndef WORD_HH_
#define WORD_HH_

#include "columbuscore.h"

/**
 * A word encapsulates a single word. That is,
 * there is no whitespace in it.
 *
 * A word's contents are immutable.
 */
class Word {
private:

    Letter *text; // Change this to a shared pointer to save memory.
    unsigned int len;
    bool hasWhitespace();
    void duplicateFrom(const Word &w);

public:
    Word();
    Word(const Word &w);
    explicit Word(const char *utf8_word);
    ~Word();

    unsigned int length() const { return len;}
    void toUtf8(char *buf, unsigned int bufSize) const;

    Letter operator[](unsigned int i) const;
    bool operator==(const Word &w) const;
    bool operator!=(const Word &w) const;
    Word& operator=(const Word &w); // Should we allow assignment? Is it the proper semantic thing to do?
};

#endif /* WORD_HH_ */
