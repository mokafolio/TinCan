#include <TinCan/Posix/PosixSerialPortImpl.hpp>

#include <Stick/FileUtilities.hpp>
#include <Stick/FileSystem.hpp>
#include <Stick/Path.hpp>

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>

namespace tinCan
{
    namespace detail
    {
        using namespace stick;

        SerialPortImpl::SerialPortImpl() :
            m_nativePort(-1)
        {
        }

        SerialPortImpl::~SerialPortImpl()
        {
        }

        Error SerialPortImpl::open(const SerialPortInfo & _info)
        {
            Error err;
            m_nativePort = ::open(_info.path().cString(), O_RDWR | O_NOCTTY | O_NONBLOCK);

            if (m_nativePort == -1)
                err = Error(ec::SystemErrorCode(errno), "open failed.", STICK_FILE, STICK_LINE);

            if (!err)
            {
                Int32 res = ::fcntl(m_nativePort, F_SETFL, 0);
                if (res == -1)
                    err = Error(ec::SystemErrorCode(errno), "fcntl failed.", STICK_FILE, STICK_LINE);

                if (!err)
                {
                    struct termios options;
                    struct termios oldoptions;
                    res = tcgetattr(m_nativePort, &oldoptions);
                    if (res == -1)
                        err = Error(ec::SystemErrorCode(errno), "tcgetattr failed.", STICK_FILE, STICK_LINE);

                    if (!err)
                    {
                        //options = oldoptions;
                        memset(&options, 0, sizeof(options));
                        res = cfsetispeed(&options, B9600);
                        if (res == -1)
                            err = Error(ec::SystemErrorCode(errno), "cfsetispeed failed.", STICK_FILE, STICK_LINE);

                        if (!err)
                        {
                            res = cfsetospeed(&options, B9600);
                            if (res == -1)
                                err = Error(ec::SystemErrorCode(errno), "cfsetospeed failed.", STICK_FILE, STICK_LINE);

                            if (!err)
                            {
                                //no parity
                                options.c_cflag |= (CLOCAL | CREAD);
                                options.c_cflag &= ~PARENB;
                                options.c_cflag &= ~CSTOPB;
                                options.c_cflag &= ~CSIZE;
                                options.c_cflag |= CS8;

                                res = tcsetattr(m_nativePort, TCSANOW, &options);
                                if (res == -1)
                                    err = Error(ec::SystemErrorCode(errno), "tcsetattr failed.", STICK_FILE, STICK_LINE);
                            }
                        }
                    }
                }
            }

            //setBlocking(true);
            if (err)
                close();

            return err;
        }

        void SerialPortImpl::close()
        {
            ::close(m_nativePort);
            m_nativePort = -1;
        }

        Result<Size> SerialPortImpl::write(const ByteArray & _bytes, size_t _offset)
        {
            return write(_bytes.ptr(), _bytes.byteCount(), _offset);
        }

        Result<Size> SerialPortImpl::write(const void * _bytes, size_t _size, size_t _offset)
        {
            Int32 result = ::write(m_nativePort, _bytes, _size);
            if (result == -1)
                return Error(ec::SystemErrorCode(errno), "write failed.", STICK_FILE, STICK_LINE);

            return (Size)result;
        }

        Result<Size> SerialPortImpl::read(ByteArray & _buffer)
        {
            return read(_buffer, 0);
        }

        Result<Size> SerialPortImpl::read(ByteArray & _buffer, size_t _offset)
        {
            Int32 result = ::read(m_nativePort, reinterpret_cast<void *>(_buffer.ptr() + _offset), _buffer.byteCount() - _offset);
            if (result == -1)
                return Error(ec::SystemErrorCode(errno), "read failed.", STICK_FILE, STICK_LINE);

            return (Size)result;
        }

        Result<char> SerialPortImpl::readByte()
        {
            ByteArray tmp(1);
            auto res = read(tmp);
            if (res)
                return tmp.first();
            else
                return res.error();
        }

        Error SerialPortImpl::setBlocking(bool _b)
        {
            Int32 flags = fcntl(m_nativePort, F_GETFL, 0);

            //@TODO: Read errno
            /*if(flags == -1)
             return -1;*/

            if (!_b)
                fcntl(m_nativePort, F_SETFL, flags | O_NONBLOCK);
            else
                fcntl(m_nativePort, F_SETFL, flags & ~O_NONBLOCK);

            return Error();
        }

        bool SerialPortImpl::isBlocking() const
        {
            int flags = fcntl(m_nativePort, F_GETFL, 0);

            if (flags == -1)
                return false;

            int ret = (flags & O_NONBLOCK) == 0;

            return ret;
        }

        bool SerialPortImpl::isOpen() const
        {
            return m_nativePort != -1 ? true : false;
        }

        Size SerialPortImpl::available() const
        {
            UInt32 bytes;

            ioctl(m_nativePort, FIONREAD, &bytes);
            return (size_t)bytes;
        }

        SerialPortInfoArray SerialPortImpl::ports()
        {
            SerialPortInfoArray ret;

            DynamicArray<String> prefixMatch;
            prefixMatch.append("cu.");
            prefixMatch.append("tty.");

            fs::DirectoryIterator iter("/dev");

            Size id = 0;
            while (iter != fs::DirectoryIterator::End)
            {
                String base = path::fileName(iter->path());
                for (size_t i = 0; i < prefixMatch.count(); ++i)
                {
                    //if (base.sub(0, prefixMatch[i].length()) == prefixMatch[i])
                    {
                        ret.append(SerialPortInfo(iter->path(), toString((Int64)id)));
                    }
                }

                ++iter;
                ++id;
            }
            return ret;
        }

        SerialPortInfo SerialPortImpl::findPort(const String & _port)
        {
            SerialPortInfoArray ps = ports();
            auto it = ps.begin();
            for (; it != ps.end(); ++it)
            {
                if ((*it).path() == _port)
                {
                    return *it;
                }
            }

            return SerialPortInfo("", "");
        }
    }
}
