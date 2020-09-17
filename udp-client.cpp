/* simple udp client using boost asio
   gweinstock 9/17/2020
 */

#include <iostream>
#define BOOST_USE_WINDOWS_H 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <chrono>

using boost::asio::ip::udp;

udp::socket *sock = nullptr;
int seq = 0;

boost::array<char, 128> recv_buf;
boost::array<char, 128> send_buf = { '0', 0 };

void send_handler(const boost::system::error_code& err, std::size_t bytes_sent) {
	std::cout << "sent " << bytes_sent << " bytes: \"";
	std::cout.write(send_buf.data(), bytes_sent);
	std::cout << "\"\n";
	char ch = (char)seq;
	send_buf[0] = ch;
}

void recv_handler(const boost::system::error_code& err, std::size_t bytes_recv) {
	std::cout << "received " << bytes_recv << " bytes: \"";
	std::cout.write(recv_buf.data(), bytes_recv);
	std::cout << "\"\n";
	sock->async_receive(
		boost::asio::buffer(recv_buf), &recv_handler);
}

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

		sock = new udp::socket(io_service);
		sock->open(udp::v4());
		sock->connect(receiver_endpoint);

		// prime the pump:
		sock->async_send(boost::asio::buffer(send_buf), &send_handler);
		
		seq++;
		
		sock->async_receive(
			boost::asio::buffer(recv_buf), &recv_handler);

		while (seq < 10) {
			std::cout << "seq: " << seq << std::endl;

			std::this_thread::sleep_for(std::chrono::seconds(2));

			sock->async_send(boost::asio::buffer(send_buf), &send_handler);

			seq++;
		}

		sock->close();
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}