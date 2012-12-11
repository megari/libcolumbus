#!/usr/bin/python3 -tt
# -*- coding: utf-8 -*-

# Copyright (C) 2012 Canonical, Ltd.

# Authors:
#    Jussi Pakkanen <jussi.pakkanen@canonical.com>

# This library is free software; you can redistribute it and/or modify it under
# the terms of version 3 of the GNU Lesser General Public License as published
# by the Free Software Foundation.

# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.

# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import unittest
import columbus

class TestWord(unittest.TestCase):
    
    def test_init(self):
        d = columbus.Word("hello")
        
    def test_encoding(self):
        str1 = "hello"
        str2 = 'abcåäö'
        
        w1 = columbus.Word(str1)
        self.assertEqual(str1, w1.get_string(), "plain ASCII string did not survive round trip")

        w2 = columbus.Word(str2)
        self.assertEqual(str2, w2.get_string(), 'non-ASCII string did not survive round trip')
        
    def test_exception(self):
        str1 = 'two words'
        with self.assertRaises(ValueError):
            w1 = columbus.Word(str1)

    def test_length(self):
        str1 = "hello"
        str2 = 'abcåäö'
        
        w1 = columbus.Word(str1)
        self.assertEqual(len(str1), len(w1), "plain ASCII Word has incorrect size")

        w2 = columbus.Word(str2)
        self.assertEqual(len(str2), len(w2), 'non-ASCII Word has incorrect size')

class TestWordList(unittest.TestCase):
    
    def test_init(self):
        l = columbus.WordList()
        
    def test_size(self):
        l = columbus.WordList()
        self.assertEqual(0, len(l), 'Incorrect size for empty list')
        w1 = columbus.Word('abc')
        w2 = columbus.Word('defg')
        
        l.add_word(w1)
        self.assertEqual(1, len(l))
        l.add_word(w2)
        self.assertEqual(2, len(l))
        l.add_word(w1)
        self.assertEqual(3, len(l))
        
    def test_split(self):
        l = columbus.split_to_words('this is  my text')
        self.assertEqual(4, len(l), 'text splitting fails')

class TestDocument(unittest.TestCase):
    
    def test_init(self):
        d = columbus.Document(1)
        
    def test_doc(self):
        docid = 435
        field = columbus.Word('fieldname')
        text = columbus.split_to_words('ye olde butcherede englishe')
        d = columbus.Document(docid)
        
        self.assertEqual(d.get_id(), docid, 'Document ID got mangled.')
        self.assertEqual(d.field_count(), 0)
        
        d.add_text(field, text)
        self.assertEqual(d.field_count(), 1, 'field count did not increase')
        self.assertGreater(len(text), 0)
        self.assertEqual(len(d.get_text(field)), len(text), 'stored text got mangled')

class TestCorpus(unittest.TestCase):
    
    def test_init(self):
        c = columbus.Corpus()
        
    def test_insertion(self):
        c = columbus.Corpus()
        d = columbus.Document(55)
        
        self.assertEqual(0, len(c))
        
        c.add_document(d)
        self.assertEqual(1, len(c))

if __name__ == '__main__':
    unittest.main()
