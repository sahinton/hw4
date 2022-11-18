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
		//void zigzig(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p, AVLNode<Key, Value>* c);
		//void zigzag(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p, AVLNode<Key, Value>* c);
		void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* c);
		void removeFix(AVLNode<Key, Value>* n, int8_t diff);
		void rotateLeft(AVLNode<Key, Value>* n);
		void rotateRight(AVLNode<Key, Value>* n);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
		AVLNode<Key, Value>* newnode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
		if(this->root_ == nullptr){
			newnode->setBalance(0);
			this->root_ = newnode;
		}
		if (this->internalFind(new_item.first) != nullptr){
			this->internalFind(new_item.first)->setValue(new_item.second);
		}
		else{
			AVLNode<Key, Value>* top = (AVLNode<Key, Value>*)this->root_;
      AVLNode<Key, Value>* final;
      while(top != nullptr){
        if(new_item.first < top->getKey()){
          final = top;
          top = top->getLeft();
        }
        else if(new_item.first > top->getKey()){
          final = top;
          top = top->getRight();
        }
      }
      if(new_item.first < final->getKey()){
				if (final->getBalance() == -1 || final->getBalance() == 1){
					final->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, final));
					final->setBalance(0);
				}
				else{
					final->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, final));
					final->getLeft()->setBalance(0);
					if(final->getLeft()->getParent()->getBalance() == 0){
						final->getLeft()->getParent()->setBalance(-1);
						insertFix(final->getLeft()->getParent(), final->getLeft());
					}
				}
      }
      else if (new_item.first > final->getKey()){
				if (final->getBalance() == -1 || final->getBalance() == 1){
					final->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, final));
					final->setBalance(0);
				}
				else{
        final->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, final));
				final->getRight()->setBalance(0);
				if(final->getRight()->getParent()->getBalance() == 0){
					final->getRight()->getParent()->setBalance(1);
					insertFix(final->getRight()->getParent(), final->getRight());
				}
				}
      }
		}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* c){


	if(p == nullptr || p->getParent() == nullptr){
		return;
	}
	AVLNode<Key, Value>* g = p->getParent();

	//p is left child of g
	if(p == g->getLeft()){
		g->setBalance(g->getBalance() - 1);
		if(g->getBalance() == 0){
			return;
		}
		else if(g->getBalance() == -1){
			insertFix(g, p);
		}
		else if(g->getBalance() == -2){
			//if zigzig (c is left of parent)
			if(c == p->getLeft()){
				rotateRight(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else{
				rotateLeft(p);
				rotateRight(g);
				if(c->getBalance() == -1){
					p->setBalance(0);
					g->setBalance(1);
					c->setBalance(0);
				}
				else if(c->getBalance() == 0){
					p->setBalance(0);
					g->setBalance(0);
					c->setBalance(0);
				}
				else if(c->getBalance() == 1){
					p->setBalance(-1);
					g->setBalance(0);
					c->setBalance(0);
				}
			}

		}
	}
	//if p is right child of g
	else if(p == g->getRight()){
		g->setBalance(g->getBalance() + 1);
		if(g->getBalance() == 0){
			return;
		}
		else if(g->getBalance() == 1){
			insertFix(g, p);
		}
		else if(g->getBalance() == 2){
			//if zigzig (c is right of parent)
			if(c == p->getRight()){
				rotateLeft(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else{
				rotateRight(p);
				rotateLeft(g);
				if(c->getBalance() == 1){
					p->setBalance(0);
					g->setBalance(-1);
					c->setBalance(0);
				}
				else if(c->getBalance() == 0){
					p->setBalance(0);
					g->setBalance(0);
					c->setBalance(0);
				}
				else if(c->getBalance() == -1){
					p->setBalance(1);
					g->setBalance(0);
					c->setBalance(0);
				}
			}

		}

	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n){
	AVLNode<Key, Value>* p = n;
	AVLNode<Key, Value>* temp = p->getLeft();
	if(n == this->root_){
		p->setLeft(temp->getRight());
		if(p->getLeft() != nullptr){
			p->getLeft()->setParent(p);
		}
		temp->setRight(p);
		temp->setParent(nullptr);
		p->setParent(temp);
		this->root_ = temp;
	}
	else{
		if(p == p->getParent()->getLeft()){//if p is a left child
			p->getParent()->setLeft(temp);
		}
		else{//right
			p->getParent()->setRight(temp);
		}
		if(temp->getRight() != nullptr){ //set parent if child->right not null
			temp->getRight()->setParent(p);
		}
		p->setLeft(temp->getRight());
		temp->setRight(p);
		temp->setParent(p->getParent());
		p->setParent(temp);
	}

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n){

	AVLNode<Key, Value>* p = n;
	AVLNode<Key, Value>* temp = p->getRight();

	if(n == this->root_){
		p->setRight(temp->getLeft());
		if(p->getRight() != nullptr){
			p->getRight()->setParent(p);
		}
		temp->setLeft(p);
		temp->setParent(nullptr);
		p->setParent(temp);
		this->root_ = temp;
	}
	else{
		if(p == p->getParent()->getLeft()){//if p is a left child
			p->getParent()->setLeft(temp);
		}
		else{//right
			p->getParent()->setRight(temp);
		}
		if(temp->getLeft() != nullptr){ //set parent if child->left not null
			temp->getLeft()->setParent(p);
		}
		p->setRight(temp->getLeft());
		temp->setLeft(p);
		temp->setParent(p->getParent());
		p->setParent(temp);
	}

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
		bool swapped = false;
		int diff = 0;
    AVLNode<Key, Value>* removedNode = (AVLNode<Key, Value>*)this->internalFind(key);
    if(removedNode == nullptr){ //do nothing
        return;
    }
		if(removedNode->getParent() == nullptr && removedNode->getLeft() == nullptr && removedNode->getRight() == nullptr){
        //clear tree
			this->root_ = nullptr;
      delete removedNode;
			return;
		}
		//2 children
    if(removedNode->getLeft() != nullptr && removedNode->getRight() != nullptr){
      nodeSwap((AVLNode<Key, Value>*)this->predecessor(removedNode), removedNode); //swap with predecessor
      swapped = true;
    }
		AVLNode<Key, Value>* p = removedNode->getParent();
		if(p != nullptr){
			if (removedNode == p->getLeft()){
				diff++;
			}
			else{
				diff--;
			}
		}
		//removed has 1 left child
		if (removedNode->getLeft() != nullptr && removedNode->getRight() == nullptr){
			if(swapped == true){ //case for if swapped with predecessor previously
				if(removedNode == removedNode->getParent()->getRight()){
					removedNode->getParent()->setRight(removedNode->getLeft());
					removedNode->getLeft()->setParent(removedNode->getParent());
				}
				else{
					removedNode->getParent()->setLeft(removedNode->getLeft());
					removedNode->getLeft()->setParent(removedNode->getParent());
				}
				swapped = false;
			}
			else if(removedNode->getParent() == nullptr){//if removed is root
				this->root_ = removedNode->getLeft();
				this->root_->setParent(nullptr);
			}
			else if(removedNode->getKey() < removedNode->getParent()->getKey()){//if the removed is a left child
				removedNode->getParent()->setLeft(removedNode->getLeft());
				removedNode->getLeft()->setParent(removedNode->getParent());
			}
			else{ //right
				removedNode->getParent()->setRight(removedNode->getLeft());
				removedNode->getLeft()->setParent(removedNode->getParent());
			}
			removeFix(p, diff);
			delete removedNode;
			return;
		}
		//1 right child
		else if (removedNode->getLeft() == nullptr && removedNode->getRight() != nullptr){
			if(removedNode->getParent() == nullptr){//if removed is root
				this->root_ = removedNode->getRight();
				this->root_->setParent(nullptr);
			}
			else if(removedNode->getKey() < removedNode->getParent()->getKey()){ //if removed is a left child
				removedNode->getParent()->setLeft(removedNode->getRight());
				removedNode->getRight()->setParent(removedNode->getParent());
			}
			else{//right
				removedNode->getParent()->setRight(removedNode->getRight());
				removedNode->getRight()->setParent(removedNode->getParent());
			}
			removeFix(p, diff);
			delete removedNode;
			return;
		}
  
		//0 children
    else if (removedNode->getLeft() == nullptr && removedNode->getRight() == nullptr){
      if(removedNode == removedNode->getParent()->getLeft()){ //if removed is a left child
        removedNode->getParent()->setLeft(nullptr);
      }
      else if (removedNode == removedNode->getParent()->getRight()){ //right
        removedNode->getParent()->setRight(nullptr);
      }
			removeFix(p, diff);
      delete removedNode;
      return;
    }
	//delete removedNode;
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff)
{
    // TODO
		if(n == nullptr){
			return;
		}
		int ndiff = 0;
		AVLNode<Key, Value>* p = n->getParent();
		if(p != nullptr){
			if(n == p->getLeft()){
				ndiff++;
			}
			else{
				ndiff--;
			}
		}
		if(diff == -1){
			if(n->getBalance() + diff == -2){
				AVLNode<Key, Value>* c = n->getLeft();
				if(c->getBalance() == -1){ //zig zig
					rotateRight(n);
					n->setBalance(0);
					c->setBalance(0);
					removeFix(p, ndiff);
				}
				else if(c->getBalance() == 0){
					rotateRight(n);
					n->setBalance(-1);
					c->setBalance(1);
					return;
				}
				else if(c->getBalance() == 1){ //zig zag
					AVLNode<Key, Value>* g = c->getRight();
					rotateLeft(c);
					rotateRight(n);
					if(g->getBalance() == 1){
						n->setBalance(0);
						c->setBalance(-1);
						g->setBalance(0);
					}
					else if(g->getBalance() == 0){
						n->setBalance(0);
						c->setBalance(0);
						g->setBalance(0);
					}
					else if(g->getBalance() == -1){
						n->setBalance(1);
						c->setBalance(0);
						g->setBalance(0);
					}
					removeFix(p, ndiff);
				}
			}
			else if(n->getBalance() + diff == 2){
				AVLNode<Key, Value>* c = n->getRight();
				if(c->getBalance() == 1){ //zig zig
					rotateLeft(n);
					n->setBalance(0);
					c->setBalance(0);
					removeFix(p, ndiff);
				}
				else if(c->getBalance() == 0){
					rotateLeft(n);
					n->setBalance(1);
					c->setBalance(-1);
					return;
				}
				else if(c->getBalance() == -1){ //zig zag
					AVLNode<Key, Value>* g = c->getLeft();
					rotateRight(c);
					rotateLeft(n);
					if(g->getBalance() == -1){
						n->setBalance(0);
						c->setBalance(1);
						g->setBalance(0);
					}
					else if(g->getBalance() == 0){
						n->setBalance(0);
						c->setBalance(0);
						g->setBalance(0);
					}
					else if(g->getBalance() == 1){
						n->setBalance(-1);
						c->setBalance(0);
						g->setBalance(0);
					}
					removeFix(p, ndiff);
				}
			}
			else if(n->getBalance() + diff == -1){
				n->setBalance(-1);
				return;
			}
			else if(n->getBalance() + diff == 0){
				n->setBalance(0);
				removeFix(p, ndiff);
			}
		}
		//diff = +1
		else if(diff == 1){
			if(n->getBalance() + diff == -2){
				AVLNode<Key, Value>* c = n->getLeft();
				if(c->getBalance() == -1){ //zig zig
					rotateRight(n);
					n->setBalance(0);
					c->setBalance(0);
					removeFix(p, ndiff);
				}
				else if(c->getBalance() == 0){
					rotateRight(n);
					n->setBalance(-1);
					c->setBalance(1);
					return;
				}
				else if(c->getBalance() == 1){ //zig zag
					AVLNode<Key, Value>* g = c->getRight();
					rotateLeft(c);
					rotateRight(n);
					if(g->getBalance() == 1){
						n->setBalance(0);
						c->setBalance(-1);
						g->setBalance(0);
					}
					else if(g->getBalance() == 0){
						n->setBalance(0);
						c->setBalance(0);
						g->setBalance(0);
					}
					else if(g->getBalance() == -1){
						n->setBalance(1);
						c->setBalance(0);
						g->setBalance(0);
					}
					removeFix(p, ndiff);
				}
			}
			else if(n->getBalance() + diff == 2){
				AVLNode<Key, Value>* c = n->getRight();
				if(c->getBalance() == 1){ //zig zig
					rotateLeft(n);
					n->setBalance(0);
					c->setBalance(0);
					removeFix(p, ndiff);
				}
				else if(c->getBalance() == 0){
					rotateLeft(n);
					n->setBalance(1);
					c->setBalance(-1);
					return;
				}
				else if(c->getBalance() == -1){ //zig zag
					AVLNode<Key, Value>* g = c->getLeft();
					rotateRight(c);
					rotateLeft(n);
					if(g->getBalance() == -1){
						n->setBalance(0);
						c->setBalance(1);
						g->setBalance(0);
					}
					else if(g->getBalance() == 0){
						n->setBalance(0);
						c->setBalance(0);
						g->setBalance(0);
					}
					else if(g->getBalance() == 1){
						n->setBalance(-1);
						c->setBalance(0);
						g->setBalance(0);
					}
					removeFix(p, ndiff);
				}
			}
			else if(n->getBalance() + diff == 1){
				n->setBalance(1);
				return;
			}
			else if(n->getBalance() + diff == 0){
				n->setBalance(0);
				removeFix(p, ndiff);
			}
		}
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
