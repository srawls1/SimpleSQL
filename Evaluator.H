/* 
    File: Evaluator.H
*/

#ifndef _Evaluator_H_
#define _Evaluator_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "TokenStream.H"
#include "Value.H"
#include "Condition.H"
#include "Relation.H"
#include "DBManage.H"
#include <stack>
#include <string>
#include <algorithm>
#include <locale>

//using namespace std;

/*--------------------------------------------------------------------------*/
/* CLASS  */
/*--------------------------------------------------------------------------*/


class Evaluator{
	private:
		TokenStream& ts;//token stream of tokens to evaluate
		DBManage& dbm;//database management object to modify
		
		//stacks to be used in the next part
		stack<string> identifier_stack;//stack to hold identifiers
		stack<Value> value_stack;//stack to store values
		stack<Condition> condition_stack;//stack to store conditions
		stack<Relation> relation_stack;//stack to store relations
		stack< vector<string> > attlist_stack;//stack to store attlist
		stack< vector<Value> > litlist_stack;//stack to store literal list
		stack<Type> type_stack;//stack to strore types
		stack< vector<Type> > typelist_stack;//stack to stro vector of Types
		stack < vector<int> > max_stack;//stack to store varchar maxes
		stack< vector<string> > typename_stack;//stack to store type names vector

		vector<Condition> condition_vector = vector<Condition>(128);//vector to store copies of conditions, so pointers do not move arround
			
	public:
		Evaluator(TokenStream& t, DBManage& dbm);
	
		//Main
		bool eval();//parses query or command

		//Error
		void error();//shows remaining tokens
		
		//Generic builing block parsers
		bool par_symbol(string s);//symbol such as ; ) <- + * -
		bool par_keyword(string s);//keyword such as select, project, etc.
		bool par_identifier();//parses a symbolic name; either a relation or attribute
		bool par_int();//parses a int value
		bool par_vchar();//parses a string
	
		//DML
		bool par_command();//parses one of the following for command
		bool par_close();//parses close
		bool par_open();//psrses open
		bool par_showCmd(); //parses show
		bool par_save();//parses save
		bool par_exit();//parses exit
		bool par_show();//parses show
		bool par_create();//parses create
		bool par_update();//parses update
		bool par_insert();//parses insert
		bool par_delete();//parses delete
	
		//DQL
		bool par_query();//parses query
		bool par_expr();//parses expr
		bool par_atomic();//parses atomic
		bool par_select();//parses select
		bool par_project();//parses project
		bool par_rename();//parses rename
		bool par_algebra();//union, difference, product,& attomic

		//sub parsers
		bool par_relation();//parses a relation name
		bool par_condition();//condition parses conjunction || conjunction
		bool par_conjunction();//parses comparision && comparision
		bool par_comparison();//parses operand op operand
		bool par_literal();//parses a literal
		bool par_attlist();//list of attribute names
		bool par_litlist();//list of literials
		bool par_type();//parses a type such as VARCHAR(5) or INTEGER
		bool par_typelist();//parses a type lise
		bool par_attlitlist();//parses a update atrribute = literial list
};

#endif
