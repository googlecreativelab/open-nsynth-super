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

#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "bitbang_i2c.h"
#include "rotary.h"


#define I2C_ADDRESS 0x47

#define ADC_PORT GPIOA
#define ADC_PIN_POT0 GPIO_PIN_0
#define ADC_PIN_POT1 GPIO_PIN_1
#define ADC_PIN_POT2 GPIO_PIN_2
#define ADC_PIN_POT3 GPIO_PIN_3
#define ADC_PIN_POT4 GPIO_PIN_4
#define ADC_PIN_POT5 GPIO_PIN_5
#define NUM_POTS 6

#define ROT0_PORT GPIOA
#define ROT0_A_PIN GPIO_PIN_6
#define ROT0_B_PIN GPIO_PIN_7

#define ROT1_PORT GPIOB
#define ROT1_A_PIN GPIO_PIN_0
#define ROT1_B_PIN GPIO_PIN_1

#define ROT2_PORT GPIOA
#define ROT2_A_PIN GPIO_PIN_8
#define ROT2_B_PIN GPIO_PIN_9

#define ROT3_PORT GPIOA
#define ROT3_A_PIN GPIO_PIN_10
#define ROT3_B_PIN GPIO_PIN_11
#define NUM_ROTARIES 4


// Stores the values of all the hardware inputs.
struct Inputs {
  // The reported touch position, either 0-10 or 255 on no touch.
  uint8_t touch[2];

  // The position for each rotary encoder. From 0-255 wrapping in both
  // directions.
  uint8_t rotaries[NUM_ROTARIES];

  // The most significant 8 bits from the ADC readings on each potentiometer.
  uint8_t pots[NUM_POTS];
};


// Stores a message to be sent over the I2C bus to the master. This includes
// all the inputs and a checksum. The checksum is calculated as:
//   0xaa55aa55 +
//   the first 32 bits of inputs +
//   the second 32 bits of inputs +
//   the third 32 bits of inputs
struct I2cMessage {
    Inputs inputs;
    uint32_t chk;
};


// Global instances.
static ADC_HandleTypeDef adc_handle;
static GPIO_InitTypeDef  gpio_init;
static I2C_HandleTypeDef i2c_handle;
static TIM_HandleTypeDef tim16_handle;

static BitBangI2C touch_i2c(GPIO_PIN_5, GPIO_PIN_3 | GPIO_PIN_4);
static Inputs current_inputs = {0};
static I2cMessage i2c_message = {0};
static Rotary rotaries[NUM_ROTARIES];


// Handles errors by looping forever. This can be very useful is a debugger
// is attached as the stack trace can pin point the problem.
static void ErrorHandler() {
  while (true) {
    HAL_Delay(20);
  }
}


// Sets up the ADC hardware for reading the potentiometers.
static void InitAdc(void) {
  gpio_init.Mode = GPIO_MODE_ANALOG;
  gpio_init.Pull = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init.Pin = (
      ADC_PIN_POT0 | ADC_PIN_POT1 | ADC_PIN_POT2 |
      ADC_PIN_POT3 | ADC_PIN_POT4 | ADC_PIN_POT5);
  HAL_GPIO_Init(ADC_PORT, &gpio_init);

  ADC_ChannelConfTypeDef adcChannel;

  adc_handle.Instance = ADC1;

  adc_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  adc_handle.Init.Resolution = ADC_RESOLUTION_12B;
  adc_handle.Init.ScanConvMode = DISABLE;
  adc_handle.Init.ContinuousConvMode = DISABLE;
  adc_handle.Init.DiscontinuousConvMode = ENABLE;
  adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  adc_handle.Init.DMAContinuousRequests = DISABLE;
  adc_handle.Init.EOCSelection = DISABLE;

  HAL_ADC_Init(&adc_handle);

  for (int i=0; i<NUM_POTS; ++i) {
    adcChannel.Channel = ADC_CHANNEL_0 + i;
    adcChannel.Rank = i + 1;
    adcChannel.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

    if (HAL_ADC_ConfigChannel(&adc_handle, &adcChannel) != HAL_OK) {
      ErrorHandler();
    }
  }
}


// Sets up the I2C hardware for communicating with the master.
static void InitI2c() {
  i2c_handle.Instance = I2C1;
  //    Bits 31:28
  //        PRESC[3:0] : Timing prescaler
  //    Bits 27:24  Reserved, must be kept at reset value.
  //    Bits 23:20
  //        SCLDEL[3:0] : Data setup time
  //    Bits 19:16
  //        SDADEL[3:0] : Data hold time
  //    Bits 15:8
  //        SCLH[7:0] : SCL high period (master mode)
  //    Bits 7:0
  //        SCLL[7:0] : SCL low period (master mode)
  // The SCLDEL and SDADEL times are set to 0 to support 400KHz speeds.
  i2c_handle.Init.Timing = 0x10000f13;
  i2c_handle.Init.OwnAddress1 = I2C_ADDRESS << 1;
  i2c_handle.Init.OwnAddress2 = 0xff;
  i2c_handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  i2c_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  i2c_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  i2c_handle.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;

  if (HAL_I2C_Init(&i2c_handle) != HAL_OK) {
    ErrorHandler();
  }

  HAL_I2CEx_ConfigAnalogFilter(&i2c_handle,I2C_ANALOGFILTER_ENABLE);

  gpio_init.Mode  = GPIO_MODE_AF_OD;
  gpio_init.Pull  = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init.Alternate = GPIO_AF1_I2C1;
  gpio_init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOB, &gpio_init);
}


// Sets up the hardware for reading from the rotary encoders.
static void InitRot() {
  gpio_init.Mode = GPIO_MODE_INPUT;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

  gpio_init.Pin = ROT0_A_PIN | ROT0_B_PIN;
  HAL_GPIO_Init(ROT0_PORT, &gpio_init);

  gpio_init.Pin = ROT1_A_PIN | ROT1_B_PIN;
  HAL_GPIO_Init(ROT1_PORT, &gpio_init);

  gpio_init.Pin = ROT2_A_PIN | ROT2_B_PIN;
  HAL_GPIO_Init(ROT2_PORT, &gpio_init);

  gpio_init.Pin = ROT3_A_PIN | ROT3_B_PIN;
  HAL_GPIO_Init(ROT3_PORT, &gpio_init);

  rotaries[0].Init(
      HAL_GPIO_ReadPin(ROT0_PORT, ROT0_A_PIN),
      HAL_GPIO_ReadPin(ROT0_PORT, ROT0_B_PIN));
  rotaries[1].Init(
      HAL_GPIO_ReadPin(ROT1_PORT, ROT1_A_PIN),
      HAL_GPIO_ReadPin(ROT1_PORT, ROT1_B_PIN));
  rotaries[2].Init(
      HAL_GPIO_ReadPin(ROT2_PORT, ROT2_A_PIN),
      HAL_GPIO_ReadPin(ROT2_PORT, ROT2_B_PIN));
  rotaries[3].Init(
      HAL_GPIO_ReadPin(ROT3_PORT, ROT3_A_PIN),
      HAL_GPIO_ReadPin(ROT3_PORT, ROT3_B_PIN));

  tim16_handle.Instance = TIM16;

  tim16_handle.Init.Prescaler = (uint32_t)(SystemCoreClock / 20000);
  tim16_handle.Init.Period = 5 - 1;
  tim16_handle.Init.ClockDivision = 0;
  tim16_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  tim16_handle.Init.RepetitionCounter = 0;

  if (HAL_TIM_Base_Init(&tim16_handle) != HAL_OK) {
    ErrorHandler();
  }
}


// Sets up the touch controllers.
static void InitTouch() {
  uint8_t buf[2];

  // Put every key in the same group, disabling multiple touches.
  for (int key=0; key<11; ++key) {
    buf[0] = 28+key;
    buf[1] = 4;
    touch_i2c.Send(0x1c, buf, 2);
  }

  // Set the Detection Integrator to 1 for the fastest response times.
  buf[0] = 11;
  buf[1] = 1;
  touch_i2c.Send(0x1c, buf, 2);
}


// Starts reading from the rotary encoders.
static void StartRot() {
  if (HAL_TIM_Base_Start_IT(&tim16_handle) != HAL_OK) {
    ErrorHandler();
  }
}


// Starts handling I2C comms with the master.
static void StartI2c() {
  uint32_t flags = 0U;
  flags |= I2C_IT_ADDRI | I2C_IT_STOPI | I2C_IT_NACKI | I2C_IT_ERRI;
  flags |= I2C_IT_TCI | I2C_IT_TXI | I2C_IT_RXI;
  __HAL_I2C_ENABLE_IT(&i2c_handle, flags);
  i2c_handle.Instance->CR2 &= ~I2C_CR2_NACK;
}


// Reads from both touch sensors using I2C.
static void ReadTouch() {
  uint8_t buf[2];

  // Send address 0.
  buf[0] = 3;
  touch_i2c.Send(0x1c, buf, 1);

  // Read 2 bytes.
  touch_i2c.Recv(0x1c, 2);

  // Loop over both busses.
  static uint16_t data_pins[2] = {GPIO_PIN_3, GPIO_PIN_4};
  for (int channel=0; channel<2; ++channel) {
    uint16_t data_pin = data_pins[channel];

    if (touch_i2c.Acknowledged(data_pin)) {
      touch_i2c.GetResponse(data_pin, buf, 2);

      uint16_t keys = buf[0] | (buf[1] << 8);
      int key;
      for (key=0; key<11; ++key) {
        if (keys & (1 << key)) {
          break;
        }
      }

      if (key < 11) {
        // Invert the position so that the position is reported
        // relative to the top left corner (screen co-ordinates).
        key = 10 - key;
      } else {
        // The no-touch value.
        key = 0xff;
      }

      current_inputs.touch[channel] = key;
    }
  }
}


// Updates the rotary inputs.
static void ReadRots() {
  current_inputs.rotaries[0] += rotaries[0].Update(
      HAL_GPIO_ReadPin(ROT0_PORT, ROT0_A_PIN),
      HAL_GPIO_ReadPin(ROT0_PORT, ROT0_B_PIN));
  current_inputs.rotaries[1] += rotaries[1].Update(
      HAL_GPIO_ReadPin(ROT1_PORT, ROT1_A_PIN),
      HAL_GPIO_ReadPin(ROT1_PORT, ROT1_B_PIN));
  current_inputs.rotaries[2] += rotaries[2].Update(
      HAL_GPIO_ReadPin(ROT2_PORT, ROT2_A_PIN),
      HAL_GPIO_ReadPin(ROT2_PORT, ROT2_B_PIN));
  current_inputs.rotaries[3] += rotaries[3].Update(
      HAL_GPIO_ReadPin(ROT3_PORT, ROT3_A_PIN),
      HAL_GPIO_ReadPin(ROT3_PORT, ROT3_B_PIN));
}


// Copies the current inputs to the i2c_message, also calculating the checksum.
static void CopyInputs() {
  static_assert(
      sizeof(current_inputs) == sizeof(uint32_t) * 3,
      "Hard coded copy does not match source size.");
  static_assert(
      sizeof(i2c_message.inputs) == sizeof(uint32_t) * 3,
      "Hard coded copy does not match destination size.");

  // This is hard coded as 3 uint32_t copies, which ties in with the checksum
  // calculation.
  uint32_t *src = (uint32_t *)&current_inputs;
  uint32_t *dst = (uint32_t *)&i2c_message.inputs;

  uint32_t chk = 0xaa55aa55;
  uint32_t data;
  data = *src++;
  *dst++ = data;
  chk += data;
  data = *src++;
  *dst++ = data;
  chk += data;
  data = *src++;
  *dst++ = data;
  chk += data;

  i2c_message.chk = chk;
}


// Handles IRQs for the master I2C hardware.
extern "C"
void I2C1_IRQHandler(void) {
  static unsigned int tx_idx = 0;
  uint32_t it_flags   = READ_REG(i2c_handle.Instance->ISR);

  if (it_flags & I2C_FLAG_TXIS) {
    // Need to send some data.
    if (tx_idx < sizeof(i2c_message)) {
      i2c_handle.Instance->TXDR = ((uint8_t *)&i2c_message)[tx_idx++];
    } else {
      i2c_handle.Instance->TXDR = 0x00;
    }
  }

  if (it_flags & I2C_FLAG_ADDR) {
    // Address match.
    uint8_t transferdirection = I2C_GET_DIR(&i2c_handle);

    __HAL_I2C_CLEAR_FLAG(&i2c_handle,I2C_FLAG_ADDR);

    if (transferdirection == I2C_DIRECTION_RECEIVE) {
      // Start sending the first byte of the i2c_message.
      i2c_handle.Instance->TXDR = ((uint8_t *)&i2c_message)[0];
      tx_idx = 1;
    } else {
      // Writing to this device causes the inputs to be copied to the
      // send buffer.
      CopyInputs();
    }
  }

  if (it_flags & I2C_FLAG_RXNE) {
    // Received some data.
    volatile uint8_t data = i2c_handle.Instance->RXDR;
    UNUSED(data);
  }

  if (it_flags & (I2C_FLAG_AF | I2C_FLAG_STOPF)) {
    // Finished.
    __HAL_I2C_CLEAR_FLAG(&i2c_handle, I2C_FLAG_AF);
    __HAL_I2C_CLEAR_FLAG(&i2c_handle, I2C_FLAG_STOPF);
    __HAL_I2C_CLEAR_FLAG(&i2c_handle, I2C_FLAG_ADDR);
    I2C_RESET_CR2(&i2c_handle);
    if (__HAL_I2C_GET_FLAG(&i2c_handle, I2C_FLAG_TXIS) != RESET) {
      i2c_handle.Instance->TXDR = 0x00U;
    }
    if (__HAL_I2C_GET_FLAG(&i2c_handle, I2C_FLAG_TXE) == RESET) {
      __HAL_I2C_CLEAR_FLAG(&i2c_handle, I2C_FLAG_TXE);
    }

    i2c_handle.Instance->CR2 |= I2C_CR2_NACK;
  }

  __HAL_I2C_CLEAR_FLAG(&i2c_handle, I2C_FLAG_BERR);
  __HAL_I2C_CLEAR_FLAG(&i2c_handle, I2C_FLAG_OVR);
  __HAL_I2C_CLEAR_FLAG(&i2c_handle, I2C_FLAG_ARLO);

  i2c_handle.Instance->CR2 &= ~I2C_CR2_NACK;
}


// Handles IRQs for timer 16 - used for the rotary encoders.
extern "C"
void TIM16_IRQHandler(void) {
  if (__HAL_TIM_GET_FLAG(&tim16_handle, TIM_FLAG_UPDATE) != RESET) {
    if (__HAL_TIM_GET_IT_SOURCE(&tim16_handle, TIM_IT_UPDATE) != RESET) {
      __HAL_TIM_CLEAR_IT(&tim16_handle, TIM_IT_UPDATE);
      HAL_TIM_PeriodElapsedCallback(&tim16_handle);
      ReadRots();
    }
  }
}


// Handles IRQs for ADC1.
extern "C" void ADC1_IRQHandler(void) {
  HAL_ADC_IRQHandler(&adc_handle);
}


// Handles ADC results.
extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  static int channel = 0;
  static int sample = 0;
  static uint16_t history[NUM_POTS][16] = {0};
  static uint32_t last_value[NUM_POTS] = {0};

  uint16_t adc = (uint16_t)HAL_ADC_GetValue(&adc_handle);

  // Take the average of 16 samples.
  history[channel][sample] = adc;
  uint32_t sum = 0;
  for (int i=0; i<16; ++i) {
      sum += uint32_t(history[channel][i]);
  }
  uint32_t average = sum >> 4;

  // Perform an addition filter while still working with 12 bits.
  // If the value changes less than 12 do not update it.
  uint32_t delta;
  if (last_value[channel] > average) {
    delta = last_value[channel] - average;
    if (delta >= 12) {
      last_value[channel] = average;
    }
  } else {
    delta = average - last_value[channel];
    if (delta >= 12) {
      last_value[channel] = average;
    }
  }

  // Right shift by 4 to reduce the 12 bit result to 8 bits.
  current_inputs.pots[channel] = last_value[channel] >> 4;

  if (++channel >= NUM_POTS) {
    channel = 0;
    sample = (sample + 1) % 16;
  }

  // Start another conversion.
  HAL_ADC_Start_IT(&adc_handle);
}


// Configures the system clocks for 48MHz operation.
void SystemClockConfig() {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  // Select HSI as the temporary system clock source.
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
    ErrorHandler();
  }

  // Activate PLL with HSI/2 as source.
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK) {
    ErrorHandler();
  }

  // Select PLL as system clock source and configure the HCLK, PCLK1 clocks
  // dividers.
  RCC_ClkInitStruct.ClockType = (
      RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK) {
    ErrorHandler();
  }
}


int main(void) {
  HAL_Init();
  SystemClockConfig();

  // Set up all the hardware.
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_ADC1_CLK_ENABLE();
  __HAL_RCC_I2C1_CLK_ENABLE();
  __HAL_RCC_TIM16_CLK_ENABLE();
  __HAL_RCC_TIM17_CLK_ENABLE();

  touch_i2c.Setup(GPIOB);

  InitAdc();
  InitI2c();
  InitRot();
  InitTouch();

  // Comms to the RPi are given top priority so that they do not fail.
  HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_IRQn);

  // The rotary timer is given second priority so that the encoders are
  // read consistently.
  HAL_NVIC_SetPriority(TIM16_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(TIM16_IRQn);

  // The potentiometers are given last priority.
  HAL_NVIC_SetPriority(ADC1_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(ADC1_IRQn);

  // Perform an initial read of all inputs and then start I2C comms
  // with the host.
  StartRot();
  HAL_ADC_Start_IT(&adc_handle);
  ReadTouch();
  HAL_Delay(16);

  StartI2c();

  while (true) {
    ReadTouch();
    HAL_Delay(4);
  }
}
