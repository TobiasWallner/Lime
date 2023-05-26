
#include <iostream>
#include <string_view>
#include <fstream>
#include <Windows.h>
#include <regex>
#include <string>
#include <filesystem>

struct {
    std::filesystem::path verified;
    std::filesystem::path error;
    std::filesystem::path n;

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


} Folders;

int get_curr_dir(char* pBuf, size_t& len){
    int bytes = GetModuleFileNameA(NULL, pBuf, len);
    return bytes ? bytes : -1;
}


void write_to_file(std::string folder, std::string fileName, std::string_view content){

    std::ofstream file(fileName);

    file << content;

    file.close();

}

void search_regex(std::regex re, std::string search){
    std::smatch match2;
    if(std::regex_search(search, match2, re)){
        std::cout << "File name " << match2[0] << std::endl;
        std::cout << "Not defined is " << match2[1] << std::endl;
        std::cout << "File path " << match2.prefix() << std::endl;
        std::cout << "suffix " << match2.suffix() << std::endl;
    }
}

std::pair<std::string, std::string> get_path_and_filename(char* fullPath){
    std::regex re("[^\\\\]+$"); //Matches with a word that starts after a \ and ends with the end of the line
    std::smatch match;
    std::string s(fullPath);
    std::pair<std::string, std::string> out;
    if (std::regex_search(s, match, re)){
        out = std::make_pair(match.prefix(), match[0]);
    }
    return out;
}

void listFiles(std::string &path){
    for(const auto & entry : std::filesystem::directory_iterator(path)){
        std::cout << entry.path() << std::endl;
    }

}

int checkFolders(std::filesystem::path currPath){
    std::vector<std::string> folders{"Verified", "Error", "New"};
 
    for (const auto& dirEntry : std::filesystem::directory_iterator(currPath)){
        if(currPath / folders[0] == dirEntry.path()){
            Folders.verified = dirEntry.path();
            continue;
        }else if(currPath / folders[1] == dirEntry.path()){
            Folders.error = dirEntry.path();
            continue;
        }else if(currPath / folders[2] == dirEntry.path()){
            Folders.n = dirEntry.path();
            continue;
        }
    }

    if(!Folders.all()){
        for(const auto& folder : Folders.empty()){
            std::filesystem::create_directory(currPath / folder);
        }
    }
    


    return 0;
}



bool checkIfFileInFolder(const std::filesystem::path& path){

    return std::filesystem::is_regular_file(std::filesystem::status(path));

}


int main(int argc, char *argv[]){

    char pBuf[256];
    size_t len = sizeof(pBuf); 
    if (!get_curr_dir(pBuf, len)){
        std::cout << "Cannot find current working directory\n";
        return -1;
    }

    std::pair<std::string, std::string> file = get_path_and_filename(pBuf);


    const std::filesystem::path path{file.first};
    
    std::filesystem::path currPath = std::filesystem::current_path();



    
    checkFolders(currPath);

    for (const auto& dirEntry : std::filesystem::directory_iterator(currPath)){
        if(checkIfFileInFolder(dirEntry.path())){

            std::cout << dirEntry << " is A FILE" << std::endl;
        }

    }

    return 0;



    // // r = "[^\\]+$";
    // std::smatch match2;
    
    // if(std::regex_search(s, match2, r)){
    //     std::cout << "File name " << match2[0] << std::endl;
    //     std::cout << "Not defined is " << match2[1] << std::endl;
    //     std::cout << "File path " << match2.prefix() << std::endl;
    //     std::cout << "suffix " << match2.suffix() << std::endl;
    // }
    // try {
    //     std::regex re("[^\\\\]+$");
    //     search_regex(re, pBuf);
    // }
    // catch (const std::regex_error& e) {
    //     std::cout << "regex_error caught: " << e.what() << '\n';
    //     std::cout << "[^\\\\]+$" << std::endl;
    //     if (e.code() == std::regex_constants::error_brack) {
    //         std::cout << "The code was error_brack\n";
    //     }
    // }

    // try {
    //     std::regex re("[^\\\\]+$", std::regex_constants::multiline);
    //     search_regex(re, pBuf);
    // }
    // catch (const std::regex_error& e) {
    //     std::cout << "regex_error caught: " << e.what() << '\n';
    //     std::cout << "[^\\\\]+$ multiline" << std::endl;
    //     if (e.code() == std::regex_constants::error_brack) {
    //         std::cout << "The code was error_brack\n";
    //     }
    // }
    // try {
    //     std::regex re(R"regex((.*?)([^\\]+$))regex");
    //     search_regex(re, pBuf);


    // }
    // catch (const std::regex_error& e) {
    //     std::cout << "regex_error caught: " << e.what() << '\n';
    //     std::cout << "regex((.*?)([^\\]+$))regex\n";
    //     if (e.code() == std::regex_constants::error_brack) {
    //         std::cout << "The code was error_brack\n";
    //     }
    // }
    // try {
    //     std::regex re(R"regex((.*?)([^\\]+$))regex", std::regex_constants::multiline);
    //     search_regex(re, pBuf);
    // }
    // catch (const std::regex_error& e) {
    //     std::cout << "regex_error caught: " << e.what() << '\n';
    //     std::cout << "regex((.*?)([^\\]+$))regex multiline\n";
    //     if (e.code() == std::regex_constants::error_brack) {
    //         std::cout << "The code was error_brack\n";
    //     }
    // }



    // std::string string_to_split(pBuf);
    // std::regex rgx("[^\\]+", std::regex_constants::ECMAScript|std::regex_constants::multiline);
    // std::sregex_token_iterator iter(string_to_split.begin(),
    //     string_to_split.end(),
    //     rgx,
    //     -1);
    // std::sregex_token_iterator end;
    // for ( ; iter != end; ++iter)
    //     std::cout << *iter << '\n';



    //std::string_view content{"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."};

    //write_to_file("test", "test.txt", content);

}




// int test_function(std::string_view data, std::string filename){
    




// }




