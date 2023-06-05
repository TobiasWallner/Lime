
// TODO (Sondre): make a header file for this source file


// TODO(Sondre): sort the header files, like in: \GroupIProject\lime\Lime.cpp
#include <iostream>
#include <string_view>
#include <fstream>
#include <Windows.h>
#include <regex>
#include <string>
#include <filesystem>


 
// TODO(Sondre): make this function void and remove the return
// TODO(Sondre): if this function is not being used outside of this source file change it to static linking to not convolute the global namespace in the object files. 
int writeToFile(std::filesystem::path& path, const std::string& filename, std::string_view content){
    std::ofstream file(path / filename);
    file << std::string(content);
    file.close();
    return 0; //remove this
}


// TODO(Sondre): add descriptions of what ressources this type manages and how it does that. 
// TODO(Sondre): since this struct is doing more than one thig -> make it a class instead. 
struct Folders{
	
	// TODO(Sondre) : since you have now probably made this struct into a call make those members private
	// TODO(Sondre) : Do the paths change after the constructor of this class has been called? if not -> make those member variables 'const'
    std::filesystem::path verified;
    std::filesystem::path error;
    std::filesystem::path n; // TODO(Sondre): rename to new

	// TODO(Sondre): add descriptions
    Folders(std::filesystem::path currPath) {
        
		// TODO (Sondre): the variable folder does not change over the course of this function -> make it a 'const std::vector<std::string>'
		// TODO (Sondre): the strings in the vector do not change over the course of the function -> make it a 'const std::vector<const std::string_view>'
		// TODO (Sondre): std::vector needs to allocate dynamic memory. One would only use a vector if the sice of something changes, however, it does not -> make it and array instead 'const std::array<const std::string_view>'
		std::vector<std::string> folders{"Verified", "Error", "New"};
		
		
        if(this->verified.empty()){
            std::cout << "Verified folder does not exist creating folder" <<std::endl;
            std::filesystem::create_directory(currPath / "Verified"); // Why did you not use the variable folders here if you created it extra? (leave this as it is)
        }
        if(this->error.empty()){
            std::cout << "Error folder does not exist, creating folder" << std::endl;
            std::filesystem::create_directory(currPath / "Error");
        }
        if(this->n.empty()){
            std::cout << "New folder does not exist, creating folder" << std::endl;
            std::filesystem::create_directory(currPath / "New");
        }

         for (const auto& dirEntry : std::filesystem::directory_iterator(currPath)){
            //TODO (Sondre): replace the folders variable with the actual string as you did above, makes things more readable.
			//TODO (Sondre): then you do not need the folders variable anymore and you can delete it -> juhu ... we went from 4 dynamic memory allocations (1 for the vector and 1 for each std::string) to zero.
			if(currPath / folders[0] == dirEntry.path()){
                this->verified = dirEntry.path();
                continue;
            }else if(currPath / folders[1] == dirEntry.path()){
                this->error = dirEntry.path();
                continue;
            }else if(currPath / folders[2] == dirEntry.path()){
                this->n = dirEntry.path();
                continue;
            }
        }
    }

	// TODO(Sondre): add descriptions
	// TODO(Sondre): this function does not change any state -> make this function 'const'
    bool folderEmpty(std::filesystem::path folder){
		return std::filesystem::is_empty(folder);
    }

	// TODO(Sondre): add descriptions
	// TODO(Sondre): make this function void
    int writeNew(const std::string& filename, std::string_view content){
        return writeToFile(this->n, filename, content);
    }

	// TODO(Sondre): add descriptions
	// TODO(Sondre): make this function void
    int writeError(const std::string& filename, std::string_view content){
        return writeToFile(this->error, filename, content);
    }

	// TODO(Sondre): add descriptions + rename the function to something that is more
	// descriptive of what is does. Just the name 'all' does not tell me anything.
	// TODO(Sondre): this function does not change any state -> make this function 'const'
    bool all(){
        if(this->verified.empty() || this->error.empty() || this->n.empty()){
            return false;
        }else{
            return true;
        }
    }

	// TODO(Sondre): If you look at the standard library, you may notice that all containers like 'list' and 'vector'
	// have a method called empty(). empty is usually a function that checks if the container is empty() and returns a bool.
	// This function does not follow that convention.
	// Therefore, please rename it.
	// + this function does not change any state -> make it a 'const' member function
    std::vector<std::string> empty(){
		// TODO (Sondre): do not use a vector here. A vector uses dynamic memory and this can also be solved without that.
        std::vector<std::string> out; 

        if(this->verified.empty())
            out.push_back("Verified");
        if(this->error.empty())
            out.push_back("Error");
        if(this->n.empty())
            out.push_back("New");

		// TODO (Sondre): do not return a vector here -> consider returning a struct with 3 bool or something like that.
        return out;
    }


} ;

// TODO(Sondre): add descriptions
// TODO(Sondre): if this function is not being used outside of this source file change it to static linking to not convolute the global namespace in the object files. 
void listFiles(std::string &path){
    for(const auto & entry : std::filesystem::directory_iterator(path)){
        std::cout << entry.path() << std::endl;
    }
}

// TODO(Sondre): add descriptions
// TODO(Sondre): check if file in folder ... what? exists? -> rename the function maybe to 'does_file_exist(const std::filesystem::path& path)'
bool checkIfFileInFolder(const std::filesystem::path& path){
    return std::filesystem::is_regular_file(std::filesystem::status(path));
}


//TODO(Sondre): add descriptions
bool isEqual(std::string_view file_content, std::string_view verification_content){
	// TODO (Sondre): you could have just written: 
	// return file_content == verification_content;
	// don't overcomplicate things. And to further simplify just delete that function since the operator == does exactly
	// what this function does and everyone knows what '==' does for string_views

    if (file_content == verification_content){ 
        return true;
    }

    return false;
}


//TODO(Sondre): add descriptions, what does the functions return value say?
int testFunction(const std::string& filename, std::string_view content){

	//TODO (Sondre): what if I want to have the folders at a different location? -> make the path that the folders are created in a function parameter
    std::filesystem::path currPath = std::filesystem::current_path();
    Folders f(currPath); //TODO (Sondre): this instance of the Folder has some meaning. Try do find a descriptive name.


    // Check if folders exist in current folder
	// TODO(Sondre): why do you need to check if the folders exist? That is what the constructor Folder::Folder just did.
	// TODO(Sondre): remove the check here or remove it from the constructor
	if(f.folderEmpty(f.verified)){
        std::cout << "Folder vertified do not exist or is empty, new file generated." << std::endl;
		
		// TODO(Sondre): Why do you write a new file if the verified folder is empty? what if I want to make a second different test. 
		// 		The verified foler would then not be empty and therefore no new file would be generated for the new test. -> seems like a bug.
        f.writeNew(filename, content);
        //create and write to file
        
		// TODO(Sondre): what does 0 mean as a return value? return EXIT_SUCCESS instead or make your own enum, or explin what it means in the function description
		return 0;
    }
	
	// TODO (Sondre): everyone either already knows that or can read it on cppreference if they want. delete the following comment.
    // the "/" appends a file or folder to a path.
    if(!checkIfFileInFolder(f.verified / filename)){
         std::cout << "File does not exist in verified, new file generated in new." << std::endl;

        //create and write to file
        f.writeNew(filename, content);
        return 0;
    }
    
    std::ifstream file;
    std::string file_content; // TODO (Sondre): replace this with the std::stringstream
    file.open(f.verified / filename);
    if(file.is_open()){
        std::stringstream buffer;
        buffer << file.rdbuf();
        file_content = buffer.str(); // TODO(Sondre): remove this
    }
    file.close();

	// TODO(Sondre): replace this with: file_content.str() == content
	// TODO(Sondre): since the result variable will not change -> make it 'const'
	// TODO(Sondre): rename this variable, just result is not very descriptive -> consider is_content_verified = file_content.str() == content
    bool result = isEqual(file_content, content); 
    if(!result){
        std::cout << "File " << filename << " in verified is NOT equal input!" << std::endl;
        f.writeError(filename, "Files are not equal");
        return 0;
    } // TODO (Sondre): add an else there that deletes the file in the error folder if the file is verified and still exists 

    std::cout << "File: " << filename << " in verified is to input" << std::endl;
    return 0;
}


// TODO (Sondre): make a folder called test and a new file called verification_test.cpp and put the following main function inside that file.
int main(int argc, char *argv[]){


    testFunction("testing.txt", "Content1234");

    return 0;


}

/* TODO (Sondre): make a CMakeLists.txt and turn your verification into a library. 

	you can do it like I did it in with utf8_string:
	
	---------- utf8_string CMakeLists ------------
	
	add_library(utf8_string INTERFACE
		utf8_char.hpp
		utf8_string.hpp
	)

	target_include_directories(utf8_string INTERFACE 
		${CMAKE_CURRENT_SOURCE_DIR}
	)
	
	add_subdirectory(test)
	
	---------------- END -----------------
	
	However after **add_library(** you will enter the name of this library maybe:
	
		add_library(verification


	Since this library will contain .cpp files and .hpp files it will be a 'static' library.
	My utf8_string only contains header files and no source files, that is why it has to use the INTERFACE command
	So use the STATIC instead.
	
	Then list all files that your library contains in in the brackets.
	
	just copy the target_include_directories and replace the library name.
	
	then add the subdirectory for the tests.
	
	in your test folder also create CMake lists that adds your test function with the main to the CMake test environment.
	
	Therefore just copy paste the CMakeLists.txt file from GroupIProject\modules\utf8_string\test and rename all executables and targets.
	
	---------- utf8_string/test CMakeLists ------------
	
	# ------------ Char ----------------
	add_executable(utf8_char_test
		test_utf8_char.cpp
	)

	target_link_libraries(utf8_char_test utf8_string)

	add_test(utf8_char_ctest utf8_char_test)


	# ------------- String ----------------

	add_executable(utf8_string_test
		test_utf8_string.cpp
	)

	target_link_libraries(utf8_string_test utf8_string)

	add_test(utf8_string_ctest utf8_string_test)
	
	---------------- END -----------------

	If you have any questions do not hesitate, I am happy to help.
	
	I know that this was much, but I think you did great work useing all those functions from the std::filesystem.
	I even learned a little bit from you, because I did not know that the function std::filesystem::is_regular_file() existed.
	I would have used std::filesystem::exists() but yours is the better choice. 

*/
