#!/bin/bash

apt-get install bc

start_cat=`date +%s.%N`

file=$1
cat $file

end_cat=`date +%s.%N`

runtime_cat=$(echo "$end_cat-$start_cat"|bc)


start_cat2=`date +%s.%N`
./cat2 $file
end_cat2=`date +%s.%N`
runtime_cat2=$(echo "$end_cat2 - $start_cat2"|bc)

echo $start_cat

echo ""
echo "File used for both cat and cat2 commands [$file]"

echo "Runtime performance of Linux cat command: $runtime_cat ms"
echo "Runtime performance of cat2 command in c with system calls: $runtime_cat2 ms"
