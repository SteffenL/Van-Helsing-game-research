#include <vanhelsing/engine/io/n2pk/N2pkFile.h>
#include <vanhelsing/engine/log.h>
#include <nowide/args.hpp>
#include <nowide/convert.hpp>
#include <nowide/iostream.hpp>
#include <string>

int main(int argc, char* argv[])
{
    nowide::args args(argc, argv);
    --argc;
    argv = &argv[1];

    if (argc < 1) {
        nowide::cout << "Usage: <path to n2pk file> [destination directory]" << std::endl;
        nowide::cout << "Destination directory: Use this when extracting files; omit to view contents." << std::endl;
        return 1;
    }

    using vanhelsing::engine::Log;

    try {
        using namespace vanhelsing::engine;
        std::string filePath(argv[0]);
        io::n2pk::N2pkFile package(filePath);
        auto fileCount = package.GetFileCount();
        Log() << "Files (" << fileCount << "):" << std::endl;
        Log::Indent();
        for (unsigned int i = 0; i < fileCount; ++i) {
            auto& fileEntry = package.GetFileEntry(i);
            auto stream = package.GetFile(i);

            Log() << fileEntry.GetName() << std::endl;
        }

        Log::Outdent();
    }
    catch (std::runtime_error& ex) {
         Log() << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
