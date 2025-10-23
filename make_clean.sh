rm -rf out/
source /opt/toolchains/mars/mars.sh
make
cp out/rom.bin /mnt/retro/roms/sega/megadrive/dev/
/home/nate/programs/blastem64-0.6.2/blastem out/rom.bin