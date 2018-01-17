#!/bin/zsh

find=$1
replace=$2

echo `egrep -r $find sources/*.cpp headers/*.h Makefile -oh | wc -l`
echo $find
echo $replace

sed_str='s/'$find'/'$replace'/g'

for x in sources/*.cpp headers/*.h Makefile;do sed -e $sed_str $x > $x.tmp && mv $x.tmp $x;done

