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

#include<sys/mman.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<stdexcept>
#include<string>

using namespace std;

COL_NAMESPACE_START

typedef uint32_t offset;

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
};

struct TriePrivate {
    FILE *f;
    void *map;
    TrieHeader *h;
};

Trie::Trie() {
    p = new TriePrivate();
    p->f = tmpfile();
    p->map = nullptr;
    expand();
    p->h->firstFree = sizeof(TrieHeader);
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
    p->map = mmap(NULL, newSize, PROT_READ | PROT_WRITE, MAP_PRIVATE,
                  fileno(p->f), 0);
    if(p->map == MAP_FAILED) {
        string err = "MMap failed: ";
        err += strerror(errno);
        throw runtime_error(err);
    }
    p->h = (TrieHeader*)p->map;
    p->h->totalSize = newSize;
}

COL_NAMESPACE_END
