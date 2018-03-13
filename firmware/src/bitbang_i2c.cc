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

#include "bitbang_i2c.h"

// The standard delay to use for the clock. Since the timer is set to
// 10MHz waiting 50 ticks gives a clock of 200KHz.
#define DELAY (50)

static TIM_HandleTypeDef tim17_handle;


// Busy waits for a number of timer ticks.
static inline void Delay(uint32_t ticks) {
  uint32_t start = tim17_handle.Instance->CNT;

  // The timer must be set up for 16bit operation (period = 0xffff)
  // for this logic to work.
  while (((tim17_handle.Instance->CNT - start) & 0xffff) < ticks) {
  }
}


BitBangI2C::BitBangI2C(uint16_t clock_pins, uint16_t data_pins)
  : clock_pins_(clock_pins),
    data_pins_(data_pins),
    bsrr_(NULL),
    idr_(NULL) {
}


void BitBangI2C::Setup(GPIO_TypeDef *port) {
  // Set up the GPIO pins.
  GPIO_InitTypeDef gpio_init_struct;
  gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;
  gpio_init_struct.Pull = GPIO_PULLUP;
  gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init_struct.Pin = clock_pins_ | data_pins_;
  HAL_GPIO_Init(port, &gpio_init_struct);

  bsrr_ = &port->BSRR;
  idr_ = &port->IDR;

  *bsrr_ = clock_pins_ | data_pins_;

  // Set up the timer as a 16bit up counter running at 10MHz.
  tim17_handle.Instance = TIM17;

  tim17_handle.Init.Prescaler         = (uint32_t)(SystemCoreClock / 10000000);
  tim17_handle.Init.Period            = 0xffff;
  tim17_handle.Init.ClockDivision     = 0;
  tim17_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  tim17_handle.Init.RepetitionCounter = 0;

  HAL_TIM_Base_Init(&tim17_handle);
  HAL_TIM_Base_Start(&tim17_handle);
}


// This define code releases the clock lines and waits for them all to go
// high. This supports clock stretching by the slaves.
#define RELEASE_SCL do { \
  *bsrr_ = clock_pins_; \
  while ((*idr_ & clock_pins_) != clock_pins_) { \
  } \
} while(0)


void BitBangI2C::Send(uint8_t addr, uint8_t *src, int len) {
  uint32_t SDA_UP = data_pins_;
  uint32_t SDA_DN = SDA_UP << 16;
  uint32_t SCL_UP = clock_pins_;
  uint32_t SCL_DN = SCL_UP << 16;

  // Send the start sequence.
  Delay(DELAY);
  *bsrr_ = SDA_DN;
  Delay(DELAY);
  *bsrr_ = SCL_DN;
  Delay(DELAY);

  // The first byte is the 7bit address and the write bit set to 0.
  uint8_t sending = addr << 1;

  for (int i=-1;;) {
    // Send each bit.
    for (int bit=0; bit<8; ++bit) {
      *bsrr_ = SCL_DN | (sending & (0x80) ? SDA_UP : SDA_DN);
      Delay(DELAY);
      RELEASE_SCL;
      Delay(DELAY);
      sending <<= 1;
    }

    // Send the acknowlegment bit.
    *bsrr_ = SCL_DN | SDA_UP;
    Delay(DELAY);
    RELEASE_SCL;
    Delay(DELAY);

    if (++i >= len) {
      break;
    }

    // Send the payload.
    sending = src[i];
  }

  // Hang up.
  *bsrr_ = SCL_DN | SDA_DN;
  Delay(DELAY);
  RELEASE_SCL;
  Delay(DELAY);
  *bsrr_ = SDA_UP;
  Delay(DELAY);
}


void BitBangI2C::Recv(uint8_t addr, int len) {
  uint32_t SDA_UP = data_pins_;
  uint32_t SDA_DN = SDA_UP << 16;
  uint32_t SCL_UP = clock_pins_;
  uint32_t SCL_DN = SCL_UP << 16;

  // Send the start sequence.
  Delay(DELAY);
  *bsrr_ = SDA_DN;
  Delay(DELAY);
  *bsrr_ = SCL_DN;
  Delay(DELAY);

  // The first byte is the 7bit address and the write bit set to 1.
  uint8_t sending = (addr << 1) | 1;
  int idx = 0;

  for (int i=-1;;) {
    for (int bit=0; bit<8; ++bit) {
      *bsrr_ = SCL_DN | (sending & (0x80) ? SDA_UP : SDA_DN);
      Delay(DELAY);
      RELEASE_SCL;
      if (i >= 0) {
        rx_buf_[idx++] = *idr_;
      }
      Delay(DELAY);
      sending <<= 1;
    }

    // Acknowlegment bit.
    if ((i < 0) || (i == len-1)) {
      *bsrr_ = SCL_DN | SDA_UP;
    } else if (i+1 < len) {
      *bsrr_ = SCL_DN | SDA_DN;
    }
    Delay(DELAY);
    RELEASE_SCL;
    if (i < 0) {
      rx_buf_[idx++] = *idr_;
    }
    Delay(DELAY);

    if (++i >= len) {
      break;
    }

    // Do not drive the data lines for the rest of the transfer.
    sending = 0xff;
  }

  // Hang up.
  *bsrr_ = SCL_DN | SDA_DN;
  Delay(DELAY);
  RELEASE_SCL;
  Delay(DELAY);
  *bsrr_ = SDA_UP;
  Delay(DELAY);
}


bool BitBangI2C::Acknowledged(uint16_t data_pin) {
  return (rx_buf_[0] & data_pin) == 0;
}


void BitBangI2C::GetResponse(uint16_t data_pin, uint8_t *dst, int len) {
  int idx = 1;
  int i, bit;
  uint8_t data;

  for (i=0; i<len; ++i) {
    data = 0;
    for (bit=0; bit<8; ++bit) {
      data <<= 1;
      data |= rx_buf_[idx] & data_pin ? 1 : 0;
      ++idx;
    }
    dst[i] = data;
  }
}
