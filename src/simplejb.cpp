/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "simplejb.h"

SimpleJB::SimpleJB() :
	frameSize_(512), // FEHLER???? 512
	nChannels_(1), // 2
	sampleRate_(8000), // 44100
	format_(util::AudioBuffer::FLOAT32),
	locked(false)
{}

SimpleJB::~SimpleJB() {}

void SimpleJB::addToBuffer(std::vector<uint8_t> data) {
	if (!locked) {
		setLock(true);

		if (!waitingPackages.empty()) {
			// move waiting packages to buffer
			for (size_t i = 0; i < waitingPackages.size(); i++)
			{
				jBuffer.push_back(waitingPackages[i]);
			}
			waitingPackages.clear();
		}
		// add data to buffer
		for (size_t i = 0; i < data.size(); i++)
		{
			jBuffer.push_back(data[i]);
		}

		setLock(false);
	}
	else {
		// add to waiting buffer
		for (size_t i = 0; i < data.size(); i++)
		{
			waitingPackages.push_back(data[i]);
		}
	}
}

void SimpleJB::configureFsChSr(int fs, unsigned int nCh, int sr) {
	SimpleJB::checkLocking();

	frameSize_ = fs;
	nChannels_ = nCh;
	sampleRate_ = sr;

	jBuffer.clear();
	waitingPackages.clear();
	setLock(false);
}

std::vector<uint8_t> SimpleJB::getFrame() {
	SimpleJB::checkLocking();

	uint32_t nBytes = frameSize_ * nChannels_ * (format_ / 8);

	std::vector<uint8_t> frame(nBytes, 0);

	if (jBuffer.empty()) {
		setLock(false);
		return frame;
	}

	if (jBuffer.size() >= nBytes) {

		::memcpy(&frame[0], &jBuffer[0], nBytes);

		jBuffer.erase(jBuffer.begin(), jBuffer.begin() + nBytes);
	}

	setLock(false);

	return frame;
}

void SimpleJB::checkLocking() {
	if (!locked) {
		setLock(true);

		if (!waitingPackages.empty()) {
			for (size_t i = 0; i < waitingPackages.size(); i++)
			{
				jBuffer.push_back(waitingPackages[i]);
			}
			waitingPackages.clear();
		}
	}
	else {
		while (locked) {}
		SimpleJB::checkLocking();
	}
}


void SimpleJB::setLock(bool lock) {
	locked = lock;
}