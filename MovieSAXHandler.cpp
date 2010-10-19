#include <stdexcept>
#include "MovieSAXHandler.hpp"
#include "RepresentationStrategy.hpp"
#include "Exceptions.hpp"
#include "Tokenizer.hpp"
#include "Node.hpp"
#include "xerces_strings.hpp"





MovieSAXHandler::MovieSAXHandler()
{
  this->indexOfCurrentNode = 0; 
}

MovieSAXHandler::~MovieSAXHandler()
{
}


void MovieSAXHandler::startElement(const XMLCh* const name,
                           AttributeList& attributes)
{

  // get the attributes
  map<string, string> atts;
  for (unsigned int i = 0; i < attributes.getLength(); i++) {
    string attName = toNative(attributes.getName(i));
    string attVal = toNative(attributes.getValue(i)); 
    atts[attName] = attVal;
  }

  this->indexOfCurrentNode++;

  cout << "index: " << this->indexOfCurrentNode << "\n";
  Node* n = new NodeID(toNative(name), atts, this->indexOfCurrentNode);
  this->et.insert(*n);

}


void MovieSAXHandler::characters(const XMLCh* const chars, const XMLSize_t length) {
  
  // add to MetaIndex
  const string text = toNative(chars);
  this->et.current()->getData()->setNodeValue(text);
 
}

void MovieSAXHandler::endElement(const XMLCh* const name) {

  string nodeName = toNative(name);

  RepresentationStrategy* rep;
  RepresentationStrategyFactory repFactory;

  if(nodeName == "entity") {
    rep = repFactory.getRepresentation(repFactory.TAG_ENTITY);
    // execute the representation strategy
    rep->execute(et);
    //delete current representation strategy
    delete rep;
  }
  else if(nodeName == "syn") {
    rep = repFactory.getRepresentation(repFactory.TAG_SYN);
    rep->execute(et);
    delete rep;
  }
  else if(nodeName == "name") {
    rep = repFactory.getRepresentation(repFactory.TAG_NAME);
    rep->execute(et);
    delete rep;
  }
  else if(nodeName == "txt") {
    rep = repFactory.getRepresentation(repFactory.TAG_WORD);
    rep->execute(et);
    delete rep;
  }
    
  //remove current node
  et.remove();

}

void MovieSAXHandler::fatalError(const SAXParseException& exception)
{
    char* message = XMLString::transcode(exception.getMessage());
    cout << "Fatal Error: " << message
         << " at line: " << exception.getLineNumber()
         << endl;
    XMLString::release(&message);
}
