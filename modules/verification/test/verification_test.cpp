
#include "../verification.hpp"

int main(int argc, char *argv[]){

    std::filesystem::path currPath = std::filesystem::current_path();
    testFunction(currPath, "testing.txt", "Content123456");

    return 0;


}