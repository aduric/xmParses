#ifndef XERCES_STRINGS_HPP_INCLUDED
#define XERCES_STRINGS_HPP_INCLUDED

#include <string>
#include <boost/scoped_array.hpp>

typedef std::basic_string<XMLCh> XercesString;

// converts from narrow-character to a wide-character string
inline XercesString fromNative(const char* str) {
  boost::scoped_array<XMLCh> ptr(xercesc::XMLString::transcode(str));
  return XercesString(ptr.get());
}

// converts from narrow-character to a wide-character string
inline XercesString fromNative(const std::string& str) {
  return fromNative(str.c_str());
}

//converts from a wide-character to a narrow-character string
inline std::string toNative(const XMLCh* str) {
    boost::scoped_array<char> ptr(xercesc::XMLString::transcode(str));
    return std::string(ptr.get());
}

//converts from a wide-character to a narrow-character string
inline std::string toNative(const XercesString& str) {
  return toNative(str.c_str());
}

#endif

