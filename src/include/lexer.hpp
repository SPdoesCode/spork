/*

    A lexer for spork.

*/
#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include 
typedef struct ASTsector {
    std::string name, value, type, bitwith;
    std::vector<struct ASTsector> children;


} ASTsector_t;

#endif