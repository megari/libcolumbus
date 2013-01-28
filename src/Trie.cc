/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include"Trie.hh"
#include"Word.hh"
#include<sys/mman.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<stdexcept>
#include<string>
#include<cassert>

using namespace std;

COL_NAMESPACE_START

struct TrieHeader {
    offset totalSize;
    offset firstFree;
};

struct TriePtrs {
    Letter l;
    offset child;
    offset sibling;
};

struct TrieNode {
    WordID word;
//    TriePtrs ptrs;
};

struct TriePrivate {
    FILE *f;
    char *map;
    TrieHeader *h;
    offset root;
};

Trie::Trie() {
    p = new TriePrivate();
    p->f = tmpfile();
    p->map = nullptr;
    expand();
    p->h->firstFree = sizeof(TrieHeader);
    p->root = p->h->firstFree;
    addNewNode();
}


Trie::~Trie() {
    fclose(p->f);
    delete p;
}

void Trie::expand() {
    offset newSize;
    if(p->map) {
        offset oldSize = p->h->totalSize;
        newSize = oldSize*2;
        munmap(p->map, oldSize);
    } else {
        newSize = 1024;
    }
    if(ftruncate(fileno(p->f), newSize) != 0) {
        string err = "Truncate failed: ";
        err += strerror(errno);
        throw runtime_error(err);
    }
    p->map = (char*)mmap(NULL, newSize, PROT_READ | PROT_WRITE, MAP_PRIVATE,
                  fileno(p->f), 0);
    if(p->map == MAP_FAILED) {
        string err = "MMap failed: ";
        err += strerror(errno);
        throw runtime_error(err);
    }
    p->h = (TrieHeader*)p->map;
    p->h->totalSize = newSize;
}

offset Trie::append(const char *data, const int size) {
    offset result;
    while(p->h->firstFree + size >= p->h->totalSize)
        expand();
    memcpy(p->map + p->h->firstFree, data, size);
    result = p->h->firstFree;
    p->h->firstFree += size;
    return result;
}

offset Trie::addNewNode() {
    TrieNode n;
    TriePtrs ptr;
    offset nodeoffset;
    n.word = INVALID_WORDID;
    ptr.child = ptr.sibling = ptr.l = 0;
    nodeoffset = append((char*)&n, sizeof(n));
    append((char*)&ptr, sizeof(ptr));
    return nodeoffset;
}

offset Trie::addNewSibling(const offset sibling, Letter l) {
    TriePtrs ptr;
    ptr.l = l;
    ptr.child = addNewNode();
    ptr.sibling = 0;
    //return append((char*) &ptr, sizeof(ptr));
    return ptr.child;
}

void Trie::insertWord(const Word &word, const WordID wordID) {
    size_t i=0;
    offset node = p->root;
    while(word.length() > i) {
        Letter l = word[i];
        offset searcher = node;
        //TrieNode *n = (TrieNode*)(p->map + searcher);
        offset sibl = searcher + sizeof(TrieNode);
        TriePtrs *ptrs = (TriePtrs*)(p->map + sibl);
        while(ptrs->sibling != 0 && ptrs->l != l) {
            sibl = ptrs->sibling;
            ptrs = (TriePtrs*)(p->map + sibl);
        }

        if(ptrs->l == l) {
            node = ptrs->child;
        } else {
            node = addNewSibling(sibl, l);
        }
        i++;
    }
    TrieNode *final = (TrieNode*)(p->map + node);
    if (final->word == INVALID_WORDID) {
        final->word = wordID;
    }
    /*
     * Theoretically there is nothing wrong with adding the same word with
     * different IDs. In our case it probably means that the word deduplicator
     * is not working and there is a leak somewhere. So check explicitly.
     */
    assert(final->word == wordID);

}

#if 0
void LevenshteinIndex::insertWord(const Word &word, const WordID wordID) {
    trieInsert(p->root, word, wordID);
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
#endif

COL_NAMESPACE_END
