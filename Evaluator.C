#include "Evaluator.H"

//helper methods
string to_lower(string in)//converts a string to lower case
{
	string out = in;
	std::transform(out.begin(), out.end(), out.begin(), ::tolower);//does to conversion to lower case
	return out;//return lower case string
}

bool to_lower_equal(string s1, string s2)//conpares to strings in lower case
{
	return to_lower(s1) == to_lower(s2);
}


Evaluator::Evaluator(TokenStream& t, DBManage& db):ts(t),dbm(db)//constructor
{}


//throws error in the evaluator for reading bad syntax
void Evaluator::error()
{
	cerr << "Syntax Error: ";
	while(ts.hasNext())//prints all token up to and including ;
	{
		Token t = ts.next();
		if(t.type == SYMBOL && t.s == ";"){//break if ; token
			cerr << ";\n";
			break;
		}else if(t.type == SYMBOL)//print token
			cerr << " " << t.s << " ";
		else
			cerr << " " << t.i << " ";
	}
}

//parses symbols
bool Evaluator::par_symbol(string s)
{
	Token t = ts.next();//get token
	if (t.type == SYMBOL && t.s == s){ //check if correct symbol in token
		//cerr << "trueSY |" << t.s << "|\n";
		return true;
	}else {
		ts.putBack(t);//putback since not a match
		//cerr << "falseSY |" << t.s << "| wanted |" <<s<<"|\n";
		return false;
	}
}

//parses keywords, returns true if succeeded
bool Evaluator::par_keyword(string s)
{
	Token t = ts.next();//get token
	if (t.type == IDENTIFIER && to_lower_equal(t.s,s)){//check if identifier matches lowercase
		//cerr << "trueK |" << t.s << "|\n";
		return true;
	}else {
		//cerr << "falseK |" << t.s << "|wanted |" <<s<<"|\n";
		ts.putBack(t);//putback since failure
		return false;
	}
}

//parses names, returns true if success
bool Evaluator::par_identifier()
{
	Token t = ts.next();//get token
	if(t.type == IDENTIFIER)//check if it is an identifier
	{
		//check char & char|digit
		if(t.s.size() == 0)//check size >= 1
			return false;
		else if(!isalpha(t.s[0]))//check if first char is a character
			return false;
		else
			for(int i = 1; i < t.s.size(); i++)//check remaining chars
				if(!isalnum(t.s[i]) && !(t.s[i] == '_'))//if not alpha, digit, or _ fail
					return false;
		
		identifier_stack.push(to_lower(t.s));//add lowercase identifier to stack
		//cerr << "trueI |" << t.s << "|\n";
		return true;
	}else
	{
		//cerr << "falseI |" << t.s << "|\n";
		ts.putBack(t);//put back unused token
		return false;
	}
}

//parses integers
bool Evaluator::par_int()
{
	Token t = ts.next();//get token
	if(t.type == INT_LIT)//verify type
	{
		//cerr << "trueINT |" << t.i << "|\n";
		value_stack.push(Value(t.i));//add to value stack
		return true;
	}else
	{
		//cerr << "falseINT |" << t.i << "|\n";
		ts.putBack(t);//put back ununsed token
		return false;
	}
}

//parses strings
bool Evaluator::par_vchar()
{
	Token t = ts.next();//get token
	if(t.type == STR_LIT)//verify type
	{
		//cerr << "trueST |" << t.s << "|\n";
		value_stack.push(Value(t.s));//add to value stack
		return true;
	}else
	{
		//cerr << "falseST |" << t.s << "|\n";
		ts.putBack(t);//put back unused token
		return false;
	}
}

//main parser call, breaks it down into command and querys
bool Evaluator::eval()
{
	bool flag = par_command() || par_query();//parse query or command
	condition_vector.erase(condition_vector.begin(),condition_vector.end());//erase any conditions on vector used for reference nesting
	return flag;//return bool
}



//DML
//breaks down into each command call, returns the one it hits
bool Evaluator::par_command(){
	return (par_close() || par_open() || par_save() || par_exit() || par_showCmd() || par_create() || par_update() || par_insert()
	 || par_delete()) && par_symbol(";");
}
//hits on close funtion, returns true if syntax is correct
bool Evaluator::par_close(){ 
	if(par_keyword("CLOSE") && par_identifier()){
		dbm.closeCmd(identifier_stack.top());//call close
		identifier_stack.pop();//remove from stack
		return 1;
	}
	else
		return 0;
}
//hits on open funtion, returns true if syntax is correct
bool Evaluator::par_open(){ 
	if(par_keyword("OPEN") && par_identifier()){
		dbm.openCmd(identifier_stack.top());//call open
		identifier_stack.pop();//remove from stack
		return 1;
	}
	return 0;
}
//hits on save funtion, returns true if syntax is correct
bool Evaluator::par_save(){ 
	if(par_keyword("SAVE") && par_identifier()){
		dbm.saveCmd(identifier_stack.top());//call save
		identifier_stack.pop();//remvoe from stack
		return 1;
	}
	return 0;
}
//hits on exit funtion, returns true if syntax is correct
bool Evaluator::par_exit(){ 
	if(par_keyword("EXIT")){
		dbm.exit();//call exit
		return 1;
	}
	return 0;
}
//hits on create funtion, returns true if syntax is correct
bool Evaluator::par_create(){ 
	if(par_keyword("CREATE") && par_keyword("TABLE") && par_identifier() && par_symbol("(") && par_typelist()
		&& par_symbol(")") && par_keyword("PRIMARY") && par_keyword("KEY") && par_symbol("(")&& par_attlist() && par_symbol(")")){

		vector<string> names = typename_stack.top();//get vector of attribute names
		typename_stack.pop();//remove from stack
		vector<Type> types = typelist_stack.top();//get vector of types
		typelist_stack.pop();//remove from stack
		dbm.createCmd(identifier_stack.top(),names , attlist_stack.top(),types, max_stack.top());//call create
		identifier_stack.pop();//remove identifer from stack
		max_stack.pop();//remove maxes from stack
		attlist_stack.pop();//remove attlist from stack
        	return 1;
    }else
           return 0;
}
//hits on show funtion, returns true if syntax is correct
bool Evaluator::par_showCmd()
{
	if(par_keyword("SHOW") && par_atomic())
	{
		dbm.showCmd(relation_stack.top());//call show
		relation_stack.pop();//remove from stack
		return 1;
	}else
		return 0;
}
//hits on update funtion, returns true if syntax is correct
bool Evaluator::par_update(){
	if(par_keyword("UPDATE") && par_identifier() && par_keyword("SET") && par_attlitlist() && par_keyword("WHERE") && par_condition()){
		dbm.updateCmd(identifier_stack.top(), condition_stack.top(), attlist_stack.top(), litlist_stack.top());//call update
		identifier_stack.pop();//remove from stack data
		condition_stack.pop();
		attlist_stack.pop();
		litlist_stack.pop();
       		return 1;
    }else
           return 0;
}
//hits on delete funtion, returns true if syntax is correct
bool Evaluator::par_delete(){
	if(par_keyword("DELETE") && par_keyword("FROM") && par_identifier() && par_keyword("WHERE") && par_condition()){
		dbm.deleteCmd(identifier_stack.top(), condition_stack.top());//call delete
		identifier_stack.pop();//remove for stack
		condition_stack.pop();
		return 1;
	}
	return 0;
}
//hits on insert funtion, returns true if syntax is correct
//also checks for each case of insert
bool Evaluator::par_insert(){ 
	if(par_keyword("INSERT") && par_keyword("INTO") && par_identifier() && par_keyword("VALUES") && par_keyword("FROM")){
 		if(par_keyword("RELATION") && par_expr()){ //if input is a releation
			Relation r = relation_stack.top();//copy relation
			relation_stack.pop();//remove from stack
			dbm.insertCmd(identifier_stack.top(), r, false);//call insert without overwrite
			identifier_stack.pop();//remov identifier from stack
			return 1;
		}else if(par_symbol("(") && par_litlist() && par_symbol(")")){ //if value list
			dbm.insertCmd(identifier_stack.top(), litlist_stack.top());//call insert
			identifier_stack.pop();//remove from stack
			litlist_stack.pop();
			return 1;
		}else
			return 0;
	}else
		return 0;
}


//DQL
//parses querys
bool Evaluator::par_query()
{
	if(par_identifier() && par_symbol("<-") && par_expr() && par_symbol(";"))
	{
		Relation r = relation_stack.top();//get relation
		relation_stack.pop();//remove from stack
		dbm.insertCmd(identifier_stack.top(),r,true);//call insert with overwrite
		identifier_stack.pop();//remove from stack
		return true;
	}else
		return false;
}

//parses all expressions
bool Evaluator::par_expr()
{
	return (par_select() || par_project() || par_rename() || par_algebra());
}
//parses mathmatically symbols
bool Evaluator::par_algebra()
{
	if(!par_atomic())//get first atomic
		return false;

	if(par_symbol("+")){//union
		if(par_atomic()){//get second atomic
			Relation rB = relation_stack.top();//get second relation
			relation_stack.pop();
			Relation rA = relation_stack.top();//get first relation
			relation_stack.pop();
			relation_stack.push(dbm.setUnion(&rA,&rB));//call union and add to stack
			return true;
		}else
			return false;
	}else if(par_symbol("-")){//difference
		if(par_atomic()){//get second atomin
			Relation rB = relation_stack.top();//get second relation
			relation_stack.pop();
			Relation rA = relation_stack.top();//get first relation
			relation_stack.pop();
			relation_stack.push(dbm.setDifference(&rA,&rB));//call diff and add to stack
			return true;
		}else
			return false;

	}else if(par_symbol("*")){//product
		if(par_atomic()){//get second atomic
			Relation rB = relation_stack.top();//get second relation
			relation_stack.pop();
			Relation rA = relation_stack.top();//get first relation
			relation_stack.pop();
			relation_stack.push(dbm.product(&rA,&rB));//call product and add to stack
			return true;
		}else
			return false;

	}else
		return true;
}

//parses atomic expression
bool Evaluator::par_atomic()
{
	return ((par_symbol("(") && par_expr() && par_symbol(")")) || par_relation());
}
//parses the select function
bool Evaluator::par_select()
{
	if(par_keyword("select") && par_symbol("(") && par_condition() && par_symbol(")") && par_atomic())
	{
		Relation r = relation_stack.top();//get relation
		relation_stack.pop();//remove from stack
		relation_stack.push(dbm.selection(&r,condition_stack.top()));//add new slection to stack
		condition_stack.pop();//remove condition
		return true;
	}else
		return false;

}
//parses project fuction, returns true on correct syntax
bool Evaluator::par_project()
{
	if(par_keyword("project") && par_symbol("(") && par_attlist() && par_symbol(")") && par_atomic())
	{
		Relation r = relation_stack.top();//get relation
		relation_stack.pop();//remove from stack
		relation_stack.push(dbm.projection(&r,attlist_stack.top()));//add relation projection to stack
		attlist_stack.pop();//remove from stack
		return true;
	}else
		return false;

}
//parses rename function
bool Evaluator::par_rename()
{
	if(par_keyword("rename") && par_symbol("(") && par_attlist() && par_symbol(")") && par_atomic())
	{
		Relation r = relation_stack.top();//get relation
		relation_stack.pop();//remove from stack
		relation_stack.push(dbm.renameAttribute(&r,attlist_stack.top()));//add relation of renamed attributes to stack
		attlist_stack.pop();//remove from stack
		return true;
	}else
		return false;

}


//parses a list of attributes, or single attribute
bool Evaluator::par_attlist()
{
	vector<string> attlist;//create list
	if(!par_identifier())//if not at least one fail
		return false;

	attlist.push_back(identifier_stack.top());//get from stack and add to list
	identifier_stack.pop();//remove from stack

	while(par_symbol(","))//parse remaining attributes
		if(par_identifier()){
			attlist.push_back(identifier_stack.top());//add to list and remove from stack
			identifier_stack.pop();
		}else
			return false;
	attlist_stack.push(attlist);//add list to stack
	return true;
}
//parses a list of literals or a single literal
bool Evaluator::par_litlist(){
	vector<Value> litlist;//create list
	if(!par_literal())//parse required literal
		return false;

	litlist.push_back(value_stack.top());//add to list and remove from stack
	value_stack.pop();

	while(par_symbol(",")){//parse remaining literals
		if(par_literal()){
			litlist.push_back(value_stack.top());//add to list and remove from stack
			value_stack.pop();
		}
		else
			return false;
	}
	litlist_stack.push(litlist);//add to list stack
	return true;
}
//parses a single type or list of types for create
bool Evaluator::par_typelist(){
	vector<string> typenamelist;//create type name list
	vector<Type> typelist;//create type list
	vector<int> maxes;//create max list
	max_stack.push(maxes);//add max list to stack

	if(!(par_identifier() && par_type()))//parse first name & type pair
		return false;

	typenamelist.push_back(identifier_stack.top());//add name to list
	identifier_stack.pop();//remove from stack
	typelist.push_back(type_stack.top());//add type to list
	type_stack.pop();//remove from stack

	while(par_symbol(",")){//parse remaining pairs
		if(par_identifier() && par_type()){
			typenamelist.push_back(identifier_stack.top());//add type name to list
			identifier_stack.pop();//remove from stack
			typelist.push_back(type_stack.top());//add to list
			type_stack.pop();//remove from stack
		}else
			return false;
	}

	//add list to stack
	typelist_stack.push(typelist);
	typename_stack.push(typenamelist);
	return true;
}
//parses attribute literal lists or a single one
bool Evaluator::par_attlitlist(){
	vector<string> attlist;//create list for attributes and literals
	vector<Value> litlist;

	if(!(par_identifier() && par_symbol("=") && par_literal()))//parse first
		return false;

	attlist.push_back(identifier_stack.top());//add to list
	litlist.push_back(value_stack.top());
	value_stack.pop();//remove from stack
	identifier_stack.pop();

	while(par_symbol(",")){//parse remaining
		if(par_identifier() && par_symbol("=") && par_literal()){
			attlist.push_back(identifier_stack.top());//add to list
			litlist.push_back(value_stack.top());
			value_stack.pop();//remove from stacks
			identifier_stack.pop();
		}
		else
			return false;
	}

	//add list to stack
	attlist_stack.push(attlist);
	litlist_stack.push(litlist);
	return true;
}
//parses a type
bool Evaluator::par_type(){
	if(par_keyword("VARCHAR") && par_symbol("(") && par_int() && par_symbol(")")){//if vchar type
		type_stack.push(VCHAR);//add to type stack
		max_stack.top().push_back(value_stack.top().i);//add to max stack
		value_stack.pop();//remove from value stack
		return 1;
	}
	if(par_keyword("INTEGER")){//if integer type
		type_stack.push(INT);//add to type stack
		max_stack.top().push_back(0);//add to max stack
		return 1;
	}
	return 0;
}
//parses conditions
bool Evaluator::par_condition()
{
	if(!par_conjunction())//parse lower conjunciton first
		return false;

	if(par_symbol("||"))//if || of two conjunctions
		if(par_conjunction())
		{
			Condition c;//create new conditions
			c.comp = OR;
			c.cond2 = &(condition_vector[condition_vector.size()-1]);//add first reference
			condition_stack.pop();//remove from stack
			c.cond1 = &(condition_vector[condition_vector.size()-2]);//add second reference
			condition_stack.pop();//remove from stack
			condition_stack.push(c);//add to stack
			condition_vector.push_back(c);
			return true;
		}else
			return false;
	else
		return true;
}
//parses conjunctions
bool Evaluator::par_conjunction()
{
	if(!par_comparison())//parse lower comparision first
		return false;

	if(par_symbol("&&"))//if && of two comparisions
		if(par_comparison())
		{
			Condition c;//create new condition
			c.comp = AND;
			c.cond2 = &(condition_vector[condition_vector.size()-1]);//add 1st reference
			condition_stack.pop();//remove from stack
			c.cond1 = &(condition_vector[condition_vector.size()-2]);//add 2nd reference
			condition_stack.pop();//remove from stack
			condition_stack.push(c);//add to stack
			condition_vector.push_back(c);
			return true;
		}else
			return false;
	else
		return true;
}
//parse the comparison function
bool Evaluator::par_comparison()
{
	if(par_symbol("("))//if nested condition
		return (par_condition() && par_symbol(")"));


	Condition c;//create new condition
	c.comp = OPER;
	if(par_literal()){//if value for argument one
		c.value1 = value_stack.top();//add to condition and remove from stack
		value_stack.pop();
	}else if(par_identifier()){//if attribute name for 1st
		c.attribute1 = identifier_stack.top();//add to condition and remove from stack
		identifier_stack.pop();
	}else
		return false;

	//set op in condition
	if(par_symbol("=="))
		c.op = EQUAL;
	else if(par_symbol("!="))
		c.op = NOTEQUAL;
	else if(par_symbol(">="))
		c.op = GREATEREQUAL;
	else if(par_symbol("<="))
		c.op = LESSEQUAL;
	else if(par_symbol(">"))
		c.op = GREATER;
	else if(par_symbol("<"))
		c.op = LESS;
	else
		return false;

	
	if(par_literal()){//if 2nd is a value
		c.value2 = value_stack.top();//add to condition and remove from stack
		value_stack.pop();
	}else if(par_identifier()){//if 2nd is an attribute name
		c.attribute2 = identifier_stack.top();//add to conditon and remove from stack
		identifier_stack.pop();
	}else
		return false;
	
	//add to stack and reference vector for possible nesting of conditions
	condition_vector.push_back(c);
	condition_stack.push(c);
	return true;
}
//pareses a relation name
bool Evaluator::par_relation()
{
	if(par_identifier())//if identifier for relation name given
	{
		relation_stack.push(*dbm.getRelation(identifier_stack.top()));//add relation from memory to stack
		identifier_stack.pop();//remove identifier from stack
		return true;
	}else
		return false;
}
//parses a literal
bool Evaluator::par_literal()
{
	if(par_int())//parse a int value
		return true;
	else if(par_symbol("\"") && par_vchar() && par_symbol("\""))//parse a vchar value
	{
		return true;
	}else
		return false;

}
