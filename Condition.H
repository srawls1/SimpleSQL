/* 
    File: Condition.H
*/

#ifndef _condition_H_                 // include file only once
#define _condition_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "Value.H"
#include "Tuple.H"
#include "Relation.H"
#include <string>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

enum Comp {OPER,OR,AND};//how to evalutate the condition

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS  */
/*--------------------------------------------------------------------------*/

class Condition
{

private:
	//none
public:

	Comp comp;//type of comparision
	Op op; //operation used when comparing two values
	Condition* cond1; //values to evaluate and compares other conditions
	Condition* cond2;
	string attribute1 = "";//attributes to name and compare
	string attribute2 = "";
	Value value1;//values to name and compare
	Value value2;

	/*-- CONSTRUCTOR/DESTRUCTOR */

	Condition();
	~Condition();
	
	/* -- FUNCTIONS */
	bool evaluate(Relation* relation, Tuple t);//evaluate on a relation per tuple
};


#endif


