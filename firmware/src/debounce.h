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

#ifndef __DEBOUNCE_H
#define __DEBOUNCE_H

#include <stdint.h>


// Debounces the input from a GPIO pin. The reported value only changes
// if 4 updates in a row are the same value.
class Debounce {
 public:

  // Initializes the instance with an intial value.
  void Init(int value);

  // Updates the instance with the current value of a pin.
  void Update(int value);

  // Gets the debounced value.
  int GetValue();

  // Stores a history of readings.
  uint8_t history_;
  // Stores the current (debounced) value.
  int current_value_;
};

#endif // __DEBOUNCE_H
