/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "rtp_packer.h"

RtpPacker::RtpPacker() :
	ver(2),
	padding(0),
	ext(0),
	csrcCnt(0),
	marker(0),
	pt(0),
	sqn(0),
	ts(0),
	ssrc(0)
{
	setStartSequNumber();

	setMySSRC();

	setStartTimestamp();
}

RtpPacker::~RtpPacker() {}

void RtpPacker::setStartSequNumber() {
	srand(time(0));
	sqn_b2 = rand() % 256;
	sqn_b1 = rand() % 256;
	sqn = 0;
	sqn = (sqn_b1 << 8) | sqn_b2;
}

void RtpPacker::setMySSRC() {
	srand(time(0));
	ssrc_b4 = rand() % 256;
	ssrc_b3 = rand() % 256;
	ssrc_b2 = rand() % 256;
	ssrc_b1 = rand() % 256;

	ssrc = 0;

	ssrc = (ssrc_b1 << 8) | ssrc_b2;
	ssrc = (ssrc << 8) | ssrc_b3;
	ssrc = (ssrc << 8) | ssrc_b4;
}

void RtpPacker::setStartTimestamp() {
	lastTime = time(0);

	srand(time(0));
	uint8_t ts_b4 = rand() % 256;
	uint8_t ts_b3 = rand() % 256;
	uint8_t ts_b2 = rand() % 256;
	uint8_t ts_b1 = rand() % 256;

	ts = 0;

	ts = (ts_b1 << 8) | ts_b2;
	ts = (ts << 8) | ts_b3;
	ts = (ts << 8) | ts_b4;
}

void RtpPacker::createHeader() {
	header.clear();

	uint8_t byte1 = 0;
	uint8_t byte2 = 0;

	byte1 = (ver << 1) | padding;
	byte1 = (byte1 << 1) | ext;
	byte1 = (byte1 << 4) | csrcCnt;

	byte2 = (marker << 7) | pt;

	header.push_back(byte1);
	header.push_back(byte2);

	header.push_back(sqn_b1);
	header.push_back(sqn_b2);

	uint8_t *pTs = (uint8_t *)&ts;
	header.push_back(pTs[3]);
	header.push_back(pTs[2]);
	header.push_back(pTs[1]);
	header.push_back(pTs[0]);

	header.push_back(ssrc_b1);
	header.push_back(ssrc_b2);
	header.push_back(ssrc_b3);
	header.push_back(ssrc_b4);
}

std::vector<uint8_t> RtpPacker::packData(uint8_t payloadType, std::vector<uint8_t>& data) {
	uint32_t actualTime = time(0);
	ts = ts + (actualTime - lastTime);
	lastTime = actualTime;

	pt = payloadType;

	createHeader();

	std::vector<uint8_t> rtpPacket(header.size() + data.size(), 0);
	::memcpy(&rtpPacket[0], &header[0], header.size());
	::memcpy(&rtpPacket[header.size()], &data[0], data.size());

	sqn++;

	return rtpPacket;
}

