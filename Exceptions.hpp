/*
 * The exceptions class for Representation Strategies
 *
 */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS


// standard exceptions
#include <iostream>
#include <exception>

using namespace std;

class UnknownNodeException: public exception
{
  virtual const char* what() const throw()
  {
    return "Node cannot be handled.";
  }
};

class UnknownAttributeException: public exception
{
  virtual const char* what() const throw()
  {
    return "Attribute does not exist.";
  }
};



#endif
