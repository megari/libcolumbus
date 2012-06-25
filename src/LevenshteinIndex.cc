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

#include <stdio.h>
#include <cassert>
#include <map>
#include <vector>
#include "LevenshteinIndex.hh"
#include "MatchRow.hh"
#include "ErrorValues.hh"
#include "Word.hh"

COL_NAMESPACE_START
using namespace std;

typedef vector<pair<Letter, TrieNode*> > ChildList;
typedef ChildList::iterator ChildListIter;
typedef ChildList::const_iterator ChildListConstIter;


/*
 * There will be a LOT of these, so it must be small.
 * Vector takes 24 bytes, map 48 and unordered_map 56.
 */
struct TrieNode {
    TrieNode *parent;
    ChildList children;
    WordID currentWord; // The word that ends in this node.
};

typedef map<WordID, size_t> WordCount;

struct LevenshteinIndexPrivate {
    TrieNode *root;
    WordCount wordCounts; // How many times the word has been added to this index.
    size_t maxCount; // How many times the most common word has been added.
    size_t numNodes;
    size_t numWords; // How many words are in this index in total.
};

/**
 * Store all rows allocated during search to this and then
 * free them all at once.
 *
 * Replace with memory pool when more performance is required.
 */
class MemoryCleaner {
private:
    vector<MatchRow*> l;

public:
    MemoryCleaner() {}
    ~MemoryCleaner() {
        for(vector<MatchRow*>::iterator i=l.begin(); i != l.end(); i++)
            delete *i;
    }

    void addRow(MatchRow *m) {
        l.push_back(m);
    }
};

void gather_all_nodes(TrieNode *root, vector<TrieNode*> &nodes) {
    nodes.push_back(root);
    for(ChildListIter c=root->children.begin(); c != root->children.end(); c++) {
        gather_all_nodes(c->second, nodes);
    }
}

LevenshteinIndex::LevenshteinIndex() {
    p = new LevenshteinIndexPrivate();
    p->root = new TrieNode();
    p->root->parent = 0;
    p->root->currentWord = INVALID_WORDID;
    p->maxCount = 0;
}

LevenshteinIndex::~LevenshteinIndex() {
    vector<TrieNode*> nodes;
    gather_all_nodes(p->root, nodes);
    for(size_t i=0; i< nodes.size(); i++)
        delete nodes[i];
    delete p;
}

int LevenshteinIndex::getDefaultError() {
    return ErrorValues::getDefaultError();
}

void LevenshteinIndex::insertWord(const Word &word, const WordID wordID) {
    if(word.length() == 0)
        return;
    WordCount::const_iterator it = p->wordCounts.find(wordID);
    size_t newCount;
    if(it != p->wordCounts.end()) {
        newCount = p->wordCounts[wordID] + 1;
    } else {
        newCount = 1;
    }
    trieInsert(p->root, word, wordID);
    p->wordCounts[wordID] = newCount;
    if(p->maxCount < newCount)
        p->maxCount = newCount;
    return;
}

void LevenshteinIndex::trieInsert(TrieNode *node, const Word &word, const WordID wordID) {
    size_t i = 0;
    while(word.length() > i) {
        Letter l = word[i];
        ChildListIter child = node->children.begin();
        while(child != node->children.end() && child->first != l)//= node->children.find(l);
            child++;
        TrieNode *c;

        if(child == node->children.end()) {
            pair<Letter, TrieNode*> n;
            c = new TrieNode();
            c->parent = node;
            c->currentWord = INVALID_WORDID;
            n.first = l;
            n.second = c;
            node->children.push_back(n);
            p->numNodes++;
        } else {
            c = child->second;
        }
        assert(c != 0);
        node = c;
        i++;
    }
    if(node->currentWord == INVALID_WORDID) {
        node->currentWord = wordID;
        p->numWords++;
    }
    /*
     * Theoretically there is nothing wrong with adding the same word with
     * different IDs. In our case it probably means that the word deduplicator
     * is not working and there is a leak somewhere. So check explicitly.
     */
    assert(node->currentWord == wordID);
}

bool LevenshteinIndex::hasWord(const Word &word) const {
    TrieNode *node = p->root;
    size_t i = 0;
    while(word.length() > i) {
        Letter l = word[i];
        ChildListConstIter child = node->children.begin();
        while(child != node->children.end() && child->first != l)//= node->children.find(l);
            child++;

        if(child == node->children.end())
            return false;
        node = child->second;
        i++;
    }

    if(node->currentWord != INVALID_WORDID) {
         //assert(node->current_word == word); FIXME, re-enable this.
         return true;
     }
     return false;
}

void LevenshteinIndex::findWords(const Word &query, const ErrorValues &e, const int max_error, IndexMatches &matches) const {
    MemoryCleaner cleaner;
    MatchRow *first_row = new MatchRow(query.length()+1, e.getInsertionError());
    cleaner.addRow(first_row);
    assert(first_row->getValue(0) == 0);
    if(query.length() > 0)
        assert(first_row->getValue(1) == e.getInsertionError());
    for(ChildListIter i = p->root->children.begin(); i != p->root->children.end(); i++) {
        searchRecursive(query, i->second, e, i->first, 0, first_row, matches, max_error, cleaner);
    }
    matches.sort();
}

void LevenshteinIndex::searchRecursive(const Word &query, TrieNode *node, const ErrorValues &e, const Letter letter, const Letter previousLetter, const MatchRow *previousRow, IndexMatches &matches, const int max_error, MemoryCleaner &cleaner) const {
    MatchRow *currentRow = new MatchRow(previousRow, e.getDeletionError());
    cleaner.addRow(currentRow);

    for(size_t i = 1; i < query.length()+1; i++) {
        int insertError = currentRow->getValue(i-1) + e.getInsertionError();
        int deleteError = previousRow->getValue(i) + e.getDeletionError();
        int substituteError = previousRow->getValue(i-1) + e.getSubstituteError(query[i-1], letter);

        int transposeError;
        if(i > 1 && query[i - 1] == previousLetter && query[i - 2] == letter) {
            transposeError = previousRow->getParent()->getValue(i-2) + e.getTransposeError();
        } else {
            transposeError = insertError + 10000; // Ensures this will not be chosen.
        }
        int min_error = min(insertError, min(deleteError, min(substituteError, transposeError)));
        currentRow->setValue(i, min_error);
    }

    // Error row evaluated. Now check if a word was found and continue recursively.
    if(currentRow->totalError() <= max_error && node->currentWord != INVALID_WORDID) {
        matches.addMatch(query, node->currentWord, currentRow->totalError());
    }
    if(currentRow->minError() <= max_error) {
        for(ChildListIter i = node->children.begin(); i != node->children.end(); i++) {
            searchRecursive(query, i->second, e, i->first, letter, currentRow, matches, max_error, cleaner);
        }
    }
}

size_t LevenshteinIndex::wordCount(const WordID queryID) const {
    WordCount::const_iterator i = p->wordCounts.find(queryID);
    if(i == p->wordCounts.end())
        return 0;
    return i->second;
}

size_t LevenshteinIndex::maxCount() const {
    return p->maxCount;
}

size_t LevenshteinIndex::numNodes() const {
    return p->numNodes;
}

size_t LevenshteinIndex::numWords() const {
    return p->numWords;
}

COL_NAMESPACE_END
