#!/bin/bash

cd $(dirname $(realpath $0))

npm install

echo "Cleaning up build..."
rm -fR build/*

echo "Building static production build..."
npm run build

rsync -avz ./build/ ../esp32/controller/data/
