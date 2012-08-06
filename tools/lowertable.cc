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

#include <unicode/uchar.h>
#include <limits.h>
#include <cstdio>

void findLimits() {
    unsigned int biggest, smallest;
    biggest = 0;
    smallest = UINT_MAX;
    for(unsigned int i=0; i<UINT_MAX; i++) {
        unsigned int conv = u_tolower(i);
        if (conv != i) {
            if(i < smallest)
                smallest = i;
            if(i > biggest)
                biggest = i;
        }
    }
    printf("Smallest: %u\nBiggest: %u\n", smallest, biggest);
}

void findCrossings() {
    UChar32 i;
    for(i=0; i+1<65535; i++) {
        bool ch1 = u_tolower(i) != i ? 1 : 0;
        bool ch2 = u_tolower(i+1) != i+1 ? 1 : 0;
        if(ch1 != ch2) {
           printf("Crossing at %u.\n", i);
        }
    }
}

int main(int argc, char **argv) {
    findLimits();
    //findCrossings();
    return 0;
}
