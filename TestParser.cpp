//
//  TestParser.cpp
//
//
//  Created by Prem Pokharel on 9/17/15.
//
//


//includes
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "Parser.H"
#include "TokenStream.H"
#include <sstream>
#include <iostream>


//main
int main(){

    //Create a parser which has a tokenstream object and evaluator object
    Parser p = Parser();

    vector<string> sqlcommands  = { //create test vector
        
	//Command Tests
	"OPEN records;",
	"CLOSE records;",
	"SAVE records;",
	"EXIT;",
	"SHOW records;",
	"SHOW (select (Name == \"bob\") records);",
	"CREATE TABLE records (Name VARCHAR(20), Birthdate VARCHAR(10), Age INTEGER) PRIMARY KEY (Name, Birthdate);",
	"INSERT INTO records VALUES FROM (\"Jane\", \"1-1-1970 00:00\", 45);",
	"INSERT INTO records VALUES FROM RELATION project (n, b, a) records1;",
	"INSERT INTO records VALUES FROM RELATION records1;",
	"UPDATE records SET Name= \"BOB\", AGE=55 WHERE Name == \"John\";",
	"DELETE FROM records WHERE Name == \"BOB\";",

	//select functions Tests
	"records <- select (Age != 0) records;",
	"records1 <- select (Name == \"bob\" && Age <= 5 || Age >= 90) records;",
	"records1 <- select (Name == \"bob\" && (Age <= 5 && Age >= 90)) records;",
	"records2 <- select (Name == \"bob\") (select (Age >= 25) records);",
	"records <- select (Name == \"bob\") (project (Name) (select (Age >= 45) (records1 + records2)));",
	"records <- select (Name == \"bob\") (rename (FullName) (select (Age >= 45) (records1 - records2)));",
	"records <- select (Name == \"bob\") (project (Name) (select (Age >= 45) (rename (FullName, DOB) (records1 * records2))));",
	"records <- select (Name == \"bob\" && Age <= 5) (select (Age >= 90) records);",

	//Project functions Tests
	"records <- project (Name, Age) records;",
	"records <- project (S124) records;",
	"records1 <- select (Name == \"bob\" && Age >= 5) records;",
	"records2 <- select (Name == \"bob\" && Age <= 70) records;",
	"records3 <- project (Name) (select (Name == \"bob\" && Age >= 15) (records1 * records2));",

	//Rename function Tests
	"records <- rename (Full_Name, Birth, Age) records;",
	"records <- rename (Name, DOB) (project (FullName, Age) records);",
	"records <- rename (Name, DOB) (project (FullName, Age) (rename (FullName) (select (Age >= 45) records)));",

	//Simple Union, Difference and Cross Product Relations
	"records <- records1 + records2;",
	"records <- records1 - records2;",
	"records <- records1 * records2;",
	"records <- (records1);",
	"records <- ((records1 + records2) - records3);",
	"records <- records;",
			
	//Failing Cases
	"2fail <- records;",
	"_fail <- records;",
	"fail <- records",
	"fail < records;",
	"records <- records1 + records2 + records3;",
	"OPEN project (S124) records;",
	"CLOSE project (S124) records;",
	"CREATE TABLE records (Name VARCHAR, Birthdate VARCHAR, Age INTEGER) PRIMARY KEY (Name, Birthdate);",
	"CREATE TABLE records (Name, Birthdate, Age) PRIMARY KEY (Name, Birthdate);",
	"CREATE TABLE records (Name VARCHAR(20), Birthdate VARCHAR(10), Age INTEGER);",
	"records <- select (Age = 0) records;",
	"records <- select (Age == 0 | Age == 1) records;",
	"records <- select (Age == 0 & Age == 1) records;",
	"records <- project Name, Age records;",
	"records <- project () records;",
	"DELETE FROM records WHERE *;"
	};//end of test vector
    
    
    for(int i = 0; i < sqlcommands.size(); i++)
    {
		if(i < sqlcommands.size()-16)//ouput whether the command should pass or fail
			cout << "Should Pass: " << endl;
		else
			cout << "Should Fail: " << endl;
		
		cout << sqlcommands[i] << endl;//output tested sql
        
		if (p.parse(sqlcommands[i]))//Parse each SQL command/querry in the vector and check if it passes
            cout << "Passed\n\n\n";//if passed
        else//if failed in red
           cout << "\033[1;31mFailed\033[0m\n\n\n";
    }

}
