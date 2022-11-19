#ifndef RBBST_H
#define RBBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
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
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* node); //rotates node to the left
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* node); //rotates node to the right
    bool isLeftChild(AVLNode<Key, Value>* node); //returns true if node is the left child of it's parent
    bool isRightChild(AVLNode<Key, Value>* node); //returns true if node is the right child of it's parent
    AVLNode<Key, Value>* insertHelper(AVLNode<Key, Value>* root, const std::pair<const Key, Value> &item, AVLNode<Key, Value>* parent);
    int height(AVLNode<Key, Value>* root); //calculates height of given node 
    AVLNode<Key, Value>* balanceTree(AVLNode<Key, Value>* root, const std::pair<const Key, Value> &item); //balances tree 


};

template<class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* root)
{
  if (root == nullptr) return 0;
  int leftHeight = height(root->getLeft());
  int rightHeight = height(root->getRight());
  return std::max(leftHeight, rightHeight) + 1;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insertHelper(AVLNode<Key, Value>* root, const std::pair<const Key, Value> &item, AVLNode<Key, Value>* parent)
{ 
      // insert node in BST fashion
    if (root == nullptr){
      AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(item.first, item.second, parent);
      return newNode;
    }
    if (item.first < root->getItem().first) //item to be inserted is less than root
        root->setLeft( insertHelper(root->getLeft(), item, root) );
      
    else if (item.first > root->getItem().first)  //item to be inserted is greater than root 
        root->setRight( insertHelper(root->getRight(), item, root) );
      
    else if (item.first == root->getItem().first) // item to be inserted is equal to root
        root->setValue(item.second);  //overwrite the value with newest value
  
    //update balance factor after each insertion 
    root->setBalance(height(root->getLeft()) - height(root->getRight())); //balance factor is LH - RH
    int balance = root->getBalance(); 

  if (balance == 2 || balance == -2){
      return balanceTree(root, item); //updated root after balance
  } 
  
    return root; //root did not change

}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::balanceTree(AVLNode<Key, Value>* root, const std::pair<const Key, Value> &item)
{
    //if balance is 2(left heavy) and this newly inserted node is in the left subtree, then this is a LEFT LEFT 
    if (root->getBalance() == 2 && item.first < root->getLeft()->getItem().first){
        return rotateRight(root);
    }

  //if balance is -2(right heavy) and this newly inserted node is in the in right rubstree, then this is a RIGHT RIGHT 
    else if (root->getBalance() == -2 && item.first > root->getRight()->getItem().first){
        return rotateLeft(root);
    }
        
  //if balance is 2(left heavy) and this newly inserted node is in the right subtree, then this is a LEFT RIGHT
   else  if (root->getBalance() == 2 && item.first > root->getLeft()->getItem().first){
        root->setLeft( rotateLeft(root->getLeft()) );
        return rotateRight(root);
    }

  //if balance is -2(right heavy) and this newly inserted node is in the left subtree, then this is a RIGHT LEFT
   else if (root->getBalance() == -2 && item.first < root->getRight()->getItem().first){
        root->setRight(rotateRight(root->getRight()));
    }
     return rotateLeft(root);
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  AVLNode<Key, Value> * root = static_cast<AVLNode<Key, Value>*>(this->root_);
  if( this->root_ == nullptr){ //if empty tree
    root  = insertHelper(static_cast<AVLNode<Key, Value>*>(this->root_), new_item, nullptr);
    this->root_ = root;
  }
  else {
    root = insertHelper(static_cast<AVLNode<Key, Value>*>(this->root_), new_item, nullptr);
    this->root_ = root;
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* root = static_cast<AVLNode<Key, Value>*>(this->root_);

  //BST Remove
  AVLNode<Key, Value>* keyToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  if (keyToRemove == nullptr) return; 
  AVLNode<Key, Value>* parent = keyToRemove->getParent(); // Parent of node to be deleted  
  AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(keyToRemove));
  bool isRoot = (keyToRemove == root); //if deleted node was the root, the bst root needs to be updated 
  bool leftChild = isLeftChild(keyToRemove); //stores if node is left child or not.

  if (pred == nullptr){ 
    //if pred does not exist we are promoting the right child
    AVLNode<Key, Value>* childToPromote = keyToRemove->getRight();
    
    if (isRoot){
      root = childToPromote;
      if (childToPromote != nullptr) childToPromote->setParent(nullptr);
    }else{
      AVLNode<Key, Value>* keyToRemoveParent = keyToRemove->getParent(); 
      if (keyToRemove == keyToRemoveParent->getLeft()) keyToRemoveParent->setLeft(childToPromote);
      if (keyToRemove == keyToRemoveParent->getRight()) keyToRemoveParent->setRight(childToPromote); 
      if (childToPromote != nullptr) childToPromote->setParent(parent);
    }
    delete keyToRemove;
  }else { 
    //if pred does exist we are swapping pred with keyToRemove and updating keyToRemove's parent 
    nodeSwap(keyToRemove, pred);
    AVLNode<Key, Value>* childToPromote = keyToRemove->getLeft();
    
    if (isRoot) root = pred;
    
    AVLNode<Key, Value>* keyToRemoveParent = keyToRemove->getParent(); 
    if (childToPromote != nullptr) childToPromote->setParent(keyToRemoveParent);
    if (keyToRemove == keyToRemoveParent->getLeft()) keyToRemoveParent->setLeft(childToPromote);
    if (keyToRemove == keyToRemoveParent->getRight()) keyToRemoveParent->setRight(childToPromote); 
    delete keyToRemove;
  }
  if (root == nullptr){ //removed last node, no need to check balance.
    this->root_ = root;
    return;
  }

  //after node removal, update the balance of ancestors and rebalance tree if necessary
  if (isRoot) //rebalance is differnt for when the root node was just removed
  {
   root->setBalance(height(root->getLeft()) - height(root->getRight())); //update balance of new root
    if (root->getBalance() == 2 || root->getBalance() == -2){
      if (height(root->getLeft()) < height(root->getRight())) {//if root is right hevy, rotate left
        this->root_ = rotateLeft(root);
        return;
        }
      else{ 
        this->root_ = rotateRight(root); //if root is left hevy, rotate right
        return;
        }
    }
    this->root_ = root; //no need for rebalance
  }
  else // rebalance for non-root nodes
  {
  AVLNode<Key, Value>* temp = parent;
  
  while (temp != nullptr){
    temp->setBalance(height(temp->getLeft()) - height(temp->getRight())); //update balance of ancestor of deleted node

    if (temp->getBalance() == 2 || temp->getBalance() == -2){ //tree got unbalanced during delete & needs to be balanced
      if (leftChild) {
        if (temp == this->root_) this->root_ = rotateLeft(temp); //if root is getting changed after rebalancing 
        else rotateLeft(temp); //if deleted node was a left child, right rotation is needed.
        //break;
        }
      else{
        if (temp == this->root_) this->root_ = rotateRight(temp);
        else rotateRight(temp); 
       // break;
      } 
    }
    temp = temp->getParent(); //proceed to the next ancestor 
  }
    } // end of else
  
}

//returns the node with its new children and parent after rotation. set prev node = rotateLeft
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
      AVLNode<Key, Value>* temp = node->getRight();
      //if (temp == nullptr) return temp;
  
    node->setRight(temp->getLeft());
 
    // Update parent pointer of the
    // right child of the root node
    if (temp->getLeft() != NULL)
        temp->getLeft()->setParent(node);
 
    // Update the left child of the
    // temp to root
    temp->setLeft(node);
 
    // Update parent pointer of
    // the temp
    temp->setParent(node->getParent());
 
    // Update the parent pointer
    // of the root
    node->setParent(temp);
 
    // Update tmpnode as the left or
    // the right child of its parent
    // pointer according to its key value
    if (temp->getParent() != nullptr && node->getItem().first < temp->getParent()->getItem().first) {
        temp->getParent()->setLeft(temp);
    }
    else {
        if (temp->getParent() != nullptr)
            temp->getParent()->setRight(temp);
    }
 
    // Make tmpnode as the new root
    node = temp;
 
    // Return the root node
    return node;

    /*
  AVLNode<Key, Value>* temp = node->getRight();
  node->setRight(temp->getLeft());
  temp->setLeft(node);

  temp->getLeft()->setParent(temp);
  node->setParent(temp);
  temp->setParent(node->getParent());
  return temp;
  */
}

//returns the node with its new children and parent after rotation. set prev node = rotateReft
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
      AVLNode<Key, Value>* temp = node->getLeft();
      if (temp == nullptr) return temp;
  node->setLeft(temp->getRight());

  if (temp->getRight() != nullptr)
    temp->getRight()->setParent(node);

  temp->setRight(node);
  temp->setParent(node->getParent());
  node->setParent(temp);
  
  if (temp->getParent() != nullptr && node->getItem().first < temp->getParent()->getItem().first) {
        temp->getParent()->setLeft(temp);
    }
    else {
        if (temp->getParent() != NULL)
            temp->getParent()->setRight(temp);
    }
 
    node = temp;
  return node;



    /*
  AVLNode<Key, Value>* temp = node->getLeft();
  node->setLeft(temp->getRight());
  temp->setRight(node);

  temp->setParent(node->getParent());

  temp->getRight()->setParent(temp);
  node->setParent(temp);
  return temp; 
  */
  
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild(AVLNode<Key, Value>* node)
{
    Node<Key, Value>* nodeParent = node->getParent(); 
    if (nodeParent == nullptr) return false;
    return (node == nodeParent->getLeft());
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChild(AVLNode<Key, Value>* node)
{
    Node<Key, Value>* nodeParent = node->getParent(); 
    if (nodeParent == nullptr) return false;
    return (node == nodeParent->getRight());
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
