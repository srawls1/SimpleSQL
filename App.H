#ifndef APP_H
#define APP_H

#include "Parser.H"
#include "DBManage.H"
#include <iostream>
#include <sstream>
#include <time.h>

using namespace std;

class App{
	private:
		DBManage* dbm;
		Parser parser;

		//last ids of db for 
		int num_users;
		int num_boards;
		int num_messages;
		int num_articles;

		//current user and major menu
		int userid = -1;
		int menuid = 0;

		int num_groups;
		int num_events;

	public:
		App(): num_users(0), num_groups(0), num_articles(0), num_messages(0), num_events(0), userid(0), menuid(0)  { //setup database by init tables and nums
			dbm = parser.get_dbm();
			set_up_accounts();
			set_up_groups();
			set_up_messages();
			set_up_articles();
			set_up_menus();
			set_up_boards();
		}

		~App()
		{
			close_db();
		}

		//board management
		void set_up_boards();
		void create_board(int id, string board_name, int status);

		//user management
		void set_up_accounts();
		void create_user(int id, string username, string password, string full_name,
			string phone, string fax, string address);

		//article management
		void set_up_articles();
		void create_article(); //handles post and edit functions
		void delete_article();	//handles the admin and normal user

		//goups
		void set_up_groups();
		void list_groups_menu();
		void join_group_menu();
		void new_group_menu();
		void edit_group_menu(int groupid);
		void modify_groupmember_menu(int groupid);
		void modify_groupmember_menu();
		void modify_group_menu();

		//menus
		void set_up_menus();
		void display_menu();
		void process_menu(string s);


		//account menus
		void login_menu();
		void new_account_menu();
		void account_menu(int uid);
		void list_user_menu();
		void modify_user_menu();

		
		//board & article menus
		void goto_board_menu();
		void create_board_menu();
		void delete_board_menu();
		void edit_article_menu(int articleid, int boardid);
		void create_article_menu(int boardid);
		void view_article_menu(int articleid, int boardid);
		void delete_article_menu(int articleid, int boardid);
		void view_board_menu(int boardid);
		void view_user_article_menu(int uid);
		void list_board_menu();

		//message menus
		void set_up_messages();//init
		void send_message_menu(string sender);
		void view_message_menu(int useridto);
		void create_message_menu();
		void delete_message_menu();
		void list_message_menu();
		
		//log
		void log_menu();
		
		//menu menus
		void modify_menus_menu();

		//save add dbs
		void close_db();
};

#endif
