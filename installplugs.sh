#! /usr/bin/env bash

printf 'Copying plugin man pages\n';
printf '%s\0' mpy/*/ | xargs -0 -L1 bash -c 'cd -- "$1" && cp *.7 ../../userconf/man/man7/.' _;
printf 'Copying lang man pages\n';
printf '%s\0' mpy/lang/*/ | xargs -0 -L1 bash -c 'cd -- "$1" && cp *.7 ../../../userconf/man/man7/.' _;
printf 'Copying lang/syntax man pages\n';
printf '%s\0' mpy/lang/syntax/*/ | xargs -0 -L1 bash -c 'cd -- "$1" && cp *.7 ../../../../userconf/man/man7/.' _;
printf 'Copying lang/tools man pages\n';
printf '%s\0' mpy/lang/tools/*/ | xargs -0 -L1 bash -c 'cd -- "$1" && cp *.7 ../../../../userconf/man/man7/.' _;

printf 'Building plugins\n';
printf '%s\0' mpy/*/ | xargs -0 -L1 bash -c 'cd -- "$1" && ./build.sh && cp *.so ../../userconf/plugins/.' _;
printf 'Building lang plugins\n';
printf '%s\0' mpy/lang/*/ | xargs -0 -L1 bash -c 'cd -- "$1" && ./build.sh && cp *.so ../../../userconf/plugins/lang/.' _;
printf 'Building style plugins\n'
printf '%s\0' mpy/styles/*/ | xargs -0 -L1 bash -c 'cd -- "$1" && ./build.sh && cp *.so ../../../userconf/plugins/styles/.' _;
printf 'Building syntax plugins\n';
printf '%s\0' mpy/lang/syntax/*/ | xargs -0 -L1 bash -c 'cd -- "$1" && ./build.sh && cp *.so ../../../../userconf/plugins/lang/syntax/.' _;
printf 'Building lang tools plugins\n';
printf '%s\0' mpy/lang/tools/*/ | xargs -0 -L1 bash -c 'cd -- "$1" && ./build.sh && cp *.so ../../../../userconf/plugins/lang/tools/.' _;

printf 'Final touches...\n';
