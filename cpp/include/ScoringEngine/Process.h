// This file use of Boost libraries, including boost/process, for process management.

#pragma once

#include <boost/process.hpp>
#include <boost/process/async.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

namespace bp = boost::process;

class Process
{
public:
    Process(const std::string& cmd, float timeout);
    void run(std::string& stdOut, const std::string& stdIn);

    bool is_timeout() const { return killed; }

private:
    void timeout_handler(boost::system::error_code ec);

    const std::string command;
    const int timeout_ms;
    bool killed = false;
    boost::asio::io_service ios;
    boost::process::group group;
    boost::asio::deadline_timer deadline_timer;
};

Process::Process(const std::string& cmd, float timeout)
    : command(cmd), timeout_ms(static_cast<int>(timeout * 1000)), deadline_timer(ios) {}

void Process::timeout_handler(boost::system::error_code ec)
{
    if (ec == boost::asio::error::operation_aborted)
        return;
    if (deadline_timer.expires_at() <= boost::asio::deadline_timer::traits_type::now())
    {
        group.terminate();
        killed = true;
        deadline_timer.expires_at(boost::posix_time::pos_infin);
    }
}

void Process::run(std::string& stdOut, const std::string& stdIn = "")
{
    std::future<std::string> dataOut;

    deadline_timer.expires_from_now(boost::posix_time::milliseconds(timeout_ms));
    deadline_timer.async_wait(boost::bind(&Process::timeout_handler, this, boost::asio::placeholders::error));

    bp::opstream pipe_stream;

    bp::child child(
        command,
        bp::std_in < pipe_stream,
        bp::std_out > dataOut,
        ios,
        group,
        bp::on_exit([this](int e, std::error_code ec) -> void
        {
            deadline_timer.cancel();
        })
    );

    pipe_stream << stdIn << std::endl;

    ios.run();
    stdOut = dataOut.get();
    child.wait();
}
