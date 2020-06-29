#!/bin/bash
echo 'Setting variables...'
export CROSS_COMPILE=../../../prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-
export ARCH=arm64 && export SUBARCH=arm64

echo 'Cleaning build files...'
make clean &> /dev/null
make mrproper &> /dev/null

echo 'Building defconfig...'
make lineage-a5y17lte_defconfig &> /dev/null

echo 'Building kernel...'
make -j 32

if [ $? -ne 0 ]; then
  echo 'The build did not complete successfully. Exitting now...'
  exit 1
fi

echo 'Exporting zImage...'
mv ./arch/arm64/boot/Image ../../../AIK/split_img/3.18.14_boot.img-zImage

echo 'Repacking boot image with new zImage...'
../../../AIK/repackimg.sh &> /dev/null

declare -r imgLoc="../../../AIK/images/$(make kernelversion)_boot.img"
mv ../../../AIK/image-new.img $imgLoc
echo "New boot image is located at '$imgLoc'"

exit 0
