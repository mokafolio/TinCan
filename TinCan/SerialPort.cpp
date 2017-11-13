#include <TinCan/SerialPort.hpp>
#include <TinCan/Posix/PosixSerialPortImpl.hpp>

namespace tinCan
{
    using namespace stick;

    SerialPort::SerialPort() :
        m_pimpl(makeUnique<detail::SerialPortImpl>(defaultAllocator()))
    {

    }

    SerialPort::~SerialPort()
    {
        if (isOpen())
            close();
    }

    Error SerialPort::open(const SerialPortInfo & _info)
    {
    	return m_pimpl->open(_info);
    }

    void SerialPort::close()
    {
    	m_pimpl->close();
    }

    Result<Size> SerialPort::write(const ByteArray & _bytes, size_t _offset)
    {
        return m_pimpl->write(_bytes, _offset);
    }

    Result<Size> SerialPort::write(const void * _bytes, size_t _size, size_t _offset)
    {
		return m_pimpl->write(_bytes, _size, _offset);
    }

    Result<Size> SerialPort::read(ByteArray & _buffer)
    {
        return m_pimpl->read(_buffer);
    }

    Result<Size> SerialPort::read(ByteArray & _buffer, size_t _offset)
    {
    	return m_pimpl->read(_buffer, _offset);
    }

    Result<char> SerialPort::readByte()
    {
    	return m_pimpl->readByte();
    }

    Error SerialPort::setBlocking(bool _b)
    {
    	return m_pimpl->setBlocking(_b);
    }

    bool SerialPort::isBlocking() const
    {
    	return m_pimpl->isBlocking();
    }

    bool SerialPort::isOpen() const
    {
        return m_pimpl->isOpen();
    }

    Size SerialPort::available() const
    {
    	return m_pimpl->available();
    }

    SerialPortInfoArray SerialPort::ports()
    {
    	return detail::SerialPortImpl::ports();
    }

    SerialPortInfo SerialPort::findPort(const String & _port)
    {
    	return detail::SerialPortImpl::findPort(_port);
    }
}
