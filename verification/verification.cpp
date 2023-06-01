
// TODO (Sondre): make a header file for this source file
#include "verification.hpp"



Folders::Folders(const std::filesystem::path& currPath) : verified(currPath / "Verified"), error(currPath / "Error"), new_output(currPath / "New") {
    
    if(this->verified.empty()){
        std::cout << "Verified folder does not exist creating folder" <<std::endl;
        std::filesystem::create_directory(currPath / "Verified"); // Why did you not use the variable folders here if you created it extra? (leave this as it is)
    }
    if(this->error.empty()){
        std::cout << "Error folder does not exist, creating folder" << std::endl;
        std::filesystem::create_directory(currPath / "Error");
    }
    if(this->new_output.empty()){
        std::cout << "New folder does not exist, creating folder" << std::endl;
        std::filesystem::create_directory(currPath / "New");
    }

}

// TODO(Sondre): add descriptions
// TODO(Sondre): this function does not change any state -> make this function 'const'
const bool folderEmpty(std::filesystem::path folder){
    return std::filesystem::is_empty(folder);
}


const bool Folders::verifiedEmpty(){return std::filesystem::is_empty(this->verified);}
const bool Folders::errorEmpty(){return std::filesystem::is_empty(this->error);}
const bool Folders::newEmpty(){return std::filesystem::is_empty(this->new_output);}
const bool Folders::doesFileInVerifiedExist(const std::string& filename){return doesFileExist(this->verified / filename);}
const bool Folders::doesFileInErrorExist(const std::string& filename){return doesFileExist(this->error / filename);}
const bool Folders::doesFileInNewExist(const std::string& filename){return doesFileExist(this->new_output / filename);}


const bool Folders::removeErrorFile(const std::string& filename){return std::filesystem::remove(this->error / filename);}


std::stringstream Folders::getContentOfVerifiedFile(const std::string& filename){
    std::ifstream file;
    std::stringstream file_content; // TODO (Sondre): replace this with the std::stringstream
    file.open(this->verified / filename);
    if(file.is_open()){
        file_content << file.rdbuf();
    }
    file.close();
    return file_content;
}

// TODO(Sondre): add descriptions
void Folders::writeNew(const std::string& filename, std::string_view content){
    return writeToFile(this->new_output, filename, content);
}

// TODO(Sondre): add descriptions
void Folders::writeError(const std::string& filename, std::string_view content){
    return writeToFile(this->error, filename, content);
}

// TODO(Sondre): add descriptions + rename the function to something that is more
// descriptive of what is does. Just the name 'all' does not tell me anything.
const bool Folders::allEmpty(){
    if(this->verified.empty() || this->error.empty() || this->new_output.empty()){
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
const std::array<bool, 3>& Folders::emptyFolders(){
    std::array<bool, 3> out = {false, false, false};
    if(this->verified.empty())
        out[0] = true;
    if(this->error.empty())
        out[1] = true;
    if(this->new_output.empty())
        out[2] = true;

    return out;
}





// TODO (Sondre): make a folder called test and a new file called verification_test.cpp and put the following main function inside that file.


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