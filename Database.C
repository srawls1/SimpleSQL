/*
    File: Database.C
*/

#include "Database.H"

Database::Database():name("default") //name the database default
{}

Database::Database(string n):name(n)//assign a given name
{}

Database::~Database()
{}

int Database::findRelation(string relationName)//find relation index by name
{
	for(int i = 0; i < relations.size(); i++)//search relations to find database
		if(relations[i].name == relationName)//return location if found
			return i;

	return -1;//otherwise return -1
}

Relation* Database::getRelation(string relationName)//return relation when given name
{
	int i = findRelation(relationName);//get index of relation
	if(i==-1){//check if relation was found
		cerr << "Error: Relation Not Found [" << relationName << "]\n";
		return NULL;//return null if not found
	}
	return &(relations[findRelation(relationName)]);//return relation if found
}
