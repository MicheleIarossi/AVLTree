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

#ifndef AVLTree_h
#define AVLTree_h

namespace mathsophy
{

template<class T>
class AVLTree;

template<class T>
class AVLNode
{
public:
    // constructor
    AVLNode(T k=T{}, int h=1, int b=0, AVLNode<T>* l=nullptr, AVLNode<T>* r=nullptr) :
            key(k), height(h), balance(b), left(l), right(r) {};
    // getter and setter functions
    T           get_key() const             { return key; };
    void        set_key(T k)                { key = k; };
    int         get_height() const          { return height; };
    void        set_height(int h)           { height = h; };
    int         get_balance() const         { return balance; };
    void        set_balance(int b)          { balance = b; };
    AVLNode<T>* get_left() const            { return left; };
    void        set_left(AVLNode<T>* node)  { left=node; };
    AVLNode<T>* get_right() const           { return right; };
    void        set_right(AVLNode<T>* node) { right=node; };
    // friend
    friend class AVLTree<T>;
private:
    T       key;
    int     height;
    int     balance;
    AVLNode *left;
    AVLNode *right;
};

template<class T>
class AVLTree
{
public:
    // constructor
    AVLTree() : root(nullptr) { };
    // copy constructor
    AVLTree(AVLTree<T>& tree) { *this = tree; };
    // destructor
    virtual ~AVLTree() { clear(); };
    // assignment operator
    AVLTree<T>& operator=(const AVLTree<T>& tree);
    // getter and setter functions
    AVLNode<T>* get_root() const        { return root; };
    void        set_root(AVLNode<T>* r);
    // balanced insertion of a new element
    void        insert(T key);
    // unbalanced insertion of a new element
    void        unbalanced_insert(T key);
    // find an element
    AVLNode<T>* find(T key);
    // balanced removal of an element
    void        remove(T key);
    // unbalanced removal of an element
    void        unbalanced_remove(T key);
    // test for balanced tree
    bool        is_balanced() const;
    bool        is_not_balanced() const { return !is_balanced(); };
    // test for empty tree
    bool        is_empty() const { return root == nullptr; };
    bool        is_not_empty() const { return root != nullptr; };
protected:
    // private helper functions
    void        clear();
    void        update_height_node(AVLNode<T>* node);
    AVLNode<T>* new_node(T key) { return new AVLNode<T>(key); };
    void        insertnb(T key, std::vector<AVLNode<T>*>& path);
    void        removenb(T key, std::vector<AVLNode<T>*>& path);
    void        cut_off_node(AVLNode<T>* node, AVLNode<T>* parent, std::vector<AVLNode<T>*>& path);
    int         compute_node_balance(AVLNode<T>* node) const;
    void        rebalance(std::vector<AVLNode<T>*>& path);
    AVLNode<T>* rebalance_to_right(AVLNode<T>* node, AVLNode<T>* parent);
    AVLNode<T>* rebalance_to_left(AVLNode<T>* node, AVLNode<T>* parent);
    AVLNode<T>* rotate_left(AVLNode<T>* node);
    AVLNode<T>* rotate_right(AVLNode<T>* node);
    AVLNode<T>* root;
};

// assignement operator
// precondition: valid tree is given
// postcondition: input tree is copied to the current tree
template <class T>
AVLTree<T>& AVLTree<T>::operator=(const AVLTree<T>& tree)
{
    if (is_not_empty())
        clear();
    
    std::vector<AVLNode<T>*> q, qc;
    q.push_back(tree.root);
    
    this->root = new_node(0);
    qc.push_back(this->root);
    
    // check each node by breadth first traversal
    while (!q.empty())
    {
        AVLNode<T>* node      = q.front();
        AVLNode<T>* copy_node = qc.front();
        
        q.erase(q.begin());
        qc.erase(qc.begin());
        
        copy_node->key     = node->key;
        copy_node->height  = node->height;
        copy_node->balance = node->balance;
        
        if (node->left)
        {
            q.push_back(node->left);
            copy_node->left = new_node(0);
            qc.push_back(copy_node->left);
        }
        if (node->right)
        {
            q.push_back(node->right);
            copy_node->right = new_node(0);
            qc.push_back(copy_node->right);
        }
    }
    
    return *this;
}

// set the input node as the new root pointer
// precondition: valid node pointer is given
// postcondition: root pointer updated
template <class T>
void AVLTree<T>::set_root(AVLNode<T>* r)
{
    if ( is_not_empty() )
        clear();
    root = r;
}

// insert a new key into the tree by keeping the tree balanced
// precondition: none
// postcondition: new node with the given key is inserted
// and the tree is kept balanced, node heights correctly updated
template <class T>
void AVLTree<T>::insert(T key)
{
    std::vector<AVLNode<T>*> path;
    
    // unbalanced insert
    insertnb(key,path);
    
    // rebalance the tree by rebalancing
    // the traversed nodes during insertion
    rebalance(path);
}

// insert a new key into the tree without balancing the tree
// precondition: none
// postcondition: new node with the given key is inserted
template <class T>
void AVLTree<T>::unbalanced_insert(T key)
{
    std::vector<AVLNode<T>*> path;
    
    // unbalanced insert
    insertnb(key,path);
    
    AVLNode<T>* node     = nullptr;
    while (!path.empty())
    {
        // get a node from the traversed path
        node = path.back();
        path.pop_back();
        
        (void)compute_node_balance(node);
        
        update_height_node(node);
    }
}

// find a key in the tree
// precondition: none
// postcondition: return the pointer to the node if the key is found,
// otherwise return a nullptr if the key is not found
template <class T>
AVLNode<T>* AVLTree<T>::find(T key)
{
    if ( is_empty() )
        return nullptr;
    
    AVLNode<T>* node = root;
    
    // tree traversal
    while (node)
    {
        if (key > node->key)
            node = node->right;
        else if (key < node->key)
            node = node->left;
        else
        // key found!
            return node;
    }
    
    return nullptr;
}

// remove an element from the tree by keeping the tree balanced
// precondition: none
// postcondition: node with the given key is removed
// and the tree is kept balanced, node heights correctly updated
template <class T>
void AVLTree<T>::remove(T key)
{
    std::vector<AVLNode<T>*> path;
    
    // unbalanced remove
    removenb(key,path);
    
    // rebalance the tree by rebalancing
    // the traversed nodes during removal
    rebalance(path);
}

// remove an element from the tree without balancing the tree
// precondition: none
// postcondition: node with the given key is removed
// and the tree is kept balanced, node heights correctly updated
template <class T>
void AVLTree<T>::unbalanced_remove(T key)
{
    std::vector<AVLNode<T>*> path;
    
    // unbalanced insert
    removenb(key,path);
    
    AVLNode<T>* node     = nullptr;
    while (!path.empty())
    {
        // get a node from the traversed path
        node = path.back();
        path.pop_back();
        
        (void)compute_node_balance(node);
        
        update_height_node(node);
    }
}


// check whether the tree is balanced
// precondition: none
// postcondition: returns true if balance is not violated, false otherwise
template <class T>
bool AVLTree<T>::is_balanced() const
{
    if ( is_empty() )
        return true;
    
    // check each node by breadth first traversal
    std::vector<AVLNode<T>*> q;
    q.push_back(root);
    while (!q.empty())
    {
        AVLNode<T>* node = q.front();
        q.erase(q.begin());
        if (node->left)
            q.push_back(node->left);
        if (node->right)
            q.push_back(node->right);
        
        int balance = compute_node_balance(node);
        if ( (balance < -1) || (balance > 1))
            return false;
    }
    
    return true;
}

// free all nodes of the tree
// precondition: none
// postcondition: all nodes freed and root set to nullptr
template <class T>
void AVLTree<T>::clear()
{
    if ( is_empty() )
        return;
    
    // delete each node by breadth first traversal
    std::vector<AVLNode<T>*> q;
    q.push_back(root);
    while (!q.empty())
    {
        AVLNode<T>* node = q.front();
        q.erase(q.begin());
        if (node->left)
            q.push_back(node->left);
        if (node->right)
            q.push_back(node->right);
        delete node;
    }
    
    root = nullptr;
}

// update the height attribute of an AVL node
// after the node has been affected by a tree manipulation
// precondition: valid node pointer is given
// postcondition: height updated
template <class T>
void AVLTree<T>::update_height_node(AVLNode<T> *node)
{
    // the node has 2 children
    if (node->right && node->left)
        if (node->left->height > node->right->height)
            node->height = node->left->height + 1;
        else
            node->height = node->right->height + 1;
    // the node has only 1 child
    else if (node->left)
        node->height = node->left->height + 1;
    else if (node->right)
        node->height = node->right->height + 1;
    // the node has no child
    else
        node->height = 1;
}

// insert a new key into the tree without balancing and updating heights
// precondition: none
// postcondition: new node with the given key is inserted, the
// resulting tree might be unbalanced, the heights are not updated
template <class T>
void AVLTree<T>::insertnb(T key, std::vector<AVLNode<T>*>& path)
{
    if ( is_empty() )
    {
        root = new_node(key);
        return;
    }
    
    AVLNode<T>* parent = root;
    AVLNode<T>* node   = root;
    
    // tree traversal
    while (node)
    {
        parent = node;
        path.push_back(parent);
        if (key > node->key)
            node = node->right;
        else if (key < node->key)
            node = node->left;
        else
        // key already present!
        {
            path.clear();
            return;
        }
    }
    
    // insert node
    if (key > parent->key)
        parent->right = new_node(key);
    else
        parent->left = new_node(key);
}

// remove a key from the tree without balancing and updating heights
// precondition: none
// postcondition: existing node with the given key is removed, the
// resulting tree might be unbalanced, the heights are not updated
template <class T>
void AVLTree<T>::removenb(T key, std::vector<AVLNode<T>*>& path)
{
    if ( is_empty() )
        return;
    
    AVLNode<T>* parent = root;
    AVLNode<T>* node   = root;
    bool found = false;
    
    // tree traversal
    while (node && !found)
    {
        parent = node;
        path.push_back(parent);
        if (key > node->key)
            node = node->right;
        else if (key < node->key)
            node = node->left;
        else
        // key found!
            found = true;
    }
    
    if (!found)
    {
        path.clear();
        return;
    }
    else
    {
        // remove the node from the path
        path.pop_back();
        
        // check whether the node is the root
        if (node == root)
            parent = nullptr;
        else
            // take pointer to parent node
            parent = path.back();
        
        // cut off the node from the tree
        cut_off_node(node,parent,path);
    }
}

// cut off a node from the tree
// precondition: none
// postcondition: all the references to the given node are removed from the
// parent, the child with the greater height or key is promoted as the
// parent's child and the sibling is reinserted into the tree
template <class T>
void AVLTree<T>::cut_off_node(AVLNode<T>* node, AVLNode<T>* parent, std::vector<AVLNode<T>*>& path)
{
    // no children?
    if (!node->left && !node->right)
    {
        // update root
        if (!parent)
            root = nullptr;
        // update parent node
        else if (parent->left == node)
            parent->left  = nullptr;
        else
            parent->right = nullptr;
        
        delete node;
    }
    // no right child
    else if (node->left && !node->right)
    {
        // update root
        if (!parent)
            root = node->left;
        // update parent node
        else if (parent->left == node)
            parent->left  = node->left;
        else
            parent->right = node->left;
        
        delete node;
    }
    // no left child
    else if (!node->left && node->right)
    {
        // update root
        if (!parent)
            root = node->right;
        // update parent node
        else if (parent->left == node)
            parent->left  = node->right;
        else
            parent->right = node->right;
        
        delete node;
    }
    // both children are there
    else
    {
        AVLNode<T>* child    = nullptr;
        AVLNode<T>* sibling  = nullptr;
        
        // decide which node to promote
        if (node->left->height > node->right->height)
        {
            child    = node->left;
            sibling  = node->right;
        }
        else if (node->left->height < node->right->height)
        {
            child    = node->right;
            sibling  = node->left;
        }
        else if (node->left->key > node->right->key)
        {
            child    = node->left;
            sibling  = node->right;
        }
        else
        {
            child    = node->right;
            sibling  = node->left;
        }
        
        // update root
        if (!parent)
            root = child;
        // update parent node
        else if (parent->left == node)
            parent->left  = child;
        else
            parent->right = child;
        
        delete node;
        
        // insert sibling starting from the child
        node = child;
        while (node)
        {
            parent = node;
            path.push_back(parent);
            if (sibling->key > node->key)
                node = node->right;
            else if (sibling->key < node->key)
                node = node->left;
        }
        
        // insert node
        if (sibling->key > parent->key)
            parent->right = sibling;
        else
            parent->left  = sibling;
    }
}

// return the balance factor of a node as the difference
// between the left and right subtree heights
// precondition: valid node pointer is given
// postcondition: balance factor calculated
template <class T>
int AVLTree<T>::compute_node_balance(AVLNode<T>* node) const
{
    // no node?
    if (!node)
        return 0;
    // the node has 2 children
    else if (node->left && node->right)
        node->balance = node->left->height - node->right->height;
    // the node has only 1 child
    else if (node->left)
        node->balance = node->left->height;
    else if (node->right)
        node->balance = -node->right->height;
    // the node has no child
    else
        return node->balance = 0;
    
    return node->balance;
}

// rebalance the tree by rebalancing the traversed nodes during insertion
// precondition: none
// postcondition: tree rebalanced and heights updated
template <class T>
void AVLTree<T>::rebalance(std::vector<AVLNode<T>*>& path)
{
    AVLNode<T>* node     = nullptr;
    AVLNode<T>* new_node = nullptr;
    
    while (!path.empty())
    {
        // get a node from the traversed path
        node = path.back();
        path.pop_back();
        
        // get its previous node
        AVLNode<T>* parent = nullptr;
        if (!path.empty())
            parent = path.back();
        
        int balance = compute_node_balance(node);
        
        // check if the node is unbalanced to the left
        if (balance > 1)
        {
            // rebalance to the right
            new_node = rebalance_to_right(node,parent);
            // insert back new node and old node to be checked again
            // after rotation
            path.push_back(new_node);
            path.push_back(node);
        }
        else if (balance < -1)
        {
            // rebalance to the left
            new_node = rebalance_to_left(node,parent);
            // insert back new node and old node to be checked again
            // after rotation
            path.push_back(new_node);
            path.push_back(node);
        }
        else
            new_node = node;
        
        update_height_node(new_node);
    }
}

// rebalance to right. Perform either a rotate right or a left-right rotation
// precondition: valid node and parent pointers are given
// postcondition: return the new node after the rotation
template <class T>
AVLNode<T>* AVLTree<T>::rebalance_to_right(AVLNode<T>* node, AVLNode<T>* parent)
{
    AVLNode<T>* new_node = nullptr;
    
    int balance_child = compute_node_balance(node->left);
    
    // check first if child node is unbalanced to the right
    // then a left-right rotation is needed
    if (balance_child < 0)
    {
        // perform first left rotation of child node
        node->left = rotate_left(node->left);
    }
    
    new_node = rotate_right(node);
    
    // if this is not the root
    // then the previous node needs an update
    if (parent)
        if (parent->left == node)
            parent->left  = new_node;
        else
            parent->right = new_node;
    else
        root = new_node;
    
    return new_node;
}

// rebalance to left. Perform either a rotate left or a right-left rotation
// precondition: valid node and parent pointers are given
// postcondition: return the new node after the rotation
template <class T>
AVLNode<T>* AVLTree<T>::rebalance_to_left(AVLNode<T>* node, AVLNode<T>* parent)
{
    AVLNode<T>* new_node = nullptr;
    
    int balance_child = compute_node_balance(node->right);
    
    // check first if child node is unbalanced to the left
    // then a right-left rotation is needed
    if (balance_child > 0)
    {
        // perform first right rotation of child node
        node->right = rotate_right(node->right);
    }
    
    new_node = rotate_left(node);
    
    // if this is not the root
    // then the previous node needs an update
    if (parent)
        if (parent->left == node)
            parent->left  = new_node;
        else
            parent->right = new_node;
    else
        root = new_node;
    
    return new_node;
}

// perform left rotation
// precondition: valid node pointer is given
// postcondition: return the new parent of the subtree where the right node of
// the input node becomes the new parent and the input node becomes its new
// left node
template <class T>
AVLNode<T>* AVLTree<T>::rotate_left(AVLNode<T>* node)
{
    AVLNode<T>* new_left   = node;
    AVLNode<T>* new_parent = node->right;
    
    // new left right takes over new parent left
    new_left->right = new_parent->left;
    
    // update new parent left with new left
    new_parent->left = new_left;
    
    // update new left height
    update_height_node(new_left);
    
    // update new parent height
    update_height_node(new_parent);
    
    return new_parent;
}

// perform right rotation
// precondition: valid node pointer is given
// postcondition: return the new parent of the subtree where the left node of
// the input node becomes the new parent and the input node becomes its new
// right node
template <class T>
AVLNode<T>* AVLTree<T>::rotate_right(AVLNode<T>* node)
{
    AVLNode<T>* new_right  = node;
    AVLNode<T>* new_parent = node->left;
    
    // new right left takes over new parent right
    new_right->left = new_parent->right;
    
    // update new parent right with new right
    new_parent->right = new_right;
    
    // update new right height
    update_height_node(new_right);
        
    // update new parent height
    update_height_node(new_parent);
    
    return new_parent;
}

}
#endif /* AVLTree_h */
