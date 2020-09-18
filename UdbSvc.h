#pragma once

#include <iostream>
#define BOOST_USE_WINDOWS_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <map>

void send_handler(const boost::system::error_code&, std::size_t);
void recv_handler(const boost::system::error_code&, std::size_t);

using boost::asio::ip::udp;

extern boost::array<char, 4096> recv_buf;
extern udp::socket* sock = nullptr;

class UdpSvc
{
public:
	UdpSvc();
	~UdpSvc();
	void connect(std::string, std::string);
	boost::asio::io_service io_service;
	void send(std::string);
	void on(std::string, void(*)(std::string));

private:
	std::map<std::string, void(*)(std::string)> callbacks;

};

void UdpSvc::on(std::string msgType, void(*fn)(std::string)) {
	callbacks[msgType] = fn;
}

UdpSvc::UdpSvc()
{
}

UdpSvc::~UdpSvc()
{
	if (sock) {
		sock->close();
	}
}

void UdpSvc::send(std::string msg) {
	boost::array<char, 4096> send_buf;
	strcpy(send_buf.data(), msg.c_str());
	sock->async_send(boost::asio::buffer(send_buf), &send_handler);
}

void UdpSvc::connect(std::string host, std::string port) {
	try {
		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), host, port); // port 41234
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		sock = new udp::socket(io_service);
		sock->open(udp::v4());
		sock->connect(receiver_endpoint);

		boost::array<char, 4096> send_buf;
		std::string msg = "connect";
		strcpy(send_buf.data(), msg.c_str());
		sock->async_send(boost::asio::buffer(send_buf), &send_handler);

		sock->async_receive(
			boost::asio::buffer(recv_buf), &recv_handler);
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}