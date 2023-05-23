#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//A node is one element of the doubly linked list. Stores one line as a string.
struct node{
    string line;
    node* next;
    node* prev;
    ~node() {
        if(next) delete[] next;
        next = nullptr;
        if(prev) delete[] prev;
        prev = nullptr;
    }
};

//The list stores all the lines of the written file.
struct list{
    size_t length;
    node* head;
    node* tail;
    ~list() {
        if(head) delete[] head;
        head = nullptr;
        if(tail) delete[] tail;
        tail = nullptr;
    }
};

//Reads a file and saves each line inside a node inside a list.
void readFile(list &list){
    ifstream file ("data.txt");
    while(file.good()){
        node* line = new node;
        if(list.head == NULL){
            list.head = line;
            list.tail = line;
            line->next = NULL;
            line->prev = NULL;
        }
        else{
            list.tail->next = line;
            line->prev = list.tail;
            line->next = NULL;
            list.tail = line;
        }
        getline(file, line->line);
        list.length++;
    }
}

//Writes a doubly linked list to a file
void writeFile(list &list){
    node* temp = list.head;
    ofstream file ("data.txt");
    if(file.is_open()){
        while(temp){
            file << (std::move(temp->line)) << "\n";
            temp = temp->next;
        }
        file.close();
    }
    else cout << "Error: Could not open file to write" << endl;
}