/*
Taken from http://stackoverflow.com/a/7657367
Written by BHS

May 28 2013: Fixed bugs in underflow() and uflow().
- Steffen André Langnes

Usage:
using namespace std;

void somefunc(ifstream &bigifs)
{
    substreambuf sbuf(bigifs.rdbuf(),100,100);
    //new istream with the substreambuf as its streambuf
    istream isub(&sbuf);

    //use isub normally
}
*/

#include <streambuf>

class substreambuf : public std::streambuf
{
public:

    substreambuf(std::streambuf *sbuf, std::streamoff pos, std::streamsize len)
        : m_sbuf(sbuf), m_pos(pos), m_len(len), m_read(0)
    {
        m_sbuf->pubseekpos(pos);
        setbuf(NULL,0);
    }

protected:

    int underflow()
    {
        if (m_read >= m_len)
            return traits_type::eof();
        return m_sbuf->sgetc();
    }
    
    int uflow()
    {
        if (m_read >= m_len)
            return traits_type::eof();
        m_read += 1;
        return m_sbuf->sbumpc();
    }

    std::streampos seekoff ( std::streamoff off, std::ios_base::seekdir way,
            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out )
    {
        if (way == std::ios_base::beg)
            off += m_pos;
        else if (way == std::ios_base::cur)
            off += m_pos+m_read;
        else if (way == std::ios_base::end)
            off += m_pos+m_len;

        return m_sbuf->pubseekpos(off,which)-m_pos;
    }

    std::streampos seekpos ( std::streampos sp,
            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
    {
        sp += m_pos;

        if (sp > m_pos+m_len)
            return -1;

        return m_sbuf->pubseekpos(sp,which)-m_pos;
    }

private:
    std::streambuf *m_sbuf;
    std::streampos m_pos;
    std::streamsize m_len;
    std::streampos m_read;
};
