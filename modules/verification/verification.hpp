// C++ std
#include <iostream>
#include <string_view>
#include <fstream>
#include <string>
#include <filesystem>
#include <array>

// Function to be run from main to test the file.
void testFunction(const std::filesystem::path&, const std::string&, std::string_view );


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

    Folders(const std::filesystem::path&);

    //Sees if the folder in path is empty
    const bool folderEmpty(const std::filesystem::path&); 
 
    //Following functions checks if verified, error or new is empty using std::filesystem
    const bool verifiedEmpty(); 
    const bool errorEmpty();
    const bool newEmpty();

    //Following checks if the given file exists in either verified, error or new 
    const bool doesFileInVerifiedExist(const std::string&);
    const bool doesFileInErrorExist(const std::string&);
    const bool doesFileInNewExist(const std::string&);

    // removes the given file from the error directory. There are no file for new or error as these
    // should be managed by the user themselves. 
    const bool removeErrorFile(const std::string&);

    // Opens and extracts the content of the given filename in the verified folder
    std::stringstream getContentOfVerifiedFile(const std::string&);

    // Writes the given content to a new file filename in the folder new
    void writeNew(const std::string&, std::string_view);

    // Writes the given content to a new file filename in the folder error
    void writeError(const std::string&, std::string_view);

    // returns true if all folders verified, error and new is empty
    const bool allEmpty();

    // returns a boolean array of the states of the 3 folders, true is empty, false is full
    // The structure is [verified, error, new]
    const std::array<bool, 3> emptyFolders();
};



