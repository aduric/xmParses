/*
 * RepresentationFactory decides what representation strategy to use
 * depending on the type of Node
 *
 */

#ifndef REPRESENTATION_STRATEGY_HPP
#define REPRESENTATION_STRATEGY_HPP

#include "Tokenizer.hpp"
#include "InvertedIndex.hpp"

InvertedIndex *InvertedIndex::e_instance = 0;

class RepresentationStrategy {
public:
  virtual void execute(EntityTree& e) = 0;
};


/*
 * RepresentationStrategy subclasses that pull information required from the given EntityTree
 * transform it and post that information to the InvertedIndex
 */
class EntityRepresentationStrategy : public RepresentationStrategy {
public:
  void execute(EntityTree& e) {
    // transform the current() entity tree node

    string nodeValue = e.current()->getData()->getAttribute("value");
    vector<string> nodeTokens = tokenize(nodeValue);

    int nodeID = ((NodeID*)e.current()->getData())->getID();
    int parentParentID = ((NodeID*)e.parent()->getParent()->getData())->getID();

    // map to vocab index
    vector<int> nodeIndex;
    for(vector<string>::iterator it = nodeTokens.begin(); it != nodeTokens.end(); it++) {
      nodeIndex.push_back(InvertedIndex::instance()->addToVocab(*it));
      //cout << "word: " << *it << " -> " << nodeIndex.back() << "\n";
    }

    int nodeWeight = atoi(e.current()->getData()->getAttribute("weight").c_str());

    // post to InvertedIndex's entities <id, word id>
    for(vector<int>::iterator it = nodeIndex.begin(); it != nodeIndex.end(); it++)
      InvertedIndex::instance()->entities.add(nodeID, *it);

    // post to InvertedIndex's entities <id, weight>
    InvertedIndex::instance()->entity_weights.add(nodeID, nodeWeight);

    // post to InvertedIndex's doc_entities <parentparentID, id>
    InvertedIndex::instance()->doc_entities.add(parentParentID, nodeID);
    
    
  }
};

class SynRepresentationStrategy : public RepresentationStrategy {
public:
  void execute(EntityTree& e) {
    string nodeContents = e.current()->getData()->getNodeValue();
    vector<string> nodeTokens = tokenize(nodeContents);

    int nodeID = ((NodeID*)e.current()->getData())->getID();
    int parentNodeID = ((NodeID*)e.parent()->getData())->getID();

    // map to vocab index
    vector<int> nodeIndex;
    for(vector<string>::iterator it = nodeTokens.begin(); it != nodeTokens.end(); it++)
      nodeIndex.push_back(InvertedIndex::instance()->addToVocab(*it));
    
    // post to InvertedIndex's syns <id, word id>
    for(vector<int>::iterator it = nodeIndex.begin(); it != nodeIndex.end(); it++)
      InvertedIndex::instance()->syns.add(nodeID, *it);

    // post to InvertedIndex's entities <id, weight>
    InvertedIndex::instance()->entity_syns.add(parentNodeID, nodeID);


  }

};

class NameRepresentationStrategy : public RepresentationStrategy {
public:
  void execute(EntityTree& e) {
    string nodeContents = e.current()->getData()->getNodeValue();
    vector<string> nodeTokens = tokenize(nodeContents);

    int nodeID = ((NodeID*)e.current()->getData())->getID();
    int parentNodeID = ((NodeID*)e.parent()->getData())->getID();

    // map to vocab index
    vector<int> nodeIndex;
    for(vector<string>::iterator it = nodeTokens.begin(); it != nodeTokens.end(); it++)
      nodeIndex.push_back(InvertedIndex::instance()->addToVocab(*it));
    
    // post to InvertedIndex's syns <id, word id>
    for(vector<int>::iterator it = nodeIndex.begin(); it != nodeIndex.end(); it++)
      InvertedIndex::instance()->names.add(nodeID, *it);

    // post to InvertedIndex's entities <id, weight>
    InvertedIndex::instance()->entity_names.add(parentNodeID, nodeID);


  }

};

class DocRepresentationStrategy : public RepresentationStrategy {
public:
  void execute(EntityTree& e) {
    string nodeContents = e.current()->getData()->getNodeValue();
    vector<string> nodeTokens = tokenize(nodeContents);

    int parentNodeID = ((NodeID*)e.parent()->getData())->getID();

    // map to vocab index
    vector<int> nodeIndex;
    for(vector<string>::iterator it = nodeTokens.begin(); it != nodeTokens.end(); it++)
      nodeIndex.push_back(InvertedIndex::instance()->addToVocab(*it));
    
    // post to InvertedIndex's docs <id, word id>
    for(vector<int>::iterator it = nodeIndex.begin(); it != nodeIndex.end(); it++)
      InvertedIndex::instance()->docs.add(parentNodeID, *it);
  }
};

class RepresentationStrategyFactory {
public:

  static RepresentationStrategy* getRepresentation(int rep) {
    switch(rep) {
    case TAG_WORD:
      return new DocRepresentationStrategy();
      break;
    case TAG_ENTITY:
      return new EntityRepresentationStrategy();
      break;
    case TAG_SYN:
      return new SynRepresentationStrategy();
      break;
    case TAG_NAME:
      return new NameRepresentationStrategy();
      break;
    default:
      return new DocRepresentationStrategy();
      break;
    }

  }

  enum TAG_TYPES {
    TAG_WORD,
    TAG_ENTITY,
    TAG_SYN,
    TAG_NAME
  };
};

#endif
