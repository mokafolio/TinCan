#ifndef TIN_CAN_POSIX_POSIX_SERIAL_PORT_IMPL_HPP
#define TIN_CAN_POSIX_POSIX_SERIAL_PORT_IMPL_HPP

#include <TinCan/SerialPortInfo.hpp>
#include <Stick/FileUtilities.hpp>
#include <Stick/Result.hpp>

namespace tinCan
{
    namespace detail
    {
        class STICK_LOCAL SerialPortImpl
        {
        public:

            SerialPortImpl();
            
            ~SerialPortImpl();


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


            stick::Int32 m_nativePort; //unix file descriptor
        };
    }
}

#endif //TIN_CAN_POSIX_POSIX_SERIAL_PORT_IMPL_HPP
