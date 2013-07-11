#!/bin/sh
# muflax <mail@muflax.com>, 2013

# decompress all .Z files
for f in $(find TempleOS -type f -name "*.Z"); do
  echo "decompressing $f..."

  Linux/tsz $f && mv $f $(dirname $f)/$(basename $f .Z)
done
