/* 
    File: DBMShell.cpp
*/


/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include "Parser.H"
#include <algorithm>
using namespace std;


/*--------------------------------------------------------------------------*/
/* MAIN */
/*--------------------------------------------------------------------------*/

int main(){
  
  //Create a parser which has a tokenstream object and evaluator object
  Parser p = Parser();

  while(true)//loop until exit kills
  {
    cout << "DBMS Shell>";//print prompt
    string command;//string to hold command
    getline(cin,command);//get command from user or i/o redirection
    
    if (command.length() == 0 || command[0] == '#')
    {
	cout << endl;
	continue;  
    }
    
    cout << command << endl << endl;//output the command
	  
    if (p.parse(command))//Parse each SQL command/querry  and check if it passes
      cout << "\n\n\n";//if passed
    else//if failed in red
      cout << "\033[1;31mFailed Parse\033[0m\n\n\n";
  }
  
  return 0;//return success code
}
