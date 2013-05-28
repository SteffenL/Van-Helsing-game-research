#ifndef __vanhelsing_engine_io_n2pk_N2pkFile__
#define __vanhelsing_engine_io_n2pk_N2pkFile__

#include <iostream>
#include <memory>
#include <streambuf>
#include <string>

namespace vanhelsing { namespace engine { namespace io { class StreamHelper; }}}

namespace vanhelsing { namespace engine { namespace io { namespace n2pk {

class FileEntry
{
public:
    FileEntry(std::streamoff offset, std::streamsize size, const std::string& name);
    std::streamoff GetOffset() const;
    std::streamsize GetSize() const;
    std::string GetName() const;

    std::streamoff Offset;
    std::streamsize Size;
    std::string Name;
};

class N2pkFile
{
public:
    N2pkFile(const std::string& filePath);
    virtual ~N2pkFile();
    unsigned int GetFileCount() const;
    std::unique_ptr<std::streambuf> GetFile(unsigned int index) const;
    std::unique_ptr<std::streambuf> GetFile(const std::string& path) const;
    FileEntry GetFileEntry(unsigned int index) const;
    FileEntry GetFileEntry(const std::string& path) const;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
    std::unique_ptr<std::istream> m_stream;
    std::streamoff m_dataffset;
    std::streamoff m_entryTableOffset;

    void readFileTable(StreamHelper& stream);
};

}}}} // namespace
#endif // guard
