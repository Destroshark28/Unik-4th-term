#include <iostream>
#include <dlfcn.h>
#include <string>
#include <list>

using namespace std;

using ConcatFunction = void (*)(list<string> sourceFiles, string outputFileName);

static const char *ASYNC_IO_LIB_NAME = "libAsyncIO.so";
static const char *ASYNC_IO_FUNCTION_NAME = "PerformConcat";
static const char *OUTPUT_FILE_NAME = "output.txt";

int main() {
    void *soHandle = nullptr;
    ConcatFunction concatFunction = nullptr;
    list<string> fileNames;

    soHandle = dlopen(ASYNC_IO_LIB_NAME, RTLD_NOW);
    if (soHandle == nullptr) {
        cout << "Error while opening so lib" << dlerror() << endl;
        exit(EXIT_FAILURE);
    }

    concatFunction = (ConcatFunction) dlsym(soHandle, ASYNC_IO_FUNCTION_NAME);
    if (concatFunction == nullptr) {
        cout << "Error while calling function" << endl;
        exit(EXIT_FAILURE);
    }

    fileNames.emplace_back("read_files/first.txt");
    fileNames.emplace_back("read_files/second.txt");
    fileNames.emplace_back("read_files/third.txt");
    fileNames.emplace_back("read_files/fourth.txt");
    fileNames.emplace_back("read_files/fifth.txt");

    cout << "Start processing..." << endl;
    // concatFunction(fileNames, OUTPUT_FILE_NAME);
    cout << "Finish" << endl;

    dlclose(soHandle);
    exit(EXIT_SUCCESS);
}