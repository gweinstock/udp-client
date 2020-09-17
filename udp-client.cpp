/* simple udp client using boost asio
   gweinstock 9/17/2020
 */

#include <iostream>
#define BOOST_USE_WINDOWS_H 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: client <host>" << std::endl; // host: '192.168.68.114'
			return 1;
		}

		boost::asio::io_service io_service;

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), argv[1], "41234"); // port 41234
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		udp::socket socket(io_service);
		socket.open(udp::v4());
		socket.connect(receiver_endpoint);

		boost::array<char, 3> send_buf = { 'h', 'i', 0 };
		socket.send(boost::asio::buffer(send_buf));

		boost::array<char, 128> recv_buf;
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(
			boost::asio::buffer(recv_buf), sender_endpoint);

		std::cout.write(recv_buf.data(), len);
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}