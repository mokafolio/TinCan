#ifndef TIN_CAN_SERIAL_PORT_INFO_HPP
#define TIN_CAN_SERIAL_PORT_INFO_HPP

#include <Stick/StringConversion.hpp>
#include <Stick/DynamicArray.hpp>

namespace tinCan
{
    class STICK_API SerialPortInfo
    {
    public:

        SerialPortInfo(const stick::String & _path, const stick::String & _uniqueID);

        stick::String name() const;

        const stick::String & path() const;

        const stick::String & portID() const;

    private:

        stick::String m_path;
        stick::String m_portID;
    };


    using SerialPortInfoArray = stick::DynamicArray<SerialPortInfo>;
}

#endif //TIN_CAN_SERIAL_PORT_INFO_HPP
