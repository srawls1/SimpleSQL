/* 
    File: Evaluator.C
*/


#include "TokenStream.H"
#include <cctype>

//vector of valid punctuations
vector<string> punctuations = {",", "+", "-", "*", "/", ";", "(", ")",
                         "<", ">", "<=", ">=", "<-", "&&", "||", "==", "!="};

//helper method to find index of punctuation in vector above
template <class T>
int index_of(vector<T> v, T target) {
  for (int i=0; i<v.size(); ++i) {
    if (v[i] == target)//return found location
      return i;
  }
  return -1;//if punctuation is not found
}

//Token constructor
Token::Token(TokenType ty, string str, int v)
: type(ty), s(str), i(v)
{}

//TokenStream constructor
TokenStream::TokenStream(istream& in)
: stream(in), tokenBuffer(), charBuffer()
{}

//gets next char from stream or buffer
char TokenStream::nextChar()
{
  char c;

  //removes char from buffer if not empty
  if (!charBuffer.empty()) {
    c = charBuffer.front();
    charBuffer.pop();
    return c;
  }

  //if no more chaers in the istream return ' '
  if (stream.eof()) {
    return ' ';
  }
  
  //get next char in istream
  c = stream.get();
  return c;
}

//returns whether there is more input to parse
bool TokenStream::hasNext()
{
  return !stream.eof();
}

//gets next token
Token TokenStream::next()
{
  //returns next token on buffer if not empty
  if (!tokenBuffer.empty()) {
    Token t = tokenBuffer.top();
    tokenBuffer.pop();
    return t;
  }

  char c;
  
  //removes whitespace arround tokens
  do {
    c = nextChar();
  } while (iswspace(c));

  if (c == '\"') { // parse as str_lit
    string s;
    while(true){ //get remaining chars until "
      c = nextChar();
      if(c == '\"')
        break;
      s += c;
    }
    string s2 = "\"";
    putBack(Token(SYMBOL, s2, 0));//adds " token to stack
    putBack(Token(STR_LIT, s, 0));//adds string token to the stack
    return Token(SYMBOL, s2, 0);//returns a " token
  }
  else if (isalpha(c)) { // parse as identifier
    string s;
	
	//gets entire name of identifier
    do {
      s += c;
      c = nextChar();
    } while (isalnum(c) || c == '_');
    if (!iswspace(c)) {
      putBack(c);
    }
    return Token(IDENTIFIER, s, 0);//returns token with string value
  }
  else if (isdigit(c)) { // parse as int_lit
    string s;
	
	//gets all the digits
    do {
      s += c;
      c = nextChar();
    } while (isdigit(c));
    if (!iswspace(c)) {
      putBack(c);
    }
    return Token(INT_LIT, "", stoi(s));//returns roken with int value
  }
  else if (ispunct(c)) { // parse as symbol
    string s;
	
	//parses symbols in punctuation vector above
    do {
      s += c;
      c = nextChar();
      int i = index_of(punctuations, s);
      if (i == -1 && s.length() == 1) continue;
      if (i < 8) break;
      if (i >= 8 && s.length() == 1) continue;
      if (i >= 8 && s.length() > 1) break;
      if (i == -1 && s.length() > 1) {
        putBack(s[s.length()-1]);
        s.erase(s.length()-1, 1);
        break;
      }
    } while (ispunct(c));

    if (!iswspace(c)) {
      putBack(c);
    }
    return Token (SYMBOL, s, 0);//returns symbol token
  }
  else {
    return Token(NILL, "", 0);//returns null token
  }
}

//adds token to token buffer
void TokenStream::putBack(char c)
{
  charBuffer.push(c);
}

//adds char to char buffer
void TokenStream::putBack(Token t)
{
  tokenBuffer.push(t);
}
