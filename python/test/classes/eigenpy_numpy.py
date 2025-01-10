# This file is part of Bertini 2.
# 
# python/test/mpfr_test.py is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# python/test/mpfr_test.py is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with python/test/mpfr_test.py.  If not, see <http://www.gnu.org/licenses/>.
# 
#  Copyright(C) 2024-2025 by Bertini2 Development Team
# 
#  See <http://www.gnu.org/licenses/> for a copy of the license, 
#  as well as COPYING.  Bertini2 is provided with permitted 
#  additional terms in the b2/licenses/ directory.

#  individual authors of this file include:
# 
#   silviana amethyst
#   Max Planck Institute of Molecular Cell Biology and Genetics
#   Fall 2024, Spring 2025

# the purpose of this test suite is to make ensure numpy functionality with the custom types defined by Bertini (via EigenPy).

import numpy as np
import pybertini as pb

from pybertini import multiprec as mp

import unittest
import pdb


class TestFloat(unittest.TestCase):

    def setUp(self):
        mp.default_precision(30);
        self.shape = (5,10)

    def test_make_array_empty(self):
        A = np.empty(self.shape, dtype = mp.Float) 

        A[0,0] = mp.Float(0)

    def test_make_array_zeros(self):
        A = np.zeros(self.shape, dtype = mp.Float) 

        A[0,0] = mp.Float(1)

    def test_make_array_zeros_with_conversion(self):
        A = np.array( np.zeros(self.shape), dtype = mp.Float) 

    def test_make_array_zeros_with_conversion_and_astype_int64(self):
        A = np.array( np.zeros(self.shape).astype(np.int64), dtype = mp.Float) 


    def test_make_array_ones(self):
        A = np.ones(self.shape, dtype = mp.Float) 
        A[0,0] = mp.Float(2)

    def test_make_array_ones_with_conversion(self):
        A = np.array( np.ones(self.shape), dtype = mp.Float) 

    def test_make_array_ones_with_conversion_and_astype_int64(self):
        A = np.array( np.ones(self.shape).astype(np.int64), dtype = mp.Float) 


class TestComplex(unittest.TestCase):

    def setUp(self):
        mp.default_precision(30);
        self.shape = (5,10)

    def test_make_array_empty(self):
        A = np.empty(self.shape, dtype = mp.Complex) 

        A[0,0] = mp.Complex(0)

    def test_make_array_zeros(self):
        A = np.zeros(self.shape, dtype = mp.Complex) 

        A[0,0] = mp.Complex(1)

    def test_make_array_zeros_with_conversion(self):
        A = np.array( np.zeros(self.shape), dtype = mp.Complex) 

    def test_make_array_zeros_with_conversion_and_astype_int64(self):
        A = np.array( np.zeros(self.shape).astype(np.int64), dtype = mp.Complex) 


    def test_make_array_ones(self):
        A = np.ones(self.shape, dtype = mp.Complex) 
        A[0,0] = mp.Complex(2)

    def test_make_array_ones_with_conversion(self):
        A = np.array( np.ones(self.shape), dtype = mp.Complex) 

    def test_make_array_ones_with_conversion_and_astype_int64(self):
        A = np.array( np.ones(self.shape).astype(np.int64), dtype = mp.Complex) 

if __name__ == '__main__':
    unittest.main();