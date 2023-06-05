/*This file includes
    a function to read a file into a list
    and a function to write a list into a file.
use something like:
	useing Line = std::string;
	useing Text = std::list<Line>;
*/
#include <string>
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

//Writes a file into a list, each line is a string
void readFile(std::list<std::string> &list, ifstream &file){
    while(file.good()){
        string line;
        getline(file, line);
        list.push_back(std::move(line));
    }
}
 
//Writes a doubly linked list into a file
void writeFile(std::list<std::string> &list, ofstream &file){
    if(file.is_open()){
        for(string line : list){
            file << line << "\n";
        }
        file.close();
    }
    else cout << "Error: Could not open file to write" << endl;
}