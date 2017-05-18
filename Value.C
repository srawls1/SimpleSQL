/* 
    File: Value.C
*/

#include "Value.H"

using namespace std;
/* -- Functions */

bool Value::comparison(Value value, Op op)
{
	if(type == EMPTY && value.type == EMPTY)//compares Nulls
	{
		switch(op)
		{
			case EQUAL:			return true;
			case NOTEQUAL:		return false;
			case GREATER:		return false;
			case LESS:			return false;
			case GREATEREQUAL:	return true;
			case LESSEQUAL:		return true;
			default:			cerr << "Error: Op Not Found"; //need to convert to thrown error;
		}
	}
	else if (type == INT && value.type == INT)//compares ints
	{
		switch(op)
		{
			case EQUAL:			return i == value.i;
			case NOTEQUAL:		return i != value.i;
			case GREATER:			return i > value.i;
			case LESS:			return i < value.i;
			case GREATEREQUAL:	return i >= value.i;
			case LESSEQUAL:		return i <= value.i;
			default:			cerr << "Error: Op Not Found"; //need to convert to thrown error;
		}
	}else if(type == VCHAR && value.type == VCHAR)//compares strings
	{
		switch(op)
		{
			case EQUAL:			return s == value.s;
			case NOTEQUAL:		return s != value.s;
			case GREATER:		return s > value.s;
			case LESS:			return s < value.s;
			case GREATEREQUAL:	return s >= value.s;
			case LESSEQUAL:		return s <= value.s;
			default:			cerr << "Error: Op Not Found"; //need to convert to thrown error;
		}
	}else//default error
		cerr << "Error: Comparison Type Mismatch"; //need to convert to thrown error
}

string Value::toScreen()//prints in screen format
{
	stringstream output;
	switch(type){
		case EMPTY : return "NULL, "; break;
		case INT : 
					output<<""<<i<< ", "; 
					return output.str();
					break;
		case VCHAR : output<< "\""<<s<<"\", ";
						return output.str();break;
		default : return ""; //make this an error
		}
}

string Value::toSQL()//prints in SQL like format, used to save to file
{
	stringstream output;
	switch(type){
		case EMPTY : return ""; break;
		case INT : 
					output<<""<<i<< ", "; 
					return output.str();
					break;
		case VCHAR : output<< "\""<<s<<"\", ";
						return output.str();break;
		default : return ""; //make this an error
		}
}

