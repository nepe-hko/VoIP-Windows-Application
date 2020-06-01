/******************************************************************************/
/*
 *  Project: Voice-over-IP
 *  Author: Matthias Tessmann <matthias.tessmann@th-nuernberg.de
 *  Date: October, 5th 2015
 *
 *  Contents: Application wrapper class skeleton.
 *            Intended for educational purposes.
 *
 *  Additoinal notes:
 *         - There is very little error handling.
 *         - The implementation may contain bugs of any kind!
 *
 * The MIT License (MIT)
 *
 *  Copyright (c) 2015 Matthias Teßmann <matthias.tessmann@th-nuernberg.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
/******************************************************************************/

#ifndef VOIP_COMM_H
#define VOIP_COMM_H

#include "soundcard.h"
#include "sender.h"
#include "receiver.h"
#include "simplejb.h"

// This is the application entry point
class VoIPComm : public util::AudioIO {
public:
	
  VoIPComm();
  ~VoIPComm();
  VoIPComm(VoIPComm const&)            = delete;
  VoIPComm(VoIPComm&&)                 = delete;
  VoIPComm& operator=(VoIPComm const&) = delete;
  VoIPComm& operator=(VoIPComm&&)      = delete;

  int exec(int argc, char *argv[]);

  // Callback for Soundcard
  int process(util::AudioBuffer& output, util::AudioBuffer const& input);

private:
  bool init(int argc, char *argv[]);
  void listDevices();
  void initSoundcard();
  util::SoundCard soundcard_;

  Sender		s;
  Receiver		r;
  SimpleJB		jbBuffer;

  std::string	ip_;
  unsigned int	rPort_;
  unsigned int	lPort_;

  int			inDev_;
  int			outDev_;
  unsigned int	inCh_;
  unsigned int	outCh_;
  int			sr_;
  int			fs_;
};

#endif /* VOIP_COMM_H */
