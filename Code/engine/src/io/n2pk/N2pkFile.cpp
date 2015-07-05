#include <vanhelsing/engine/io/n2pk/N2pkFile.h>
#include <vanhelsing/engine/io/StreamHelperReader.h>
#include <vanhelsing/engine/exceptions/VanHelsingEngineError.h>

#include <nowide/fstream.hpp>
#include <iomanip>
#include <misc/substreambuf.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

namespace bmi = boost::multi_index;

namespace vanhelsing { namespace engine { namespace io { namespace n2pk {

class N2pkFile::Impl
{
public:
    struct FileTableMap
    {
        typedef bmi::multi_index_container<
            FileEntry,
            bmi::indexed_by<
                bmi::random_access<>,
                bmi::ordered_unique<bmi::member<FileEntry, std::string, &FileEntry::Name>>
            >
        > type;
    };

    FileTableMap::type Files;
};

FileEntry::FileEntry(std::streamoff offset, std::streamsize size, const std::string& name)
    : Offset(offset), Size(size), Name(name) {}

std::streamoff FileEntry::GetOffset() const
{
    return Offset;
}

std::streamsize FileEntry::GetSize() const
{
    return Size;
}

std::string FileEntry::GetName() const
{
    return Name;
}

unsigned int N2pkFile::GetFileCount() const
{
    return m_impl->Files.size();
}

N2pkIStream::N2pkIStream(std::streambuf* buf) : m_buf(buf), std::istream(buf) {}

N2pkIStream::~N2pkIStream() {}

std::unique_ptr<N2pkIStream> N2pkFile::GetFile(unsigned int index) const
{
    auto& fileEntry = GetFileEntry(index);
    auto buf = new substreambuf(m_stream->rdbuf(), fileEntry.GetOffset(), fileEntry.GetSize());
    return std::make_unique<N2pkIStream>(buf);
}

std::unique_ptr<N2pkIStream> N2pkFile::GetFile(const std::string& path) const
{
    auto& fileEntry = GetFileEntry(path);
    auto buf = new substreambuf(m_stream->rdbuf(), fileEntry.GetOffset(), fileEntry.GetSize());
    return std::make_unique<N2pkIStream>(buf);
}

vanhelsing::engine::io::n2pk::FileEntry N2pkFile::GetFileEntry(unsigned int index) const
{
    return m_impl->Files.at(index);
}

vanhelsing::engine::io::n2pk::FileEntry N2pkFile::GetFileEntry(const std::string& path) const
{
    auto& view = m_impl->Files.get<1>();
    auto it = view.find(path);
    if (it == view.end()) {
        throw VanHelsingEngineError("File not found: " + path);
    }

    return *it;
}

N2pkFile::N2pkFile(const std::string& filePath) : m_entryTableOffset(0), m_impl(std::make_unique<Impl>()), m_logger(common::LogLevel::Trace)
{
    m_logger << "Opening Neocore Package: " << filePath << std::endl;

    auto fileStream = new nowide::ifstream(filePath.c_str(), std::ios::binary);
    if (!fileStream->is_open()) {
        throw VanHelsingEngineError("Couldn't open file: " + filePath);
    }

    fileStream->rdbuf()->pubsetbuf(m_buffer, sizeof(m_buffer));

    m_stream.reset(fileStream);
    StreamHelperReader stream(*m_stream);
    if ((stream.Read<int>() != 15) ||
        (nowide::narrow(stream.ReadWString(15)) != "Neocore Package")) {
        throw VanHelsingEngineError("Invalid signature");
    }

    stream.SeekI(2); // Null-terminator?
    m_entryTableOffset = stream.Read<unsigned int>();
    stream.Read<int>();
    m_entryTableOffset += m_stream->tellg();
    m_dataffset = m_stream->tellg();

    readFileTable(stream);
}

N2pkFile::~N2pkFile() {}

void N2pkFile::readFileTable(StreamHelperReader& stream)
{
    m_logger << "Reading file table..." << std::endl;
    stream.SeekI(m_entryTableOffset, std::ios::beg);
    m_impl->Files.clear();
    auto fileCount = stream.Read<unsigned int>();
    for (unsigned int i = 0; i < fileCount; ++i) {
        auto v1 = stream.Read<unsigned int>();
        auto nameLength = stream.Read<unsigned int>();
        auto& name = nowide::narrow(stream.ReadWString(nameLength));
        auto v2 = stream.Read<unsigned short>(); // Null-terminator?
        auto dataOffset = stream.Read<unsigned int>();
        auto v3 = stream.Read<int>();
        auto size = stream.Read<unsigned int>();
        auto v4 = stream.Read<int>();

        m_logger << std::left << std::setw(40) << std::setfill(' ') << name << "Unknown: " << v1 << ", " << v2 << ", " << v3 << ", " << v4 << std::endl;

        FileEntry fileEntry(m_dataffset + dataOffset, size, name);
        m_impl->Files.push_back(fileEntry);
    }
}

}}}}
