
#include "App.H"
#include <iostream>
#include <algorithm>

using namespace std;

//read in account and log into db, init num_users and num_events
void App::set_up_accounts() {
	stringstream ss;
	ss<<"OPEN accounts;";
	parser.parse(ss.str());
	vector<int> tmp0 = dbm->get_int_field("accounts", "userid");
	if(tmp0.size() == 0)
		num_users = 0;
	else
		num_users = tmp0[tmp0.size()-1];


	parser.parse("OPEN log;");
	vector<int> tmp1 = dbm->get_int_field("log", "eventid");
	if(tmp1.size() == 0)
		num_events = 0;
	else
		num_events = tmp1[tmp1.size()-1];
}

//read in group and groupmembers into db, init num_groups
void App::set_up_groups() {
	parser.parse("OPEN groups;");
	parser.parse("OPEN groupmember;");
	vector<int> tmp0 = dbm->get_int_field("groups", "groupid");
	if(tmp0.size() == 0)
		num_groups = 0;
	else
		num_groups = tmp0[tmp0.size()-1];
}

//read in message into db, init num_messages
void App::set_up_messages(){
	parser.parse("OPEN message;");
	vector<int> tmp0 = dbm->get_int_field("message", "messageid");
	if(tmp0.size() == 0){
		num_messages = 0;
	}
	else
		num_messages = tmp0.size();
}
//read in boards, access, and article; init num_boards
void App::set_up_boards() {
	parser.parse("OPEN boards;");
	parser.parse("OPEN access;");
	vector<int> tmp0 = dbm->get_int_field("boards", "boardid");
	if(tmp0.size() == 0)
		num_boards = 0;
	else
		num_boards = tmp0[tmp0.size()-1];
}

//read in boards, access, and article; init num_boards
void App::set_up_menus() {
	stringstream ss0, ss1;//, ss2;
	ss0 << "OPEN menus;";
	parser.parse(ss0.str());
	ss1 << "OPEN menuitem;";
	parser.parse(ss1.str());
	//ss2 << "SHOW menus;";
	//parser.parse(ss2.str());
}

//read in article; init num_articles
void App::set_up_articles() {
	stringstream ss;
	ss << "OPEN article;";
	parser.parse(ss.str());
	vector<int> tmp0 = dbm->get_int_field("article", "articleid");
	if(tmp0.size() == 0)
		num_articles = 0;
	else
		num_articles = tmp0[tmp0.size()-1];
}

//helper method to create board
void App::create_board(int id, string board_name, int status) {
	stringstream ss;
	ss << "INSERT INTO boards VALUES FROM (" << id << ", \"" << board_name << "\", " << status <<");";
	parser.parse(ss.str());
	parser.parse("SAVE boards");
}

//helper method to create user
void App::create_user(int id, string username, string password, string full_name,
	string phone, string fax, string address) {

  stringstream ss;
  ss << "INSERT INTO accounts VALUES FROM (" << id << ", \"" << username
		<< "\", \"" << password << "\", \"" << full_name << "\", \"" << phone
		<< "\", \"" << fax << "\", \"" << address << "\", 0);";
  parser.set_input(ss);
	parser.parse("SAVE accounts;");
}

//helper method to create article
void App::create_article() {

	cout <<"Enter title: ";
	string articleTitle;
	cin >> articleTitle;

	cout << "Type in the article:";
	string article;
	getline(cin,article);

	stringstream ss;

}

//helper method to del article
void App::delete_article() {
	cout << "Enter title: ";
	string articleTitle;
	cin >> articleTitle;

	stringstream ss;
	ss << "DELETE FROM article WHERE title == \"" << articleTitle << "\";";
	parser.parse(ss.str());
	parser.parse("SAVE article;");
}

//main menu display function, data stored in db
void App::display_menu()
{
	stringstream ss;
	ss << "current_menu <- select ( menuid == " << menuid << " ) menus;";//get menu name
	parser.parse(ss.str());


	vector<string> tmp0 = dbm->get_string_field("current_menu", "menuname");

	if(tmp0.size() != 1)
		cerr << "App Error: no menu results found";

	cout << "***************\n** " << tmp0[0] << " **\n***************\n\n";//print menu title

	stringstream ss1;
	ss1 << "current_menu_item <- select ( menuid == " << menuid << " ) menuitem;";//get men items
	parser.parse(ss1.str());


	vector<string> tmp1 = dbm->get_string_field("current_menu_item", "menuitemid");

	vector<string> tmp2 = dbm->get_string_field("current_menu_item", "menuitemname");

	vector<int> tmp3 = dbm->get_int_field("current_menu_item", "status");

	if(tmp1.size() == 0 || tmp2.size() == 0 || tmp3.size() == 0)
		cerr << "App Error: no menu item results found";

	for(int i = 0; i < tmp1.size(); i++)//output menu items
		if(tmp3[i] == 1)
			cout << tmp1[i] << ". " << tmp2[i] << endl;

	string s;
	while(true)//read in user selection loop
	{
		cout << endl << "Enter choice: ";
		cin >> s;

		std::transform(s.begin(), s.end(), s.begin(), ::toupper);//converts to upper case

		bool found = false;
		for(int i = 0; i < tmp1.size(); i++)
		{
			string temp = tmp1[i];
			std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);//converts to upper case

			if(temp == s)//if user selection is valid
			{
				if(tmp3[i] == 0)
					cout << "Sorry that option is disabled\n";
				else
				{
					found = true;
					break;
				}
			}
		}

		if(found)
			break;
		else
			cout << "Not a valid choice\n";
	}

	process_menu(s);//determines action, function call
}

void App::process_menu(string s)
{
	//startup
	if(menuid == 0)
	{
		if(s == "1")
			login_menu();
		else if(s == "2")
			new_account_menu();
		else if(s == "E")
		{
			menuid = 0;
			userid = -1;
			//close_db();
		}
	}
	//main menu
	else if (menuid == 1)
	{
		if(s == "1")//account
			account_menu(userid);
		else if (s == "2")//baords
		{
			menuid = 2;
			display_menu();
		}
		//MESSAGES
		else if(s == "3")
		{
			menuid =3;
			display_menu();

		}
		else if(s == "4")//groups
		{
			menuid=4;
			display_menu();
		}
		else if(s == "5")//users
		{
			menuid=5;
			display_menu();
		}
		else if(s == "6")//log
		{
			if(userid != 0)
			{
				cout << "Option only allowed for Admin" << endl;
				menuid = 1;
				display_menu();
			}else
				log_menu();
		}
		else if(s == "7")//menu menus
		{
			if(userid != 0)
			{
				cout << "Option only allowed for Admin" << endl;
				menuid = 1;
				display_menu();
			}else
				modify_menus_menu();
		}
		else if(s == "L")//logout
		{
			time_t rawtime;
			struct tm* timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			string time = asctime(timeinfo);

			stringstream ll;
			ll << "INSERT INTO log VALUES FROM (" << ++num_events << ", " << userid << ", \"" << time  << "\", \"Logout\" )";
			parser.parse(ll.str());
			parser.parse("SAVE log;");

			menuid = 0;
			userid = -1;
			display_menu();
		}
	}
	//board menu
	else if (menuid == 2)
	{
		if(s == "1")
			list_board_menu();
		else if (s == "2")
			goto_board_menu();
		else if(s == "3")//crate board
			if(userid != 0)
			{
				cout << "Option only allowed for Admin" << endl;
				menuid = 1;
				display_menu();
			}
			else
				create_board_menu();
		else if(s == "4")//del board
			if(userid != 0)
			{
				cout << "Option only allowed for Admin" << endl;
				menuid = 1;
				display_menu();
			}
			else
				delete_board_menu();
		else if(s == "M")
		{
			menuid = 1;
			display_menu();
		}
	}
	//messages menu
	else if(menuid == 3){
		if(s == "1"){//list messages
			list_message_menu();
		}
		else if (s == "2"){
			create_message_menu();
		}
		else if(s == "3"){ //main menu
			menuid=1;
			display_menu();
		}
	}
	// groups
	else if(menuid == 4){
		if(s == "1"){//request to join
			join_group_menu();
		}
		else if (s == "2"){//admin modify user access
			modify_groupmember_menu();
		}
		else if(s == "3"){//list groups / edit
			list_groups_menu();
		}
		else if(s == "4"){//create groups
			if(userid != 0)
			{
				cout << "Option only allowed for Admin" << endl;
				menuid = 1;
				display_menu();
			}
			else
				new_group_menu();
		}else if(s == "M")
		{
			menuid = 1;
			display_menu();
		}
	}
	// users
	else if(menuid == 5){
		if(s == "1"){ //list users and their articles
			list_user_menu();
		}
		else if (s == "2"){ // modify users
			if(userid != 0)
			{
				cout << "Option only allowed for Admin" << endl;
				menuid = 1;
				display_menu();
			}else
				modify_user_menu();
		}else if(s == "M")
		{
			menuid = 1;
			display_menu();
		}
	}
}

//login prompt
void App::login_menu()
{
	stringstream ss;
	string username, password;

	//get creds
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	cin >> password;

	ss << "current_account <- select ( user_name == \"" + username + "\" && password  == \"" + password + "\" ) accounts;";
	parser.parse(ss.str());

	//get db of creds for that user
	vector<int> tmp0 = dbm->get_int_field("current_account", "userid");
	vector<int> tmp1 = dbm->get_int_field("current_account", "status");

	//check status
	if(tmp0.size() != 1 && tmp0.size() != 1)
	{
		cout << "Sorry, Login Failed\n";
		login_menu();
	}
	else if(tmp1[0] == 0)//if user disabled
	{
		cout << "Sorry, Login Disabled\n";

		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		string time = asctime(timeinfo);

		//log event
		stringstream ll_0;
		ll_0 << "INSERT INTO log VALUES FROM (" << ++num_events << ", " << userid << ", \"" << time  << "\", \"Failed Login, Disabled\" )";
		parser.parse(ll_0.str());
		parser.parse("SAVE log;");

		login_menu();
	}
	else //if able to login
	{
		userid = tmp0[0];

		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		string time = asctime(timeinfo);

		//log event
		stringstream ll;
		ll << "INSERT INTO log VALUES FROM (" << ++num_events << ", " << userid << ", \"" << time  << "\", \"Login\" )";
		parser.parse(ll.str());
		parser.parse("SAVE log;");

		menuid = 1;
		display_menu();
	}
}

//create a new account
void App::new_account_menu()
{

	stringstream ss;
	string username;
    string password;

	cout << "Enter username: ";
	cin >> username;

	cout << "Enter password: ";
	cin >> password;

	//Check if user name already exists
    ss << "current_account <- select ( user_name == \"" + username + "\") accounts;";
    parser.parse(ss.str());
    
    vector<string> tmp  = dbm -> get_string_field("current_account","user_name");
    
    if (tmp.size() == 0){ //create account
			ss << "INSERT INTO accounts VALUES FROM (" << ++num_users << ", \"" + username + "\" , \"" + password + "\", \" \", \" \", \" \", \" \", 0);";

			parser.parse(ss.str());
			parser.parse("SAVE accounts;");

			cout <<"\n***************\nNew account with Username:-" << username << " created successfully\n***************\n\n";
			display_menu();
			}
	else{
			cout << "Username Already exists; Provide a different name \n";
			new_account_menu();
		}
}

//edit an existing acount
void App::account_menu(int uid)
{
	stringstream ss, dd;
	ss << "acct_info <- select (userID == " << uid << ") accounts;";
	parser.parse(ss.str());

	//git data from db
	vector<string> username = dbm->get_string_field("acct_info", "user_name");
	vector<string> password = dbm->get_string_field("acct_info", "password");
	vector<string> full_name = dbm->get_string_field("acct_info", "full_name");
	vector<string> phone = dbm->get_string_field("acct_info", "phone");
	vector<string> fax = dbm->get_string_field("acct_info", "fax");
	vector<string> address = dbm->get_string_field("acct_info", "address");
	string status = (dbm->get_int_field("acct_info", "status"))[0] ? "Enabled" : "Not Enabled";

	cout << "****************\n* Account Menu *\n****************" << endl;

	//prompt user
	cout << "1. Username: " << username[0] << endl;
	cout << "2. Password: " << password[0] << endl;
	cout << "3. Full Name: " << full_name[0] << endl;
	cout << "4. Phone Number: " << phone[0] << endl;
	cout << "5. Fax Number: " << fax[0] << endl;
	cout << "6. Address: " << address[0] << endl;
	cout << "7. Status:" << status << endl;
	cout << "(Enter the number of the field you want to change.)" << endl;
	cout << "D. Delete account" << endl;
	cout << "B. Return to Main Menu" << endl;
	cout << endl << "Enter your choice: ";

	string response;
	cin >> response;

	//get response
	if (response.length() == 1) {
		char c = response[0];
		string field;
		switch (c) {
			case '1': field = "username"; break;
			case '2': field = "password"; break;
			case '3': field = "full_name"; break;
			case '4': field = "phone"; break;
			case '5': field = "fax"; break;
			case '6': field = "address"; break;
			case '7': field = "status"; break;
			case 'D':

				dd << "DELETE FROM accounts WHERE userid == " << uid << " ;";
				parser.parse(dd.str());
				parser.parse("SAVE accounts;");
				if(uid == userid)
				{
					userid = -1;
					menuid = 0;
					return display_menu();
				}else
				{
					menuid = 1;
					return display_menu();
				}
			case 'B': menuid = 1; return display_menu();
			default: cerr << "Not a valid choice.";
		}
		getline(cin, response);
		cout << "Enter new " << field << ": ";
		string value;
		getline(cin, value);
		stringstream ss;
		if (c != '7') { //make updates
			ss << "UPDATE accounts SET " << field << " = \"" << value << "\" WHERE userID == " << uid << ';';
		} else {
			ss << "UPDATE accounts SET " << field << " = " << value << "WHERE userID == " << uid << ';';
		}
		parser.parse(ss.str());
		parser.parse("SAVE accounts;");
	} else {
		cerr << "Not a valid choice.";
	}
	return account_menu(uid);
}

//list boards available to a user
void App::list_board_menu()
{

	//get list of boards
	vector<int> tmp0 = dbm->get_int_field("boards", "boardid");
	vector<string> tmp1 = dbm->get_string_field("boards", "title");
	vector<int> tmp2 = dbm->get_int_field("boards", "status");

	//find which baords a user can access
	stringstream ss;
	ss << "current_access <- select ( gid == groupid ) ( ( rename ( gid, uid, state ) ( select ( userid == " << userid <<  " && status == 1 ) groupmember ) ) *  access ) ;";
	parser.parse(ss.str());

	vector<int> tmp3 = dbm->get_int_field("current_access", "boardid");
	int boardid;
	string s;
	bool found = false;
	int position = 0;
	while(true)
	{
		cout << "***************\n** Boards **\n***************\n\n";
		if(tmp0.size() == 0 || tmp1.size() == 0 || tmp2.size() == 0)
			cout << "No boards found";
		else{
			int temp1 = position+10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++){ //moveable list
				if(tmp2[i] != 0)
				{
					bool allowed = false;
					for(int bid : tmp3)
						if(bid == tmp0[i])
						{
							allowed = true;
							break;
						}

					stringstream xx;
					xx << "current_manager <- select ( groupName == \"" << tmp1[i] <<  "\") groups ;";
					parser.parse(xx.str());

					vector<int> tmp4 = dbm->get_int_field("current_manager", "groupmanager");


					if(allowed || userid == 0 || (tmp4.size() == 1 && tmp4[0] == userid))//print boards
						cout << tmp0[i] << ". \t\"" << tmp1[i] << " \"\n";
				}
			}

		}

		//prompt
		cout << endl;
		cout << "##. To view a board\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "B. Back to Board Menu\n";
		cout << "M. Back to Board Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;

		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "B" || s == "M")
			break;
		else
		{
			//if board is selected
			stringstream zz;
			zz << s;
			zz >> boardid;
			for(int id : tmp0)
				if(boardid == id)
				{
					found = true;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

	if(s == "B")
	{
		menuid = 2;
		display_menu();
	}else if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
		view_board_menu(boardid);
	}
}

//goto a board based on name
void App::goto_board_menu()
{
	vector<string> tmp0 = dbm->get_string_field("boards", "title");//get list
	vector<int> tmp1 = dbm->get_int_field("boards", "boardid");
	while(true)
	{
		cout << "***************\n** Goto Board **\n***************\n\n";//prompts
		cout << "Please type the first few characters: ";
		string s;
		cin >> s;

		int boardid = -1;
		int count = 0;
		for(int i = 0; i < tmp0.size(); i++)
		{
			if((tmp0[i]).substr(0,s.size()) == s)//finds match
			{
				count++;
				boardid = tmp1[i];
				break;
			}
		}

		if(boardid != -1 && count == 1)
			view_board_menu(boardid);//goes to that board
		else if(boardid != -1)
		{
			cout << "\nMultiple boards found, please try again\n";
			count = 0;
		}
		else
			cout << "\nNo board found, please try again: ";
	}

}

//view a boards articles
void App::view_board_menu(int boardid)
{
	stringstream ss;
	ss << "current_board <- select ( boardid == " << boardid << " ) boards;";
	parser.parse(ss.str());

	vector<string> tmp0 = dbm->get_string_field("current_board", "title");

	if(tmp0.size() != 1)
		cerr << "App Error: no board results found";

	//cout << "***************\n** " << tmp0[0] << " **\n***************\n\n";

	stringstream zz;
	zz << "current_article <- select ( boardid == " << boardid << " ) article;";
	parser.parse(zz.str());

	vector<int> tmp1 = dbm->get_int_field("current_article", "articleid");
	vector<int> tmp2 = dbm->get_int_field("current_article", "userid");
	vector<string> tmp3 = dbm->get_string_field("current_article", "time");
	vector<string> tmp4 = dbm->get_string_field("current_article", "title");


	vector<int> tmp5 = dbm->get_int_field("accounts", "userid");
	vector<string> tmp6 = dbm->get_string_field("accounts", "full_name");

	if(tmp5.size() == 0 || tmp6.size() == 0)
		cerr << "App Error: no article item results found";

	string s;
	int articleid;

	bool found = false;
	int position = 0;

	while(true)
	{
		cout << "***************\n** " << tmp0[0] << " **\n***************\n\n";

		if(tmp1.size() == 0 || tmp2.size() == 0 || tmp3.size() == 0)
			cout << "No articles found\n\n";
		else
		{
			int temp1 = position+10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++)
			{
				string fullname = to_string(tmp2[i]);
				for (int j = 0; j < tmp5.size(); j++)
					if(tmp5[j] == tmp2[i])
					{
						fullname = tmp6[j];
						break;
					}
				cout << tmp1[i] << ". [" << fullname << "]\t\t" << " \"" << tmp4[i] << "\t\"" << tmp3[i];
			}
		}
		cout << "##. To view an article\n";
		cout << "C. Create article\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "B. Back to Board Menu\n";
		cout << "M. Back to Main Menu\n";

		cin >> s;

		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){
			int temp1 = position+10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}

		else if(s == "B" || s == "M" || s == "C")
			break;
		else
		{
			stringstream xx;
			xx << s;
			xx >> articleid;

			for(int id : tmp1)
				if(articleid == id)
				{
					found = true;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

	if(s == "C")
	{
		create_article_menu(boardid);
	}else if(s == "B")
	{
		menuid = 2;
		display_menu();
	}else if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
		view_article_menu(articleid,boardid);
	}
}
void App::create_message_menu(){
	cout << "***************\n** New Message **\n***************\n\n";
	string to, from,tim, title, content;
	int id = num_messages;
	++num_messages;
	cout << "Enter Recipiant: ";
	getline(cin,to);
	getline(cin,to);

	stringstream zz;
	zz << "current_account <- select ( userid == " <<userid << " ) accounts;";
	parser.parse(zz.str());
	vector<string> tmp = dbm->get_string_field("current_account" , "full_name");
	from = tmp[0];

	cout << "Title: ";
	getline(cin,title);

	cout << "Insert content: ";
	getline(cin,content);

	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	tim = asctime(timeinfo);

	vector<string> names = dbm->get_string_field("accounts", "user_name");
	bool isFound = false;
	for(int i = 0; i < names.size(); ++i){
		if(names[i] == to){
			isFound = true;
		}
	}
	if(isFound){
		//cout << "found" << endl;
		stringstream xx;
		xx << "INSERT INTO message VALUES FROM (" << id << ", \"" << from << "\", \"" << to;
		xx << "\", \"" << tim << "\",\" " << title << "\", \" " << content << "\");";
		parser.parse(xx.str());
		parser.parse("SAVE message;");
		cout << "MESSAGE SENT" << endl;
		menuid = 3;
		display_menu();
	}
	else{
		cout << "That user name was not found" << endl;
		menuid = 3;
		display_menu();
	}


}
void App::list_message_menu()
{
	cout << "***************\n** Messages **\n***************\n\n";
	string my_name;
	stringstream zz;
	zz << "current_account <- select ( userid == " <<userid << " ) accounts;";
	parser.parse(zz.str());
	vector<string> tmp = dbm->get_string_field("current_account" , "user_name");
	stringstream ss;
	stringstream temp;
	temp << "\"" << tmp[0] << "\"";
	ss << "current_message <- select ( useridto == " << temp.str() << " || useridfrom == " << temp.str() << " ) message;";
	parser.parse(ss.str());
	vector<int> tmp0 = dbm->get_int_field("current_message", "messageid");
	vector<string> tmp1 = dbm->get_string_field("current_message", "useridfrom");
	vector<string> tmp2 = dbm->get_string_field("current_message", "useridto");
	vector<string> tmp3 = dbm->get_string_field("current_message", "time");
	vector<string> tmp4 = dbm->get_string_field("current_message", "title");
	vector<string> tmp7 = dbm->get_string_field("current_message", "content");
	vector<int> tmp5 = dbm->get_int_field("accounts", "userid");
	vector<string> tmp6 = dbm->get_string_field("accounts", "full_name");
	string s;
	bool found = false;
	int position = 0;

	if(tmp5.size() == 0 || tmp6.size() == 0)
		cerr << "App Error: no user item results found";

	if(tmp0.size() == 0 || tmp1.size() == 0 || tmp2.size() == 0 || tmp3.size() == 0){
		cout << "No messages found";
		menuid =1;
		display_menu();
	}
	else
	{


		while(true)
		{

			int temp1 = position+10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++)
			{
				string fullname = tmp6[i];
				for (int j = 0; j < tmp5.size(); j++)
					if(tmp6[j] == tmp1[i])
					{
						fullname = tmp6[j];
						break;
					}
				cout << tmp0[i] << ". \nFrom: [" << fullname << "]\nTo: " << tmp2[i] << "\nTime:" << tmp3[i] << "\nTitle: "
				<< tmp4[i] << "\nContent: " << tmp7[i] << "\n\n"<<endl;;
			}

			//cout << "##. To view an article\n";
			cout << "S. Send new message\n";
			cout << "E. Erase messages\n";
			cout << "U. Page up\n";
			cout << "D. Page down\n";
			cout << "L. Message Menu\n";
			cout << "M. Main Menu\n";
			cout << "Enter Choice: ";
			cin >> s;

			if(s == "U")
				position = max(position-10,0);
			else if(s == "D"){
				int temp1 = position+10;
				int temp2 = tmp1.size();
				position = min(temp1,temp2);
			}
			else if(s == "S" || s == "M" || s == "L" || s == "E")
				break;
			/*else
			{
				int s_num;
				stringstream zz;
				zz << s;
				zz >> s_num;
				for(int id : tmp0)
					if(s_num == id)
					{
						found = true;
						break;
					}
				if(!found)
					cout << "Not a valid choice\n";
			}
			*/
			//else if(found)
			//	break;
		}
	}

	if(s == "S")
	{
		create_message_menu();
	}
	else if(s == "L"){
		menuid = 3;
		display_menu();
	}
	else if(s == "E"){
		cout << "Enter Message Number to be deleted: ";
		int num;
		cin >> num;
		stringstream blah;
		blah << "DELETE FROM message WHERE messageid == " << num << ";";
		parser.parse (blah.str());
		parser.parse("SAVE message;");
		--num_messages;
		list_message_menu();
	}
	else if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
		ss << s;
		int articleid;
		ss >> articleid;
		view_message_menu(articleid);
	}
}

void App::view_message_menu(int useridto)
{
	stringstream ss;
	ss << "current_message <- select ( userid == " << useridto << " ) message;";
	parser.parse(ss.str());

	vector<int> tmp0 = dbm->get_int_field("current_message", "messageid");
	vector<int> tmp1 = dbm->get_int_field("current_message", "useridfrom");
	vector<string> tmp2 = dbm->get_string_field("current_message", "time");
	vector<string> tmp3 = dbm->get_string_field("current_message", "title");
	vector<string> tmp4 = dbm->get_string_field("current_message", "content");

	vector<int> tmp5 = dbm->get_int_field("accounts", "userid");
	vector<string> tmp6 = dbm->get_string_field("accounts", "full_name");

	if(tmp5.size() == 0 || tmp6.size() == 0)
		cerr << "App Error: no user item results found";

	string fromname = tmp6[0];
	for (int j = 0; j < tmp5.size(); j++)
		if(tmp5[j] == tmp1[j])
			{
				fromname = tmp6[j];
				break;
			}

	string s;

	while(true)
	{
		cout << "***************\n" << tmp0[0] << ". " << tmp3[0] << "\n By: ";
		cout << fromname << "On: \n" << tmp2[0] << "\\n***************\n\n";

		cout << tmp4[0] << "\n\n";

		cout << "B. Send a new message\n";
		cout << "B. Back to Message Menu\n";
		cout << "M. Main Menu\n";

		cin >> s;

		if(s == "S" || s == "B" || s == "M")
			break;
		else
			cout << "Not a valid choice\n";
	}
	if(s == "S")
	{
		//send_message_menu();
	}
	else if(s == "B")
	{
		list_message_menu();
	}else if(s == "M")
	{
		menuid = 1;
		display_menu();
	}
}

void App::view_article_menu(int articleid, int boardid)
{

	stringstream ss;
	ss << "current_article <- select ( articleid == " << articleid << " ) article;";
	parser.parse(ss.str());

	vector<int> tmp0 = dbm->get_int_field("current_article", "articleid");
	vector<int> tmp1 = dbm->get_int_field("current_article", "userid");
	vector<string> tmp2 = dbm->get_string_field("current_article", "time");
	vector<string> tmp3 = dbm->get_string_field("current_article", "title");
	vector<string> tmp4 = dbm->get_string_field("current_article", "content");
	vector<int> tmp5 = dbm->get_int_field("current_article", "length");
	

	
	if(tmp0.size() != 1)
		cerr << "App Error: no article results found";

	vector<int> tmp6 = dbm->get_int_field("accounts", "userid");
	vector<string> tmp7 = dbm->get_string_field("accounts", "full_name");

	if(tmp6.size() == 0 || tmp7.size() == 0)
		cerr << "App Error: no user results found";

	string byname = ""+tmp1[0];
	for (int j = 0; j < tmp6.size(); j++)
		if(tmp6[j] == tmp1[0])
			{
				byname = tmp7[j];
				break;
			}

	string s;
	
	while(true)
	{
		cout << "***************\n" << tmp0[0] << ". " << tmp3[0] << "\nBy: ";
		cout << byname << "\nOn: " << tmp2[0] << "Length: " << tmp5[0];
		cout <<"\n***************\n\n";

		cout << tmp4[0] << "\n\n";

		cout << "E. To edit article\n";
		cout << "D. To delete article\n";
		if(boardid != -1)
			cout << "C. Back to current Board\n";
		cout << "B. Back to Board menu\n";
		cout << "M. Back to Main menu\n";

		cin >> s;

		if(s == "E" || s == "B" || s == "M" || (s == "C" && boardid != -1) || s == "D")
			break;
		else
			cout << "Not a valid choice\n";
	}

	if(s == "C")
	{
		view_board_menu(boardid);
	}else if( s == "D")
	{
		delete_article_menu(articleid,boardid);
	}else if(s == "B")
	{
		menuid = 2;
		display_menu();
	}else if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(s == "E")
	{
		edit_article_menu(articleid,boardid);
	}
}

void App::create_article_menu(int boardid){

	while(true)
	{
		cout << "***************\n** Create Article **\n***************\n\n";
		cout << "\nArticle Title\\Description: ";
		string title;
		getline(cin,title);
		getline(cin,title);
		cout << "\nContent: ";
		string content;
		getline(cin,content);
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		string time = asctime(timeinfo);

		if(title.size() > 32)
			cout << "\nTitle\\Description to long, max is 32 characters\n";
		else if(content.size() > 65536)
			cout << "\nContent to long, max is 65536 characters\n";
		else
		{
			stringstream ss0;
			ss0 << "INSERT INTO article VALUES FROM (" << ++num_articles << ", " << userid << ", \"" << time << "\", \""  << title << "\", \"" << content << "\", " << boardid <<", " << content.length() << " );";
			parser.parse(ss0.str());
			parser.parse("SAVE article;");
			break;
		}
	}

	view_board_menu(boardid);
}

void App::edit_article_menu(int articleid, int boardid){

	stringstream ss;
	ss << "current_article <- select ( articleid == " << articleid << " ) article;";
	parser.parse(ss.str());

	vector<string> tmp0 = dbm->get_string_field("current_article", "title");
	vector<string> tmp1 = dbm->get_string_field("current_article", "content");
	vector<int> tmp2 = dbm->get_int_field("current_article", "userid");
	string title = tmp0[0];
	string content = tmp1[0];
	int owner = tmp2[0];
	char c;
	if(userid != owner && userid != 0)
		cout << "Sorry, only the owner can edit the article\n";
	else
	{
		while(true)
		{
			cout << "***************\n** Create Article **\n***************\n\n";

			cout << "\nDo you wish to modify the title (Y/N): ";
			cin >> c;
			if(c == 'Y')
			{
				cout << "\nArticle Title\\Description: ";
				getline(cin,title);
				getline(cin,title);
			}

			cout << "\nDo you wish to modify the content (Y/N): ";
			cin >> c;
			if(c == 'Y')
			{
				cout << "\nContent: ";
				getline(cin,content);
				getline(cin,content);
			}

			time_t rawtime;
			struct tm* timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			string time = asctime(timeinfo);

			if(title.size() > 32)
				cout << "\nTitle\\Description to long, max is 32 characters\n";
			else if(content.size() > 65536)
				cout << "\nContent to long, max is 65536 characters\n";
			else
			{
				stringstream ss0;
				ss0 << "UPDATE article SET userid = " << userid << ", time = \"" << time << "\", title = \""  << title << "\", content = \"" << content << "\", length = " << content.length() << " WHERE articleid == " << articleid << " );";
				parser.parse(ss0.str());
				parser.parse("SAVE article;");
				break;
			}
		}
	}
	view_board_menu(boardid);
}

void App::delete_article_menu(int articleid, int boardid){

	stringstream ss;
	ss << "current_article <- select ( articleid == " << articleid << " ) article;";
	parser.parse(ss.str());

	vector<int> tmp0 = dbm->get_int_field("current_article", "userid");
	int owner = tmp0[0];
	if(userid != owner && userid != 0)
		cout << "Sorry, only the owner can delete the article\n";
	else
	{
		stringstream ss0;
		ss0 << "DELETE FROM article WHERE articleid == " << articleid << " ;";
		parser.parse(ss0.str());
		parser.parse("SAVE article;");
	}
	view_board_menu(boardid);
}

void App::send_message_menu(string sender)
{
	cout << "***************\n** New Message **\n***************\n\n";

	string to, from,tim, title, content;
	to = sender;
	int id = num_messages;
	++num_messages;

	stringstream zz;
	zz << "current_account <- select ( userid == " <<userid << " ) accounts;";
	parser.parse(zz.str());
	vector<string> tmp = dbm->get_string_field("current_account" , "user_name");
	from = tmp[0];

	cout << "Title: ";
	getline(cin,title);
	getline(cin,title);
	cout << "Insert content: ";
	getline(cin,content);

	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	tim = asctime(timeinfo);

	cout << title << endl;
	cout << content << endl;
	cout << "from: " << from << endl;
	cout << "to: " << to << endl;
	cout << "id: " << id << endl;
	cout << "time: " << tim << endl;

	vector<string> names = dbm->get_string_field("accounts", "user_name");
	bool isFound = false;
	for(int i = 0; i < names.size(); ++i){
		if(names[i] == to){
			isFound = true;
		}
	}
	if(isFound){
		//cout << "found" << endl;
		stringstream xx;
		xx << "INSERT INTO message VALUES FROM (" << id << ", \"" << from << "\", \"" << to;
		xx << "\", \"" << tim << "\",\" " << title << "\", \" " << content << "\");";
		parser.parse(xx.str());
		parser.parse("SAVE message;");
		cout << "MESSAGE SENT" << endl;
		menuid = 3;
		display_menu();
	}
	else{
		cout << "That user name was not found" << endl;
		menuid = 3;
		display_menu();
	}

}

void App::create_board_menu()
{
	vector<string> tmp0 = dbm->get_string_field("boards", "title");
	while(true)
	{
		cout << "***************\n** Create Board **\n***************\n\n";
		cout << "Board Title\\Description: ";
		string title;
		cin >> title;

		if(title.size() > 32)
			cout << "\nTitle\\Description to long, max is 32 characters\n";
		else
		{
			bool found = false;
			for(string s : tmp0)
				if(title == s)
				{
					found = true;
					break;
				}

			if(found)
				cout << "\nTitle\\Description already exists, please pick another\n";
			else
			{
				stringstream ss0;
				ss0 << "INSERT INTO boards VALUES FROM (" << ++num_boards << ", \"" << title << "\", 1);";
				parser.parse(ss0.str());
				parser.parse("SAVE boards;");
				stringstream ss1;
				ss1 << "INSERT INTO groups VALUES FROM ("<< ++num_groups <<", \"" << title << "\", 0, 1;";
				parser.parse(ss1.str());
				parser.parse("SAVE groups;");
				stringstream ss2;
				ss2 << "INSERT INTO groupmember VALUES FROM ("<< num_groups <<", 0, 1);";
				parser.parse(ss2.str());
				parser.parse("SAVE groupmember;");
				stringstream ss3;
				ss3 << "INSERT INTO access VALUES FROM (" << num_boards << ", " << num_groups << ");";
				parser.parse(ss3.str());
				parser.parse("SAVE access;");
				break;
			}
		}
	}

	menuid = 2;
	display_menu();
}

void App::delete_board_menu()
{
	vector<int> tmp0 = dbm->get_int_field("boards", "boardid");
	vector<string> tmp1 = dbm->get_string_field("boards", "title");

	int delboardid;
	string s;
	bool found = false;
	int position = 0;
	while(true)
	{
		cout << "***************\n** Delete Board **\n***************\n\n";
		if(tmp0.size() == 0 || tmp1.size() == 0)
			cout << "No boards found";
		else
		{
			int temp1 = position + 10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1, temp2); i++)
				cout << tmp0[i] << ". \t\"" << tmp1[i] << "\"\n";
		}

		cout << endl;
		cout << "##. To delete a board\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "B. Back to Board Menu\n";
		cout << "M. Back to Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;

		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "B" || s == "M")
			break;
		else
		{

			stringstream zz;
			zz << s;
			zz >> delboardid;
			for(int id : tmp0)
				if(delboardid == id)
				{
					found = true;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

	if(s == "B")
	{
		menuid = 2;
		display_menu();
	}
	else if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
		stringstream ss;
		ss << "DELETE FROM boards WHERE boardid == " << delboardid << " ;";
		parser.parse(ss.str());
		parser.parse("SAVE boards;");
		menuid = 2;
		display_menu();
	}

}

//function to modify user menu
void App::modify_user_menu()
{
    //vectors to store userID, username and fullname
	vector<int> tmp0 = dbm->get_int_field("accounts", "userid");
	vector<string> tmp1 = dbm->get_string_field("accounts", "user_name");
	vector<string> tmp2 = dbm->get_string_field("accounts", "full_name");

	int uid;
	string s;
	bool found = false;
	int position = 0;
	while(true)
	{
		cout << "***************\n* Modify Users *\n***************\n\n";
		if(tmp0.size() == 0 || tmp1.size() == 0 || tmp2.size() == 0)
			cout << "No users found";
		else{
			int temp1 = position+10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++){
				cout << tmp0[i] << ". \t[" << tmp1[i] << "]\t" << tmp2[i] << endl;
			}

		}
        //Display the available options for the current menu
		cout << endl;
		cout << "##. To modify a user\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "M. Back to Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;
        //execute the functions based on the input with simple if-statments
		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "M")
			break;
		else
		{
            //user menu selected
			stringstream zz;
			zz << s;
			zz >> uid;
			for(int j = 0; j < tmp0.size(); j++)
				if(uid == tmp0[j])
				{
					found = true;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

    if(s == "M") //loopback to the display_menu option
	{
		menuid = 1;
        display_menu();
	}else if(found)
	{
        account_menu(uid); // navigate to account menu with the given UID
	}
}

//function to create the log menu items
void App::log_menu()
{
    //necessary vectors of eventID, userID, time and message from log relation
	vector<int> tmp0 = dbm->get_int_field("log", "eventid");
	vector<int> tmp1 = dbm->get_int_field("log", "userid");
	vector<string> tmp2 = dbm->get_string_field("log", "time");
	vector<string> tmp3 = dbm->get_string_field("log", "message");
    
    //vectors of userID and username from accounts relation
	vector<int> tmp4 = dbm->get_int_field("accounts", "userid");
	vector<string> tmp5 = dbm->get_string_field("accounts", "user_name");
	int dellogid;
	string s;
	bool found = false;
	int position = 0;
	while(true)
	{
		cout << "***************\n** Log **\n***************\n\n";

        if(tmp0.size() == 0 || tmp1.size() == 0) //No proper eventID or userID
			cout << "No log entries found";
		else
		{
			int temp1 = position + 10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++)
			{
				string username = to_string(tmp1[i]);
				for (int j = 0; j < tmp4.size(); j++)
					if(tmp4[j] == tmp1[i])
					{
						username = tmp5[j];
						break;
					}
				cout << tmp0[i] << ". \t[" << username << "]\t\"" << tmp3[i] << "\" " << tmp2[i] << "\n";

			}
		}
        
        //prompt display for this menu option
		cout << endl;
		cout << "##. To delete from log\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "M. Back to Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;

        //all other navigation controls
		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "M")
			break;
		else
		{
            //proceed to delete option for log
			stringstream zz;
			zz << s;
			zz >> dellogid;
			for(int id : tmp0)
				if(dellogid == id)
				{
					found = true;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

	if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
        //if true create the proper SQL command and execute it
		stringstream ss;
		ss << "DELETE FROM log WHERE eventid == " << dellogid << " ;";
		parser.parse(ss.str());
        parser.parse("SAVE log;"); //save the log relation and navigate back to log_menu
		log_menu();
	}
}

//function to modify menus
void App::modify_menus_menu()
{

    //store menuIDs and menuname in proper vectors
	vector<int> tmp4 = dbm->get_int_field("menus", "menuid");
	vector<string> tmp5 = dbm->get_string_field("menus", "menuname");

	string s;
	string s1;
	int s2, s3;
	bool found = false;
	int position = 0;
	while(true)
	{
        //get proper attribute values from menuitem relation to store in a vector
		vector<string> tmp0 = dbm->get_string_field("menuitem", "menuitemid");
		vector<int> tmp1 = dbm->get_int_field("menuitem", "menuid");
		vector<string> tmp2 = dbm->get_string_field("menuitem", "menuitemname");
		vector<int> tmp3 = dbm->get_int_field("menuitem", "status");

		cout << "***************\n** Modify Menu **\n***************\n\n";

        if(tmp0.size() == 0 || tmp1.size() == 0) //check for empty relations
			cout << "No log entries found";
		else
		{
            //for each menuID and menuname, compare it with menus relation and check the staus to enable or disable
			int temp1 = position + 10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++)
			{
				string menuname = to_string(tmp1[i]);
				for (int j = 0; j < tmp4.size(); j++)
					if(tmp4[j] == tmp1[i])
					{
						menuname = tmp5[j];
						break;
					}
				cout << menuname << "\t[" << tmp1[i] << tmp0[i] << "]\t\"" << tmp2[i] << "\" \t\t";
				if(tmp3[i] == 0)
					cout << "Disabled\n";
				else
					cout << "Enabled\n";

			}
		}

        //prompt options for this menu
		cout << endl;
		cout << "##. To enable\\disable menuitem\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "M. Back to Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;

        //all other navigation option menus
		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "M")
			break;
		else
		{
            //enabling / disabling the menuitem
			for(int j = 0; j < tmp0.size(); j++)
			{
				stringstream comp;
				comp << tmp1[j] << tmp0[j];
				cout << s << "|" << comp.str() << endl;
				if(s == comp.str())
				{
					found = true;
					s1 = tmp0[j];
					s2 = tmp1[j];
					if(tmp3[j] == 0)
						s3 = 1;
					else
						s3 = 0;
					break;
				}
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

	if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
        //if found set true, create proper SQL command with proper status assigned previously and save the menuitem relation
		stringstream ss;
		ss << "UPDATE menuitem SET status = " << s3 << "  WHERE ( menuitemid == \"" << s1 << "\" && menuid == " << s2 << " ) ;";
		parser.parse(ss.str());
		parser.parse("SAVE menuitem;");
		modify_menus_menu();
	}

}

//function to join group
void App::join_group_menu()
{
    //load current groups with projecting groupIDs and make a relation current_missingmember with proper relational algebra
	parser.parse("current_groups <- project (groupid) groups;");
	stringstream xx;
	xx <<"current_groupmember <- project (groupid) (select ( userid == " << userid << " ) groupmember );";
	parser.parse(xx.str());
	parser.parse("current_missingmember <- current_groups - current_groupmember;");
	vector<int> tmp5 = dbm->get_int_field("current_missingmember", "groupid");

    //if there are members not assigned to the group, insert into groupmember with proper ID and userID, and save it
	if(tmp5.size() != 0)
		for( int id : tmp5)
		{
			stringstream ss2;
			ss2 << "INSERT INTO groupmember VALUES FROM ( " << id << " , " << userid << ", 0);";
			parser.parse(ss2.str());
			parser.parse("SAVE groupmember;");
		}

    //load all the current members with given gid using the cross product of two relations
	stringstream ss;
	ss << "current_member <- select ( gid == groupid ) ( ( rename ( gid, uid, member ) ( select ( userid == " << userid <<  " ) groupmember ) ) *  groups ) ;";
	parser.parse(ss.str());

    //store groupid, member and groupname into vectors from relation created above.
	vector<int> tmp0 = dbm->get_int_field("current_member", "groupid");
	vector<int> tmp1 = dbm->get_int_field("current_member", "member");
	vector<string> tmp2 = dbm->get_string_field("current_member", "groupname");


	int gid;
	string s;
	bool found = false;
	int position = 0;
	while(true)
	{
		cout << "***************\n** Join Group **\n***************\n\n";
        //check if group exists with given gid or groupname
		if(tmp0.size() == 0 || tmp1.size() == 0)
			cout << "No groups found";
		else
		{
            //check the status of each group member in the current_member relation
			int temp1 = position + 10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++)
			{
				cout << tmp0[i] << ". \t[" << tmp2[i] << "]\t ";
				if(tmp1[i] == 0)
					cout << "Pending\n";
				else
					cout << "Member\n";

			}
		}

        //prompt menu options
		cout << endl;
		cout << "##. To join\\leave group\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "G. Back to Group Menu\n";
		cout << "M. Back to Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;

        //navigation menu options for this menu
		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "M" || s == "G")
			break;
		else
		{
            //user selects to join/leave group
			stringstream zz;
			zz << s;
			zz >> gid;
            //set found to true and break after traversing through each groupID
			for(int j = 0; j < tmp0.size(); j++)
				if(gid == tmp0[j])
				{
					found = true;
					tmp1[j] = 0;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}
	if(s == "G")
	{
		menuid = 4;
		display_menu();
	}else if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
        //create proper SQL command with assigned gid and userID and execute and save the groupmember relation
		stringstream ss;
		ss << "UPDATE groupmember SET status = 0  WHERE ( groupid == "  << gid << " && userid == " << userid << " ) ;";
		parser.parse(ss.str());
		parser.parse("SAVE groupmember;");
		join_group_menu();
	}

}

//fmenu to modify groupmember
void App::modify_groupmember_menu()
{
    //store proper groupID, groupname, status and groupmanager into poper vectors from groups relation
	vector<int> tmp0 = dbm->get_int_field("groups", "groupid");
	vector<string> tmp1 = dbm->get_string_field("groups", "groupname");
	vector<int> tmp2 = dbm->get_int_field("groups", "status");
	vector<int> tmp3 = dbm->get_int_field("groups", "groupmanager");

    //set proper access
	bool notamanager = true;
	for(int id : tmp3)
		if(id == userid)
			notamanager = false;

	int gid;
	string s;
	bool found = false;
	int position = 0;
	while(true)
	{
		cout << "***************\n** Select Group to Modify Members **\n***************\n\n";
        //check for empty groups
		if(tmp0.size() == 0 || tmp1.size() == 0 || tmp2.size() == 0 || (userid != 0 && notamanager))
			cout << "No groups found";
		else{
            //check the status of the groups
			int temp1 = position+10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++){
				if(tmp3[i] == userid || userid == 0)
				{
					cout << tmp0[i] << ". \t[" << tmp1[i] << "]\t";
					if(tmp2[i] == 0)
						cout <<"Disabled\n";
					else
						cout <<"Enabled\n";
				}
			}

		}

        //prompt menu options
		cout << endl;
		cout << "##. To modify a groups members\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "G. Back to Group Menu\n";
		cout << "M. Back to Board Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;
        //other navigation options
		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "M" || s == "G")
			break;
		else
		{
            //user selects to modify group member
			stringstream zz;
			zz << s;
			zz >> gid;
			for(int j = 0; j < tmp0.size(); j++)
				if(gid == tmp0[j] && (tmp3[j] == userid || userid == 0))
				{
					found = true;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}
	if(s == "G")
	{
		menuid = 4;
		display_menu();
	}else if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
        //if true call other menu function with proper gid
		modify_groupmember_menu(gid);
	}
}

//function to modify groupemember with groupID
void App::modify_groupmember_menu(int groupid)
{
    //create proper SQL command with proper gid and parse it
	stringstream ss;
	ss << "current_member <- select ( gid == groupid ) ( ( rename ( gid, uid, member ) ( groupmember ) ) *  ( select (groupid == " << groupid << " ) groups ) ) ;";
	parser.parse(ss.str());

    //load proper attribute values in different vectors from current_member relation
	vector<int> tmp0 = dbm->get_int_field("current_member", "uid");
	vector<int> tmp1 = dbm->get_int_field("current_member", "member");
	vector<string> tmp2 = dbm->get_string_field("current_member", "groupname");

    //store proper attribute values from accounts relation
	vector<int> tmp5 = dbm->get_int_field("accounts", "userid");
	vector<string> tmp6 = dbm->get_string_field("accounts", "full_name");

	int uid, ns;
	string s;
	bool found = false;
	int position = 0;
	while(true)
	{
		cout << "***************\n** Modify Group Members **\n***************\n\n";

        //check for empty relations
		if(tmp0.size() == 0 || tmp1.size() == 0)
			cout << "No groups found";
		else
		{
			int temp1 = position + 10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++)
			{
				string fullname = to_string(tmp0[i]);
				for (int j = 0; j < tmp5.size(); j++)
					if(tmp5[j] == tmp1[i])
					{
						fullname = tmp6[j];
						break;
					}
				cout << tmp0[i] << ". \t[" << fullname << "]\t ";
				if(tmp1[i] == 0)
					cout << "Pending\\Disabled\n";
				else
					cout << "Member\n";

			}
		}

        //prompt menu options
		cout << endl;
		cout << "##. To modify member status\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "G. Back to Group Menu\n";
		cout << "M. Back to Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;

        //navigation menu options for this menu
		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "M" || s == "G")
			break;
		else
		{
            //selects to modify status; set found to true
			stringstream zz;
			zz << s;
			zz >> uid;
			for(int j = 0; j < tmp0.size(); j++)
				if(uid == tmp0[j])
				{
					found = true;
					if(tmp1[j] == 0)
						ns = 1;
					else
						ns = 0;
					tmp1[j] = ns;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

	if(s == "G")
	{
		menuid = 4;
		display_menu();
	}else if(s == "M")
	{
		menuid = 1;
		display_menu();
    }else if(found) //when found is true from above comparisions, create proper SQL command as below, execute and save relation groupmember
	{
		stringstream ss;
		ss << "UPDATE groupmember SET status = " << ns << "  WHERE ( groupid == " << groupid << " && userid == " << uid << " ) ;";
		parser.parse(ss.str());
		parser.parse("SAVE groupmember;");
		modify_groupmember_menu(groupid);
	}

}

//menu to list groups
void App::list_groups_menu()
{
    //store proper attribute values in the vectors from relations groups
	vector<int> tmp0 = dbm->get_int_field("groups", "groupid");
	vector<string> tmp1 = dbm->get_string_field("groups", "groupname");
	vector<int> tmp2 = dbm->get_int_field("groups", "status");
	vector<int> tmp3 = dbm->get_int_field("groups", "groupmanager");

    //set the status
	bool notamanager = true;
	for(int id : tmp3)
		if(id == userid)
			notamanager = false;

	int gid;
	string s;
	bool found = false;
	int position = 0;
	while(true)
	{
		cout << "***************\n** Groups **\n***************\n\n";
        //check for an empty relation and proper access
		if(tmp0.size() == 0 || tmp1.size() == 0 || tmp2.size() == 0 || (userid != 0 && notamanager))
			cout << "No groups found";
		else{
			int temp1 = position+10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++){
				if(tmp3[i] == userid || userid == 0)
				{
					cout << tmp0[i] << ". \t[" << tmp1[i] << "]\t";
					if(tmp2[i] == 0)
						cout <<"Disabled\n";
					else
						cout <<"Enabled\n";
				}
			}

		}

        //prompt menu options
		cout << endl;
		cout << "##. To edit a group\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "G. Back to Group Menu\n";
		cout << "M. Back to Board Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;
        
        //navigation options
		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "M" || s == "G")
			break;
		else
		{
            //set the status of found as user selects to edit a group
			stringstream zz;
			zz << s;
			zz >> gid;
			for(int j = 0; j < tmp0.size(); j++)
				if(gid == tmp0[j] && (tmp3[j] == userid || userid == 0))
				{
					found = true;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

	if(s == "G")
	{
		menuid = 4;
		display_menu();
	}else if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
        //if found is set true, call edit group menu with proper gid passed
		edit_group_menu(gid);
	}
}

//function to edit group menu with gid
void App::edit_group_menu(int groupid)
{
    //create proper SQL command and parse it
	stringstream ss;
	ss << "current_group <- select ( groupid == " << groupid << " ) groups;";
	parser.parse(ss.str());

    //store poper attribute values in the given vectors
	vector<string> tmp0 = dbm->get_string_field("current_group", "groupname");
	vector<int> tmp1 = dbm->get_int_field("current_group", "status");
	vector<int> tmp2 = dbm->get_int_field("current_group", "groupmanager");
	string name = tmp0[0];
	int status = tmp1[0];
	int owner = tmp2[0];
	char c;
    if(userid != owner && userid != 0) //check the access
		cout << "Sorry, only the manager can edit the article\n";
	else
	{
		while(true)
		{
            //other group functions
			cout << "***************\n** Edit Group **\n***************\n\n";

			cout << "\nDo you wish to delete the group (Y/N): ";
			cin >> c;
			if(c == 'Y')
			{
				stringstream dd;
				dd << "DELETE FROM groups WHERE groupid == " << groupid << " ;";
				parser.parse(dd.str());
				parser.parse("SAVE groups;");
				break;
			}
			else
			{

				if(status == 0)
				{
					cout << "\nDo you wish to enable the group (Y/N): ";
					cin >> c;
					if(c == 'Y')
					{
						status = 1;
					}
				}
				else
				{
					cout << "\nDo you wish to disable the group (Y/N): ";
					cin >> c;
					if(c == 'Y')
					{
						status = 0;
					}
				}

				cout << "\nDo you wish to modify the group name (Y/N): ";
				cin >> c;
				if(c == 'Y')
				{
					cout << "\nGroup Name: ";
					getline(cin,name);
					getline(cin,name);
				}

				bool error = false;
				cout << "\nDo you wish to modify the group manager (Y/N): ";
				cin >> c;
				if(c == 'Y')
				{
					string manager;
					cout << "\nGroup Manager Username: ";
					getline(cin,manager);
					getline(cin,manager);

					stringstream ss;
					ss << "current_account <- select ( user_name == \"" << manager << "\" ) accounts;";
					parser.parse(ss.str());
					vector<int> tmp5 = dbm->get_int_field("current_account", "userid");

					if(tmp5.size() != 1)
					{
						cout << "\nUsername not found\n";
						error = true;
					}
					else
						owner = tmp5[0];
				}

				if(error)
				{

				}else if(name.size() > 32)
					cout << "\nGroup Name to long, max is 32 characters\n";
				else
				{
					stringstream ss0;
					ss0 << "UPDATE groups SET groupname = \"" << name << "\", status = " << status << ", groupmanager = " << owner <<  " WHERE groupid == " << groupid << " );";
					parser.parse(ss0.str());
					parser.parse("SAVE groups;");
					break;
				}
			}
		}
	}
	list_groups_menu();


}

//menu option for new group
void App::new_group_menu(){

	while(true)
	{
        //get Group attribute name from user and create a group
		cout << "***************\n** Create Group **\n***************\n\n";
		cout << "\nGroup Name: ";
		string name;
		getline(cin,name);
		getline(cin,name);

		string manager;
		cout << "\nGroup Manager Username: ";
		getline(cin,manager);
		getline(cin,manager);

		stringstream ss;
		ss << "current_account <- select ( user_name == " << manager << " ) accounts;";
		parser.parse(ss.str());
		vector<int> tmp5 = dbm->get_int_field("current_account", "userid");

        //check for the empty relation
		if(tmp5.size() != 1)
		{
			cout << "\nUsername not found\n";
		}else if(name.size() > 32)
			cout << "\nGroup name to long, max is 32 characters\n";
		else
		{
			stringstream ss0;
			ss0 << "INSERT INTO groups VALUES FROM (" << ++num_groups << ", \"" << name << "\", " << tmp5[0] << ", 1);";
			parser.parse(ss0.str());
			parser.parse("SAVE groups;");
			break;
		}
	}

	menuid = 4;
	display_menu();
}

//menu to list users
void App::list_user_menu()
{

    //store proper attributes from accounts relation
	vector<int> tmp0 = dbm->get_int_field("accounts", "userid");
	vector<string> tmp1 = dbm->get_string_field("accounts", "user_name");
	vector<string> tmp2 = dbm->get_string_field("accounts", "full_name");

	int uid;
	string s;
	bool found = false;
	int position = 0;
	while(true)
	{
		cout << "***************\n** Users **\n***************\n\n";
        //check for empty relations
		if(tmp0.size() == 0 || tmp1.size() == 0 || tmp2.size() == 0)
			cout << "No users found";
		else{
			int temp1 = position+10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++){
				cout << tmp0[i] << ". \t[" << tmp1[i] << "]\t" << tmp2[i] << endl;
			}

		}

        //menu prompt options
		cout << endl;
		cout << "##. To view articles by a user\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "M. Back to Board Main Menu\n";
		cout << "Enter choice: ";

		cin >> s;

        //navigation options
		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){

			int temp1 = position + 10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}
		else if(s == "M")
			break;
		else
		{
            //set founr to true if user selects to view article
			stringstream zz;
			zz << s;
			zz >> uid;
			for(int j = 0; j < tmp0.size(); j++)
				if(uid == tmp0[j])
				{
					found = true;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

	if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
        //if found set true, call another proper function with uid passed
		view_user_article_menu(uid);
	}
}

//menu to view article for specific user
void App::view_user_article_menu(int uid)
{
    //create a relation with given uid
	stringstream ss;
	ss << "current_account <- select ( userid == " << uid << " ) accounts;";
	parser.parse(ss.str());

	vector<string> tmp0 = dbm->get_string_field("current_account", "full_name");
    //check for the boards
	if(tmp0.size() != 1)
		cerr << "App Error: no board results found";

	//cout << "***************\n** " << tmp0[0] << " **\n***************\n\n";

    //load articles for the given userid
	stringstream zz;
	zz << "current_article <- select ( userid == " << uid << " ) article;";
	parser.parse(zz.str());

    //store proper article attribute values from current_article relations
    vector<int> tmp1 = dbm->get_int_field("current_article", "articleid");
	vector<string> tmp3 = dbm->get_string_field("current_article", "time");
	vector<string> tmp4 = dbm->get_string_field("current_article", "title");



	string s;
	int articleid;

	bool found = false;
	int position = 0;

	while(true)
	{
		cout << "***************\n** " << tmp0[0] << " **\n***************\n\n";

        //check for empty relations
		if(tmp1.size() == 0 || tmp4.size() == 0 || tmp3.size() == 0)
			cout << "No articles found\n\n";
		else
		{
			int temp1 = position+10;
			int temp2 = tmp1.size();
			for(int i = position; i < min(temp1,temp2); i++)
			{
				cout << tmp1[i] << ". \t\t\"" << tmp4[i] << "\"" << tmp3[i];
			}
		}
        //menu prompt options
		cout << "##. To view an article\n";
		cout << "U. Page up\n";
		cout << "D. Page down\n";
		cout << "M. Back to Main Menu\n";

		cin >> s;

        //navigation options for this menu
		if(s == "U")
			position = max(position-10,0);
		else if(s == "D"){
			int temp1 = position+10;
			int temp2 = tmp1.size();
			position = min(temp1,temp2);
		}

		else if(s == "M")
			break;
		else
		{
            //set found to true if user selets the option to view article
			stringstream xx;
			xx << s;
			xx >> articleid;

			for(int id : tmp1)
				if(articleid == id)
				{
					found = true;
					break;
				}
			if(!found)
				cout << "Not a valid choice\n";
		}

		if(found)
			break;
	}

	if(s == "M")
	{
		menuid = 1;
		display_menu();
	}else if(found)
	{
        //if found set true, call the view article menu with the articlid.
		view_article_menu(articleid,-1);
	}
}

//close menu
void App::close_db()
{
    //close all the relations used during the session
	parser.parse("CLOSE accounts;");
	parser.parse("CLOSE access;");
	parser.parse("CLOSE article;");
	parser.parse("CLOSE boards;");
	parser.parse("CLOSE groupmember;");
	parser.parse("CLOSE groups;");
	parser.parse("CLOSE log;");
	parser.parse("CLOSE menus;");
	parser.parse("CLOSE menuitem;");
	parser.parse("CLOSE message;");
}
