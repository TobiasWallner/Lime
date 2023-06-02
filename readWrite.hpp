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

// TODO(Helena): never, ever write the following line in a header file!
using namespace std;

/*	TODO(Helena): the functions you wrote is better now but will produce linker errors
	either do one of the following: mark them as 'inline' or put them in a .cpp file.
*/
//Writes a file into a list, each line is a string
void readFile(std::list<std::string> &list){
    ifstream file ("data.txt");
    while(file.good()){
        string line;
        getline(file, line);
        list.push_back(std::move(line));
    }
}

// TODO(Helena): make the output file name a parameter of the function 
//Writes a doubly linked list into a file
void writeFile(std::list<std::string> &list){
    ofstream file ("data.txt");
    if(file.is_open()){
        for(string line : list){
            file << line << "\n";
        }
        file.close();
    }
    else cout << "Error: Could not open file to write" << endl;
}