#!/bin/sh
# muflax <mail@muflax.com>, 2013

# write essential stuff on new image, but keep directory structure intact
mkisofs -V TempleOS -J -r -l -graft-points -b 0000Boot/0000Kernel.BIN.C -no-emul-boot -o TempleOS.iso \
  "/0000Boot/=0000Boot" \
  "/Linux/=Linux" \
  "/Windows/=Windows" \
  "/TempleOS/=TempleOS"
