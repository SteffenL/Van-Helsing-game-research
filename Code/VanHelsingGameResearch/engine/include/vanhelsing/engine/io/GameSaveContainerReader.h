#ifndef __vanhelsing_engine_io_GameSaveContainerReader__
#define __vanhelsing_engine_io_GameSaveContainerReader__

#include <iostream>

namespace vanhelsing { namespace engine { namespace io {

class GameSaveContainerReader
{
public:
    GameSaveContainerReader(std::istream& inStream);
    virtual ~GameSaveContainerReader();
    std::istream& getStream() const;

protected:
    unsigned int m_unknown1;

private:
    std::istream& m_inStream;
};

}}} // namespace
#endif // guard
