/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#include "rtp_depacker.h"


RtpDepacker::RtpDepacker() :
	ver(0),
	padding(0),
	ext(0),
	csrcCnt(0),
	marker(0),
	pt(0),
	sqn(0),
	ts(0),
	ssrc(0) {}

RtpDepacker::~RtpDepacker() {}

std::vector<uint8_t> RtpDepacker::dePack(std::vector<uint8_t>& rtpPacket) {
	payload.clear();
	csrc.clear();

	if (rtpPacket.empty()) {
		return payload;
	}

	uint8_t byte1 = rtpPacket.at(0);
	uint8_t byte2 = rtpPacket.at(1);

	ver = (byte1 >> 6) & 0x3;

	if (ver != 2)
		return payload;

	padding = (byte1 >> 5) & 0x1;
	ext = (byte1 >> 4) & 0x1;
	csrcCnt = byte1 & 0xF;

	marker = (byte2 >> 7) & 0x1;
	pt = byte2 & 0x7F;

	if (pt != 10 && pt != 11)
		return payload;

	sqn_b1 = rtpPacket.at(2);
	sqn_b2 = rtpPacket.at(3);

	sqn = 0;
	sqn = (sqn_b1 << 8) | sqn_b2;

	ts_b1 = rtpPacket.at(4);
	ts_b2 = rtpPacket.at(5);
	ts_b3 = rtpPacket.at(6);
	ts_b4 = rtpPacket.at(7);

	ts = 0;
	ts = (ts_b1 << 8) | ts_b2;
	ts = (ts << 8) | ts_b3;
	ts = (ts << 8) | ts_b4;

	ssrc_b1 = rtpPacket.at(8);
	ssrc_b2 = rtpPacket.at(9);
	ssrc_b3 = rtpPacket.at(10);
	ssrc_b4 = rtpPacket.at(11);

	ssrc = 0;
	ssrc = (ssrc_b1 << 8) | ssrc_b2;
	ssrc = (ssrc << 8) | ssrc_b3;
	ssrc = (ssrc << 8) | ssrc_b4;

	if (csrcCnt > 0) {
		for (size_t i = 0; i < csrcCnt; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				csrc.push_back(rtpPacket.at(12 + j + 4 * i));
			}
		}
	}

	paddingSize = 0;
	if (padding == 1) {
		paddingSize = rtpPacket.back();
	}

	int headerSize = 12 + 4 * csrcCnt;
	int payloadSize = rtpPacket.size() - (headerSize + paddingSize);

	payload.resize(payloadSize, 0);

	::memcpy(&payload[0], &rtpPacket[headerSize], payloadSize);

	return payload;
}