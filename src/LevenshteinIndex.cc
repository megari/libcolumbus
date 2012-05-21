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

#include <stdio.h>
#include <cassert>
#include <map>
#include <vector>
#include "LevenshteinIndex.hh"
#include "MatchRow.hh"

using namespace std;

struct TrieNode {
    TrieNode *parent;
    std::map<Letter, TrieNode*> children;
    Letter letter;
    std::string current_word; // The word that ends in this node.
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
    MemoryCleaner() {};
    ~MemoryCleaner() {
        for(vector<MatchRow*>::iterator i=l.begin(); i != l.end(); i++)
            delete *i;
    }

    void addRow(MatchRow *m) {
        l.push_back(m);
    }
};

typedef map<Letter, TrieNode*>::iterator mapiter;
typedef map<Letter, TrieNode*>::const_iterator mapiter_c;

void gather_all_nodes(TrieNode *root, vector<TrieNode*> &nodes) {
    nodes.push_back(root);
    for(mapiter c=root->children.begin(); c != root->children.end(); c++) {
        gather_all_nodes(c->second, nodes);
    }
}

LevenshteinIndex::LevenshteinIndex() :
    insertion_error(DEFAULT_ERROR),
    deletion_error(DEFAULT_ERROR),
    transpose_error(DEFAULT_ERROR),
    substitute_error(DEFAULT_ERROR) {
    root = new TrieNode();
    root->parent = 0;
    root->letter = 0;
}

LevenshteinIndex::~LevenshteinIndex() {
    vector<TrieNode*> nodes;
    gather_all_nodes(root, nodes);
    for(size_t i=0; i< nodes.size(); i++)
        delete nodes[i];
}


void LevenshteinIndex::insertWord(const std::string &word) {
    TrieNode *node = root;
    size_t i = 0;
    while(word.length() > i) {
        Letter l = word[i];
        mapiter child = node->children.find(l);
        TrieNode *c;

        if(child == node->children.end()) {
            c = new TrieNode();
            c->letter = l;
            c->parent = node;
            node->children[l] = c;
        } else {
            c = child->second;
        }
        assert(c != 0);
        node = c;
        i++;
    }
    node->current_word = word;
    return;
}

bool LevenshteinIndex::hasWord(const std::string &word) const {
    TrieNode *node = root;
    size_t i = 0;
    while(word.length() > i) {
        Letter l = word[i];
        mapiter_c child = node->children.find(l);

        if(child == node->children.end())
            return false;
        node = child->second;
        i++;
    }

    if(node->current_word.length() > 0) {
         assert(node->current_word == word);
         return true;
     }
     return false;
}

void LevenshteinIndex::findWords(const std::string &word, const int max_error, IndexMatches &matches) const {
    MemoryCleaner cleaner;
    MatchRow *first_row = new MatchRow(word.length()+1, getInsertionError());
    cleaner.addRow(first_row);
    assert(first_row->getValue(0) == 0);
    assert(first_row->getValue(1) == getInsertionError());
    for(mapiter i = root->children.begin(); i != root->children.end(); i++) {
        searchRecursive(word, i->second, i->first, 0, first_row, matches, max_error, cleaner);
    }
    matches.sort();
}

void LevenshteinIndex::searchRecursive(const std::string &word, TrieNode *node, Letter letter, Letter previousLetter, MatchRow *previous_row, IndexMatches &matches, const int max_error, MemoryCleaner &cleaner) const {
    MatchRow *current_row = new MatchRow(previous_row, getDeletionError());
    cleaner.addRow(current_row);

    for(size_t i = 1; i < word.length()+1; i++) {
        int insert_cost = current_row->getValue(i-1) + getInsertionError();
        int delete_error = previous_row->getValue(i) + getDeletionError();
        int substitute_error = previous_row->getValue(i-1) + getSubstituteError(word[i-1], letter);

        int min_error = min(insert_cost, min(delete_error, substitute_error));
        current_row->setValue(i, min_error);
    }

    // Error row evaluated. Now check if a word was found and continue recursively.
    if(current_row->totalError() < max_error && node->current_word.length() > 0) {
        matches.addMatch(node->current_word, current_row->totalError());
    }
    if(current_row->minError() <= max_error) {
        for(mapiter i = node->children.begin(); i != node->children.end(); i++) {
            searchRecursive(word, i->second, i->first, letter, current_row, matches, max_error, cleaner);
        }
    }
}
