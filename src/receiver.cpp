/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "receiver.h"
#include <iostream>

Receiver::Receiver() : self_() {}

Receiver::~Receiver() {
	if (running_) {
		stop();
	}
}

void Receiver::start(int port, SimpleJB* jb, int maxDataSize) {
	jBuffer = jb;
	maxUnitSize = maxDataSize;

	running_ = true;
	util::Ipv4SocketAddress addr("", port);
	saddr = addr;
	socket.open();

	if (!socket.bind(addr)) {
		std::cerr << "Error binding socket!" << std::endl;
		socket.close();
		exit(-1);
	}

	self_ = std::thread([=] { receive(); });
}

void Receiver::stop() {
	running_ = false;
	self_.join();
	socket.close();
}

void Receiver::receive() {

	while (running_) {
		std::vector<uint8_t> data(maxUnitSize, 0);
		util::Ipv4SocketAddress from;

		socket.recvfrom(from, data, maxUnitSize);

		std::vector<uint8_t> depackedData = rtpDepacker.dePack(data);

		std::cout << "Received Bytes: " << data.size() << " from: " << from.toString(true) << std::endl;

		if (!depackedData.empty())
			jBuffer->addToBuffer(depackedData);
	}
}

void Receiver::getData(util::AudioBuffer& scOutput) {

	std::vector<uint8_t> frame = jBuffer->getFrame();

	::memcpy(scOutput.data(), &frame[0], frame.size());
}

bool Receiver::isRunning() {
	return running_;
}