/*
    AVLTree C++ class
    Copyright (C) 2021 Michele Iarossi - michele@mathsophy.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef tests_h
#define tests_h


// generate a vector of random keys
int generate_random_keys(int range, int total_keys, std::vector<unsigned int>& keys);

// example test case for balanced trees
int test_case_balanced_tree(std::vector<unsigned int>& keys);

// example test case for unbalanced trees
int test_case_unbalanced_tree(std::vector<unsigned int>& keys);

#endif /* tests_h */
