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
#include <vector>
#include "tests.h"

int main(int argc, const char * argv[])
{
    // change these constants for different range and tree depth
    constexpr int max_range   = 256;  // [0,range[
    constexpr int number_keys = 17;    // chosen out of the range
    std::vector<unsigned int> keys;
    
    // generate keys
    generate_random_keys(max_range,number_keys,keys);
    
    (void)test_case_balanced_tree(keys);
    
    (void)test_case_unbalanced_tree(keys);

    return 0;
}
