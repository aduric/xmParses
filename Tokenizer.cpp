/*
 * Tokenizer.cpp
 *
 * The Tokenizer functions
 *
 * - breaks up a string into separate words
 * - ignores punctuation
 * - applies lowercase to all tokens
 *
 */

#include "Tokenizer.hpp"
#include <cctype>
#include <iostream>
#include <fstream>

using namespace std;
using namespace boost;


// tokenizes string str into string tokens and places tokens into vector
// the TokenizerFunction is set to default separators (spaces and punctuation)
// NOTE: tokens are also checked against BOOST's isalpha()
vector<string> tokenize(const string& str) {
  vector<string> tokens;
  typedef tokenizer<char_separator<char> > tokenizer;
  char_separator<char> sep(",.!?\"()*&^%$#@~`+=[]{}:;/<>| ", "\n");
  tokenizer tok(str, sep);
  
   for(tokenizer::iterator beg=tok.begin(); beg!=tok.end();++beg){
     string tokenCopy = *beg;
     // check if the tokenCopy is an alphanumeric character
     // NOTE: i wanted to use BOOST's is_alpha() but couldnt't figure it out
     // we are also keeping the \n characters as they are important in differentiating sent boundaries
     if(!isalpha(tokenCopy[0]) && tokenCopy[0] != '\n')
       continue;
     for(unsigned int i = 1; i < tokenCopy.length(); i++)
       if(!isalnum(tokenCopy[i]))
	  continue;
     // save the lower case version of the token
     to_lower(tokenCopy);
     tokens.push_back(tokenCopy);
   }

   return tokens;

}
/*
int main() {

  //string test = "Hel'lo , s23455d, so!f 98 98syd! f9si$%$bspfahd, %$%08d0 as : $#% EWTRWE %$%2345 # 3 tan-dsfd-rer";
  char test[255];
  FILE* myfile  = fopen("testtxt.txt", "r");
  if (myfile != NULL)
  {
    fread(test, sizeof(char), 255, myfile);
    fclose(myfile);
  }

  string line = string(test);

  vector<string> tokenized = tokenize(line);

  for(vector<string>::iterator it = tokenized.begin(); it != tokenized.end(); it++)
    cout << "'" << *it << "'" << "\n";

}
*/

