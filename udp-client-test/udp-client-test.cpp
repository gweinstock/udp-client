#include <iostream>
#include "../UdbSvc.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

UdpSvc udpSvc;
const bool DISCONNECT=false;
std::string username;

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
	if (seq > 25 && udpSvc.connected && DISCONNECT) {
		std::cout << "send disconnect\n";
		char buf[256];
		sprintf(buf, "{\"msgType\":\"disconnect\",\"id\":%d}", udpSvc.id);
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Value msgType("disconnect");
		std::cout << "set msgType\n";
		doc.AddMember("msgType", msgType, doc.GetAllocator());
		std::cout << "udpSvc.send\n";
		udpSvc.send(doc);
		udpSvc.connected = false;
	}
	char buf[256];
	sprintf(buf, "{\"msgType\":\"hello\",\"name\":\"%s\"}", username.c_str());
	// {msgType:'hello',name:'<name>'}
	rapidjson::Document hellodoc;
	hellodoc.Parse(buf);
	udpSvc.send(hellodoc);
}

void onHello(rapidjson::Document& doc) {
	// {name:'Gabe'}
	rapidjson::Value& name = doc["name"];
	std::string str = name.GetString();
	std::cout << "Hello from \"" << str << "\"\n";
}

int main(int argc, char** argv) {
	if (argc != 4)
	{
		std::cerr << "Usage: client <host> <port> <name>" << std::endl; // host: '192.168.68.114'
		return 1;
	}
	username = argv[3];
	udpSvc.on("connect", &onConnect);
	udpSvc.on("ping", &onPing);
	udpSvc.on("hello", &onHello);
	udpSvc.connect(argv[1], argv[2]);
	udpSvc.io_service.run();
	return 0;
}