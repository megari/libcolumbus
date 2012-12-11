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

if __name__ == '__main__':
    unittest.main()
