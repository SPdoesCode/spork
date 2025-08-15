#include "include/tokenizer.hpp"

#include <string>
#include <vector>

tokenizer::tokenizer(std::string src) : source(src), start(0), pos(0), line(1), row(1) {};

std::vector<token_t> tokenizer::tokenize() {
    std::vector<token_t> tokens;

    while (!isEOF()) {
        token_t t;
        start = pos;
        t = scantoken();

        if (t.tokentype != tokentype_t::COMMENT)
            tokens.push_back(t);
    }
    tokens.push_back(maketoken(tokentype_t::END_OF_FILE, bittype_t::NONE, "EOF"));
    return tokens;
}

bool tokenizer::isEOF() {
    return pos >= source.size();
}

char tokenizer::advance() {
    row++;
    return source[pos++];
}

char tokenizer::peek() {
    return isEOF() ? '\0' : source[pos];
}

char tokenizer::peeknext() {
    return (pos + 1 >= source.size()) ? '\0' : source[pos + 1];
}

bool tokenizer::match(const char c) {
    if (isEOF() || source[pos] != c) return false;
    pos++;
    row++;
    return true;
}

void tokenizer::skipwhitespace() {
    while (!isEOF()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t')
            advance();
        else if (c == '\n') {
            line++;
            row = 1;
            advance();
        } else {
            break;
        }
    }
}

void tokenizer::skipcomm() {
    while (peek() != '\n' && !isEOF()) {
        advance();
    }
}

token_t tokenizer::maketoken(tokentype_t token, bittype_t bittype, std::string value) {
    return {token, bittype, value, (size_t) row - (int)value.size(), (size_t) line};
}

token_t tokenizer::scantoken() {
    skipwhitespace();
    start = pos;
    if (isEOF()) return maketoken(tokentype_t::END_OF_FILE, bittype_t::NONE, "EOF");

    char c = advance();

    switch (c) {
    case '(': return maketoken(tokentype_t::O_PARAN, bittype_t::NONE, "(");
    case ')': return maketoken(tokentype_t::C_PARAN, bittype_t::NONE, ")");
    case '{': return maketoken(tokentype_t::O_BRACE, bittype_t::NONE, "{");
    case '}': return maketoken(tokentype_t::C_BRACE, bittype_t::NONE, "}");
    case '[': return maketoken(tokentype_t::O_BRACKET, bittype_t::NONE, "[");
    case ']': return maketoken(tokentype_t::C_BRACKET, bittype_t::NONE, "]");
    case '"': return string();
    case '\'': return character();
    case '.': return maketoken(tokentype_t::DOT, bittype_t::NONE, ".");
    case ',': return maketoken(tokentype_t::COMMA, bittype_t::NONE, ",");
    case ';': return maketoken(tokentype_t::SEMI, bittype_t::NONE, ";");
    case '\\': return maketoken(tokentype_t::BSLASH, bittype_t::NONE, "\\");
    case '?': return maketoken(tokentype_t::QUESTION, bittype_t::NONE, "?");
    case '+': return maketoken(tokentype_t::PLUS, bittype_t::NONE, "+");
    case '-': return maketoken(tokentype_t::MINUS, bittype_t::NONE, "-");
    case '*': return maketoken(tokentype_t::MULT, bittype_t::NONE, "*");
    case '/': 
        if (match('/')) { skipcomm(); return maketoken(tokentype_t::COMMENT, bittype_t::NONE, "//"); }
        return maketoken(tokentype_t::DIV, bittype_t::NONE, "/");
    case '%': return maketoken(tokentype_t::MOD, bittype_t::NONE, "%");
    case '^': return maketoken(tokentype_t::B_XOR, bittype_t::NONE, "^");
    case '|': return match('|') ? maketoken(tokentype_t::IF_OR, bittype_t::NONE, "||")
                                : maketoken(tokentype_t::B_OR, bittype_t::NONE, "|");
    case '&': return match('&') ? maketoken(tokentype_t::IF_AND, bittype_t::NONE, "&&")
                                : maketoken(tokentype_t::B_AND, bittype_t::NONE, "&");
    case '~': return maketoken(tokentype_t::B_NOT, bittype_t::NONE, "~");
    case '!': return match('=') ? maketoken(tokentype_t::NOT_EQUAL, bittype_t::NONE, "!=")
                                : maketoken(tokentype_t::NOT, bittype_t::NONE, "!");
    case '=': return match('=') ? maketoken(tokentype_t::EQUAL_EQUAL, bittype_t::NONE, "==")
                                : maketoken(tokentype_t::EQUALS, bittype_t::NONE, "=");
    case '<': 
        if (match('=')) return maketoken(tokentype_t::L_EQUAL, bittype_t::NONE, "<=");
        if (match('<')) return maketoken(tokentype_t::B_LSHIFT, bittype_t::NONE, "<<");
        return maketoken(tokentype_t::LESS_T, bittype_t::NONE, "<");
    case '>':
        if (match('=')) return maketoken(tokentype_t::G_EQUAL, bittype_t::NONE, ">=");
        if (match('>')) return maketoken(tokentype_t::B_RSHIFT, bittype_t::NONE, ">>");
        return maketoken(tokentype_t::GREAT_T, bittype_t::NONE, ">");
    case ':': 
        if (std::isdigit(peek())) {
            std::string bits;
            while (std::isdigit(peek())) bits += advance();
            int width = std::stoi(bits);
            bittype_t bt = bittype_t::NONE;
            if (width == 8) bt = bittype_t::B_EIGHT;
            else if (width == 16) bt = bittype_t::B_SIXTEEN;
            else if (width == 32) bt = bittype_t::B_TTWO;
            else if (width == 64) bt = bittype_t::B_SFOUR;
            else bt = bittype_t::OTHER;
            return maketoken(tokentype_t::TO, bt, ":" + bits);
        }
        return maketoken(tokentype_t::TO, bittype_t::NONE, ":");
    case '@': return maketoken(tokentype_t::COMP_AND, bittype_t::NONE, "@");
    case '\n': return maketoken(tokentype_t::NEWLINE, bittype_t::NONE, "\n");
    default:
        if (std::isalpha(c) || c == '_') return identifier();
        if (std::isdigit(c)) return num();
        break;
    }
    return maketoken(tokentype_t::IDENTIFIER, bittype_t::NONE, std::string(1, c));
}

token_t tokenizer::character() {
    std::string val;
    size_t startCol = row - 1; 

    if (peek() == '\\') {
        advance();
        char next = advance();
        switch(next) {
            case 'n': val = '\n'; break;
            case 't': val = '\t'; break;
            case 'r': val = '\r'; break;
            case '\'': val = '\''; break;
            case '\\': val = '\\'; break;
            default: val = next; break;
        }
    } else {
        val += advance(); 
    }

    if (peek() == '\'') advance(); 
    return {tokentype_t::CHAR, bittype_t::NONE, val, startCol, (size_t)line};
}

token_t tokenizer::string() {
    std::string val;
    size_t startCol = row - 1;

    while (!isEOF() && peek() != '"') {
        char c = advance();
        if (c == '\\') { 
            if (isEOF()) break;
            char next = advance();
            switch (next) {
                case 'n': val += '\n'; break;
                case 't': val += '\t'; break;
                case 'r': val += '\r'; break;
                case '\\': val += '\\'; break;
                case '"': val += '"'; break;
                default: val += next; break;
            }
        } else {
            val += c;
        }
    }

    if (!isEOF()) advance();
    return {tokentype_t::STRING, bittype_t::NONE, val, startCol, (size_t)line};
}

token_t tokenizer::identifier() {
    std::string val;
    size_t startCol = row - 1;

    val += source[start];

    while (!isEOF() && (std::isalnum(peek()) || peek() == '_')) {
        val += advance();
    }

    return {getidentity(val), bittype_t::NONE, val, startCol, (size_t)line};
}


token_t tokenizer::num() {
    std::string val;
    size_t startCol = row - 1;

    while (std::isdigit(peek())) {
        val += advance();
    }

    return {tokentype_t::INT, bittype_t::NONE, val, startCol, (size_t)line};
}

tokentype_t tokenizer::getidentity(std::string text) {
    if (text == "fun") return tokentype_t::FUNC;
    if (text == "keep") return tokentype_t::KEEP;
    if (text == "priv") return tokentype_t::PRIV;
    if (text == "shared") return tokentype_t::SHARED;
    if (text == "un") return tokentype_t::UN;
    if (text == "volatile") return tokentype_t::VOL;
    if (text == "size") return tokentype_t::SIZE;
    if (text == "uint") return tokentype_t::UINT;
    if (text == "int") return tokentype_t::INT;
    if (text == "long") return tokentype_t::LONG;
    if (text == "float") return tokentype_t::FLOAT;
    if (text == "char") return tokentype_t::CHAR;
    if (text == "string") return tokentype_t::STRING;
    if (text == "auto") return tokentype_t::AUTO;
    if (text == "enum") return tokentype_t::ENUM;
    if (text == "struct") return tokentype_t::STRUCT;

    return tokentype_t::IDENTIFIER;
}
