#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/StorageGameSaveReader.h>

#include <nowide/args.hpp>
#include <nowide/convert.hpp>
#include <nowide/fstream.hpp>
#include <nowide/iostream.hpp>
#include <string>

int main(int argc, char* argv[])
{
    nowide::args args(argc, argv);
    --argc;
    argv = &argv[1];

    if (argc < 1) {
        return 1;
    }

    try {
        using namespace vanhelsing::engine;
        GameSave gameSave;
        std::string filePath(argv[0]);
        nowide::ifstream inStream(filePath.c_str(), std::ios::binary);
        if (!inStream.is_open()) {
            nowide::cout << "Couldn't open file: " << filePath << std::endl;
            return 1;
        }

        io::StorageGameSaveReader reader(gameSave, inStream);
        nowide::cout << "Seems like the file was read properly!" << std::endl;
    }
    catch (std::runtime_error& ex) {
        nowide::cout << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
