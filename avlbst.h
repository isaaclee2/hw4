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

    void left_rotate(AVLNode<Key,Value>* node);
    void right_rotate(AVLNode<Key,Value>* node);
    void insert_AVL(AVLNode<Key,Value>* prev, AVLNode<Key,Value>* new_node);
    void remove_AVL(AVLNode<Key,Value>* node, int n);

};

template<class Key, class Value>
void AVLTree<Key, Value>::left_rotate(AVLNode<Key,Value>* node){
  AVLNode<Key,Value>* right = node->getRight();
  node->setRight(right->getLeft());
  if(right->getLeft() != nullptr){
    right->getLeft()->setParent(node);
  }
  right->setParent(node->getParent());

  if(node->getParent() == nullptr){
    this->root_ = right;
  }
  else if(node->getParent()->getLeft() == node){
    node->getParent()->setLeft(right);
  }
  else{
    node->getParent()->setRight(right);
  }
  right->setLeft(node);
  node->setParent(right);
}

template<class Key, class Value>
void AVLTree<Key, Value>::right_rotate(AVLNode<Key,Value>* node){
  AVLNode<Key,Value>* left = node->getLeft();
  node->setLeft(left->getRight());
  if(left->getRight() != nullptr){
    left->getRight()->setParent(node);
  }
  left->setParent(node->getParent());

  if(node->getParent() == nullptr){
    this->root_ = left;
  }
  else if(node->getParent()->getLeft() == node){
    node->getParent()->setLeft(left);
  }
  else{
    node->getParent()->setRight(left);
  }
  left->setRight(node);
  node->setParent(left);
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
  // empty tree
  if(this->root_ == nullptr){
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    return;
  }
  // find right position
  AVLNode<Key, Value>* curr_node = static_cast<AVLNode<Key, Value>*>(this->root_);
  AVLNode<Key, Value>* prev_node = nullptr;
  
  while(curr_node != nullptr){
    prev_node = curr_node;
    if(new_item.first == curr_node->getKey()){
      curr_node->setValue(new_item.second);
      return;
    }
    else if(curr_node->getKey() > new_item.first){
      curr_node = curr_node->getLeft();
    }
    else{
      curr_node = curr_node->getRight();
    }
  }
  // make new node, add, adjust
  AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, prev_node);
  if(new_item.first < prev_node->getKey()){
    prev_node->setLeft(new_node);
  }
  else{
    prev_node->setRight(new_node);
  }

  int balance = prev_node->getBalance();
  if(balance == -1 ||balance == 1){
    prev_node->setBalance(0);
  }
  else{
    if(prev_node->getLeft() == new_node){
      prev_node->setBalance(1);
    }
    else{
      prev_node->setBalance(-1);
    }
    insert_AVL(prev_node, new_node);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_AVL(AVLNode<Key,Value>* prev, AVLNode<Key,Value>* new_node){
  if(prev == nullptr || prev->getParent() == nullptr){
    return;
  }
  AVLNode<Key, Value>* prevprev = prev->getParent();
  if(prevprev->getLeft() == prev){ // parent is left of grandparent
    prevprev->updateBalance(1);
    if(prevprev->getBalance() == 2){
      if(prev->getLeft() == new_node){ // zig zig
        right_rotate(prevprev);
        prev->setBalance(0);
        prevprev->setBalance(0);
      }
      else{ // zig zag
        left_rotate(prev);
        right_rotate(prevprev);
        if(new_node->getBalance() == 0){
          prev->setBalance(0);
          prevprev->setBalance(0);
        }
        else if(new_node->getBalance() == 1){
          prev->setBalance(0);
          prevprev->setBalance(-1);
        }
        else{
          prev->setBalance(1);
          prevprev->setBalance(0);
        }
        new_node->setBalance(0);
      }
    }
    else if(prevprev->getBalance() == 1){
      insert_AVL(prevprev, prev);
    }
    else if(prevprev->getBalance() == 0){
      return;
    }
  }
  else{
    prevprev->updateBalance(-1);
    if(prevprev->getBalance() == -2){
      if(prev->getRight() == new_node){ // zig zig
        left_rotate(prevprev);
        prev->setBalance(0);
        prevprev->setBalance(0);
      }
      else{
        //zig zag
        right_rotate(prev);
        left_rotate(prevprev);
        if(new_node->getBalance() == 0){
          prev->setBalance(0);
          prevprev->setBalance(0);
        }
        else if(new_node->getBalance() == -1){
          prev->setBalance(0);
          prevprev->setBalance(1);
        }
        else{
          prev->setBalance(-1);
          prevprev->setBalance(0);
        }
        new_node->setBalance(0);
      }
    }
    else if(prevprev->getBalance() == -1){
      insert_AVL(prevprev, prev);
    }
    else if(prevprev->getBalance() == 0){
      return;
    }
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>::remove_AVL(AVLNode<Key,Value>* node_to_remove, int n){
  //TODO
  if(node_to_remove == nullptr){
    return;
  }

  AVLNode<Key,Value>* prev = node_to_remove->getParent();
  int d = 0;
  if(prev != nullptr){
    if(prev->getLeft() != node_to_remove){
      d = 1;
    }
    else{
      d = -1;
    }
  }

  if(n == 1){
    if(node_to_remove->getBalance() + n == 2){
      AVLNode<Key,Value>* taller_child_node = node_to_remove->getLeft();
      if(taller_child_node->getBalance() == 1){
        right_rotate(node_to_remove);
        node_to_remove->setBalance(0);
        taller_child_node->setBalance(0);
        remove_AVL(prev, d);
      }
      else if(taller_child_node->getBalance() == 0){
        right_rotate(node_to_remove);
        node_to_remove->setBalance(1);
        taller_child_node->setBalance(-1);
      }
      else{
        AVLNode<Key,Value>* childchild = taller_child_node->getRight();
        left_rotate(taller_child_node);
        right_rotate(node_to_remove);
        if(childchild->getBalance() == 0){
          node_to_remove->setBalance(0);
          taller_child_node->setBalance(0);
        }
        else if(childchild->getBalance() == 1){
          node_to_remove->setBalance(-1);
          taller_child_node->setBalance(0);
        }
        else{
          node_to_remove->setBalance(0);
          taller_child_node->setBalance(1);
        }

        childchild->setBalance(0);
        remove_AVL(prev, d);
      }
    }
    else if(node_to_remove->getBalance() + n == 1){
      node_to_remove->setBalance(1);
    }
    else{
      node_to_remove->setBalance(0);
      remove_AVL(prev, d);
    }
  }
  else{
    if(node_to_remove->getBalance() + n == -2){
      AVLNode<Key,Value>* taller_child_node = node_to_remove->getRight();
      if(taller_child_node->getBalance() == -1){
        left_rotate(node_to_remove);
        node_to_remove->setBalance(0);
        taller_child_node->setBalance(0);
        remove_AVL(prev, d);
      }
      else if(taller_child_node->getBalance() == 0){
        left_rotate(node_to_remove);
        node_to_remove->setBalance(-1);
        taller_child_node->setBalance(1);
      }
      else{
        AVLNode<Key,Value>* childchild = taller_child_node->getLeft();
        right_rotate(taller_child_node);
        left_rotate(node_to_remove);
        if(childchild->getBalance() == 0){
          node_to_remove->setBalance(0);
          taller_child_node->setBalance(0);
        }
        else if(childchild->getBalance() == -1){
          node_to_remove->setBalance(1);
          taller_child_node->setBalance(0);
        }
        else{
          node_to_remove->setBalance(0);
          taller_child_node->setBalance(-1);
        }
        childchild->setBalance(0);
        remove_AVL(prev, d);
      }
    }
    else if(node_to_remove->getBalance() + n == -1){
      node_to_remove->setBalance(-1);
    }
    else{
      node_to_remove->setBalance(0);
      remove_AVL(prev, d);
    }
  }
}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  AVLNode<Key, Value>* node_to_remove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  if(node_to_remove == nullptr){
    return;
  }
  // if two children, swap with predecessor and remove
  if(node_to_remove->getLeft() != nullptr && node_to_remove->getRight() != nullptr){
    AVLNode<Key, Value>* predecessor_node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node_to_remove));
    nodeSwap(node_to_remove, predecessor_node);
  }
  // 1 or 0 children
  AVLNode<Key, Value>* prev = node_to_remove->getParent();
  AVLNode<Key, Value>* left = node_to_remove->getLeft();
  AVLNode<Key, Value>* right = node_to_remove->getRight();
  AVLNode<Key, Value>* child;
  if(left == nullptr){
    child = right;
  }
  else{
    child = left;
  }
  int n = 0;
  if(node_to_remove->getParent() != nullptr){
    if(prev->getLeft() != node_to_remove){
      n = 1;
    }
    else{
      n = -1;
    }
  }
  if(prev == nullptr){
    this->root_ = child;
  }
  else if(prev->getLeft() != node_to_remove){
    prev->setRight(child);
  }
  else{
    prev->setLeft(child);
  }
  if(child != nullptr){
    child->setParent(prev);
  }
  delete node_to_remove;
  remove_AVL(prev, n);
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
