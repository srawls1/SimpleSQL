/* 
    File: Condition.H
*/
#include "Condition.H"

Condition::Condition(){}//default constructor

Condition::~Condition(){}

/* -- FUNCTIONS */

//helper to hind index of object in vector
template <class T>
int loc_of(vector<T> v, T target) {
	for (int i=0; i<v.size(); ++i)//search through list
		if (v[i] == target)//return found index
			return i;
	return -1;//return failure
}

bool Condition::evaluate(Relation* relation, Tuple t)//evaluates a conditon on a relation and tuple
{
	if(attribute1 != "")//if first parameter is attribute
	{
		int i1 = loc_of(relation->attributes,attribute1);//get tuple value
		if(i1 == -1)//if attribute not found
			cerr << "Error: Attribute Not Found" << attribute1;
		else
			value1 = t.values[i1];//set to tuple value
	}
	if(attribute2 != "")//if second paramert is attribute
	{
		int i2 = loc_of(relation->attributes,attribute2);//get tuple value
		if(i2 == -1)//if attribute not found
			cerr << "Error: Attribute Not Found" << attribute2;
		else
			value2 = t.values[i2];//set tuple value
	}
	if(comp==OPER)//if comparing values
		return value1.comparison(value2,op);
	else if(comp==OR)//if or between conditions
		return cond1->evaluate(relation,t) || cond2->evaluate(relation,t);
	else if(comp==AND)//if and between condtions
		return cond1->evaluate(relation,t) && cond2->evaluate(relation,t);
	else//default to always true if no condition given
	{
		cerr << "Failed to find case\n";
		return true;
	}
}

