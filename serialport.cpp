#include <serialport.hh>

SerialPort::SerialPort(const std::string& device, unsigned baudRate, std::size_t bufSize):
    timer(io),
    port(io),
    buf(bufSize)
{
    try
    {
        port.open(device);
        port.set_option(boost::asio::serial_port::baud_rate(baudRate));
    }
    catch(const std::exception&)
    {
        throw std::runtime_error("no device available: " + device);
    }
}

std::string SerialPort::readData(unsigned timeoutMs)
{
    std::string output;

    port.async_read_some(boost::asio::buffer(buf), [this, &output]
                         (const boost::system::error_code& error, std::size_t bytesTransferred)
    {
        if(error == boost::asio::error::operation_aborted)
            return;
        else if(error)
            throw std::runtime_error(error.message());

        timer.cancel();
        output.reserve(bytesTransferred);
        for(unsigned i = 0; i < bytesTransferred; ++i)
            output.push_back(buf[i]);
    });

    timer.expires_from_now(boost::posix_time::millisec(timeoutMs));
    timer.async_wait([this](const boost::system::error_code error)
    {
        if(error)
            return;
        port.cancel();
    });
    io.run();
    io.reset();
    return output;
}
