#include <TinCan/SerialPortInfo.hpp>
#include <Stick/Path.hpp>

namespace tinCan
{
	using namespace stick;
	
    SerialPortInfo::SerialPortInfo(const String & _path, const String & _uniqueID) :
        m_path(_path),
        m_portID(_uniqueID)
    {

    }

    String SerialPortInfo::name() const
    {
        return path::fileName(m_path);
    }

    const String & SerialPortInfo::path() const
    {
        return m_path;
    }

    const String & SerialPortInfo::portID() const
    {
        return m_portID;
    }
}
