/* 
    File: Database.H
*/

#ifndef _database_H_                   // include file only once
#define _database_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <vector>
#include <string>
#include "Relation.H"

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

class Database
{

private:
	//none
public:

	/* -- INTERNAL DATA STRUCTURES*/
	string name;//name of database
	vector<Relation> relations;//holds relations
	
	/* -- CONSTRUCTOR/DESTRUCTOR */

	Database();//default
	Database(string n);//assigns a name
	~Database();
	
	/* -- FUNCTIONS -- */
	int findRelation(string relationName);//returns index or -1 if not found
	Relation* getRelation(string relationName);//returns relation from name or null
};


#endif


