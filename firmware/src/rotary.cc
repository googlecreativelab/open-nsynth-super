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

#include "rotary.h"


// Uncomment this if using encoders with half pulses.
//#define ROTARY_HALF_PULSES


void Rotary::Init(int a, int b) {
  debounced_a_.Init(a);
  debounced_b_.Init(b);
  last_a_ = a;
}


int Rotary::Update(int a, int b) {
  debounced_a_.Update(a);
  a = debounced_a_.GetValue();

  debounced_b_.Update(b);
  b = debounced_b_.GetValue();

  int turned = 0;
#ifdef ROTARY_HALF_PULSES
  // Dials with half pulses transition 'a' once on each detent,
  // the turn is clockwise if the new value of 'a' is the same as
  // 'b'.
  if (a != last_a_) {
    turned = a == b ? 1 : -1;
  }
#else
  // Dials with full pulses transition 'a' twice on each detent,
  // 'b' is read on 'a's rising edge.
  if (a && !last_a_) {
    turned = b ? 1 : -1;
  }
#endif
  last_a_ = a;
  return turned;
}
