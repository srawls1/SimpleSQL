/* 
    File: TokenStream.H
*/

#ifndef _TokenStream_H
#define _TokenStream_H

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <string>
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

//type to classify the tokens
enum TokenType {
  IDENTIFIER,//keyword, relation name, or attribute name
  SYMBOL,// ; , <- ( ) == != > < >= <= or etc.
  INT_LIT,//integer
  STR_LIT,//string
  NILL//null
};

struct Token {
  TokenType type;// hold type information
  string s; // holds identifier, symbol, or string literal
  int i; // holds int

  Token(TokenType ty, string str, int v);//constructor
};

/*--------------------------------------------------------------------------*/
/* CLASS  */
/*--------------------------------------------------------------------------*/

class TokenStream {
  istream& stream;//stream of input to process
  queue<char> charBuffer;//queue of chars to process
  stack<Token> tokenBuffer;//stack of tokens put back
  char nextChar();//get next char from stream or buffer
  void putBack(char c);//put back a char on the buffer
  
public:
  TokenStream(istream& in);//constructor
  bool hasNext();//return true if there is another token
  Token next();//gets next token
  void putBack(Token t);//put back a token
};

#endif
