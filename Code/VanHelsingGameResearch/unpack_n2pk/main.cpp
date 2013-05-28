#include <vanhelsing/engine/io/n2pk/N2pkFile.h>
#include <vanhelsing/engine/log.h>
#include <nowide/args.hpp>
#include <nowide/convert.hpp>
#include <nowide/iostream.hpp>
#include <nowide/fstream.hpp>
#include <string>
#include <iterator>

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

    std::string filePath(argv[0]);
    std::string targetDir(argc > 1 ? argv[1] : std::string());
    bool shouldUnpack = !targetDir.empty();

    using vanhelsing::engine::Log;

    try {
        using namespace vanhelsing::engine;
        io::n2pk::N2pkFile package(filePath);
        auto fileCount = package.GetFileCount();
        Log() << "Files (" << fileCount << "):" << std::endl;
        Log::Indent();
        for (unsigned int i = 0; i < fileCount; ++i) {
            auto& fileEntry = package.GetFileEntry(i);

            Log() << fileEntry.GetName() << std::endl;

            if (shouldUnpack) {
                auto packedFile = package.GetFile(i);
                auto unpackedFilePath = targetDir;
                unpackedFilePath += "/";
                unpackedFilePath += fileEntry.GetName();

                nowide::ofstream unpackedFile(unpackedFilePath.c_str(), std::ios::binary);
                if (!unpackedFile.is_open()) {
                    std::string ex("Couldn't open file: ");
                    ex += unpackedFilePath;
                    throw std::runtime_error(ex);
                }

                // For some reason std::copy(), skips whitespace?
                //std::copy(std::istream_iterator<char>(*packedFile), std::istream_iterator<char>(), std::ostream_iterator<char>(unpackedFile));
                char buf[8192];
                do {
                    packedFile->read(buf, sizeof(buf));
                    if (packedFile->gcount() > 0) {
                        unpackedFile.write(buf, packedFile->gcount());
                    }
                } while (!packedFile->eof() && packedFile->good());
            }
        }

        Log::Outdent();
    }
    catch (std::runtime_error& ex) {
         Log() << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
