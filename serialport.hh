#ifndef SERIALPORT_HH
#define SERIALPORT_HH

#include <boost/asio.hpp>
#include <string>
#include <vector>

// default asio options:
// baud rate: 0
// flow control: none
// parity: none
// stop bits: one
// character size: 8
class SerialPort
{
public:
    SerialPort(const std::string& device, unsigned baudRate, std::size_t bufSize);
    std::string readData(unsigned timeoutMs);
    void writeData(char byte);
    void writeData(const std::string& data);

private:
    boost::asio::io_service io;
    boost::asio::deadline_timer timer;
    boost::asio::serial_port port;
    std::vector<char> buf;
};

inline void SerialPort::writeData(char byte)
{
    port.write_some(boost::asio::buffer(&byte, 1));
}

inline void SerialPort::writeData(const std::string& data)
{
    port.write_some(boost::asio::buffer(data));
}

#endif // SERIALPORT_HH
