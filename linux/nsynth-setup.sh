# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#     http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


setup_audio() {
    if grep -q '^dtparam=audio=on' /boot/config.txt
    then
        echo 'Disabling on board audio'
        sed -i -e 's/dtparam=audio=on/#dtparam=audio=on/' /boot/config.txt
    fi

    if ! grep -q "^dtoverlay=iqaudio-dac" /boot/config.txt
    then
        echo 'Enabling I2S + I2C audio'
        printf "\n# Enable I2S + I2C audio\ndtoverlay=iqaudio-dac\n" >> /boot/config.txt
    fi
}


setup_i2c() {
    if grep -q '^#dtparam=i2c_arm=on' /boot/config.txt
    then
        echo 'Enabling I2C'
        sed -i -e 's/#dtparam=i2c_arm=on/dtparam=i2c_arm=on/' /boot/config.txt
    fi

    if ! grep -q '^dtparam=i2c_arm_baudrate=640000' /boot/config.txt
    then
        echo 'Setting I2C to high speed'
        printf "\n# Set I2C to high speed\ndtparam=i2c_arm_baudrate=640000\n" >> /boot/config.txt
    fi

    if ! grep -q '^i2c-dev' /etc/modules
    then
        echo 'Enabling i2c-dev module'
        printf "i2c-dev\n" >> /etc/modules
    fi
}


setup_serial() {
    if grep -q 'console=serial' /boot/cmdline.txt
    then
        echo 'Disabling console serial'
        sed -i -e 's/console=serial[^ ]* //' /boot/cmdline.txt
    fi

    if ! grep -q '^dtoverlay=pi3-miniuart-bt' /boot/config.txt
    then
        echo 'Enabling serial MIDI'
        printf "\n# Enable seiral MIDI\nenable_uart=1\ndtoverlay=pi3-miniuart-bt\ndtoverlay=midi-uart0\n" >> /boot/config.txt
    fi
}


install_deps() {
    apt-get update
    apt-get install -y i2c-tools python-smbus gdb-arm-none-eabi gcc-arm-none-eabi \
        git autoconf libtool make pkg-config build-essential \
        freeglut3-dev libasound2-dev libxmu-dev libxxf86vm-dev g++ libgl1-mesa-dev \
        libglu1-mesa-dev libraw1394-dev libudev-dev libdrm-dev libglew-dev \
        libopenal-dev libsndfile-dev libfreeimage-dev libcairo2-dev \
        libfreetype6-dev libssl-dev libpulse-dev libusb-1.0-0-dev libgtk-3-dev \
        libopencv-dev libegl1-mesa-dev libgles1-mesa-dev libgles2-mesa-dev \
        libassimp-dev librtaudio-dev libboost-filesystem-dev libglfw3-dev \
        liburiparser-dev libcurl4-openssl-dev libpugixml-dev libgstreamer1.0-dev \
        libgstreamer-plugins-base1.0-dev gstreamer1.0-libav \
        gstreamer1.0-pulseaudio gstreamer1.0-x gstreamer1.0-plugins-bad \
        gstreamer1.0-alsa gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
        gstreamer1.0-omx libpoco-dev
}


setup_service() {
    svc=/etc/systemd/system/open-nsynth.service
    if ! [ -e ${svc} ]
    then
        cat > ${svc} <<EOF
[Unit]
Description=Open NSynth
After=alsa.restore

[Service]
Type=simple
User=pi
WorkingDirectory=/home/pi/opt/of/apps/open-nsynth/open-nsynth/bin
ExecStart=/home/pi/opt/of/apps/open-nsynth/open-nsynth/bin/open-nsynth
Restart=on-abort

[Install]
WantedBy=multi-user.target
EOF
        systemctl enable open-nsynth
    fi
}


setup_openframeworks() {
    if ! [ -e /home/pi/opt/of ]
    then
        echo "Cloning openFrameworks"
        mkdir -p /home/pi/opt
        (
            cd /home/pi/opt
            git clone --branch 0.10.0 --depth 1 https://github.com/openframeworks/openFrameworks.git of
            ./of/scripts/linux/download_libs.sh
        )
    fi
}


setup_openocd() {
    if ! [ -e /usr/bin/openocd ]
    then
        echo "Installing OpenOCD"
        mkdir -p /home/pi/tmp/setup
        (
            cd /home/pi/tmp/setup
            git clone --branch v0.10.0 --depth 1 git://git.code.sf.net/p/openocd/code openocd
            cd openocd
            ./bootstrap
            ./configure --prefix=/usr --enable-sysfsgpio --enable-bcm2835gpio
            make -j4
            sudo make install
        )
    fi
}


if [ "$(whoami)" = "root" ]
then
    setup_audio
    setup_i2c
    setup_serial
    install_deps
    setup_service
else
    sudo bash $0
    setup_openframeworks
    setup_openocd
fi
