#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main(int argc, char *argv[])
{
    // ===================================
    // READING PARAMETERS
    // ===================================

    // if the user doesn't provide the file path
    if (argc < 2)
    {
        std::cout << "Error: file path not specified" << std::endl;
        return argc;
    }
    // if the user provides more than one parameter
    if (argc > 2)
    {
        std::cout << "Error: too many parameters, only the file path has to be specified" << std::endl;
        return argc;
    }
    // now the number of parameters has to be 2
    // argv[1] is the file path

    // ===================================
    // OPENING FILE
    // ===================================

    std::ifstream file_stream{argv[1]};
    // check if the file path leads to a correct file
    if (!file_stream)
    {
        std::cout << "File Error: cannot locate " << argv[1] << std::endl;
        return -1;
    }
    // copy the contents inside a string
    std::string content{""};
    std::string buffer;
    while (std::getline(file_stream, buffer))
        content.append(buffer);
    // close the file stream
    file_stream.close();
    std::cout << "file content is: " << content << std::endl;
}
