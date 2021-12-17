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

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "AVLTree.h"
#include <gvc.h>

#include "tests.h"

#define E_AVLTREE_OK          0
#define E_AVLTREE_UNBALANCED  1
#define E_AVLTREE_KEY         2

#define TEST_PASSED           0
#define TEST_FAILED           1

using mathsophy::AVLTree;
using mathsophy::AVLNode;

// private functions ----------------------


// balanced insertion test
static int insert_test(AVLTree<unsigned int>& tree, unsigned int key);

// unbalanced insertion test
static int unbalanced_insert_test(AVLTree<unsigned int>& tree, unsigned int key);

// balanced deletion test
static int delete_test(AVLTree<unsigned int>& tree, unsigned int key);

// unbalanced deletion test
static int unbalanced_delete_test(AVLTree<unsigned int>& tree, unsigned int key);

// generate a directional graph from the tree
static int generate_tree_graph(AVLTree<unsigned int>& tree, std::string file_name);

// public functions implementation

// generate a vector of random keys between [0,range[
// precondition: a positive range is provided
// postcondition: return a vector of total_keys keys in the given range
int generate_random_keys(int range, int total_keys, std::vector<unsigned int>& keys)
{
    std::random_device  rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int>  distr(0, range);
    int k = 0;
    
    // generate test keys
    while (k < total_keys)
    {
        // generate a random key
        unsigned int key = distr(gen);
        
        // new key? Add it, else regenerate
        if (find(keys.begin(),keys.end(),key) == keys.end())
        {
            keys.push_back(key);
            k++;
        }
    }
    
    return 0;
}

// test case for balanced trees. Balanced insertion and removal are tested.
// After every insertion and removal the tree is checked for being balanced or
// not. Inserted and removed keys are also tested whether they can be found or not
// in the tree after insertion or deletion.
// precondition: a valid vector of keys is given
// postcondition: return TEST_PASSED if no inconsistency occurs, otherwise
// return TEST_FAILED as soon as an inconsistency is found
int test_case_balanced_tree(std::vector<unsigned int>& keys)
{
    AVLTree<unsigned int> tree;
    
    // start of the test
    std::cout << "Test of balanced insertion and removal\n";
    
    // test balanced insertion
    {
        unsigned int n = 0;
        int result     = 0;
        
        for (unsigned int key : keys)
        {
            result = insert_test(tree,key);
            
            if (result == E_AVLTREE_UNBALANCED)
            {
                std::cerr << "-> failure after insertion: tree unbalanced!\n";
                std::cerr << "\t key causing the failure = " << key << "\n";
                
                generate_tree_graph(tree,"Unbalanced_insert_of_" + std::to_string(key));
                
                return result;
            } else if (result == E_AVLTREE_KEY)
            {
                std::cerr << "-> failure after insertion: inserted key not found! \n";
                std::cerr << "\t key causing the failure = " << key << "\n";
                
                generate_tree_graph(tree,"Missing_insert_of_" + std::to_string(key));
                
                return result;
            }
            else
            {
                // comment this line if you do not want a graph to be generated after each insertion
                generate_tree_graph(tree,"Test_balanced_" +  std::to_string(++n) + "_add_" + std::to_string(key));
            }
        }
    }
    
    // comment this line if you do not want a graph to be generated
    generate_tree_graph(tree,"Test_balanced_tree_graph");
    
    // test balanced removal
    {
        unsigned int n = 0;
        int result     = 0;
        
        for (unsigned int key : keys)
        {
            result = delete_test(tree,key);
            
            if (result == E_AVLTREE_UNBALANCED)
            {
                std::cerr << "-> failure after deletion: tree unbalanced!\n";
                std::cerr << "\t key causing the failure = " << key << "\n";
                
                generate_tree_graph(tree,"Unbalanced_removal_of_" + std::to_string(key));
                
                return result;
            } else if (result == E_AVLTREE_KEY)
            {
                std::cerr << "-> failure after deletion: deleted key found! \n";
                std::cerr << "\t key causing the failure = " << key << "\n";
                
                generate_tree_graph(tree,"Missing_removal_of_" + std::to_string(key));
                
                return result;
            }
            else
            {
                // comment this line if you do not want a graph to be generated after each removal
                generate_tree_graph(tree,"Test_balanced_" +  std::to_string(++n) + "_remove_" + std::to_string(key));
            }
        }
    }
    
    // check if the tree is empty
    if ( tree.is_not_empty() )
    {
        std::cerr << " -> failure after deletion: tree not empty!\n";
        generate_tree_graph(tree,"Tree_not_empty");
        return TEST_FAILED;
    }
    else
        std::cout << " -> passed\n";
    
    std::cout << std::endl;
    
    return TEST_PASSED;
}

// test case for unbalanced trees. Unbalanced insertion and removal are tested.
// Inserted and removed keys are tested whether they can be found or not
// in the tree after insertion or deletion.
// precondition: a valid vector of keys is given
// postcondition: return TEST_PASSED if no inconsistency occurs, otherwise
// return TEST_FAILED as soon as an inconsistency is found
int test_case_unbalanced_tree(std::vector<unsigned int>& keys)
{
    AVLTree<unsigned int> unbalanced_tree;
    
    // start of the test
    std::cout << "Test of unbalanced insertion and removal\n";
    
    // test unbalanced insertion
    {
        unsigned int n = 0;
        int result     = 0;
        
        for (unsigned int key : keys)
        {
            result = unbalanced_insert_test(unbalanced_tree,key);
            
            if (result == E_AVLTREE_KEY)
            {
                std::cerr << "-> failure after insertion: inserted key not found! \n";
                std::cerr << "\t key causing the failure = " << key << "\n";
                
                // generate_tree_graph(tree,"Missing_insert_of_" + std::to_string(key));
                
                return result;
            }
            else
            {
                // comment this line if you do not want a graph to be generated after each insertion
                generate_tree_graph(unbalanced_tree,"Test_unbalanced_" +  std::to_string(++n) + "_add_" + std::to_string(key));
            }
        }
    }
    
    // comment this line if you do not want a graph to be generated
    generate_tree_graph(unbalanced_tree,"Test_unbalanced_tree_graph");

    // test unbalanced removal
    {
        unsigned int n = 0;
        int result     = 0;
        
        for (unsigned int key : keys)
        {
            result = unbalanced_delete_test(unbalanced_tree,key);
            
            if (result == E_AVLTREE_KEY)
            {
                std::cerr << "-> failure after deletion: deleted key found! \n";
                std::cerr << "\t key causing the failure = " << key << "\n";
                
                //generate_tree_graph(tree,"Missing_removal_of_" + std::to_string(key));
                
                return result;
            }
            else
            {
                // comment this line if you do not want a graph to be generated after each removal
                generate_tree_graph(unbalanced_tree,"Test_unbalanced_" +  std::to_string(++n) + "_remove_" + std::to_string(key));
            }
        }
    }
    
    // check if the tree is empty
    if ( unbalanced_tree.is_not_empty() )
    {
        std::cerr << " -> failure after deletion: unbalanced tree not empty!\n";
        // generate_tree_graph(tree,"Unbalanced_tree_not_empty");
        return TEST_FAILED;
    }
    else
        std::cout << " -> passed\n";
    
    std::cout << std::endl;
    
    return TEST_PASSED;
}

// private functions implementation

// balanced insertion test of a single key
// precondition: a valid key is given
// postcondition: return E_AVLTREE_UNBALANCED if the tree is unbalanced,
// return E_AVLTREE_KEY if the key cannot be found in the tree
// after insertion, return E_AVLTREE_OK if no inconsistency is found
int insert_test(AVLTree<unsigned int>& tree, unsigned int key)
{
    // insert
    tree.insert(key);
    
    // check if the tree is balanced
    if ( tree.is_not_balanced() )
        return E_AVLTREE_UNBALANCED;
    
    // check if the key has been inserted
    if ( tree.find(key) == nullptr )
        return E_AVLTREE_KEY;
    
    return E_AVLTREE_OK;
}

// unbalanced insertion test of a single key
// precondition: a valid key is given
// postcondition: return E_AVLTREE_KEY if the key cannot be found in the tree
// after insertion, return E_AVLTREE_OK if no inconsistency is found
int unbalanced_insert_test(AVLTree<unsigned int>& tree, unsigned int key)
{
    // unbalanced insert
    tree.unbalanced_insert(key);
    
    // check if the key has been inserted
    if ( tree.find(key) == nullptr )
        return E_AVLTREE_KEY;
    
    return E_AVLTREE_OK;
}

// balanced deletion test of a single key
// precondition: a valid key is given
// postcondition: return E_AVLTREE_UNBALANCED if the tree is unbalanced,
// return E_AVLTREE_KEY if the key is still found in the tree
// after deletion, return E_AVLTREE_OK if no inconsistency is found
int delete_test(AVLTree<unsigned int>& tree, unsigned int key)
{
    // remove
    tree.remove(key);
    
    // check if the tree is balanced
    if ( tree.is_not_balanced() )
        return E_AVLTREE_UNBALANCED;
    
    // check if the key has been removed
    if ( tree.find(key) )
        return E_AVLTREE_KEY;
    
    return E_AVLTREE_OK;
}

// unbalanced deletion test of a single key
// precondition: a valid key is given
// postcondition: return E_AVLTREE_KEY if the key is still found in the tree
// after deletion, return E_AVLTREE_OK if no inconsistency is found
int unbalanced_delete_test(AVLTree<unsigned int>& tree, unsigned int key)
{
    // remove
    tree.unbalanced_remove(key);
    
    // check if the key has been removed
    if ( tree.find(key) )
        return E_AVLTREE_KEY;
    
    return E_AVLTREE_OK;
}

// generate a tree graph by means of the graphviz library
// precondition: valid tree and file name are given
// postcondition: generate an image of the tree as a png file
int generate_tree_graph(AVLTree<unsigned int>& tree, std::string file_name)
{
    Agraph_t *g;
    Agnode_t *parent, *left_child, *right_child;
    Agedge_t *left_edge, *right_edge;
    GVC_t *gvc;
    int k = 0; // label for invisible nodes

    // set up a graphviz context
    gvc = gvContext();

    // Create a simple digraph
    g = agopen((char*)"AVLTree", Agdirected, 0);
    
    // visit each node by breadth first traversal
    std::vector< AVLNode<unsigned int>* > q;
    
    if (tree.is_not_empty())
        q.push_back(tree.get_root());
    while (!q.empty())
    {
        AVLNode<unsigned int>* node = q.front();
        q.erase(q.begin());
        
        // add the nodes and edges to the graph
        parent = agnode(g, (char*)std::to_string(node->get_key()).c_str(), 1);
        
        // Set an attribute - in this case one that affects the visible rendering
        std::string label = std::to_string(node->get_key()) + "\n(" + std::to_string(node->get_height()) + ","
                              + std::to_string(node->get_balance()) + ")";
        agsafeset(parent, (char*)"label", (char*)label.c_str(), "");
        agsafeset(parent, (char*)"width", (char*)"0.75", "");
        agsafeset(parent, (char*)"height", (char*)"0.75", "");
        agsafeset(parent, (char*)"fixedsize", (char*)"true", "");
        
        
        // add left node
        if (node->get_left())
        {
            q.push_back(node->get_left());
            
            left_child  = agnode(g, (char*)std::to_string(node->get_left()->get_key()).c_str(), 1);
            
            left_edge   = agedge(g, parent, left_child, 0, 1);
            
            std::string label = std::to_string(node->get_left()->get_key()) + "\n(" + std::to_string(node->get_left()->get_height()) + ","
                + std::to_string(node->get_left()->get_balance()) + ")";
            
            agsafeset(left_child, (char*)"xabel", (char*)label.c_str(), "");
            agsafeset(left_child, (char*)"width", (char*)"0.75", "");
            agsafeset(left_child, (char*)"height", (char*)"0.75", "");
            agsafeset(left_child, (char*)"fixedsize", (char*)"true", "");
        }
        // add invisible left node
        else
        {
            k--;
            
            left_child  = agnode(g, (char*)std::to_string(k).c_str(), 1);
            left_edge   = agedge(g, parent, left_child, 0, 1);
            
            agsafeset(left_child, (char*)"shape", (char*)"point", "");
            agsafeset(left_child, (char*)"style", (char*)"invis", "");
            agsafeset(left_edge, (char*)"style", (char*)"invis", "");
            
        }
        // add rihtg node
        if (node->get_right())
        {
            q.push_back(node->get_right());
            
            right_child = agnode(g, (char*)std::to_string(node->get_right()->get_key()).c_str(), 1);
            
            right_edge  = agedge(g, parent, right_child, 0, 1);
            
            std::string label = std::to_string(node->get_right()->get_key()) + "\n(" + std::to_string(node->get_right()->get_height()) + ","
                + std::to_string(node->get_right()->get_balance()) + ")";
            
            agsafeset(right_child, (char*)"label", (char*)label.c_str(), "");
            agsafeset(right_child, (char*)"width", (char*)"0.75", "");
            agsafeset(right_child, (char*)"height", (char*)"0.75", "");
            agsafeset(right_child, (char*)"fixedsize", (char*)"true", "");
        }
        // add invisible right node
        else
        {
            k--;
            
            right_child  = agnode(g, (char*)std::to_string(k).c_str(), 1);
            right_edge   = agedge(g, parent, right_child, 0, 1);
            
            agsafeset(right_child, (char*)"shape", (char*)"point", "");
            agsafeset(right_child, (char*)"style", (char*)"invis", "");
            
            agsafeset(right_edge, (char*)"style", (char*)"invis", "");
        }
    }
    
    // Compute a layout using layout engine from command line args
    gvLayout (gvc, g, "dot");
    
    // Write the graph to file
    gvRenderFilename (gvc, g, "png", (char *)(file_name+".png").c_str());

    // Free layout data
    gvFreeLayout(gvc, g);

    // Free graph structures
    agclose(g);

    // lose output file, free context, and return number of errors
    return (gvFreeContext(gvc));
}
