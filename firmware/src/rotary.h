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

#ifndef __ROTARY_H
#define __ROTARY_H

#include "debounce.h"


// Reads changes from two inputs connected to a rotary encoder.
class Rotary {
 public:
  // Initializes the instance with intial values of the a and b pins.
  void Init(int a, int b);

  // Updates the instance with current values of the a and b pins.
  // Returns the change detected: -1, 0 or 1.
  int Update(int a, int b);

  // The inputs are debounced through these members.
  Debounce debounced_a_, debounced_b_;
  // The state of the a pin during the last Update().
  int last_a_;
};

#endif // __ROTARY_H
