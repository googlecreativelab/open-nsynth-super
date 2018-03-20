/*
Copyright 2016 Google Inc. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "main.h"


// This I2C implementation has a blocking interface for sending and
// receiving data. Internally it uses TIM17 for timing.
//
// The interface does not assume a single bus - this is useful
// for talking to multiple devices with the same fixed address which must
// therefor be on separate busses. Writes are broadcast on all the busses.
// Reads are also performed in parallel and the individual responses can
// be obtained.
class BitBangI2C {
 public:
  BitBangI2C(uint16_t clock_pins, uint16_t data_pins);
  // Sets up the GPIO pins and timer so the busses are ready for transfers.
  void Setup(GPIO_TypeDef *port);

  // Broadcasts a MOSI on all busses.
  void Send(uint8_t addr, uint8_t *src, int len);

  // Performs a MISO transfer on all busses. The results are stored in an
  // internal buffer, they can subsequently be checked with Acknowledged()
  // and GetResponse().
  void Recv(uint8_t addr, int len);
  // Tests for slave acknowledgement on a single bus.
  bool Acknowledged(uint16_t data_pin);
  // Retreives the data received on a single bus.
  void GetResponse(uint16_t data_pin, uint8_t *dst, int len);

  // Internal.

  // A bitset of GPIO pins used as clock lines.
  uint32_t clock_pins_;
  // A bitset of GPIO pins used as data lines.
  uint32_t data_pins_;
  // The GPIO port set/reset register used for driving lines.
  volatile uint32_t *bsrr_;
  // The GPIO port input register used for reading lines.
  volatile uint32_t *idr_;

  // An upper limit for the number of bytes that will be read from the
  // busses.
  const static int BIT_BANG_I2C_RECV_MAX_LEN = 2;

  // The rx_buf_ contains samples from the GPIO bus.
  // It is populated with 1 sample for the acknowledge bit,
  // followed by 8 samples per byte read.
  const static int BUF_LEN = 1 + BIT_BANG_I2C_RECV_MAX_LEN*8;
  uint32_t rx_buf_[BUF_LEN];
};
