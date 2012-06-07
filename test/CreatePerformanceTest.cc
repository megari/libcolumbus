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
 * This exe is meant to be used to measure speed and
 * memory consumption of Index building.
 */

#include "columbus.h" // This app should only need public API from Columbus.
#include <fstream>
#include <string>
#include <climits>

using namespace std;

Matcher* build_matcher(const char *dataFile, int maxLines) {
    Matcher *m;
    Corpus *c = new Corpus();
    Word field("name");
    double dataReadStart, dataReadEnd, buildEnd;
    int i = 0;

    ifstream ifile(dataFile);
    if(ifile.fail()) {
        printf("Could not open file %s.\n", dataFile);
        exit(1);
    }
    string line;

    // Build Corpus.
    dataReadStart = hiresTimestamp();
    while(getline(ifile, line)) {
        if(line.size() == 0)
            continue;
        Document d(line.c_str());
        d.addText(field, line.c_str());
        c->addDocument(d);
        i++;
        if(i >= maxLines)
            break;
    }
    dataReadEnd = hiresTimestamp();
    printf("Read in %ld documents in %.2f seconds.\n", c->size(), dataReadEnd - dataReadStart);
    m = new Matcher(c);
    buildEnd = hiresTimestamp();
    printf("Index built in %.2f seconds.\n", buildEnd - dataReadEnd);
    return m;
}

int main(int argc, char **argv) {
    int maxLines = INT_MAX;
    Matcher *m;
    if(argc == 1) {
        printf("%s datafile.txt [num of lines]\n", argv[0]);
        return 1;
    }
    if(argc > 2)
        maxLines = atoi(argv[2]);
    m = build_matcher(argv[1], maxLines);
    delete m;
    return 0;
}
