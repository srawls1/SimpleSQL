/* 
    File: DBManage.H
*/

#ifndef _dbmanageme_H_                   // include file only once
#define _dbmanageme_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Database.H"
#include "Condition.H"

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

class DBManage
{

private:
	//none
public:

	/* -- INTERNAL DATA STRUCTURES*/
	Database db;
	
	/* -- CONSTRUCTOR/DESTRUCTOR */
	DBManage();
	DBManage(Database db);//create a database with a given name
	~DBManage();
	
	/* -- FUNCTIONS -- */
	
	//DQL returs a new relation of which the following operation has been performed on the given relation
	Relation selection(Relation* relation, Condition c);//selection using condition c
	Relation projection(Relation* relation, vector<string> attributeNames);//projection of attributeNames
	Relation renameAttribute(Relation* relation, vector<string> oldAttributeNames, vector<string> newAttributeNames);//rename only a few attribute names
	Relation renameAttribute(Relation* relation, vector<string> AttributeNames);//rename all attribute names
	Relation renameRelation(Relation* relation, string newRelationName);//rename the relation
	Relation deDuplicate(Relation* relation);//remove any duplicates in the relation
	Relation setUnion(Relation* relationA, Relation* relationB);//union two relations
	Relation setDifference(Relation* relationA, Relation* relationB);//difference two relations
	Relation product(Relation* relationA, Relation* relationB);//product of two relations
	Relation* getRelation(string relationName);//retreive a relation from the database
	
	//DML
	void openCmd(string relationName);//open from file
	void closeCmd(string relationName);//save to file and remove it from memory in the database
	void saveCmd(string relationName);//save to file
	void exit();//close program
	void showCmd(string relationName);//show a relation from memory
	void showCmd(Relation& relation);//show a given relation
	void createCmd(string relationName, vector<string> attributeNames, vector<string> primaryKeys, vector<Type> types, vector<int> maxes);//create a new relation
	void updateCmd(string realtionName, Condition c, vector<string> attributeNames, vector<Value> newValues);//update an existing relation on condition c
	void insertCmd(string relationName, Relation& relation, bool overwrite);//insert a new relation or update existing
	void insertCmd(string relationName, vector<Value> newValues);//insert a new values as a tuple in a relation
	void deleteCmd(string relationName, Condition c);//delete from a relation of condition c

	//helper
	bool checkCmd(string realtionName, bool deleteR);//check if relation exists and delete if deleteR is true

	vector<int> get_int_field(string relationName, string fieldName);
	vector<string> get_string_field(string relationName, string fieldName);
};

/*--------------------------------------------------------------------------*/
/* INCLUDES That Require DBManage*/
/*--------------------------------------------------------------------------*/
#include "Parser.H"

#endif
