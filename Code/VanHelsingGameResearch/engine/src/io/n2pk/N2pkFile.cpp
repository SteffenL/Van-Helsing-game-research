#include <vanhelsing/engine/io/n2pk/N2pkFile.h>
#include <vanhelsing/engine/io/StreamHelper.h>
#include <nowide/fstream.hpp>
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

std::unique_ptr<std::streambuf> N2pkFile::GetFile(unsigned int index) const
{
    auto& fileEntry = GetFileEntry(index);
    m_stream->seekg(fileEntry.GetOffset());
    return std::unique_ptr<std::streambuf>(
        new substreambuf(m_stream->rdbuf(), fileEntry.GetOffset(), fileEntry.GetSize())
    );
}

std::unique_ptr<std::streambuf> N2pkFile::GetFile(const std::string& path) const
{
    auto& fileEntry = GetFileEntry(path);
    m_stream->seekg(fileEntry.GetOffset());
    return std::unique_ptr<std::streambuf>(
        new substreambuf(m_stream->rdbuf(), fileEntry.GetOffset(), fileEntry.GetSize())
    );
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
        throw std::runtime_error("File not found");
    }

    return *it;
}

N2pkFile::N2pkFile(const std::string& filePath) : m_entryTableOffset(0), m_impl(new Impl)
{
    auto fileStream = new nowide::ifstream(filePath.c_str(), std::ios::binary);
    if (!fileStream->is_open()) {
        throw std::runtime_error("Couldn't open file");
    }

    m_stream.reset(fileStream);
    StreamHelper stream(*m_stream);
    if ((stream.Read<int>() != 15) ||
        (nowide::narrow(stream.ReadWString(15)) != "Neocore Package")) {
        throw std::runtime_error("Invalid signature");
    }

    stream.Seek(2); // Null-terminator?
    m_entryTableOffset = stream.Read<unsigned int>();
    stream.Read<int>();
    m_entryTableOffset += stream.Tell();
    m_dataffset = stream.Tell();

    readFileTable(stream);
}

N2pkFile::~N2pkFile() {}

void N2pkFile::readFileTable(StreamHelper& stream)
{
    stream.Seek(m_entryTableOffset, std::ios::beg);
    m_impl->Files.clear();
    auto fileCount = stream.Read<unsigned int>();
    for (unsigned int i = 0; i < fileCount; ++i) {
        stream.Read<unsigned int>();
        auto nameLength = stream.Read<unsigned int>();
        auto& name = nowide::narrow(stream.ReadWString(nameLength));
        stream.Seek(2); // Null-terminator?
        auto dataOffset = stream.Read<unsigned int>();
        stream.Read<int>();
        auto size = stream.Read<unsigned int>();
        stream.Read<int>();

        FileEntry fileEntry(m_dataffset + dataOffset, size, name);
        m_impl->Files.push_back(fileEntry);
    }
}
}}}} // namespace
