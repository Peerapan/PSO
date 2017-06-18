#!/bin/bash
for i in 10 12 14 16 18 20
do
for j in 1 2 3
do
cmd="./build/test_poon data_"$i"_"$j".txt"
echo $cmd
eval $cmd
done
done