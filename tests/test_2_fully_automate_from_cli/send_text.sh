#!/bin/bash

cat 1
sleep .3
python3 ../../scripts/text_to_sign.py "$1"
sleep .3
cat 3
sleep .3
cat 4
sleep .3
cat 5