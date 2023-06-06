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


// Returns true if the file on the given path is a file.
static const bool doesFileExist(const std::filesystem::path& path){
    return std::filesystem::is_regular_file(std::filesystem::status(path));
}

class Folders{
    /*
    Class folders holds variables for the paths to the "verified", "error" and "new"
    folders of the verification. 

    */
    private:
    const std::filesystem::path verified;
    const std::filesystem::path error;
    const std::filesystem::path new_output;       

    public:

    Folders(const std::filesystem::path& currPath) : verified(currPath / "Verified"), error(currPath / "Error"), new_output(currPath / "New") {
        
        if(!std::filesystem::exists(this->verified)){
            std::cout << "Verified folder does not exist creating folder" <<std::endl;
            std::filesystem::create_directory(currPath / "Verified"); 
        }
        if(!std::filesystem::exists(this->error)){
            std::cout << "Error folder does not exist, creating folder" << std::endl;
            std::filesystem::create_directory(currPath / "Error");
        }
        if(!std::filesystem::exists(this->new_output)){
            std::cout << "New folder does not exist, creating folder" << std::endl;
            std::filesystem::create_directory(currPath / "New");
        }

    }


    //Following functions checks if verified is empty using std::filesystem
    const bool verifiedEmpty(){return std::filesystem::is_empty(this->verified);}

   

    //Following checks if the given file exists in either verified or error
    const bool doesFileInVerifiedExist(const std::string& filename){return doesFileExist(this->verified / filename);}
    const bool doesFileInErrorExist(const std::string& filename){return doesFileExist(this->error / filename);}

    // removes the given file from the error directory. There are no file for new or error as these
    // should be managed by the user themselves. 
    const bool removeErrorFile(const std::string& filename){return std::filesystem::remove(this->error / filename);}


    // Opens and extracts the content of the given filename in the verified folder
    std::stringstream getContentOfVerifiedFile(const std::string& filename){
        std::ifstream file;
        std::stringstream file_content; 
        file.open(this->verified / filename);
        if(file.is_open()){
            file_content << file.rdbuf();
        }
        file.close();
        return file_content;
    }

    // Writes the given content to a new file filename in the folder new
    void writeNew(const std::string& filename, std::string_view content){
        return writeToFile(this->new_output, filename, content);
    }
    // Writes the given content to a new file filename in the folder error
    void writeError(const std::string& filename, std::string_view content){
        return writeToFile(this->error, filename, content);
    }

};



// Full function for checking the input vs the verified files. 
// currPath: path where you have the verified, error and new folders
// filename: name of the file you want checked
// content: content you want to check with the file. 
void fileVerify(const std::filesystem::path& currPath, const std::string& filename, std::string_view content){
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


