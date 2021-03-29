#!/bin/bash

cd $(dirname $(realpath $0))

echo "Cleaning up build..."
rm -fR build/*

echo "Building static production build..."
npm run build

echo "Fixing paths..."
echo ""
echo "Fixing paths in chunks..."
find build/static/ -type f -name '*.chunk.*' -exec perl -p -i -e 's/(\.chunk\.)(css\.map|js\.map|css|js)/$1$2$3.gz/g' {} \;
echo "Fixing paths in top layer files..."
find build/ -maxdepth 1 -type f -exec perl -p -i -e 's/(\.chunk\.)(css\.map|js\.map|css|js)/$1$2$3.gz/g' {} \;

echo "Compressing build files..."
find build/static/ -type f | xargs gzip -9
