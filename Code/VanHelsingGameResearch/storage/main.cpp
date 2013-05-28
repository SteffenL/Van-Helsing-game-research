#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/log.h>

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

    using namespace vanhelsing::engine;
    try {
        GameSave gameSave;
        std::string filePath(argv[0]);
        nowide::ifstream inStream(filePath.c_str(), std::ios::binary);
        if (!inStream.is_open()) {
            Log() << "Couldn't open file: " << filePath << std::endl;
            return 1;
        }

        io::StorageGameSaveReader reader(gameSave, inStream);
        Log() << "Seems like the file was read properly!" << std::endl;
    }
    catch (std::runtime_error& ex) {
        Log() << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
