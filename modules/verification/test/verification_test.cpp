
#include "../verification.hpp"

#include <iostream>
#include <string_view>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <cstring>
#include <stdlib.h>


#define assert(condition, message) if(!(condition)){ \
                                        std::cout << "test failed at line: " << __LINE__ << std::endl; \
                                        std::cout << "with message: " << message << std::endl;\
                                        exit(EXIT_FAILURE);\
                                    }


int main(int argc, char *argv[]){

    std::filesystem::path currPath = std::filesystem::current_path();

    if(std::filesystem::remove_all(currPath / "Verified")){
        std::cout << "Verified folder exists, deleted.\n";
    }
    if(std::filesystem::remove_all(currPath / "Error")){
        std::cout << "Error folder exists, deleted.\n";
    }
    if(std::filesystem::remove_all(currPath / "New")){
        std::cout << "New folder exists, deleted.\n";
    }


    fileVerify(currPath, "testing.txt", "Content123456");
    //see if verified, new, and error is generated and is directories
    assert(std::filesystem::exists          (currPath / "Verified")           , "Error: 'Verified' folder not generated in path " << currPath);
    assert(std::filesystem::is_directory    (currPath / "Verified")           , "Error: 'Verified' is generated, but is not a folder in" << currPath);
    assert(std::filesystem::exists          (currPath / "New")                , "Error: 'New' folder not generated in path " << currPath);
    assert(std::filesystem::is_directory    (currPath / "New")                , "Error: 'New' is generated, but is not a folder in " << currPath);
    assert(std::filesystem::exists          (currPath / "Error")              , "Error: 'Error' folder not generated in path " << currPath);
    assert(std::filesystem::is_directory    (currPath / "Error")              , "Error: 'Error' is generated, but is not a folder in " << currPath);
   
    // see if the file is generated in new
    assert(std::filesystem::exists          (currPath / "New" / "testing.txt"), "Error: testing.txt not generated in New");
    assert(std::filesystem::is_regular_file (currPath / "New" / "testing.txt"), "Error: testing.txt is generated in new, but is not a file");

    { // Is content equal in new file
        std::ifstream file;
        std::stringstream file_content; 
        file.open(currPath / "New" / "testing.txt");
        if(file.is_open()){
            file_content << file.rdbuf();
        }
        file.close();
        assert(file_content.str() == "Content123456", "Content of written file: " << file_content.str() << " is not equal to generated input. Content123456");
    }

    { // Create new verified file with expected output
        std::ofstream file(currPath / "Verified" / "testing.txt");
        file << "Content123456";
        file.close();
    }
    {// Check verify with wrong content
        fileVerify(currPath, "testing.txt", "Not content123456");
        assert(std::filesystem::exists(currPath / "Error" / "testing.txt"), "File is not generated in error, even if content different");
        assert(std::filesystem::is_regular_file(currPath / "Error" / "testing.txt"), "File in error is not a regular file");
    }

    { // verify if error file is deleted on correct content. 
        fileVerify(currPath, "testing.txt", "Content123456");
        assert(!std::filesystem::exists(currPath / "Error" / "testing.txt"), "File is not removed after it is verified. ");
    }
    
    std::cout << "finished tests\n";


    return EXIT_SUCCESS;


}