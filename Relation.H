/* 
    File: Relation.H
*/

#ifndef _relation_H_                   // include file only once
#define _relation_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <vector>
#include <sstream>
#include "Tuple.H"
#include "Value.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS  */
/*--------------------------------------------------------------------------*/

class Relation
{

private:
	//none
public:

	/* -- INTERNAL DATA STRUCTURES*/
	string name;
	vector<string> attributes;
	vector<string> keys;
	vector<Value> defaults;
	vector<Tuple> tuples;
	vector<Type> tys;
	vector<int> maxes;
	
	/* -- CONSTRUCTOR/DESTRUCTOR */

	Relation(string n);
	Relation(string n, vector<string> att, vector<string> k, vector<Value> def, vector<Type> t, vector<int> m);
	Relation(const Relation &obj);
	~Relation();
	
	/* -- FUNCTIONS -- */
	
	Relation clone();//full copy
	Relation cloneStructure();//copy without tuples
	bool checkSchema(Relation relation);//compares schema, used by union and difference
	string toScreen();//to display on screen
	string toSQL();//to print to SQL like format in file
	
};


#endif


