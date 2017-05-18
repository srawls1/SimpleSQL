
#include "Parser.H"
using namespace std;
int main(){
	//Create a parser which has a tokenstream object and evaluator object
    Parser p1 = Parser();
    Parser p2 = Parser();

    vector<string> sqlcommands1  = { //create test vector
        
	//Command Tests
	"CREATE TABLE TR1 (Name VARCHAR(20), Birthdate VARCHAR(10), Age INTEGER) PRIMARY KEY (Name, Birthdate);",
	"SHOW TR1;",
	"INSERT INTO TR1 VALUES FROM (\"John Smith\", \"1970/1/1\" , 45);",
	"INSERT INTO TR1 VALUES FROM (\"Jane Doe\", \"1970/1/1\" , 45);",
	"INSERT INTO TR1 VALUES FROM (\"John Doe\", \"1970/1/1\" , 45);",
	"SHOW TR1;",
	"CREATE TABLE TR2 (Name VARCHAR(20), Birthdate VARCHAR(10), Age INTEGER) PRIMARY KEY (Name, Birthdate);",
	"INSERT INTO TR2 VALUES FROM (\"John Smith\", \"1970/1/1\", 45);",
	"INSERT INTO TR2 VALUES FROM (\"Johnnie Doe\", \"2000/1/1\", 15);",
	"INSERT INTO TR2 VALUES FROM (\"Janie Doe\", \"2000/1/1\", 15);",
	"SHOW TR2;",
	"CREATE TABLE TR3 (Name VARCHAR(20), Birthdate VARCHAR(10), Age INTEGER) PRIMARY KEY (Name, Birthdate);",
	"INSERT INTO TR3 VALUES FROM RELATION TR1;",
	"SHOW TR3;",
	"TR4 <- select (Age == 15) TR2;",
	"SHOW TR4;",
	"TR5 <- project (Name, Age) TR1;",
	"SHOW TR5;",
	"TR6 <- rename (Full_Name, Birthdate, Years_Old) TR1;",
	"SHOW TR6;",
	"TR7 <- TR1 + TR2;",
	"SHOW TR7;",
	"TR8 <- TR1 - TR2;",
	"SHOW TR8;",
	"TR9 <- TR1 * TR2;",
	"SHOW TR9;",
	"DELETE FROM TR1 WHERE Name == \"John Smith\";",
	"SHOW TR1;",
	"SAVE TR1;"};//end of test vector

    vector<string> sqlcommands2 = { //create test vector
	"OPEN TR1;",
	"SHOW TR1;",
	"UPDATE TR1 SET Name= \"Bob Doe\", Age=55 WHERE Name == \"John Doe\";",
	"SHOW TR1;",
	"EXIT;"
	};//end of test vector
    
    
    for(int i = 0; i < sqlcommands1.size(); i++)
    {
		/*if(i < sqlcommands.size()-16)//ouput whether the command should pass or fail
			cout << "Should Pass: " << endl;
		else
			cout << "Should Fail: " << endl;
		*/
		cout << sqlcommands1[i] << endl;//output tested sql
        
		if (p1.parse(sqlcommands1[i]))//Parse each SQL command/querry in the vector and check if it passes
            cout << "\n\n\n";//if passed
        else//if failed in red
           cout << "\033[1;31mFailed Parse\033[0m\n\n\n";
    }
    for(int i = 0; i < sqlcommands2.size(); i++)
    {
		/*if(i < sqlcommands.size()-16)//ouput whether the command should pass or fail
			cout << "Should Pass: " << endl;
		else
			cout << "Should Fail: " << endl;
		*/
		cout << sqlcommands2[i] << endl;//output tested sql
        
		if (p2.parse(sqlcommands2[i]))//Parse each SQL command/querry in the vector and check if it passes
            cout << "\n\n\n";//if passed
        else//if failed in red
           cout << "\033[1;31mFailed Parse\033[0m\n\n\n";
    }
}
