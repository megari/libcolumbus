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
#include "LevenshteinIndex.hh"

using namespace std;
typedef map<Letter, LevenshteinIndex*>::iterator mapiter;
typedef map<Letter, LevenshteinIndex*>::const_iterator mapiter_c;

LevenshteinIndex::LevenshteinIndex() : parent(0), letter(0) {
}

LevenshteinIndex::LevenshteinIndex(LevenshteinIndex *parent_node, Letter l) : parent(parent_node), letter(l) {
}

LevenshteinIndex::~LevenshteinIndex() {
    for(mapiter m = children.begin(); m != children.end(); m++)
        delete m->second;
}

void LevenshteinIndex::insert_word(const std::string &word) {
    insert_word(word, 0);
}

bool LevenshteinIndex::has_word(const std::string &word) const {
    return has_word(word, 0);
}

void LevenshteinIndex::insert_word(const std::string &word, size_t i) {
    if(word.length() <= i) {
        current_word = word;
        return;
    }
    Letter l = word[i];
    LevenshteinIndex *c;
    mapiter child = children.find(l);

    if(child == children.end()) {
         c = new LevenshteinIndex(this, l);
         children[l] = c;
    } else {
        c = child->second;
    }
    assert(c != 0);
    c->insert_word(word, i+1);
}

bool LevenshteinIndex::has_word(const std::string &word, size_t i) const {
    if(word.length() <= i) {
        if(current_word.length() > 0) {
            assert(current_word == word);
            return true;
        }
        return false;
    }

    Letter l = word[i];
    mapiter_c child = children.find(l);

    if(child == children.end())
        return false;
    return child->second->has_word(word, i+1);
}
