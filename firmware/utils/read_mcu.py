# Copyright 2017 Google LLC

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     https://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


#!/usr/bin/env python

# Reads the hardware inputs from the MCU and prints them.

import smbus
import struct
import time


bus = smbus.SMBus(1)


def test_checksum(data):
    uints = struct.unpack('4I', data)
    chk = 0xaa55aa55
    chk += uints[0]
    chk += uints[1]
    chk += uints[2]
    return (chk & 0xffffffff) == uints[3]


def main():
    address = 0x47

    while True:
        try:
            data = bus.read_i2c_block_data(address, 0, 16)
        except IOError:
            print 'did not respond'
            time.sleep(1)
            continue

        data = b''.join(map(chr, data))
        unpacked = struct.unpack('2b4b6BI', data)
        touch = unpacked[:2]
        rots = unpacked[2:6]
        pots = unpacked[6:12]
        chk = unpacked[12]
        print('touch={} rots={} pots={} chk=0x{:08x} {}'.format(
            touch, rots, pots, chk,
            'passed' if test_checksum(data) else 'failed',
        ))
        time.sleep(0.1)


main()
