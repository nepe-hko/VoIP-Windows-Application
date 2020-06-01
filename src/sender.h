/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_SENDER_H
#define VOIP_SENDER_H

#include <iostream>
#include "socket.h"
#include "audiobuffer.h"
#include "rtp_packer.h"

class Sender {

public:
	Sender();
	~Sender();
	void start(std::string, int, int);
	void stop();
	void sendData(util::AudioBuffer const&);
	bool isRunning();

private:
	bool        running_;
	util::Ipv4SocketAddress receiverAddress;
	util::UdpSocket socket;
	int			unitSize;
	RtpPacker	rtpPacker;
};

#endif /* VOIP_SENDER_H */
