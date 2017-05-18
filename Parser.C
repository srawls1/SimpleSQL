/*
    File: Parser.C
*/

#include "Parser.H"
using namespace std;
//default constructor
Parser::Parser()
{
	string s = " ";
	std::stringstream ss;//create stream and add " "
	ss << s;
	ts = new TokenStream(ss);//create TokenStream with no valid tokens
	dbm = new DBManage();//New default DBManage
	deleteDBM = true;
}

//constructor, given only a DBManage object
Parser::Parser(DBManage* d):dbm(d),deleteDBM(false)
{
	string s = " ";
	std::stringstream ss;//create stream and add " "
	ss << s;
	ts = new TokenStream(ss);//create TokenStream with no valid tokens
}

//constructor, token stream arguement
Parser::Parser(TokenStream* t)
{
	ts = t;//assign tokenstream
	dbm = new DBManage();
	deleteDBM = true;
	parse();//parse until nothing is left
}

//constructor, istream argument
Parser::Parser(istream& in)
{
	Parser(new TokenStream(in));//create parser given istream
}

//constructor, istream argument and DBManage argument
Parser::Parser(istream& in, DBManage* d):dbm(d),deleteDBM(false)
{
	ts = new TokenStream(in);//create parser given istream
	parse();//parse until nothing is left
}

//deconstructor
Parser::~Parser()
{
	delete ts;//delete tokenstream object
	if(deleteDBM)
		delete dbm;
}

//main parse function, calls eval
bool Parser::parse()
{
	Evaluator e = Evaluator(*ts,*dbm);//create evaluator
	while(ts->hasNext())//evaluate all tokens
		if(!e.eval())//if one eval does not pass then fail
			return false;
	return true;
}

//used for test program, takes a string a turns into
//token stream and prints to see if working correctly
bool Parser::parse(const string& s)
{
	std::stringstream ss;//create stringstream of input string
	ss << s;
	ts = new TokenStream(ss);//create tokenstream of input
	Evaluator e = Evaluator(*ts,*dbm);//create new evaluator
	return e.eval();//evaluate the input
}

void Parser::set_input(istream& i) {
	delete ts;
	ts = new TokenStream(i);
	parse();
}

DBManage* Parser::get_dbm()
{
	return dbm;
}
