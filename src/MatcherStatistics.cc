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

#include "Word.hh"
#include <map>
#include "MatcherStatistics.hh"

using namespace std;

struct MatcherStatisticsPrivate {
    map<WordID, size_t> totalWordCounts;
};

MatcherStatistics::MatcherStatistics() {
    p = new MatcherStatisticsPrivate();

}

MatcherStatistics::~MatcherStatistics() {
    delete p;
}

void MatcherStatistics::wordProcessed(const WordID w) {
    map<WordID, size_t>::iterator it = p->totalWordCounts.find(w);
    if(it == p->totalWordCounts.end()) {
        p->totalWordCounts[w] = 1;
    } else {
        it->second++;
    }
}

size_t MatcherStatistics::getTotalWordCount(const WordID w) const {
    map<WordID, size_t>::iterator it = p->totalWordCounts.find(w);
    if(it == p->totalWordCounts.end()) {
        return 0;
    } else {
        return it->second;
    }

}

void MatcherStatistics::addedWordToIndex(const WordID word, const Word &fieldName) {
    // Doesn't do anything yet.
}
