#!/bin/sh

for file in *.png; do
    grit "$file" -ftb -fh! -gb -gB16
done

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.img ../bmp
