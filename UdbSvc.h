#pragma once

#include <iostream>
#define BOOST_USE_WINDOWS_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <map>
#include "rapidjson/document.h"

void send_handler(const boost::system::error_code&, std::size_t);
void recv_handler(const boost::system::error_code&, std::size_t);

using boost::asio::ip::udp;

class UdpSvc
{
public:
	UdpSvc();
	~UdpSvc();
	void connect(std::string, std::string);
	boost::asio::io_service io_service;
	void send(std::string);
	void on(std::string, void(*)(rapidjson::Document&));

private:

};