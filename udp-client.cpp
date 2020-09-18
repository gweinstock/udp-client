/* simple udp client using boost asio
   gweinstock 9/17/2020
 */

#include <iostream>
#define BOOST_USE_WINDOWS_H 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>
#include <chrono>

using boost::asio::ip::udp;
boost::asio::io_service io_service;
udp::socket *sock = nullptr;
int seq = 0;

boost::array<char, 4096> recv_buf;

void send_handler(const boost::system::error_code& err, std::size_t bytes_sent) {
	//std::cout << "sent " << bytes_sent << " bytes.\"";
}

void recv_handler(const boost::system::error_code& err, std::size_t bytes_recv) {
	std::cout << "received " << bytes_recv << " bytes: ";
	std::string s(recv_buf.begin(), recv_buf.end());
	s = s.substr(0, bytes_recv);
	for (int i = 0; i < bytes_recv; i++) {
		//printf("%02hhx", recv_buf[i]);
	}
	s.erase(std::find(s.begin(), s.end(), '\0'), s.end());
	//int pos = s.find_last_not_of(" \t\n\0");
	//std::cout << "pos: " << pos << std::endl;
	boost::trim_right(s);
	//std::cout.write(recv_buf.data(), bytes_recv);
	std::cout << "\"" << s << "\"\n";
	sock->async_receive(boost::asio::buffer(recv_buf), &recv_handler);
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: client <host> <port>" << std::endl; // host: '192.168.68.114'
			return 1;
		}

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), argv[1], argv[2]); // port 41234
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		sock = new udp::socket(io_service);
		sock->open(udp::v4());
		sock->connect(receiver_endpoint);

		// prime the pump:
		boost::array<char, 128> send_buf = { '0', 0 };
		
		sock->async_send(boost::asio::buffer(send_buf), &send_handler);

		sock->async_receive(
			boost::asio::buffer(recv_buf), &recv_handler);

		while (seq < 10) {
			char ch = (char)(seq+48);
			send_buf[0] = ch;
			//printf_s("ch: '%c'\n", ch);
			sock->async_send(boost::asio::buffer(send_buf), &send_handler);
			//std::this_thread::sleep_for(std::chrono::seconds(1));
			seq++;
		}

		io_service.run();

		sock->close();
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}