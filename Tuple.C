/* 
    File: Tuple.C
*/
#include "Tuple.H"

Tuple::Tuple()
{
	vector<Value> values = vector<Value>();
}

Tuple::Tuple(vector<Value> v):values(v)
{}


Tuple::~Tuple()
{}

bool Tuple::equals(Tuple tuple)//check if all values equal
{
	if(values.size() != tuple.values.size())//checks size first
		return false;
	
	for(int i = 0; i < values.size(); i++)//checks each pair of values
		if(!((values[i]).comparison(tuple.values[i],EQUAL)))
			return false;

	return true;
}

string Tuple::toScreen()//writes to screen a tuple
{
	string result="";
	for(Value v : values)
		result += v.toScreen();
	return result.substr(0,result.length()-2);//removes last comma
}

string Tuple::toSQL()//writes to file format
{
	string result="";
	for(Value v : values)
		result += v.toSQL();
	return result.substr(0,result.length()-2);//removes last comma
}
