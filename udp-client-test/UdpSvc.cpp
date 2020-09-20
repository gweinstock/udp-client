#include "../UdbSvc.h"

extern boost::array<char, 4096> recv_buf;
extern udp::socket* sock;
extern std::map<std::string, void(*)(rapidjson::Document&)> callbacks;

void UdpSvc::on(std::string msgType, void(*fn)(rapidjson::Document&)) {
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
		std::string msg = "{\"msgType\":\"connect\",\"seq\":0}";
		strcpy(send_buf.data(), msg.c_str());
		sock->async_send(boost::asio::buffer(send_buf), &send_handler);

		sock->async_receive(
			boost::asio::buffer(recv_buf), &recv_handler);

		std::thread t([=]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			if (!connected) {
				// resend connect request:
				connect(host, port);
			}
		});
		t.detach();
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}