#include <vanhelsing/engine/io/StreamHelper.h>

namespace vanhelsing { namespace engine { namespace io {

StreamHelper::DummyStreamBuf::DummyStreamBuf() {}
int StreamHelper::DummyStreamBuf::sync() { return 0; }

}}}
