
#include "../verification.hpp"

int main(int argc, char *argv[]){

    std::filesystem::path currPath = std::filesystem::current_path();
    testFunction(currPath, "testing.txt", "Content123469");

    return 0;


}