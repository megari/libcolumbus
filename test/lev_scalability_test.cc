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
 * This file tests the performance using the common wordlist.
 * It does not check search results, only that the program
 * does not crash. Run under Valgrind to check for memory leaks.
 */


#include <vector>
#include <cstdio>
#include <cstring>
#include "LevenshteinIndex.hh"

using namespace std;

void readData(vector<string> &a) {
    FILE *f = fopen("/usr/share/dict/words", "r");
    char buffer[1024];
    if(!f) {
        printf("Could not open dictionary file. Skipping performance test.\n");
        exit(0);
    }
    while(fgets(buffer, 1024, f) != NULL) {
        buffer[strlen(buffer)-2] = '\0';
        string s(buffer);
        a.push_back(s);
    }
    fclose(f);
}

void runTest(vector<string> &a) {
    LevenshteinIndex ind;
    IndexMatches matches;
    const int defaultError = LevenshteinIndex::getDefaultError();

    for(size_t i=0; i < a.size(); i++)
        ind.insertWord(a[i]);

    for(size_t i=0; i < a.size(); i++) {
        ind.findWords(a[i], 2*defaultError, matches);
        matches.clear();
    }
}

int main(int argc, char **argv) {
    vector<string> a;
    readData(a);
    printf("Read in %ld words.\n", a.size());

    runTest(a);
    return 0;
}
