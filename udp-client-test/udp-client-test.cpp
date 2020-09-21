#include <iostream>
#include "../UdbSvc.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

UdpSvc udpSvc;

void onConnect(rapidjson::Document& doc) {
	udpSvc.id = doc["id"].GetInt();
	udpSvc.connected = true;
	std::cout << "id: " << udpSvc.id << std::endl;
	rapidjson::StringBuffer buf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
	doc.Accept(writer);
	std::cout << buf.GetString() << std::endl;
}

void onPing(rapidjson::Document& doc) {
	int seq = doc["seq"].GetInt();
	std::cout << "ping: " << seq << std::endl;
	if (seq > 10 && udpSvc.connected) {
		std::cout << "send disconnect\n";
		char buf[256];
		sprintf(buf, "{\"msgType\":\"disconnect\",\"id\":%d}", udpSvc.id);
		rapidjson::Document doc;
		doc["msgType"] = "disconnect";
		udpSvc.send(doc);
		udpSvc.connected = false;
	}
}

int main(int argc, char** argv) {
	if (argc != 3)
	{
		std::cerr << "Usage: client <host> <port>" << std::endl; // host: '192.168.68.114'
		return 1;
	}
	udpSvc.on("connect", &onConnect);
	udpSvc.on("ping", &onPing);
	udpSvc.connect(argv[1], argv[2]);
	udpSvc.io_service.run();
	return 0;
}