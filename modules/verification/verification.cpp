#include "verification.hpp"

// Writes the input content to the input file
// path: path where the file is
// filename: name of the file written to 
// content: content that will be written
static void writeToFile(const std::filesystem::path& path, const std::string& filename, std::string_view content){
    std::ofstream file(path / filename);
    file << std::string(content);
    file.close();
}


//Lists all files and directories in the given path
static void listFiles(std::string &path){
    for(const auto & entry : std::filesystem::directory_iterator(path)){
        std::cout << entry.path() << std::endl;
    }
}

// Returns true if the file on the given path is a file.
static const bool doesFileExist(const std::filesystem::path& path){
    return std::filesystem::is_regular_file(std::filesystem::status(path));
}


// Full function for checking the input vs the verified files. 
// currPath: path where you have the verified, error and new folders
// filename: name of the file you want checked
// content: content you want to check with the file. 
void testFunction(const std::filesystem::path& currPath, const std::string& filename, std::string_view content){

    Folders currFolder(currPath); 


	if(currFolder.verifiedEmpty()){
        std::cout << "Folder vertified do not exist or is empty, new file generated." << std::endl;
		
      currFolder.writeNew(filename, content);
 		return;
    }
	
	
    if(!currFolder.doesFileInVerifiedExist(filename)){
         std::cout << "File does not exist in verified, new file generated in new." << std::endl;

        currFolder.writeNew(filename, content);
        return;
    }
    
    std::stringstream file_content = currFolder.getContentOfVerifiedFile(filename);

    const bool is_content_verified = (file_content.str() == content); 
   
    if(!is_content_verified){
        std::cout << "File " << filename << " in verified is NOT equal input!" << std::endl;
        currFolder.writeError(filename, "Files are not equal");
        return;
    }else if(currFolder.doesFileInErrorExist(filename)){
        if(currFolder.removeErrorFile(filename)){
            std::cout << filename << " error file is removed" << std::endl;
        }
    }
    
    std::cout << "File: " << filename << " is successfully verified." << std::endl;
    return;
}







Folders::Folders(const std::filesystem::path& currPath) : verified(currPath / "Verified"), error(currPath / "Error"), new_output(currPath / "New") {
    
    if(this->verified.empty()){
        std::cout << "Verified folder does not exist creating folder" <<std::endl;
        std::filesystem::create_directory(currPath / "Verified"); 
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
    std::stringstream file_content; 
    file.open(this->verified / filename);
    if(file.is_open()){
        file_content << file.rdbuf();
    }
    file.close();
    return file_content;
}


void Folders::writeNew(const std::string& filename, std::string_view content){
    return writeToFile(this->new_output, filename, content);
}


void Folders::writeError(const std::string& filename, std::string_view content){
    return writeToFile(this->error, filename, content);
}

const bool Folders::allEmpty(){
    if(this->verified.empty() || this->error.empty() || this->new_output.empty()){
        return false;
    }else{
        return true;
    }
}


const std::array<bool, 3> Folders::emptyFolders(){
    std::array<bool, 3> out {false, false, false};
    if(this->verified.empty())
        out[0] = true;
    if(this->error.empty())
        out[1] = true;
    if(this->new_output.empty())
        out[2] = true;

    return out;
}


