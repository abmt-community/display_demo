#!/bin/bash
# SPDX-License-Identifier: MIT
#exit an any error
set -e

if (( $# != 3 )); then
	echo "Usage: mk_icons.sh icon_dir file_name" 
	exit 1
fi

START_PATH=$PWD

cd $1
SIZE=$2
DEFINE=$(echo ${3%.*} | tr '[:lower:]' '[:upper:]')

cat > tmp_header << EOL
#ifndef ${DEFINE}_H
#define ${DEFINE}_H

EOL

for i in *.svg; do
	convert -dither None -density 1200 -resize ${SIZE}x${SIZE} $i $i.mono
done


for i in *.mono; do
	xxd -i $i >> tmp_header
	rm $i
done	

echo -e "#endif\n" >> tmp_header

cd $START_PATH
cp $1/tmp_header $3

sed -i $3 -e "s/^.*_mono_len.*//"
sed -i $3 -e "s/_[a-z]*_mono/_$SIZE/"
sed -i $3 -e "s/unsigned char /const unsigned char icon_/"

