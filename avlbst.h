#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

    int getHeight() const
    {
        return height;
    }

protected:
    int8_t balance_;    // effectively a signed char
    int height;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0), height(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO

protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rightRotate(AVLNode<Key, Value>* node);
    void leftRotate(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
    //TODO
{
    if (this->root_ == NULL) {
        this->root_ =  new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        dynamic_cast<AVLNode<Key,Value>*>(this->root_)->setBalance(0);
        return;
    }

    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

        while (temp != NULL)
        {
            parent = temp;
            if (new_item.first == temp->getKey())
            {
                temp->setValue(new_item.second);
                return;
            }
            else
            {
                if(new_item.first < temp->getKey())
                {
                    temp = temp->getLeft();
                }
                else if (new_item.first > temp->getKey())
                {
                    temp = temp->getRight();
                }
            }
        }
        
        AVLNode<Key, Value>* newN = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
        newN->setBalance(0);
        if (new_item.first < parent->getKey()) 
        {
            parent->setLeft(newN);
            parent->updateBalance(-1);
        } 
        else 
        {
            parent->setRight(newN);
            parent->updateBalance(1);
        }
        //std::cout << "before fix" << std::endl;
        //this->print();
        if(parent->getBalance()) insertFix(parent, newN);
        //std::cout << "after fix" << std::endl;
        //this->print();
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if (BinarySearchTree<Key, Value>::internalFind(key) == NULL)
    {
        return;
    }

    AVLNode<Key, Value>* remove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if (remove == NULL){
        return;
    }

    if (remove->getLeft() != NULL && remove->getRight() != NULL)
    {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(remove));
        nodeSwap(remove, pred);
    }
    AVLNode<Key, Value>* parent = remove->getParent();
    int diff = 0;
    if (parent != NULL)
    {
        if (remove == parent->getLeft())
        {
            diff = 1;
        }
        else
        {
            diff = -1;
        }
    }

    AVLNode<Key, Value>* child = NULL;

    if(remove->getRight() != NULL)
    {
        child = remove->getRight();
    }
    else if (remove->getLeft() != NULL)
    {
        child = remove->getLeft();
    }

    if (child == NULL)
    {
        if(parent == NULL)
        {
            this->root_ = NULL;
        }
        else if (remove == parent->getRight())
        {
            parent->setRight(child);
        }
        else if (remove == parent->getLeft())
        {
            parent->setLeft(child);
        }
    }
    else
    {
        if (parent == NULL)
        {
            this->root_ = child;
            child->setParent(NULL);
        }
        else if (remove == parent->getRight())
        {
            child->setParent(parent);
            parent->setRight(child);
        }
        else if (remove == parent->getLeft())
        {
            child->setParent(parent);
            parent->setLeft(child);
        }
    }
    
    delete remove;
    removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* node)
//this should be correct now
{
    if (node == NULL)
    {
        return;
    }
    AVLNode<Key, Value>* x = node;
    AVLNode<Key, Value>* left = node->getLeft();
    AVLNode<Key, Value>* leftRight = left->getRight();
    AVLNode<Key, Value>* parent = node->getParent();

    node = left;
    node->setParent(parent);
    node->setRight(x);
    x->setParent(node);
    x->setLeft(leftRight);

    if (leftRight)
    {
        leftRight->setParent(x);
    }
    if(parent == nullptr)
    {
        this->root_ = node;
    }
    else
    {
        if(node->getKey() < parent->getKey())
        {
            parent->setLeft(node);
        }
        else
        {
            parent->setRight(node);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* node)
//this should be correct now
{
    if (node == NULL)
    {
        return;
    }
    AVLNode<Key, Value>* x = node;
    AVLNode<Key, Value>* right = node->getRight();
    AVLNode<Key, Value>* rightLeft = right->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();

    node = right;
    node->setParent(parent);
    node->setLeft(x);
    x->setParent(node);
    x->setRight(rightLeft);

    if (rightLeft)
    {
        rightLeft->setParent(x);
    }
    if(parent == nullptr)
    {
        this->root_ = node;
    }
    else
    {
        if(node->getKey() < parent->getKey())
        {
            parent->setLeft(node);
        }
        else
        {
            parent->setRight(node);
        }
    }
}


template <class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node)
{
    if (parent == NULL || parent->getParent() == NULL)
    {
        return;
    }
    
    else
    {
        AVLNode<Key, Value>* grandparent = parent->getParent();
        // std::cout << grandparent->getKey() << " " << (int)grandparent->getBalance() << std::endl;
        // std::cout << parent->getKey() << std::endl;
        if (parent == grandparent->getLeft())
        {
            //::cout << "left imbalance?" << std::endl;
            grandparent->setBalance(grandparent->getBalance() - 1);
            if (grandparent->getBalance() == 0)
            {
                //std::cout << "balanced" << std::endl;
                return;
            }
            else if (grandparent->getBalance() == -1)
            {
                //std::cout << "recurse" << std::endl;
                insertFix(grandparent, parent);
            }
            else if (grandparent->getBalance() == -2)
            {
                //zigzig case
                //std::cout << "zigzig" << std::endl;
                if (parent->getLeft() == node)
                {
                    rightRotate(grandparent);
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                }
                //zigzag
                else 
                {
                    //std::cout << "zigzag" << std::endl;
                    leftRotate(parent);
                    rightRotate(grandparent);
                    if (node->getBalance() == -1)
                    {
                        parent->setBalance(0);
                        grandparent->setBalance(1);
                        node->setBalance(0);
                    }
                    else if (node->getBalance() == 0)
                    {
                        parent->setBalance(0);
                        grandparent->setBalance(0);
                        node->setBalance(0);
                    }
                    else if (node->getBalance() == 1)
                    {
                        parent->setBalance(-1);
                        grandparent->setBalance(0);
                        node->setBalance(0);
                    }
                }
            }
        }
        
        else
        {
            //std::cout<< "right imbalance?" << std::endl;
            grandparent->setBalance(grandparent->getBalance() + 1);
            if (grandparent->getBalance() == 0)
            {
                //std::cout << "balanced" << std::endl;
                return;
            }
            else if (grandparent->getBalance() == 1)
            {
                //std::cout << "recurse" << std::endl;
                insertFix(grandparent, parent);
            }
            else if (grandparent->getBalance() == 2)
            {
                //zigzig case
                if (parent->getRight() == node)
                {
                    ///std::cout << "zigzig" << std::endl;
                    leftRotate(grandparent);
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                }
                //zigzag
                else 
                {
                    //std::cout << "zigzag" << std::endl;
                    rightRotate(parent);
                    leftRotate(grandparent);
                    if (node->getBalance() == 1)
                    {
                        parent->setBalance(0);
                        grandparent->setBalance(-1);
                        node->setBalance(0);
                    }
                    else if (node->getBalance() == 0)
                    {
                        parent->setBalance(0);
                        grandparent->setBalance(0);
                        node->setBalance(0);
                    }
                    else if (node->getBalance() == -1)
                    {
                        parent->setBalance(1);
                        grandparent->setBalance(0);
                        node->setBalance(0);
                    }
                }
            }
        }
    }

}

template <class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff)
{
    if (node == NULL)
    {
        return;
    }
    AVLNode<Key, Value>* parent = node->getParent();
    int ndiff = 0;
    if (parent != NULL)
    {
        if (parent->getLeft() == node)
        {
            ndiff = 1;
        }
        else
        {
            ndiff = -1;
        }

    }

    if (diff == -1)
    {
        if((node->getBalance() + diff) == -2)
        {
            AVLNode<Key, Value>* c = node->getLeft();
            if (c->getBalance() == -1)
            {
                rightRotate(node);
                node->setBalance(0);
                c->setBalance(0);

                removeFix(parent, ndiff);
            }

            else if(c->getBalance() == 0)
            {
                rightRotate(node);
                node->setBalance(-1);
                c->setBalance(1);
            }
            else if (c->getBalance() == 1)
            {
                AVLNode<Key, Value>* g = c->getRight();
                leftRotate(c);
                rightRotate(node);

                if (g->getBalance() == 1)
                {
                    node->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0)
                {
                    node->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == -1)
                {
                    node->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(parent, ndiff);
            }
        }
        else if ((node->getBalance() + diff) == -1)
        {
            node->setBalance(-1);
        }
        else if ((node->getBalance() + diff) == 0)
        {
            node->setBalance(0);
            removeFix(parent, ndiff);
        }
    }


    else if (diff == 1)
    {
        if((node->getBalance() + diff) == 2)
        {
            AVLNode<Key, Value>* c = node->getRight();
            if (c->getBalance() == 1)
            {
                leftRotate(node);
                node->setBalance(0);
                c->setBalance(0);

                removeFix(parent, ndiff);
            }

            else if(c->getBalance() == 0)
            {
                leftRotate(node);
                node->setBalance(1);
                c->setBalance(-1);
            }
            else if (c->getBalance() == -1)
            {
                AVLNode<Key, Value>* g = c->getLeft();
                rightRotate(c);
                leftRotate(node);

                if (g->getBalance() == -1)
                {
                    node->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0)
                {
                    node->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 1)
                {
                    node->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(parent, ndiff);
            }
        }
        else if ((node->getBalance() + diff) == 1)
        {
            node->setBalance(1);
        }
        else if ((node->getBalance() + diff) == 0)
        {
            node->setBalance(0);
            removeFix(parent, ndiff);
        }
    }

}

#endif
