/* 
 * A simple class for node collections
 */

#ifndef NODE_COLLECTION_HPP
#define NODE_COLLECTION_HPP


class NodeCollection {
public:
  
  void add(int uid, int idx) {
    this->data.insert(pair<int, int>(uid, idx));
  }

  // returns an iterator where keys are sorted in ascending order
  multimap<int, int>::const_iterator begin() const {
    return this->data.begin();
  }
  multimap<int, int>::const_iterator end() const {
    return this->data.end();
  }

  // returns an iterator of a vector that contains al lthe elements specified by the given id
  multimap<int, int>::const_iterator begin(int id) const {
    return this->data.equal_range(id).first;
  }
  multimap<int, int>::const_iterator end(int id) const {
    return this->data.equal_range(id).second;
  }


  int size() {
    return data.size();
  }
  
  multimap<int, int> reverseMap() const {
    multimap<int, int> rev;
    multimap<int, int>::const_iterator it;
    for(it = this->data.begin(); it != this->data.end(); it++)
      rev.insert(pair<int, int>((*it).second, (*it).first));
    return rev;
  }

  // returns the highest key
  int highest() const {
    
    return (*--(this->end())).first;
  }

private:

  //id, word id
  multimap<int, int> data;

};


#endif
