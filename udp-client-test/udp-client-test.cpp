#include <iostream>
#include "../UdbSvc.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

void onConnect(rapidjson::Document& doc) {
	rapidjson::StringBuffer buf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
	doc.Accept(writer);
	std::cout << buf.GetString() << std::endl;
}

int main(int argc, char** argv) {
	if (argc != 3)
	{
		std::cerr << "Usage: client <host> <port>" << std::endl; // host: '192.168.68.114'
		return 1;
	}
	UdpSvc udpSvc;
	udpSvc.on("connect", &onConnect);
	udpSvc.connect(argv[1], argv[2]);
	udpSvc.io_service.run();
	return 0;
}