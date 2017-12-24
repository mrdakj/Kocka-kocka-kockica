#!/bin/zsh

find=$1
replace=$2

echo `egrep -r $find *.cpp *.h -oh | wc -l`
echo $find
echo $replace

sed_str='s/'$find'/'$replace'/g'

for x in *.cpp *.h;do sed -e $sed_str $x > $x.tmp && mv $x.tmp $x;done

