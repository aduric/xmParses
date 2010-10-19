/*
 *
 * A simple Entity Tree
 * 
 * Keeps track of Entities as they are being parsed into a tree format
 * The 'current' node always points to the node that is currently being processed
 * This ensures that inserts and deletes are O(1) operations
 *
 */
#include "Node.hpp"

class TreeNode {
public:

  TreeNode(Node& data) {
    this->data_ = &data;
  }
  
  ~TreeNode() {
    delete this->data_;
  }

  TreeNode* getParent() {
    return this->parentNode_;
  }
  void setParent(TreeNode* parent) {
    this->parentNode_ = parent;
  }

  Node* getData() {
    return this->data_;
  }


private:

  TreeNode* parentNode_;
  Node* data_;

};


class EntityTree {
public:
  EntityTree() {
    
    this->currentNode = NULL;

  }
  ~EntityTree() { 
    while(this->currentNode != NULL)
      remove();
  }

  // inserts a child at the current node and the child becomes the current node
  void insert(Node& n) {
    TreeNode *node = new TreeNode(n);
    node->setParent(this->currentNode);
    this->currentNode = node;
  }

  // removes the current node and the current node becomes the parent node
  void remove() {
    if(this->currentNode != NULL) {// reached root node
      TreeNode *tmpNode = this->currentNode->getParent();
      delete this->currentNode;
      this->currentNode = tmpNode;
    }
  }

  // returns the current node
  TreeNode* current() {
    return this->currentNode;
  }

  // returns the parent of the current node
  TreeNode* parent() {
    return this->currentNode->getParent();
  }

private:

  TreeNode* currentNode;

};


// forward declaration of an entity tree
//EntityTree et;
