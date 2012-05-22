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
#include <sys/time.h>
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

void runTest(vector<string> &a, int query_size, struct timeval *build_start, struct timeval *build_end, struct timeval *query_start, struct timeval *query_end) {
    LevenshteinIndex ind;
    IndexMatches matches;
    const int defaultError = LevenshteinIndex::getDefaultError();

    gettimeofday(build_start, NULL);
    for(size_t i=0; i < a.size(); i++)
        ind.insertWord(a[i]);
    gettimeofday(build_end, NULL);

    gettimeofday(query_start, NULL);
    for(size_t i=0; i < (size_t)query_size; i++) {
        ind.findWords(a[i], 2*defaultError, matches);
        matches.clear();
    }
    gettimeofday(query_end, NULL);
}

int main(int argc, char **argv) {
    struct timeval build_start, build_end, query_start, query_end;
    double bs, be, qs, qe;
    double query_time, build_time;
    vector<string> a;
    int query_size;
    readData(a);
    printf("Read in %ld words.\n", a.size());

    if(argc > 1)
        query_size = atoi(argv[1]);
    else
        query_size = a.size();
    printf("Querying %d elements.\n", query_size);
    runTest(a, query_size, &build_start, &build_end, &query_start, &query_end);
    bs = build_start.tv_sec + build_start.tv_usec/1000000.0;
    be = build_end.tv_sec + build_end.tv_usec/1000000.0;
    qs = query_start.tv_sec + query_start.tv_usec/1000000.0;
    qe = query_end.tv_sec + query_end.tv_usec/1000000.0;
    build_time = be - bs;
    query_time = qe - qs;
    printf("Index built in %f seconds. Words per second %.2f.\n", build_time, a.size()/build_time);
    printf("Queries done in %f seconds. Queries per second %.2f.\n", query_time, query_size/query_time);
    return 0;
}
