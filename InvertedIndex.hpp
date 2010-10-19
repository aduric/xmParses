/* InvertedIndex class
 * keeps track of the current parsed elements
 */

#ifndef INVERTEDINDEX_HPP
#define INVERTEDINDEX_HPP

#include "NodeCollection.hpp"


class InvertedIndex {
public:

  static InvertedIndex *e_instance;

  // singleton class - only ONE version must exist at all times
  static InvertedIndex *instance() {
    if(!e_instance)
      e_instance = new InvertedIndex;
    return e_instance;
  }

  // adds a string to the vocab
  int addToVocab(const string& val) {
    
    int tmpIndex;
    
    // first we need to check if val is an end-of-sentence
    // newline indicates sentence boundary type, if so, tmpIndex = 0
    if(val == "\n") {
      //cout << "newline detected";
      //exit(1);
      tmpIndex = 0;
    }
    else {
      map<string, int>::iterator it = vocab.find(val);
      // if word doesn't exist in vocab, add it
      if(it == vocab.end())
	this->vocab[val] = this->vocab.size();
      
      tmpIndex = this->vocab[val];
    }
    
    return tmpIndex;

  }

  map<string, int> vocab;
  NodeCollection docs;
  NodeCollection entities;
  NodeCollection doc_entities;
  NodeCollection entity_weights;
  // entity x entity id in doc postings
  NodeCollection entity_docids;
  NodeCollection syns;
  NodeCollection entity_syns;
  NodeCollection names;
  NodeCollection entity_names;

};


#endif
