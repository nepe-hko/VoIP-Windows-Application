/*
 * This file and maybe an associated implementation
 * are provided as a means to get you started with
 * the project. You can adapt and use the provided
 * structure or start from scratch and create your
 * own implementation.
 */

#ifndef VOIP_RTP_DEPACKER_H
#define VOIP_RTP_DEPACKER_H

#include <stdint.h>
#include <vector>

class RtpDepacker {
public:
	RtpDepacker();
	~RtpDepacker();

	std::vector<uint8_t> dePack(std::vector<uint8_t>&);

private:
	uint8_t		ver;
	uint8_t		padding;
	uint8_t		ext;
	uint8_t		csrcCnt;
	uint8_t		marker;
	uint8_t		pt;
	uint16_t	sqn;
	uint32_t	ts;
	uint32_t	ssrc;

	uint8_t		sqn_b1;
	uint8_t		sqn_b2;

	uint8_t		ts_b1;
	uint8_t		ts_b2;
	uint8_t		ts_b3;
	uint8_t		ts_b4;

	uint8_t		ssrc_b1;
	uint8_t		ssrc_b2;
	uint8_t		ssrc_b3;
	uint8_t		ssrc_b4;

	uint8_t		paddingSize;

	std::vector<uint8_t> payload;
	std::vector<uint8_t> csrc;
};

#endif /* VOIP_RTP_DEPACKER_H */