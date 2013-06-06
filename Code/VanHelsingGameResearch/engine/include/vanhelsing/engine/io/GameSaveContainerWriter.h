#ifndef vanhelsing_engine_io_GameSaveContainerWriter_
#define vanhelsing_engine_io_GameSaveContainerWriter_

#include <iostream>

namespace vanhelsing { namespace engine { namespace io {

class GameSaveContainerWriter
{
public:
    GameSaveContainerWriter(std::istream& inStream, std::ostream& outStream);
    virtual ~GameSaveContainerWriter();
    std::istream& getInStream() const;
    std::ostream& getOutStream();

protected:
    unsigned int m_unknown1;

private:
    std::istream& m_inStream;
    std::ostream& m_outStream;
};

}}} // namespace
#endif // guard
