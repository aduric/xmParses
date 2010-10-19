#ifndef MOVIESAXHANDLER_HPP
#define MOVIESAXHANDLER_HPP

#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/HandlerBase.hpp>

#include <vector>
#include <string>
#include <map>
#include <stack>
#include <iostream>
#include <ostream>
#include <sstream>
#include "EntityTree.hpp"

using namespace xercesc;
using namespace std;

/*
 * The SAX Handler class
 */
class MovieSAXHandler : public HandlerBase {
public:
  MovieSAXHandler();
  ~MovieSAXHandler();

  void startElement(const XMLCh* const, AttributeList&);
  void endElement(const XMLCh* const name);
  void characters(const XMLCh* const, const XMLSize_t);
  void fatalError(const SAXParseException&);

private:

  EntityTree et;
  int indexOfCurrentNode;

};

#endif
