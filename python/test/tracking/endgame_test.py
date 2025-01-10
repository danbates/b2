# This file is part of Bertini 2.
#
# python/test/endgame_test.py is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# python/test/endgame_test.py is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with python/test/endgame_test.py.  If not, see <http://www.gnu.org/licenses/>.
#
#  Copyright(C) 2016-2018 by Bertini2 Development Team
#
#  See <http://www.gnu.org/licenses/> for a copy of the license,
#  as well as COPYING.  Bertini2 is provided with permitted
#  additional terms in the b2/licenses/ directory.

#  individual authors of this file include:
#
#   silviana amethyst
#   University of Notre Dame
#
#  silviana amethyst
#  UWEC
#  Spring 2018
#



__author__ = 'ofloveandhate'



if __name__ == '__main__':
    run_tests = True
else:
    run_tests = False


from pybertini import *
from pybertini.function_tree.symbol import *
from pybertini.function_tree.root import *
from pybertini.function_tree import *
from pybertini.tracking import *
from pybertini.tracking.config import *
from pybertini.endgame import *
from pybertini.endgame.config import *

import unittest
import numpy as np
import pdb

import pybertini.system.start_system as ss
import pybertini.multiprec as mp
from pybertini.multiprec import Float as mpfr_float
from pybertini.multiprec import Complex as mpfr_complex


class EndgameTest(unittest.TestCase):
    def setUp(self):
        self.ambient_precision = 50;

    def test_using_total_degree_ss(self):
        default_precision(self.ambient_precision);

        x = Variable("x");
        y = Variable("y");
        t = Variable("t");
        
        sys = System();
        
        var_grp = VariableGroup();
        var_grp.append(x);
        var_grp.append(y);

        sys.add_variable_group(var_grp);

        sys.add_function((x-1)**3)
        sys.add_function((y-1)**2)

        sys.homogenize();
        sys.auto_patch();

        self.assertEqual(sys.is_patched(), 1)
        self.assertEqual(sys.is_homogeneous(), 1)

        td = ss.TotalDegree(sys);

        self.assertEqual(td.is_patched(), 1)
        self.assertEqual(td.is_homogeneous(), 1)

        gamma = Rational.rand();


        final_system = (1-t)*sys + gamma*t*td;
        final_system.add_path_variable(t);

        prec_config = AMPConfig(final_system);

        stepping_pref = SteppingConfig();
        newton_pref = NewtonConfig();

        tracker = AMPTracker(final_system);

        tracker.setup(Predictor.RK4, 1e-5, 1e5, stepping_pref, newton_pref);
        tracker.precision_setup(prec_config);

        num_paths_to_track = td.num_start_points();
        n = int(str(num_paths_to_track)); # this line sucks, wtf.

        t_start = mpfr_complex(1);
        t_endgame_boundary = mpfr_complex("0.1");
        t_final = mpfr_complex(0);

        bdry_points = []

        for i in range(n):
            default_precision(self.ambient_precision);
            final_system.precision(self.ambient_precision);
            start_point = td.start_point_mp(i);

            bdry_pt = np.array( np.zeros( (3)).astype(np.int64),dtype=mpfr_complex)   

            track_success_code = tracker.track_path(bdry_pt,t_start, t_endgame_boundary, start_point);
            bdry_points.append(bdry_pt);

            self.assertEqual(track_success_code, SuccessCode.Success)


        tracker.setup(Predictor.HeunEuler, 1e-6, 1e5, stepping_pref, newton_pref);
        my_endgame = AMPCauchyEG(tracker);



        final_homogenized_solutions = [np.empty(dtype=mpfr_complex, shape=(3,)) for i in range(n)]

        for i in range(n):
            default_precision(bdry_points[i][0].precision());
            final_system.precision(bdry_points[i][0].precision());

            bdry_time = mpfr_complex(t_endgame_boundary)

            track_success_code = my_endgame.run(bdry_time,bdry_points[i]); # should be bdry_pts[i], not XXX
        

            final_homogenized_solutions[i] = my_endgame.final_approximation();

            self.assertEqual(track_success_code, SuccessCode.Success)

        dehomogenized_solns = [sys.dehomogenize_point(soln) for soln in final_homogenized_solutions]

        exact_soln = np.array([1,1])

        for soln in dehomogenized_solns:
            assert np.sqrt(np.sum((exact_soln - soln)**2)) < 1e-10



if run_tests:

    pgnm = 'this_argument_is_ignored_but_necessary'
    unittest.main(argv=[pgnm], exit=False)
