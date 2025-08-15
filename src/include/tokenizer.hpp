/*

    The tokenizer for spork

*/

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <cstddef>
#include <string>
#include <vector>

enum class tokentype_t {
    /*  General Tokens      */
    O_BRACKET,  // [
    C_BRACKET,  // ]
    O_BRACE,    // {
    C_BRACE,    // }
    O_PARAN,    // (
    C_PARAN,    // )
    QUOTE,      // "
    HQUOTE,     // '
    DOT,        // .
    COMMA,      // ,
    SEMI,       // ;
    BSLASH,     // '\'
    QUESTION,   // ?
    COMMENT,    // //
    /*  Expression Tokens   */
    PLUS,       // +
    MINUS,      // -
    EQUALS,     // =
    MULT,       // *
    DIV,        // /
    MOD,        // %
    EQUAL_EQUAL,// ==
    NOT_EQUAL,  // !=
    G_EQUAL,    // >=
    L_EQUAL,    // <=
    LESS_T,     // <
    GREAT_T,    // >
    IF_AND,     // &&
    IF_OR,      // ||
    NOT,        // !
    TO,         // :
    COMP_AND,   // @
    /*  Bitwise Tokens      */
    B_XOR,      // ^
    B_OR,       // |
    B_AND,      // &
    B_NOT,      // ~
    B_LSHIFT,   // <<
    B_RSHIFT,   // >>
    /*  Keyword Tokens      */
    IDENTIFIER, // 
    KEEP,       // keep
    PRIV,       // priv
    SHARED,     // shared
    UN,         // un
    VOL,        // volatile
    /*  Datatype Tokens     */
    SIZE,       // size | Can be :32 or :64 depending on the arch
    UINT,       // uint
    INT,        // int
    LONG,       // long
    FLOAT,      // float
    CHAR,       // char
    STRING,     // string
    AUTO,       // auto 
    FUNC,       // func
    ENUM,       // enum
    STRUCT,     // struct
    /*  EOF and \n          */
    END_OF_FILE,// eof
    NEWLINE,    // \n
};

enum class bittype_t {
    NONE,       //
    OTHER,      //
    B_EIGHT,    // :8
    B_SIXTEEN,  // :16
    B_TTWO,     // :32
    B_SFOUR,    // :64
};

typedef struct token {
    tokentype_t tokentype;
    bittype_t   bittype;
    std::string value;
    std::size_t row, line;
} token_t;

class tokenizer {
public:
    tokenizer(std::string src);
    std::vector<token_t> tokenize();
    
private:
    const std::string source;
    std::size_t start;
    std::size_t pos;
    int line, row;

    bool isEOF();
    char advance();
    char peek();
    char peeknext();
    bool match(const char c);
    void skipwhitespace();
    void skipcomm();

    token_t maketoken(tokentype_t token, bittype_t bittype, std::string value);
    token_t scantoken();
    token_t identifier();
    token_t num();
    token_t string();
    token_t character();

    tokentype_t getidentity(std::string text);
};

#endif