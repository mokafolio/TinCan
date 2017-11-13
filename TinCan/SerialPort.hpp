#ifndef TIN_CAN_SERIAL_PORT_HPP
#define TIN_CAN_SERIAL_PORT_HPP

#include <Stick/Result.hpp>
#include <Stick/UniquePtr.hpp>
#include <Stick/String.hpp>
#include <Stick/DynamicArray.hpp>
#include <Stick/FileUtilities.hpp>
#include <TinCan/SerialPortInfo.hpp>

namespace tinCan
{
    namespace detail
    {
        class SerialPortImpl;
        using SerialPortImplUniquePtr = stick::UniquePtr<SerialPortImpl>;
    }

    class STICK_API SerialPort
    {
    public:

        SerialPort();

        SerialPort(const SerialPortInfo & _info);

        ~SerialPort();


        stick::Error open(const SerialPortInfo & _info);

        void close();

        stick::Result<stick::Size> write(const stick::ByteArray & _bytes, stick::Size _offset = 0);

        stick::Result<stick::Size> write(const void * _bytes, stick::Size _size, stick::Size _offset = 0);

        stick::Result<stick::Size> read(stick::ByteArray & _buffer);

        stick::Result<stick::Size> read(stick::ByteArray & _buffer, stick::Size _offset);

        stick::Result<char> readByte();

        stick::Error setBlocking(bool _b);

        bool isOpen() const;

        bool isBlocking() const;

        stick::Size available() const;


        static SerialPortInfoArray ports();

        static SerialPortInfo findPort(const stick::String & _port);


    private:

        detail::SerialPortImplUniquePtr m_pimpl;
    };
}

#endif //TIN_CAN_SERIAL_PORT_HPP
