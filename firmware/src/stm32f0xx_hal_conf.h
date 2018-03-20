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


#ifndef __STM32F0xx_HAL_CONF_H
#define __STM32F0xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif


#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED

#if !defined (HSE_VALUE)
#define HSE_VALUE ((uint32_t)8000000)
#endif

#if !defined (HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT ((uint32_t)100)
#endif

#if !defined (HSI_VALUE)
#define HSI_VALUE ((uint32_t)8000000)
#endif

#if !defined (HSI_STARTUP_TIMEOUT)
#define HSI_STARTUP_TIMEOUT ((uint32_t)5000)
#endif

#if !defined (HSI14_VALUE)
#define HSI14_VALUE ((uint32_t)14000000)
#endif

#if !defined (HSI48_VALUE)
#define HSI48_VALUE ((uint32_t)48000000)
#endif

#if !defined (LSI_VALUE)
#define LSI_VALUE ((uint32_t)40000)
#endif

#if !defined (LSE_VALUE)
#define LSE_VALUE ((uint32_t)32768)
#endif

#if !defined (LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT ((uint32_t)5000)
#endif

#define VDD_VALUE ((uint32_t)3300)
#define TICK_INT_PRIORITY ((uint32_t)(1<<__NVIC_PRIO_BITS) - 1)
#define USE_RTOS 0
#define PREFETCH_ENABLE 1
#define INSTRUCTION_CACHE_ENABLE 0
#define DATA_CACHE_ENABLE 0


#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_dma.h"
#include "stm32f0xx_hal_cortex.h"
#include "stm32f0xx_hal_adc.h"
#include "stm32f0xx_hal_flash.h"
#include "stm32f0xx_hal_i2c.h"
#include "stm32f0xx_hal_rtc.h"
#include "stm32f0xx_hal_tim.h"


#ifdef __cplusplus
}
#endif

#define assert_param(expr) ((void)0)

#endif // __STM32F0xx_HAL_CONF_H
