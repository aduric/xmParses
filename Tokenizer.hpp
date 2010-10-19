#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP


/*
 * Tokenizer.hpp 
 *
 * The Tokenizer functions
 *
 * - breaks up a string into separate words
 * - ignores punctuation
 * - applies lowercase to all tokens
 *
 */

#include<iostream>
#include<boost/tokenizer.hpp>
#include<boost/algorithm/string/classification.hpp>
#include<boost/algorithm/string/case_conv.hpp>
#include<string>
#include<vector>

// tokenizes string str into string tokens and places tokens into vector
std::vector<std::string> tokenize(const std::string&);


#endif
