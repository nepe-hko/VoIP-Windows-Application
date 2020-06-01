/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "sender.h"

Sender::Sender()
{

}

Sender::~Sender()
{
	if (running_) {
		stop();
	}
}

void Sender::start(std::string receiverIp, int receiverPort, int dataSize)
{
	running_ = true;
	unitSize = dataSize;
	util::Ipv4SocketAddress address(receiverIp, receiverPort);
	receiverAddress = address;

	if (socket.open())
		std::cout << "Socket to " << receiverAddress.toString() << " is open." << std::endl;
	else
		std::cout << "Socket connection failed!" << std::endl;
}

void Sender::stop()
{
	socket.close();
	if (!socket.isOpen())
		running_ = false;
}

void Sender::sendData(util::AudioBuffer const & scInput)
{
	if (running_) {

		std::vector<uint8_t> data(scInput.size(), 0);
		::memcpy(&data[0], scInput.data(), scInput.size());

		float nFragments = (float) data.size() / unitSize;

		uint32_t send_cnt = 0;


		for (size_t i = 0; i < nFragments; i++)
		{
			int offset = i * unitSize;

			if (i + 1 < nFragments) {
				std::vector<uint8_t> unit(data.begin() + offset, data.begin() + (offset + unitSize));
				send_cnt = socket.sendto(receiverAddress, rtpPacker.packData(10, unit));

				//std::cout << "Sent Bytes: " << send_cnt << " to: " << receiverAddress.toString(true) << std::endl;

				unit.clear();

			}
			else {
				std::vector<uint8_t> unit(data.begin() + offset, data.end());
				send_cnt = socket.sendto(receiverAddress, rtpPacker.packData(10, unit));

				//std::cout << "Sent Bytes: " << send_cnt << " to: " << receiverAddress.toString(true) << std::endl;

				unit.clear();
			}
		}
	}
	else
		std::cerr << "Sender not started" << std::endl;
}

bool Sender::isRunning()
{
	return false;
}
