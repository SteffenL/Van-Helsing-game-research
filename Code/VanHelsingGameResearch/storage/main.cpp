#include <vanhelsing/engine/StorageGameSave.h>
#include <vanhelsing/engine/io/StorageGameSaveReader.h>
#include <vanhelsing/engine/log.h>
#include <vanhelsing/engine/GameData.h>

#include <nowide/args.hpp>
#include <nowide/convert.hpp>
#include <nowide/fstream.hpp>
#include <nowide/iostream.hpp>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[])
{
    nowide::args args(argc, argv);
    --argc;
    argv = &argv[1];

    if (argc < 1) {
        nowide::cout << "Usage: <path to storage.sav> [verbose]" << std::endl;
        nowide::cout << "Verbose: 1 = Extra output, 0 = No extra output." << std::endl;
        return 1;
    }

    std::string filePath(argv[0]);
    bool shouldBeVerbose = (argc >= 2 ? (std::string(argv[1]) == "1") : false);

    using namespace vanhelsing::engine;

    if (shouldBeVerbose) {
        Log::SetLogLevelFilter(LogLevel::Trace);
    }

    try {
        // Temp solution for specifying the game dir
        auto gameDir_c = std::getenv("VH_GAME_DIR");
        if (!gameDir_c) {
            Log(LogLevel::Error) << "Environment variable must be set: VH_GAME_DIR" << std::endl;
            return 1;
        }

        std::string gameDir(gameDir_c);
        GameData::Get().Load(gameDir);

        GameSave gameSave;
        nowide::ifstream inStream(filePath.c_str(), std::ios::binary);
        if (!inStream.is_open()) {
            Log(LogLevel::Error) << "Couldn't open file: " << filePath << std::endl;
            return 1;
        }

        io::StorageGameSaveReader reader(gameSave, inStream);
        Log(LogLevel::Info) << "Seems like the file was read properly!" << std::endl;
    }
    catch (std::runtime_error& ex) {
        Log(LogLevel::Fatal) << "Exception: " << ex.what() << std::endl;
#ifdef _DEBUG
        throw;
#endif
    }

    return 0;
}
