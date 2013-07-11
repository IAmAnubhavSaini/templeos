#!/bin/sh
# muflax <mail@muflax.com>, 2013

# compress all parts of TempleOS except for the root directory, which contains READMEs and such
for f in TempleOS/*/**/*; do
  echo "compressing $f..."

  # TODO invert tsz tool
done

