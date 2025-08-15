#include "include/tokenizer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::string readfile(const std::string path) {
  std::ifstream file(path);
  if (!file) {
    std::cout << "Error opening file: " << path << "\n";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

int main(int argc, char *argv[]) {
  std::string content = readfile("examples/01.spork");
  tokenizer t(content);
  std::vector<token_t> tokens = t.tokenize();

  std::cout << "LINE,ROW,TYPE,VALUE,BITSIZE\n";
  for (const token_t &token : tokens) {
    std::cout << "" << token.line << "," << token.row << ",";
    switch (token.tokentype) {
    case tokentype_t::O_BRACKET:
      std::cout << "O_BRACKET";
      break;
    case tokentype_t::C_BRACKET:
      std::cout << "C_BRACKET";
      break;
    case tokentype_t::O_BRACE:
      std::cout << "O_BRACE";
      break;
    case tokentype_t::C_BRACE:
      std::cout << "C_BRACE";
      break;
    case tokentype_t::O_PARAN:
      std::cout << "O_PARAN";
      break;
    case tokentype_t::C_PARAN:
      std::cout << "C_PARAN";
      break;
    case tokentype_t::QUOTE:
      std::cout << "QUOTE";
      break;
    case tokentype_t::HQUOTE:
      std::cout << "HQUOTE";
      break;
    case tokentype_t::DOT:
      std::cout << "DOT";
      break;
    case tokentype_t::COMMA:
      std::cout << "COMMA";
      break;
    case tokentype_t::SEMI:
      std::cout << "SEMI";
      break;
    case tokentype_t::BSLASH:
      std::cout << "BSLASH";
      break;
    case tokentype_t::QUESTION:
      std::cout << "QUESTION";
      break;
    case tokentype_t::COMMENT:
      std::cout << "COMMENT";
      break;

    case tokentype_t::PLUS:
      std::cout << "PLUS";
      break;
    case tokentype_t::MINUS:
      std::cout << "MINUS";
      break;
    case tokentype_t::EQUALS:
      std::cout << "EQUALS";
      break;
    case tokentype_t::MULT:
      std::cout << "MULT";
      break;
    case tokentype_t::DIV:
      std::cout << "DIV";
      break;
    case tokentype_t::MOD:
      std::cout << "MOD";
      break;
    case tokentype_t::EQUAL_EQUAL:
      std::cout << "EQUAL_EQUAL";
      break;
    case tokentype_t::NOT_EQUAL:
      std::cout << "NOT_EQUAL";
      break;
    case tokentype_t::G_EQUAL:
      std::cout << "G_EQUAL";
      break;
    case tokentype_t::L_EQUAL:
      std::cout << "L_EQUAL";
      break;
    case tokentype_t::LESS_T:
      std::cout << "LESS_T";
      break;
    case tokentype_t::GREAT_T:
      std::cout << "GREAT_T";
      break;
    case tokentype_t::IF_AND:
      std::cout << "IF_AND";
      break;
    case tokentype_t::IF_OR:
      std::cout << "IF_OR";
      break;
    case tokentype_t::NOT:
      std::cout << "NOT";
      break;
    case tokentype_t::TO:
      std::cout << "TO";
      break;
    case tokentype_t::COMP_AND:
      std::cout << "COMP_AND";
      break;

    case tokentype_t::B_XOR:
      std::cout << "B_XOR";
      break;
    case tokentype_t::B_OR:
      std::cout << "B_OR";
      break;
    case tokentype_t::B_AND:
      std::cout << "B_AND";
      break;
    case tokentype_t::B_NOT:
      std::cout << "B_NOT";
      break;
    case tokentype_t::B_LSHIFT:
      std::cout << "B_LSHIFT";
      break;
    case tokentype_t::B_RSHIFT:
      std::cout << "B_RSHIFT";
      break;

    case tokentype_t::IDENTIFIER:
      std::cout << "IDENTIFIER";
      break;
    case tokentype_t::KEEP:
      std::cout << "KEEP";
      break;
    case tokentype_t::PRIV:
      std::cout << "PRIV";
      break;
    case tokentype_t::SHARED:
      std::cout << "SHARED";
      break;
    case tokentype_t::UN:
      std::cout << "UN";
      break;
    case tokentype_t::VOL:
      std::cout << "VOL";
      break;

    case tokentype_t::SIZE:
      std::cout << "SIZE";
      break;
    case tokentype_t::UINT:
      std::cout << "UINT";
      break;
    case tokentype_t::INT:
      std::cout << "INT";
      break;
    case tokentype_t::LONG:
      std::cout << "LONG";
      break;
    case tokentype_t::FLOAT:
      std::cout << "FLOAT";
      break;
    case tokentype_t::CHAR:
      std::cout << "CHAR";
      break;
    case tokentype_t::STRING:
      std::cout << "STRING";
      break;
    case tokentype_t::AUTO:
      std::cout << "AUTO";
      break;
    case tokentype_t::ENUM:
      std::cout << "ENUM";
      break;
    case tokentype_t::STRUCT:
      std::cout << "STRUCT";
      break;
    case tokentype_t::FUNC:
      std::cout << "FUNC";
      break;
    case tokentype_t::END_OF_FILE:
      std::cout << "END_OF_FILE";
      break;
    case tokentype_t::NEWLINE:
      std::cout << "NEWLINE";
      break;
    }
    std::cout << "," << token.value << ",";
    switch (token.bittype) {
    case bittype_t::B_EIGHT: std::cout << 8; break;
    case bittype_t::B_SIXTEEN: std::cout << 16; break;
    case bittype_t::B_TTWO: std::cout << 32; break;
    case bittype_t::B_SFOUR: std::cout << 64; break;
    case bittype_t::NONE: std::cout << "NONE"; break;
    case bittype_t::OTHER: std::cout << "OTHER"; break;
}

    std::cout << "\n";
  }

  return 0;
}