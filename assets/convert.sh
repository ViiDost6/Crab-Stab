#!/bin/sh

grit Marcelo_L.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit Marcelo_R.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit arm.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit nodes.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit pinza.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit barra.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit Marcelo_2.png -ftb -fh! -gTFF00FF -gt -gB8 -m!

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img ../nitrofiles/sprite/lvl1

grit Logo.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit Menu.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit Title.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit Bg1_inferior.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit bg1_superior.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit bg2_inferior.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit goodending1.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit goodending2.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit badending1.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit badending2.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img *.map ../nitrofiles/bg

grit icon_menu.png -ftb -fh! -gTFF00FF -gt -gB8 -m!

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img ../nitrofiles/sprite/ui