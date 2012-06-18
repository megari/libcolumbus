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
 * This file tests the performance using the given wordlist.
 * It does not check search results, only that the program
 * does not crash. Run under Valgrind to check for memory leaks.
 */


#include <vector>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "LevenshteinIndex.hh"
#include "Word.hh"
#include "ColumbusHelpers.hh"
#include "ErrorValues.hh"
#include "WordStore.hh"

using namespace std;

void readData(vector<Word> &a, const char *ifilename) {
    FILE *f = fopen(ifilename, "r");
    char buffer[1024];
    if(!f) {
        printf("Could not open dictionary file. Skipping performance test.\n");
        exit(0);
    }
    while(fgets(buffer, 1024, f) != NULL) {
        unsigned int slen = strlen(buffer);
        assert(buffer[slen-1] == '\n');
        buffer[slen-1] = '\0'; // Chop the \n.
        Word s(buffer);
        a.push_back(s);
    }
    fclose(f);
}

/*
 * Separate function so it stands out in Callgrind.
 */

void runQueries(int query_size, const int defaultError, vector<Word> &a, ErrorValues &e, LevenshteinIndex &ind, IndexMatches &matches) {
    for(size_t i=0; i < (size_t)query_size; i++) {
        ind.findWords(a[i], e, 2*defaultError, matches);
        matches.clear();
    }
}

void runTest(vector<Word> &a, int querySize) {
    double buildStart, buildEnd, queryStart, queryEnd;
    double plainQueryTime, fullErrorQueryTime, buildTime;
    LevenshteinIndex ind;
    WordStore store;
    IndexMatches matches;
    ErrorValues e;
    const int defaultError = LevenshteinIndex::getDefaultError();

    buildStart = hiresTimestamp();
    for(size_t i=0; i < a.size(); i++)
        ind.insertWord(a[i], store.getID(a[i]));
    buildEnd = hiresTimestamp();

    queryStart = hiresTimestamp();
    runQueries(querySize, defaultError, a, e, ind, matches);
    queryEnd = hiresTimestamp();
    buildTime = buildEnd - buildStart;
    plainQueryTime = queryEnd - queryStart;

    e.addLatinAccents();
    queryStart = hiresTimestamp();
    runQueries(querySize, defaultError, a, e, ind, matches);
    queryEnd = hiresTimestamp();
    fullErrorQueryTime = queryEnd - queryStart;
    printf("Index built in %f seconds. Words per second %.2f.\n", buildTime, a.size()/buildTime);
    printf("Simple queries done in %f seconds. Queries per second %.2f.\n", plainQueryTime, querySize/plainQueryTime);
    printf("Heavy queries done in %f seconds. Queries per second %.2f.\n", fullErrorQueryTime, querySize/fullErrorQueryTime);
}

int main(int argc, char **argv) {
    vector<Word> a;
    int querySize;
    const char *ifile;

    if(argc == 1) {
        printf("%s input_file_name.txt <num_of_queries>\n", argv[0]);
        return 1;
    }
    ifile = argv[1];
    readData(a, ifile);
    printf("Read in %ld words.\n", a.size());
    if(argc > 2)
        querySize = atoi(argv[2]);
    else
        querySize = a.size();
    printf("Querying %d elements.\n", querySize);
    runTest(a, querySize);
    return 0;
}
