#include "DBManage.H"
#include "Value.H"
#include "Relation.H"
#include "Condition.H"
#include <stdio.h>

int main() {

	DBManage dbm = DBManage();//create control objects

	//create table/relation
	cout << "--Test createCmd TR1--\n\n";
	vector<string> att = {"Name", "Birthdate", "Age"};
	vector<string> key = {"Name", "Birthdate"};
	vector<Type> t = {VCHAR,VCHAR,INT};
	vector<int> m = {32,32,-1};
	dbm.createCmd("TR1",att,key,t,m);
	cout << "--Show TR1--\n\n";
	dbm.showCmd("TR1");

	//insert values into relation
	cout << "--Insert INTO TR1--\n\n";
	vector<Value> v = {Value("Bob Smith"), Value("1970/1/1"), Value(45)};
	dbm.insertCmd("TR1",v);
	v = {Value("John Smith"), Value("1970/1/1"), Value(45)};
	dbm.insertCmd("TR1",v);
	v = {Value("Jane Smith"), Value("1970/1/1"), Value(45)};
	dbm.insertCmd("TR1",v);
	cout << "--Show TR1--\n\n";
	dbm.showCmd("TR1");

	//demo selection
	cout << "--Show Select TR1--\n\n";
	Condition c;
	c.comp=OPER;
	c.op=GREATEREQUAL;
	c.attribute1="Name";
	c.value2=Value("Jane");
	//Relation r1 = dbm.selection(dbm.getRelation("TR1"),"Name",GREATEREQUAL,Value("Jane"));
	Relation r1 = dbm.selection(dbm.getRelation("TR1"),c);
	dbm.showCmd(r1);

	//create relation TR2
	cout << "--Create Realtion TR2--\n\n";
	dbm.createCmd("TR2",att,key,t,m);

	//insert into relation TR2
	cout << "--Insert Relation INTO TR2--\n\n";
	v = {Value("Bob Smith"), Value("2000/1/1"), Value(15)};
	dbm.insertCmd("TR2",v);
	v = {Value("John Smith"), Value("2000/1/1"), Value(15)};
	dbm.insertCmd("TR2",v);
	v = {Value("Jane Smith"), Value("2000/1/1"), Value(15)};
	dbm.insertCmd("TR2",v);
	cout << "--Show TR2--\n\n";
	dbm.showCmd("TR2");

	//demo projection
	cout << "--Project On Name & Birthdate--\n\n";
	att={"Name","Birthdate"};
	Relation r = dbm.projection(dbm.getRelation("TR1"),att);
	cout << "--Show Projection--\n\n";
	dbm.showCmd(r);

	//demo union
	cout << "--Union TR1 & selection of TR1--\n\n";
	r = dbm.setUnion((dbm.getRelation("TR1")),&r1);
	cout << "--Show Union--\n\n";
	dbm.showCmd(r);

	//demo difference
	cout << "--Difference TR1 - selection of TR1--\n\n";
	r = dbm.setDifference((dbm.getRelation("TR1")),&r1);
	cout << "--Show TR1-TR2--\n\n";
	r = dbm.renameRelation(&r,"Difference");
	dbm.showCmd(r);

	//demon rename on TR3
	cout << "--TR3: Rename On Name &Birthdate to Full & Birth--\n\n";
	vector<string> oatt = {"Birthdate","Name"};
	vector<string> natt = {"Birth","Full"};
	Relation r3 = dbm.renameAttribute(dbm.getRelation("TR1"),oatt,natt);
	cout << "--Show Rename--\n\n";
	dbm.showCmd(r3);

	//demo Product
	cout << "--Product TR1 & TR3 minus Age--\n\n";
	Relation r4 =  dbm.projection(&r3,natt);
	dbm.showCmd(r4);
	r = dbm.product((dbm.getRelation("TR1")),&r4);
	cout << "--Show Product--\n\n";
	dbm.showCmd(r);

	//demo Delete
	cout << "--Delete --\n\n";
	dbm.deleteCmd("TR1",c);
	cout << "--Show Delete--\n\n";
	dbm.showCmd("TR1");

	//demo update
	cout << "--Update --\n\n";
	vector<string> up = {"Birthdate","Age"};
	vector<Value> vals = {Value("12/12/2000"),Value(14)};
	dbm.updateCmd("TR2",c,up,vals);
	cout << "--Show Update--\n\n";
	dbm.showCmd("TR2");

	//demo condition
	Condition c2;
	c2.comp = OPER;
	c2.op = GREATEREQUAL;
	c2.value1 = Value(5);
	c2.value2 = Value(6);
	Relation *r2;
	Tuple t2;
	cout << "\n  --OPER Condtion Test-- \n" << c2.evaluate(r2,t2) << endl;
	Condition c3;
	c3.comp = OPER;
	c3.op = EQUAL;
	c3.value1 = Value("Hello");
	c3.value2 = Value("Hello");
	cout << "\n  --OPER Condtion Test-- \n" << c3.evaluate(r2,t2) << endl;
	Condition c4;
	c4.comp = OPER;
	c4.op = LESSEQUAL;
	c4.value1 = Value(4);
	c4.value2 = Value(50);
	cout << "\n  --OPER Condtion Test-- \n" << c4.evaluate(r2,t2) << endl;
	Condition c5, c6;
	c5.comp = AND;
	c6.comp = AND;
	c5.cond1 = &c2;
	c6.cond1 = &c3;
	c5.cond2 = &c3;
	c6.cond2 = &c4;
	cout << "\n  --AND Condtion Test-- \n" << c5.evaluate(r2,t2) << " " << c6.evaluate(r2,t2) << endl;
	c5.comp = OR;
	c6.comp = OR;
	cout << "\n  --OR Condtion Test-- \n" << c5.evaluate(r2,t2) << " " << c6.evaluate(r2,t2) << endl;
	cout << endl;

	//demo close
	cout << "--Close TR2 and Save TR1--\n\n";
	dbm.closeCmd("TR2");
	dbm.saveCmd("TR1");

	//demo exit
	cout <<"--Exit--\n\n";
	dbm.exit();
}
