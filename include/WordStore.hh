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

#ifndef WORDSTORE_HH_
#define WORDSTORE_HH_

/*
 * We will get words multiple times. This class assigns each word a unique
 * ID. This deduplicates data and ensures we only keep one copy
 * of each word in memory.
 */

struct WordStorePrivate;

class WordStore {
private:

    WordStorePrivate *p;

public:
    WordStore();
    ~WordStore();
};

#endif /* WORDSTORE_HH_ */
