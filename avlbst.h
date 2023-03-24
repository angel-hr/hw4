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

protected:
    int8_t balance_;    // effectively a signed char
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
    Node<Key, Value>(key, value, parent), balance_(0)
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
    void rotate(AVLNode<Key,Value>* grandparent, AVLNode<Key,Value>* parent, bool direction);
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child);
    void removeFix(AVLNode<Key,Value>* parent, int8_t difference);
};

template<typename Key, typename Value>
void AVLTree<Key,Value>::rotate(AVLNode<Key,Value>* grandparent, AVLNode<Key,Value>* parent, bool direction){
   AVLNode<Key,Value>* greatgramp = grandparent->getParent();
   parent->setParent(greatgramp);
   grandparent->setParent(parent);
   
   if (greatgramp){ //if greatgramp is not equal to NULL
    if (greatgramp->getLeft() == grandparent){greatgramp->setLeft(parent);}
    else {greatgramp->setRight(parent);}
   }
   else {
    this->root_ = parent; //if greatgramp is null then parent will be the new root
   }

    if (direction){ //if rotate-right
        grandparent->setLeft(parent->getRight());
        if (parent->getRight() != NULL){
            parent->getRight()->setParent(grandparent); 
        }
        parent->setRight(grandparent);
    }
    else { //if rotate-left
        grandparent->setRight(parent->getLeft());
        if (parent->getLeft() != NULL){
            parent->getLeft()->setParent(grandparent);
        }
        parent->setLeft(grandparent);
    }
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* current)
{
    if (parent == NULL) {return;}
    AVLNode<Key,Value>* grandparent = parent->getParent();
    if (grandparent == NULL) {return;}
    if (grandparent->getLeft() == parent){grandparent->updateBalance(-1);}
    else {grandparent->updateBalance(1);}

    if (grandparent->getBalance() == 0) {return;}
    else if (grandparent->getBalance() == 1 || grandparent->getBalance() == -1){
        insertFix(grandparent, parent);
    }
    else if (grandparent->getBalance() == -2) { //if balance is 2 or -2 
        if (parent->getBalance() == -1){ //zig-zig rotate-right
            rotate(grandparent, parent, 1);
            parent->setBalance(0); grandparent->setBalance(0);
        }
        else {  //zig-zag - left
            rotate(parent, current, 0);  //left rotate
            rotate(grandparent, current, 1); //then right rotate
            if (current->getBalance() == 0){
                parent->setBalance(0); grandparent->setBalance(0);
            }
            else if (current->getBalance() == -1){
                current->setBalance(0); parent->setBalance(0); grandparent->setBalance(1);
            }
            else if (current->getBalance() == 1){
                current->setBalance(0); parent->setBalance(-1); grandparent->setBalance(0);
            }
        }
    }
    else if (grandparent->getBalance() == 2){
        if (parent->getBalance() == 1){ //zig-zig left rotate
            rotate(grandparent, parent, 0);
            parent->setBalance(0); grandparent->setBalance(0); //no need to set the balance of child because it's zero
        }
        else{ //parent balance is -1 which means zig-zag
            rotate(parent, current, 1); //rotate right
            rotate(grandparent, current, 0);   //rotate left
            if (current->getBalance() == 0){
                parent->setBalance(0); grandparent->setBalance(0);
            }
            else if (current->getBalance() == -1){
                current->setBalance(0); parent->setBalance(1); grandparent->setBalance(0);
            }
            else if (current->getBalance() == 1){
                current->setBalance(0); parent->setBalance(0); grandparent->setBalance(-1);
            }
        }
    }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{   // TODO
    if (this->root_ == NULL){
        this->root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
        return;
    } 
    
   AVLNode<Key, Value>* current = static_cast<AVLNode<Key,Value>*>(this->internalFind(new_item.first)); //see if the Node with the key exists
   if (current != NULL) {current->setValue(new_item.second); return;}

   AVLNode<Key,Value>* avl = static_cast<AVLNode<Key,Value>*>(this->root_);
   bool found = false;
   while (!found){//while we haven't found the place to insert the value
    if (new_item.first < avl->getKey()){
        if (avl->getLeft() == NULL){ //if there is no left value
            found = true;
            current = new AVLNode<Key,Value>(new_item.first, new_item.second, avl);
            avl->setLeft(current); 
            if (avl->getBalance() != 0) {avl->setBalance(0);}
            else {
                avl->updateBalance(-1); 
                insertFix(avl, current);
            }
        }
        else {avl = avl->getLeft();}
    }
    else { //if the key is larger than key of current bst node
        if (avl->getRight() == NULL){
            found = true;
            current = new AVLNode<Key,Value>(new_item.first, new_item.second, avl);
            avl->setRight(current); 
            if (avl->getBalance() != 0) {avl->setBalance(0);}
            else{
                avl->updateBalance(1); 
                insertFix(avl, current);
            }
        }
        else {avl = avl->getRight();}
    }
   }
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* parent, int8_t difference) 
{
    if (parent == NULL){return;}
    int8_t nextdiff = 0;
    AVLNode<Key,Value>* grandparent = parent->getParent();
    if (grandparent){
        if (grandparent->getLeft() == parent){nextdiff = 1;}
        else {nextdiff =  -1;}
    }
    
    parent->updateBalance(difference); //this is b(n) + diff 
    if (parent->getBalance() == 0){removeFix(grandparent, nextdiff);}
    else if (parent->getBalance() == -2){
        AVLNode<Key,Value>* taller_child = parent->getLeft(); //because left side is heavier
        if(taller_child->getBalance() == -1){ //zig zig
            rotate(parent, taller_child, 1);
            parent->setBalance(0); taller_child->setBalance(0);
            removeFix(grandparent, nextdiff);
        }
        else if (taller_child->getBalance() == 0){
            rotate(parent, taller_child, 1);
            parent->setBalance(-1); taller_child->setBalance(1);
        }
        else if (taller_child->getBalance() == 1){
            AVLNode<Key,Value>* grandchild = taller_child->getRight();
            rotate(taller_child, grandchild, 0); //left rotate
            rotate(parent, grandchild, 1); //followed by right rotate , grandchild now occupies the location that taller_child held
            if (grandchild->getBalance() == 0){parent->setBalance(0); taller_child->setBalance(0);}
            else if (grandchild->getBalance() == -1){parent->setBalance(1); taller_child->setBalance(0); grandchild->setBalance(0);}
            else if (grandchild->getBalance() == 1){parent->setBalance(0); taller_child->setBalance(-1); grandchild->setBalance(0);}
            removeFix(grandparent, nextdiff);
        }
    }
    else if (parent->getBalance() == 2){
        AVLNode<Key,Value>* taller_child = parent->getRight();
        if (taller_child->getBalance() == 1){
            rotate(parent, taller_child, 0);
            parent->setBalance(0); taller_child->setBalance(0);
            removeFix(grandparent, nextdiff);
        }
        else if (taller_child->getBalance() == 0){
            rotate(parent, taller_child, 0);
            parent->setBalance(1); taller_child->setBalance(-1);
        }
        else if (taller_child->getBalance() == -1){
            AVLNode<Key,Value>* grandchild = taller_child->getLeft();
            rotate(taller_child, grandchild, 1); //right rotate
            rotate(parent, grandchild, 0);  //followed by left rotate
            if (grandchild->getBalance() == 0){ parent->setBalance(0); taller_child->setBalance(0);}
            else if (grandchild->getBalance() == 1){ parent->setBalance(-1); taller_child->setBalance(0); grandchild->setBalance(0);}
            else if (grandchild->getBalance() == -1) {parent->setBalance(0); taller_child->setBalance(1); grandchild->setBalance(0);}
            removeFix(grandparent, nextdiff);
        }
    }
    //if updated parent balance is 1 or -1 then don't do anything since it doesn't affect the rest
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{   // TODO
    AVLNode<Key,Value>* current = 
    static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if (current == NULL) {return;} //if they key we are trying to remove is not in the tree return
    if (current == this->root_){
        
    }

    AVLNode<Key,Value>* parent = current->getParent();

    int8_t diff = 0;
    if (parent != NULL){
        if (parent->getLeft() == current){diff = 1;}  // we are removing to from the left so it is like adding to the right
        else {diff = -1;}
    }

    if (current->getLeft() != NULL && current->getRight() == NULL){
        this->removeHelper(parent, current, current->getLeft()); 
    }
    else if (current->getLeft() == NULL && current->getRight() != NULL){
        this->removeHelper(parent, current, current->getRight());
    }
    else if (current->getLeft() == NULL && current->getRight() == NULL){
        this->removeHelper(parent, current, NULL);
    }
    else { //if current has two children
        AVLNode<Key,Value>* pred = 
        static_cast<AVLNode<Key,Value>*>(this->predecessor(current));
        nodeSwap(current, pred);
        parent = current->getParent(); //current might have a new parent
        if (parent->getLeft() == current){diff = 1;} 
        else {diff = -1;}
        this->removeHelper(parent, current, current->getLeft()); //left could be null or another child
    }

    delete current;
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


#endif
