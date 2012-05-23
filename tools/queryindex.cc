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

#include "LevenshteinIndex.hh"
#include "Word.hh"
#include "IndexMatches.hh"
#include <cstdio>
#include <stdexcept>
#include <string>
#include <cstring>

void load_data(LevenshteinIndex &ind, char *file) {
    FILE *f = fopen(file, "r");
    char buffer[1024];
    if(!f) {
        printf("Could not open file %s.\n", file);
        exit(1);
    }
    while(fgets(buffer, 1024, f) != NULL) {
        buffer[strlen(buffer)-2] = '\0'; // Chop off linefeed.
        Word s(buffer);
        ind.insertWord(s);
    }
    fclose(f);
}

void queryAndPrint(LevenshteinIndex &ind, Word &query, int maxError) {
    IndexMatches matches;
    ind.findWords(query, maxError, matches);
    if(matches.size() == 0) {
        printf("No matches.\n");
        return;
    }
    for(size_t i=0; i<matches.size(); i++) {
        const unsigned int bufSize = 1024;
        char buf[bufSize];
        matches.getMatch(i).toUtf8(buf, bufSize);
        printf("%s %d\n", buf, matches.getMatchError(i));
    }
}

int main(int argc, char **argv) {
    LevenshteinIndex ind;
    char *file;
    int maxError;
    Word *query;
    if(argc < 4) {
        printf("%s datafile queryword max_error\n", argv[0]);
        return 0;
    }
    file = argv[1];
    try {
        query = new Word(argv[2]);
    } catch(std::invalid_argument &e) {
        printf("Query term \"%s\" must not have whitespace in it.", argv[2]);
        return 1;
    }
    maxError = atoi(argv[3]);

    try {
        load_data(ind, file);
    } catch(std::invalid_argument &e) {
        printf("Data file malformed: %s\n", e.what());
        return 1;
    }

    queryAndPrint(ind, *query, maxError);
    delete query;
    return 0;
}
