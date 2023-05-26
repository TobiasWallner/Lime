#include <string>
#include <fstream>
#include <iostream>

/*!!!!!!!!!!!!!!!
	you can do this is a .cpp file but blease do not
*/
using namespace std;

/*!!!!!!!!!!!!!!!
	There is a std::list that already does everything for you
*/
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

/*!!!!!!!!!!!!!!!
	There is already a list
	
	use something like:
	
	
	useing Line = std::string;
	useing Text = std::list<Line>;
	
	or make classes instead
*/
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


/*!!!!!!!!!!!!!!!
	use some of the following functions instead: 
		
		* file.getline() https://en.cppreference.com/w/cpp/io/basic_istream/getline
			this is probably the easiest way to do it but is not the most performant one
		
		* file.read() https://en.cppreference.com/w/cpp/io/basic_istream/read
			this one is more performant, but you have to programm more yourself,
			because this function will not check for line endings.
		
		* file.readsome() https://en.cppreference.com/w/cpp/io/basic_istream/readsome
			this is probably the fastest one but we would have to benchmark different
			approaches against each other to do so.
*/
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
			/*!!!!!!!!!!!!!!!
				moveing into a filestream is pointless.
				the file will need to make a copy of the string anyway
				so you can allso pass it as you would normally do.
				no std::move needed here.
			*/
            file << (std::move(temp->line)) << "\n";
            temp = temp->next;
        }
        file.close();
    }
    else cout << "Error: Could not open file to write" << endl;
}