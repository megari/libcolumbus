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

#include "ResultGatherer.hh"
#include "IndexMatches.hh"
#include "Word.hh"
#include "Matcher.hh"
#include <map>

using namespace std;

struct ResultGathererPrivate {
    // For each index list matched words and the smallest error with which they were matched.
    map<Word, map<Word, int> > bestIndexMatches;
};

typedef map<Word, map<Word, int> >::iterator IndIterator;
typedef map<Word, int>::iterator MatchIterator;

ResultGatherer::ResultGatherer(Matcher *m) : matcher(m) {
    p = new ResultGathererPrivate();
}

ResultGatherer::~ResultGatherer() {
    delete p;
}

void ResultGatherer::addMatches(const Word &queryWord, const Word &indexName, IndexMatches &matches) {
    IndIterator it = p->bestIndexMatches.find(indexName);
    map<Word, int> *indexMatches;
    if(it == p->bestIndexMatches.end()) {
        map<Word, int> tmp;
        p->bestIndexMatches[indexName] = tmp;
        it = p->bestIndexMatches.find(indexName);
    }
    indexMatches = &(it->second);
    for(size_t i=0; i < matches.size(); i++) {
        const Word &matchWord = matches.getMatch(i);
        const int matchError = matches.getMatchError(i);
        MatchIterator mIt = indexMatches->find(matchWord);
        if(mIt == indexMatches->end()) {
            (*indexMatches)[matchWord] = matchError;
        } else {
            if(mIt->second > matchError)
                (*indexMatches)[matchWord] = matchError;
        }

    }
}

void ResultGatherer::gatherMatchedDocuments() {
    vector<const Document*> matchedDocuments;
    for(IndIterator it = p->bestIndexMatches.begin(); it != p->bestIndexMatches.end(); it++) {
        for(MatchIterator mIt = it->second.begin(); mIt != it->second.end(); mIt++) {
            vector<const Document*> tmp;
            matcher->findDocuments(mIt->first, it->first, tmp);
            for(size_t i=0; i<tmp.size(); i++)
                matchedDocuments.push_back(tmp[i]);
        }
    }
}
