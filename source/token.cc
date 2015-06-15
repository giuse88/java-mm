#include "token.hh"
#include <iostream>
#include <sstream>

///
/// @file   token.cc
/// @author Luca Martini
/// @date   Thu Feb 28 16:20:15 2008
/// 
/// @brief  Implementazioni della classe Token e derivati
/// 
/// 
///
using namespace std;

Number::~Number() {
  switch(t) {
  case INT:
    delete (int*) number;
    break;
  case FLOAT:
    delete (float*) number;
    break;
  case DOUBLE:
    delete (double*) number;
    break;
  case LONG:
    delete (long*) number;
    break;
  }
}

string Number::toString() const {
  stringstream ss;
  switch(t) {
  case INT:
    ss << "INT("<< *((int*) number)<<")";
    break;
  case FLOAT:
    ss << "FLOAT("<< *((float*) number) << ")";    
    break;
  case DOUBLE:
    ss << "DOUBLE(" << *((double*) number) << ")";
    break;
  case LONG:
    ss << "LONG(" << *((long*) number) << ")";    
    break;
  default:
    ss << "Error";
  }
  return ss.str();
}

string Identifier::toString() const {
  return "IDENTIFIER("+name+")";
}

string Symbol::toString() const {
  return "SYMBOL("+name+")";
}

string UnaryOperator::toString() const {
  return "UNARYOPERATOR("+name+")";
}

string BinaryOperator::toString() const {
  return "BINARYOPERATOR("+name+")";
}

string DataType::toString() const {
  switch(name) {
  case INT:
    return "INT";
  case DOUBLE:
    return "DOUBLE";
  case FLOAT:
    return "FLOAT";
  case LONG:
    return "LONG";
  default:
    return "ERROR";
  }
}

string Keyword::toString() const {
  switch (name) {
  case WHILE:
    return "WHILE";
  case IF:
    return "IF";
  case ELSE:
    return "ELSE";    
  case RETURN:
    return "RETURN";    
  case BREAK:
    return "BREAK";    
  case CONTINUE:
    return "CONTINUE";
  case SWITCH:
    return "SWITCH";    
  case CASE:
    return "CASE";    
  case VOID:
    return "VOID";
  case FOR:
    return "FOR";
  case DEFAULT:
    return "DEFAULT";
  case DO:
    return "DO";
  case PRINT:
    return "PRINT";
  case GET:
    return "GET";
  default:
    return "ERROR";
  }
}

string MinusPlus::toString() const {
  return "MINUSPLUS("+name+")";
}

/// Funzione globale per la stampa di un Token su uno stream
///
/// @param out un riferimento allo stream di uscita
/// @param t un puntatore al token che si intende stampare
///
/// @return lo stream di uscita modificato
ostream& operator<<(std::ostream& out, const Token* t) {
  out << t->lineNumber << ": " << t->toString();
  return out;
}
