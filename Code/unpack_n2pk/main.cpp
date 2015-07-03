#include <vanhelsing/engine/io/n2pk/N2pkFile.h>
#include <common/Log.h>
#include <nowide/args.hpp>
#include <nowide/convert.hpp>
#include <nowide/iostream.hpp>
#include <nowide/fstream.hpp>
#include <string>
#include <iterator>
#include <boost/filesystem.hpp>

int main(int argc, char* argv[])
{
    nowide::args args(argc, argv);
    --argc;
    argv = &argv[1];

    if (argc < 1) {
        nowide::cout << "Usage: <path to n2pk file> [destination directory] [verbose]" << std::endl;
        nowide::cout << "Destination directory: Use this when extracting files; omit to view contents." << std::endl;
        nowide::cout << "Verbose: 1 = Extra output, 0 = No extra output." << std::endl;
        return 1;
    }

    std::string filePath(argv[0]);
    std::string targetDir(argc >= 2 ? argv[1] : std::string());
    bool shouldUnpack = !targetDir.empty();
    bool shouldBeVerbose = (argc >= 3 ? (std::string(argv[2]) == "1") : false);

    using namespace common;
    using namespace vanhelsing::engine;

    if (shouldBeVerbose) {
        Log::SetLogLevelFilter(LogLevel::Trace);
    }

    auto& logger = Log();

    try {
        io::n2pk::N2pkFile package(filePath);
        auto fileCount = package.GetFileCount();
        logger << "Files (" << fileCount << "):" << std::endl;
        logger << Log::indent;
        for (unsigned int i = 0; i < fileCount; ++i) {
            auto& fileEntry = package.GetFileEntry(i);

            logger << fileEntry.GetName() << std::endl;

            if (shouldUnpack) {
                auto packedFile = package.GetFile(i);
                boost::filesystem::path unpackedFilePath = targetDir;
                unpackedFilePath /= fileEntry.GetName();

                // See if we actually need to unpack this file
                if (boost::filesystem::exists(unpackedFilePath)) {
                    auto fileSize = boost::filesystem::file_size(unpackedFilePath);
                    if (fileSize == fileEntry.GetSize()) {
                        continue;
                    }
                }

                if (!boost::filesystem::exists(targetDir)) {
                    boost::filesystem::create_directories(targetDir);
                }

                nowide::ofstream unpackedFile(unpackedFilePath.string().c_str(), std::ios::binary);
                if (!unpackedFile.is_open()) {
                    std::string ex("Couldn't open file: ");
                    ex += unpackedFilePath.string();
                    throw std::runtime_error(ex);
                }

                // For some reason std::copy(), skips whitespace?
                //std::copy(std::istream_iterator<char>(*packedFile), std::istream_iterator<char>(), std::ostream_iterator<char>(unpackedFile));
                char readBuf[32 * 1024];
                char writeBuf[32 * 1024];
                // Set larger buffer for faster writes
                unpackedFile.rdbuf()->pubsetbuf(writeBuf, sizeof(writeBuf));
                do {
                    packedFile->read(readBuf, sizeof(readBuf));
                    if (packedFile->gcount() > 0) {
                        unpackedFile.write(readBuf, packedFile->gcount());
                    }
                } while (!packedFile->eof() && packedFile->good());
            }
        }

        logger << Log::outdent;
    }
    catch (std::runtime_error& ex) {
        Log(LogLevel::Fatal) << "Exception: " << ex.what() << std::endl;
#ifdef _DEBUG
        throw;
#endif
    }

    return 0;
}
