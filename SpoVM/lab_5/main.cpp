#include <iostream>
#include <dlfcn.h>
#include <string>
#include <list>

using ConcatFunction = void (*)(std::list<std::string> sourceFiles, std::string outputFileName);

static const char *ASYNC_IO_LIB_NAME = "libAsyncIO.so";
static const char *ASYNC_IO_FUNCTION_NAME = "concatFiles";
static const char *OUTPUT_FILE_NAME = "files/output.txt";

int main() {
    void *soHandle = nullptr;
    ConcatFunction concatFunction = nullptr;
    std::list<std::string> fileNames;

    soHandle = dlopen(ASYNC_IO_LIB_NAME, RTLD_NOW);
    if (soHandle == nullptr) {
        std::cout << "Error while opening so lib" << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }

    concatFunction = (ConcatFunction) dlsym(soHandle, ASYNC_IO_FUNCTION_NAME);
    if (concatFunction == nullptr) {
        std::cout << "Error while calling function" << std::endl;
        exit(EXIT_FAILURE);
    }

    fileNames.emplace_back("files/first.txt");
    fileNames.emplace_back("files/second.txt");
    fileNames.emplace_back("files/third.txt");

    std::cout << "Start processing" << std::endl;
    concatFunction(fileNames, OUTPUT_FILE_NAME);
    std::cout << "Finish" << std::endl;

    dlclose(soHandle);
    exit(EXIT_SUCCESS);
}