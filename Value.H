/* 
    File: Value.H
*/

#ifndef _value_H_                   // include file only once
#define _value_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <string>
#include <sstream>
#include <iostream>





/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

enum Type {EMPTY, INT, VCHAR};
enum Op { EQUAL, NOTEQUAL, GREATER, LESS, GREATEREQUAL, LESSEQUAL };

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS  */
/*--------------------------------------------------------------------------*/



using namespace std;
class Value
{

private:
	//none
public:

	/* -- INTERNAL DATA STRUCTURES*/
	int i;
	string s;
	Type type;
	
	
	/* -- CONSTRUCTOR/DESTRUCTOR */

	Value(): type(EMPTY){}
	Value(int ii) : i(ii), type(INT), s(""){}
	Value(string ss) : s(ss), type(VCHAR), i(0){}
	~Value(){}

	/* -- Functions */
	bool comparison(Value value, Op op);
	string toScreen();
	string toSQL();
};


#endif


