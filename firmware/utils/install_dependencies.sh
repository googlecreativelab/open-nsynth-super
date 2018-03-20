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

#!/bin/bash


CUBE_URL=http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stm32cubef0.zip

repo_dir="$(readlink -m $PWD/$0/../..)"


setup_openocd() {
    openocd=$(which openocd)
    if [ -z "$openocd" ]
    then
        openocd="$PWD/openocd/bin/openocd"
        if ! [ -e "$openocd" ]
        then
            git clone git://git.code.sf.net/p/openocd/code openocd-setup
            (
                cd openocd-setup
                git checkout v0.10.0
                ./bootstrap
                ./configure --prefix="$repo_dir/openocd" --enable-sysfsgpio --enable-bcm2835gpio
                cd ..
                make -C openocd-setup -j4 install && rm -fr openocd-setup
            )
        fi
    fi

    echo "OPENOCD = sudo $openocd" >> config.mk
}


setup_cube() {
    stm32cube=""
    if [ -e STM32Cube_FW_F0_* ]
    then
        stm32cube=$(echo STM32Cube_FW_F0_*)
    fi

    if [ -z "$stm32cube" ]
    then
        if [ -e ${HOME}/opt/STM32Cube_FW_F0_* ]
        then
            ln -s ${HOME}/opt/STM32Cube_FW_F0_* .
            stm32cube=$(echo STM32Cube_FW_F0_*)
        fi
    fi

    if [ -z "$stm32cube" ]
    then
        echo "Installing Cube"
        wget -O /tmp/cube.zip $CUBE_URL
        unzip /tmp/cube.zip
        rm /tmp/cube.zip
        stm32cube=$(echo STM32Cube_FW_F0_*)
    fi

    echo "STM32CUBE = ../$stm32cube" >> config.mk
}


cd $repo_dir
echo -n > config.mk
setup_openocd
setup_cube
