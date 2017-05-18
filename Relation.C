/*
    File: Relation.C
*/

#include "Relation.H"
//constructor w/ name
Relation::Relation(string n)
{
	name=n;
	attributes=vector<string>();
	keys=vector<string>();
	defaults=vector<Value>();
	tuples=vector<Tuple>();
	tys=vector<Type>();
	maxes=vector<int>();
}
//constructor with all attributes
Relation::Relation(string n, vector<string> att, vector<string> k, vector<Value> def, vector<Type> t, vector<int> m)
: name(n), attributes(att), keys(k), defaults(def), tys(t), maxes(m)
{}
//copy constructor
Relation::Relation(const Relation &obj){
	name = obj.name;
	attributes = obj.attributes;
	keys = obj.keys;
	defaults = obj.defaults;
	tuples = obj.tuples;
	tys=obj.tys;
	maxes=obj.maxes;
}

//deconstructor 
Relation::~Relation()
{}
//copys relation
Relation Relation::clone()
{
	return Relation(*this);
}
//returns a relation with the same structure to be changed
Relation Relation::cloneStructure()
{
	return Relation(name,attributes,keys,defaults,tys,maxes);
}

bool Relation::checkSchema(Relation relation)
{
	if(attributes.size() != relation.attributes.size())
		return false;

	for(int i = 0; i < attributes.size(); i++)
		if((attributes[i]!=(relation.attributes)[i]) || (tys[i]!=(relation.tys)[i]) || (maxes[i]!=(relation.maxes)[i]))
			return false;

	return true;
}
//prints to the screen
string Relation::toScreen()
{
	string result=name+":\n";
	for(string s : attributes)
		result+=s+", ";
	result=result.substr(0,result.length()-2)+"\n";
	for(Tuple t : tuples)
		result+=t.toScreen()+"\n";
	return result;
}
//prints to be saved to a .db file
string Relation::toSQL()
{
	string result="CREATE TABLE "+name+" (";
	for(int i=0; i < attributes.size(); i++)
		if(tys[i]==VCHAR)
		{
			std::stringstream ss;
			ss << maxes[i];
			result+=attributes[i]+" VARCHAR("+ss.str()+"), ";
		}else
			result+=attributes[i]+" INTEGER, ";
	result=result.substr(0,result.length()-2)+")";

	if(keys.size()>0)
	{
		result+=" PRIMARY KEY (";
		for(string s : keys)
			result+=s+", ";
		result=result.substr(0,result.length()-2)+")";
	}
	result+=";\n";
	for(Tuple t : tuples)
		result+="INSERT INTO "+name+" VALUES FROM ("+t.toSQL()+");\n";
	return result;
}
