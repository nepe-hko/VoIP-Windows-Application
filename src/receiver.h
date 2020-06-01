/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_RECEIVER_H
#define VOIP_RECEIVER_H

#include <thread>
#include "socket.h"
#include "audiobuffer.h"
#include "simplejb.h"
#include "rtp_depacker.h"
#include "soundcard.h"

class Receiver {
public:
	Receiver();
	~Receiver();

	void start(int, SimpleJB*, int);
	void stop();
	void getData(util::AudioBuffer&);
	bool isRunning();

private:
	void receive();

	std::thread self_;
	bool			running_;

	int			port;
	util::Ipv4SocketAddress saddr;
	util::UdpSocket socket;

	SimpleJB*		jBuffer;
	int			maxUnitSize;
	RtpDepacker	rtpDepacker;

	unsigned int	outCh_;
};

#endif /* VOIP_RECEIVER_H */
