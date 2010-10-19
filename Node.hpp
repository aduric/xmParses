/*
 * The Node Class
 */

#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

/*
 * The Attribute class
 * Each attribute has an attribute role
 *
 *
class Attr {
public:
  Attr(const string& name, const string& val) : 
    name_(name), val_(val) {}
  ~Attr() {}

  // getter / setter methods
  string getName() const { return name_; }
  string getVal() const { return val_; }
 
  void setName(const string& name) { name_ = name; }
  void setVal(const string& val) { val_ = val; }

private:

  string name_;
  string val_;
};
*/

class Node {
public:

  //Constructor for Nodes without attributes
  Node(const string& name) :
    nodeName_(name), as_() {}

  //Constructor for Nodes with attributes
  Node(const string& name, map<string, string>& as) :
    nodeName_(name), as_(as) {}
 
  ~Node() {}

  // getter / setter methods
  string getNodeName() const { return nodeName_; }

  string getNodeValue() const { return val_; }

  int getNumAttributes() const { return as_.size(); } 

  // return attribute from as_ by name
  string getAttribute(const string& attname) { 
    return this->as_[attname];
  }

  map<string, string>::iterator Attr_begin() {
    return this->as_.begin();
  }

  map<string, string>::iterator Attr_end() {
    return this->as_.end();
  }


  /*
  string* getAttribute(const int& index) const {
    const Attr* a = &(this->as_[index]);
    string* str = &(a->getVal());
    return str;
  }
  */
  void setNodeName(const string& nodename) { nodeName_ = nodename; }

  void setNodeValue(const string& nodeval) { val_ = nodeval; }

  bool hasAttribute(const string& attname) {
    map<string, string>::iterator it = this->as_.find(attname);
    if(it == as_.end())
      return false;
    else
      return true;
  }

  // compares two nodes for names of attributes
  // if there are no attributes specified, it will return true
  // only if the node itself has no attributes
  /*  bool hasAttributes(vector<string>& attrs) {

    if(attrs.size() == 0) {
      if(this->getNumAttributes() == 0)
	return true;
      else if(this->getNumAttributes() > 0)
	return false;
    }

    for(vector<string>::iterator it = attrs.begin(); it != attrs.end(); it++)
      if(!this->hasAttribute(*it))
	return false;

    return true;
  }
  */

private:
  string nodeName_;
  map<string, string> as_;
  string val_;
};


//Writes it to ostream
inline std::ostream& operator<<(std::ostream& out, Node& n) {
  int a_size = n.getNumAttributes();
  out << "(" << n.getNodeName() << ")";

  if(a_size > 0) {
    out << " { ";
    for(map<string, string>::iterator it = n.Attr_begin(); it != n.Attr_end(); it++)
      out << (*it).first << " -> " << (*it).second << "\n";
    out << " }";
  }

  return out;
}


class NodeID : public Node {
public:
  NodeID(const string& nodeName, map<string, string>& as, int index) : Node(nodeName, as) {
    this->index_ = index;
  }

  int getID() {
    return this->index_;
  }

private:
  int index_;

};


#endif
