
#include <iostream>
#include <string_view>
#include <fstream>
#include <Windows.h>
#include <regex>
#include <string>
#include <filesystem>

int writeToFile(std::filesystem::path& path, const std::string& filename, std::string_view content){
    std::ofstream file(path / filename);

    file << std::string(content);

    file.close();

    return 0;
}



struct Folders{
    std::filesystem::path verified;
    std::filesystem::path error;
    std::filesystem::path n;

    Folders(std::filesystem::path currPath) {
        std::vector<std::string> folders{"Verified", "Error", "New"};
 
        for (const auto& dirEntry : std::filesystem::directory_iterator(currPath)){
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

        if(this->verified.empty()){
            std::cout << "Verified folder does not exist creating folder" <<std::endl;
            std::filesystem::create_directory(currPath / "Verified");
        }
        if(this->error.empty()){
            std::cout << "Error folder does not exist, creating folder" << std::endl;
            std::filesystem::create_directory(currPath / "Error");
        }
        if(this->n.empty()){
            std::cout << "Error folder does not exist, creating folder" << std::endl;
            std::filesystem::create_directory(currPath / "New");
        }
    }

    bool folderEmpty(std::filesystem::path folder){
        return std::filesystem::is_empty(folder);
    }

    int writeNew(const std::string& filename, std::string_view content){
        return writeToFile(this->n, filename, content);
        
    }

    int writeError(const std::string& filename, std::string_view content){
        return writeToFile(this->error, filename, content);
    }

    bool all(){
        if(this->verified.empty() || this->error.empty() || this->n.empty()){
            return false;
        }else{
            return true;
        }
    }

    std::vector<std::string> empty(){
        std::vector<std::string> out;

        if(this->verified.empty())
            out.push_back("Verified");
        if(this->error.empty())
            out.push_back("Error");
        if(this->n.empty())
            out.push_back("New");

        return out;
    }


} ;



void listFiles(std::string &path){
    for(const auto & entry : std::filesystem::directory_iterator(path)){
        std::cout << entry.path() << std::endl;
    }
}

bool checkIfFileInFolder(const std::filesystem::path& path){
    return std::filesystem::is_regular_file(std::filesystem::status(path));
}



bool isEqual(std::string_view file_content, std::string_view verification_content){

    if (file_content == verification_content){
        return true;
    }

    return false;
}



int testFunction(const std::string& filename, std::string_view content){

   
    std::filesystem::path currPath = std::filesystem::current_path();
    Folders f(currPath);
    // Check if folders exist in current folder
    if(f.folderEmpty(f.verified)){
        std::cout << "Folder vertified do not exist or is empty, new file generated." << std::endl;

        f.writeNew(filename, content);
        //create and write to file
        return 0;
    }
    // the "/" appends a file or folder to a path.
    if(!checkIfFileInFolder(f.verified / filename)){
         std::cout << "File does not exist in verified, new file generated in new." << std::endl;

        //create and write to file
        f.writeNew(filename, content);
        return 0;
    }
    

    std::ifstream file;
    std::string file_content;
    file.open(f.verified / filename);
    if(file.is_open()){
        std::stringstream buffer;
        buffer << file.rdbuf();
        file_content = buffer.str();
    }
    file.close();

    bool result = isEqual(file_content, content);


    if(!result){

        std::cout << "File " << filename << " in verified is NOT equal input!" << std::endl;

        f.writeError(filename, "Files are not equal");
        return 0;
    }

    std::cout << "File: " << filename << " in verified is to input" << std::endl;
    return 0;
}



int main(int argc, char *argv[]){


    testFunction("testing.txt", "Content1234");

    return 0;


}

