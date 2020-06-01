/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_SIMPLEJB_H
#define VOIP_SIMPLEJB_H

#include <vector>
#include "audiobuffer.h"

class SimpleJB {
public:
	SimpleJB();
	~SimpleJB();

	void addToBuffer(std::vector<uint8_t>);
	std::vector<uint8_t> getFrame();
	void configureFsChSr(int, unsigned int, int);

private:
	void checkLocking();
	void setLock(bool);

	std::vector<uint8_t> jBuffer;
	std::vector<uint8_t> waitingPackages;

	int            frameSize_;
	unsigned int   nChannels_;
	int            sampleRate_;
	util::AudioBuffer::SampleFormat	format_;
	bool				locked;
};

#endif /* VOIP_SIMPLEJB_H */
