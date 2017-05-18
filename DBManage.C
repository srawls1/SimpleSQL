/*
    File: DBManage.C
*/
#include "DBManage.H"

//find index in vector of object
template <class T>
int index_of(vector<T> v, T target) {
	for (int i=0; i<v.size(); ++i) {
		if (v[i] == target)
			return i;
	}
	return -1;
}

/* -- CONSTRUCTOR/DESTRUCTOR */
DBManage::DBManage()
{
	db=Database();
}

DBManage::DBManage(Database database):db(database)
{}

DBManage::~DBManage()
{}

/* -- FUNCTIONS -- */

//DQL

//select relation on condition
Relation DBManage::selection(Relation* relation, Condition c)
{

	Relation& r = *relation;
	Relation nr = r.cloneStructure();
	for (Tuple t : r.tuples)
		if (c.evaluate(relation,t))//if true then keep
			nr.tuples.push_back(t);
	return nr;
}


//project nly attributeNames
Relation DBManage::projection(Relation* relation, vector<string> attributeNames)
{
	Relation& r = *relation;
	vector<int> indices;//vector of locations to keep
	Relation nr = relation->name;
	for (string s : attributeNames) {
		int i = index_of(r.attributes, s);//find attribute location
		if (i < 0) {
			cerr << "Error: Attribute name not found: " << s;
		}
		indices.push_back(i);//add to list to keep
		if(index_of(r.keys,s) != 0)//keep approperiate keys
			nr.keys.push_back(s);
	}

	for (int i : indices) { //copy attributes, defaults, types, and maxes
		nr.attributes.push_back(r.attributes[i]);
		nr.defaults.push_back(r.defaults[i]);
		nr.tys.push_back(r.tys[i]);
		nr.maxes.push_back(r.maxes[i]);
	}

	for (Tuple t : r.tuples) { //loop through tuples and do the same
		Tuple nt;

		for (int i : indices)//copy only necessary attributes to new tuple
			nt.values.push_back(t.values[i]);

		nr.tuples.push_back(nt);
	}

	return nr;
}

Relation DBManage::renameAttribute(Relation* relation, vector<string> AttributeNames)
{
	return renameAttribute(relation,relation->attributes,AttributeNames);
}


//rename attributes
Relation DBManage::renameAttribute(Relation* relation, vector<string> oldAttributeNames, vector<string> newAttributeNames)
{
	if(oldAttributeNames.size() != newAttributeNames.size())//verify one to one pairs
		cerr << "Error: Attribute Length Mismatch";

	Relation newRelation = relation->clone();//copy relation
	for(int j = 0; j < oldAttributeNames.size(); j++)
	{
	bool found = false;
		for(int i = 0; i < newRelation.attributes.size(); i++)
		{
			if(oldAttributeNames[j] == newRelation.attributes[i])//search for attribute match
			{
				newRelation.attributes[i] = newAttributeNames[j];//update name
				found = true;
			}
		}
		if(!found)
			cerr << "Error: Attribute Not Found " + oldAttributeNames[j];
	}
	return newRelation;
}

//renames a relation
Relation DBManage::renameRelation(Relation* relation, string newRelationName)
{
	Relation newRelation = relation->clone();
	newRelation.name=newRelationName;
	return newRelation;
}

//remove any duplicates
Relation DBManage::deDuplicate(Relation* relation)
{
	Relation newRelation = relation->cloneStructure();//copy relation structure
	vector<Tuple> tuples = relation->tuples;//get list of tuples
	for(int i = 0; i < tuples.size(); i++)//get tuple
	{
		bool found = false;
		Tuple tupleA = tuples[i];
		for(int j = i+1; j < tuples.size(); j++)//search remaining for duplicate
		{
			Tuple tupleB = tuples[j];
			if(tupleA.equals(tupleB))//if duplicate
				found = true;

		}

		if(!found)
			newRelation.tuples.push_back(tupleA);//only if not duplicate
	}
	return newRelation;
}

//union
Relation DBManage::setUnion(Relation* relationA, Relation* relationB)
{
	if(!(relationA->checkSchema(*(relationB))))//verify schema
	{
		cerr << "Error: Schema Mismatch";
	}

	Relation newRelation = relationA->cloneStructure();//copy structure
	vector<Tuple> tuplesA = relationA->tuples;
	for(int i = 0; i < tuplesA.size(); i++)//add first relation of tuples
		newRelation.tuples.push_back(tuplesA[i]);

	vector<Tuple> tuplesB = relationB->tuples;
	for(int i = 0; i < tuplesB.size(); i++)//add second relation of tuples
		newRelation.tuples.push_back(tuplesB[i]);

	return deDuplicate(&newRelation);//remove duplicates
}

//difference
Relation DBManage::setDifference(Relation* relationA, Relation* relationB)
{
	Relation newRelation = relationA->cloneStructure();//copy structure
	vector<Tuple> tuplesA = relationA->tuples;
	vector<Tuple> tuplesB = relationB->tuples;
	for(int i = 0; i < tuplesA.size(); i++)//first relation
	{
		bool found = false;
		Tuple tupleA = tuplesA[i];
		for(int j = 0; j < tuplesB.size(); j++)//second relation
		{
			Tuple tupleB = tuplesB[j];
			if(tupleA.equals(tupleB))//if same tuple
				found = true;

		}
		if(!found)
			newRelation.tuples.push_back(tupleA);//if not match add A's tuples
	}
	return deDuplicate(&newRelation);//remove duplicates
}


Relation DBManage::product(Relation* relationA, Relation* relationB)
{
	//create new attributes vector
	vector<string> newAttributes = relationA->attributes;
	for(string temp : relationB->attributes)
		newAttributes.push_back(temp);

	//create new values vector
	vector<Value> newValues = relationA->defaults;
	for(Value temp : relationB->defaults)
		newValues.push_back(temp);

	//create new types vector
	vector<Type> newTypes = relationA->tys;
	for(Type temp : relationB->tys)
		newValues.push_back(temp);

	//create new maxes vector
	vector<int> newMaxes = relationA->maxes;
	for(int temp : relationB->maxes)
		newValues.push_back(temp);



	vector<string> keys = vector<string>();//no keys
	Relation newRelation = Relation("Product", newAttributes, keys, newValues, newTypes, newMaxes);//create relation
	vector<Tuple> tuplesA = relationA->tuples;
	vector<Tuple> tuplesB = relationB->tuples;
	for(int i = 0; i < tuplesA.size(); i++)//add first tuple
	{
		for(int j = 0; j < tuplesB.size(); j++)//add second tuple
		{
			vector<Value> newValues = (tuplesA[i]).values;
			for(Value temp : (tuplesB[j]).values)
				newValues.push_back(temp);
			Tuple newTuple = Tuple(newValues);
			newRelation.tuples.push_back(newTuple);//add new tuple
		}
	}
	return newRelation;
}

Relation* DBManage::getRelation(string relationName)
{
	return db.getRelation(relationName);
}

//DML
//implement with parser
void DBManage::openCmd(string relationName)
{
	std::ifstream file;
	file.open(relationName+".db");
	if(!file.is_open())
		cerr << "Error: Failed to open " << relationName << ".db\n";

	checkCmd(relationName,true);//delete existing
	Parser p = Parser(file,this);//add to database
}
//saves and closes
void DBManage::closeCmd(string relationName)
{
	saveCmd(relationName);
	db.relations.erase(db.relations.begin()+db.findRelation(relationName));
}
//saves to .db file
void DBManage::saveCmd(string relationName)
{
	Relation& relation = *(db.getRelation(relationName));
	std::ofstream ofs;
	ofs.open(relationName+".db");
	ofs << relation.toSQL();
	ofs.close();
}
//exits the database
void DBManage::exit()
{
	cout << "EXIT" << endl;
	std::exit(0);
}
//prints the relation
void DBManage::showCmd(string relationName)
{
	showCmd(*(db.getRelation(relationName)));
}
void DBManage::showCmd(Relation& relation)
{
	cout << relation.toScreen();
}
//creates a new relation
void DBManage::createCmd(string relationName, vector<string> attributeNames, vector<string> primaryKeys, vector<Type> ty, vector<int> ma)
{
	if(checkCmd(relationName,false))
	{
		cerr << "Error: Relation with that name already exists, please delete it first\n";
		return;
	}
	vector<Value> values = vector<Value>(attributeNames.size(),Value());
	Relation nr(relationName,attributeNames,primaryKeys,values,ty,ma);
	db.relations.push_back(nr);
}

//update tuples on condtion
void DBManage::updateCmd(string relationName, Condition c, vector<string> updateAttributeNames, vector<Value> newValues)
{
	Relation* rp = (db.getRelation(relationName));
	Relation& r = *rp;
	for(int i = 0; i < r.tuples.size(); i++)
	{
		if (c.evaluate(rp,r.tuples[i])) { //if condition true update
			for (int j = 0; j < updateAttributeNames.size(); j++) { //find attribute to update
				int loc = index_of(r.attributes, updateAttributeNames[j]);
				if (loc < 0) {
					cerr << "Error: attribute name not found: " << updateAttributeNames[j];
				}
				(r.tuples[i]).values[loc] = newValues[j];//update value
			}
		}
	}

}


//inserts a new relation
void DBManage::insertCmd(string relationName, Relation& relation, bool overwrite)
{
	bool found = checkCmd(relationName,overwrite);

	if(found && !overwrite)//if found in database and set to not overwrite
	{
		if(!(relation.checkSchema(*(db.getRelation(relationName)))))//verify schema
		{
			cerr << "Error: Relation exists and there is a Schema Mismatch";
			return;
		}

		Relation& r = db.relations[db.findRelation(relationName)];
		for (Tuple t : relation.tuples)
			r.tuples.push_back(t);
	}else{//if to overwirte or add new relation
		Relation r = relation.cloneStructure();
		r.name=relationName;
		for (Tuple t : relation.tuples)
			r.tuples.push_back(t);
		db.relations.push_back(r);
	}
}
//inserts new values into relation
void DBManage::insertCmd(string relationName, vector<Value> newValues)
{
	Relation& r = *(db.getRelation(relationName));
	Tuple newTuple;
	if (newValues.size() != r.tys.size()){
		cerr << "Error: new tuple size mismatch";
	}
	for (int i = 0; i<newValues.size(); i++){
		if((newValues[i]).type == r.tys[i]){ //check type
			if(r.tys[i] == VCHAR) 
				if((newValues[i]).s.size()>r.maxes[i]) //check size if vchar
					cerr << "Error: VCHAR too long";
			newTuple.values.push_back(newValues[i]); //add value
		}else
			cerr << "Error: Insert type mismatch";
	}

	for (int i = newValues.size(); i < r.defaults.size(); i++){ //fill left over spots with defaults
		newTuple.values.push_back((r.defaults)[i]);
	}
		r.tuples.push_back(newTuple); //add tuple
}

//delete on condition
void DBManage::deleteCmd(string relationName, Condition c)
{
	Relation* rp = (db.getRelation(relationName));
	Relation& r = *rp;
	for (int j = 0; j < r.tuples.size(); j++)
		if (c.evaluate(rp,r.tuples[j])) { //if condition true then delete tuple			
			r.tuples.erase(r.tuples.begin()+j);
			j--;
		}
}

//check if relation exists and delete if bool is true
bool DBManage::checkCmd(string relationName, bool deleteR)
{
	if(db.findRelation(relationName)!=-1)//if relation exists
	{
		if(deleteR)//delete if set to
			db.relations.erase(db.relations.begin()+db.findRelation(relationName));
		return true;
	}else
		return false;
}

vector<int> DBManage::get_int_field(string relationName, string fieldName)
{
	Relation& r = *(db.getRelation(relationName));
	int i = index_of(r.attributes, fieldName);
	vector<int> result;
	if(i == -1)
		cerr << "Attribute not found [" << fieldName << "]\n";
	else
		for (Tuple t : r.tuples)
			result.push_back((t.values[i]).i);
	return result;
}

vector<string> DBManage::get_string_field(string relationName, string fieldName)
{
	Relation& r = *(db.getRelation(relationName));
	int i = index_of(r.attributes, fieldName);
	vector<string> result;
	if(i == -1)
		cerr << "Attribute not found [" << fieldName << "]\n";
	else
		for (Tuple t : r.tuples)
			result.push_back((t.values[i]).s);
	return result;
}
