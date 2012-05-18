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

#include <cassert>
#include <map>
#include <vector>
#include "LevenshteinIndex.hh"

using namespace std;

struct TrieNode {
    TrieNode *parent;
    std::map<Letter, TrieNode*> children;
    Letter letter;
    std::string current_word; // The word that ends in this node.
};

typedef map<Letter, TrieNode*>::iterator mapiter;
typedef map<Letter, TrieNode*>::const_iterator mapiter_c;

void gather_all_nodes(TrieNode *root, vector<TrieNode*> &nodes) {
    for(mapiter c=root->children.begin(); c != root->children.end(); c++) {
        gather_all_nodes(c->second, nodes);
        nodes.push_back(c->second);
    }
}

LevenshteinIndex::LevenshteinIndex() {
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


void LevenshteinIndex::insert_word(const std::string &word) {
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

bool LevenshteinIndex::has_word(const std::string &word) const {
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
