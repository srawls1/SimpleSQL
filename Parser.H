/*
    File: Parser.H
*/

#ifndef _Parser_H_
#define _Parser_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "TokenStream.H"
#include "Evaluator.H"
#include "DBManage.H"
#include <iostream>
#include <sstream>
#include <string>

/*--------------------------------------------------------------------------*/
/* CLASS  */
/*--------------------------------------------------------------------------*/

class Parser{
	private:
		bool deleteDBM;//whether to delete the dbm
		DBManage* dbm;//database object to modify
		TokenStream* ts; //tokenstream to read from
		
	public:
		Parser(); //default constructor
		Parser(DBManage* d);//constructor for DBmanage
		Parser(TokenStream* t); //constructor, tokenstream arguement
		Parser(istream& i); //constructor, istream arguement
		Parser(istream& i, DBManage* d); //constructor, istream arguement
		~Parser();

		//Generic
		bool parse(); //main parse functions, calls evaluator
		bool parse(const string& s); // test parse function
		void set_input(istream& i);

		DBManage* get_dbm();//return dbm object
};

#endif
