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
    trieOffset totalSize;
    trieOffset firstFree;
};

struct TriePtrs {
    Letter l;
    trieOffset child;
    trieOffset sibling;
};

struct TrieNode {
    WordID word;
};

struct TriePrivate {
    FILE *f;
    char *map;
    TrieHeader *h;
    trieOffset root;
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
    trieOffset newSize;
    if(p->map) {
        trieOffset oldSize = p->h->totalSize;
        newSize = oldSize*2;
        if(munmap(p->map, oldSize) != 0) {
            string err = "Munmap failed: ";
            err += strerror(errno);
            throw runtime_error(err);
        }
    } else {
        newSize = 1024;
    }
    if(ftruncate(fileno(p->f), newSize) != 0) {
        string err = "Truncate failed: ";
        err += strerror(errno);
        throw runtime_error(err);
    }
    p->map = (char*)mmap(NULL, newSize, PROT_READ | PROT_WRITE, MAP_SHARED,
                  fileno(p->f), 0);
    if(p->map == MAP_FAILED) {
        string err = "MMap failed: ";
        err += strerror(errno);
        throw runtime_error(err);
    }
    p->h = (TrieHeader*)p->map;
    p->h->totalSize = newSize;
    assert(p->h->totalSize > p->h->firstFree);
}

trieOffset Trie::append(const char *data, const int size) {
    trieOffset result;
    assert(p->h->totalSize > p->h->firstFree);
    while(p->h->firstFree + size >= p->h->totalSize) {
        expand();
    }
    memcpy(p->map + p->h->firstFree, data, size);
    result = p->h->firstFree;
    p->h->firstFree += size;
    assert(p->h->totalSize > p->h->firstFree);
    return result;
}

trieOffset Trie::addNewNode() {
    TrieNode n;
    TriePtrs ptr;
    trieOffset nodeoffset;
    n.word = INVALID_WORDID;
    ptr.child = ptr.sibling = ptr.l = 0;
    nodeoffset = append((char*)&n, sizeof(n));
    append((char*)&ptr, sizeof(ptr));
    return nodeoffset;
}

trieOffset Trie::addNewSibling(const trieOffset sibling, Letter l) {
    TriePtrs *last = (TriePtrs*)(p->map + sibling);
    assert(last->sibling == 0);
    TriePtrs ptr;
    ptr.l = l;
    ptr.child = addNewNode();
    ptr.sibling = 0;
    last->sibling = append((char*) &ptr, sizeof(ptr));
    return ptr.child;
}

void Trie::insertWord(const Word &word, const WordID wordID) {
    size_t i=0;
    trieOffset node = p->root;
    while(word.length() > i) {
        Letter l = word[i];
        trieOffset searcher = node;
        //TrieNode *n = (TrieNode*)(p->map + searcher);
        trieOffset sibl = searcher + sizeof(TrieNode);
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

bool Trie::hasWord(const Word &word) const {
    trieOffset node = p->root;
    for(size_t i=0; word.length() > i; i++) {
        Letter l = word[i];
        trieOffset searcher = node;
        trieOffset sibl = searcher + sizeof(TrieNode);
        TriePtrs *ptrs = (TriePtrs*)(p->map + sibl);
        while(ptrs->sibling != 0 && ptrs->l != l) {
            sibl = ptrs->sibling;
            ptrs = (TriePtrs*)(p->map + sibl);
        }

        if(ptrs->l != l)
            return false;
        node = ptrs->child;
    }
    TrieNode *n = (TrieNode*)(p->map+node);
    if(n->word != INVALID_WORDID)
        return true;
    return false;
}

trieOffset Trie::getRoot() const {
    return p->root;
}


trieOffset Trie::getSiblingList(trieOffset node) const {
    TriePtrs *ptrs = (TriePtrs*)(p->map + node + sizeof(TrieNode));
    return ptrs->sibling;

}

trieOffset Trie::getNextSibling(trieOffset sibling) const {
    TriePtrs *ptrs = (TriePtrs*)(p->map + sibling);
    return ptrs->sibling;
}

Letter Trie::getLetter(trieOffset sibling) const {
    TriePtrs *ptrs = (TriePtrs*)(p->map + sibling);
    return ptrs->l;
}

trieOffset Trie::getChild(trieOffset sibling) const {
    TriePtrs *ptrs = (TriePtrs*)(p->map + sibling);
    return ptrs->child;
}

WordID Trie::getWordID(trieOffset node) const {
    TrieNode *n = (TrieNode*)(p->map + node);
    return n->word;
}

bool Trie::hasSibling(trieOffset sibling) const {
    TriePtrs *ptrs = (TriePtrs*)(p->map + sibling);
    return ptrs->sibling != 0;
}

COL_NAMESPACE_END
