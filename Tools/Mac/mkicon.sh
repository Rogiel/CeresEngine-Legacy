#!/usr/bin/env bash

icon_name=$1
image_name=$2

mkdir icon_name.iconset
sips -z 16 16     image_name.png --out icon_name.iconset/icon_16x16.png
sips -z 32 32     image_name.png --out icon_name.iconset/icon_16x16@2x.png
sips -z 32 32     image_name.png --out icon_name.iconset/icon_32x32.png
sips -z 64 64     image_name.png --out icon_name.iconset/icon_32x32@2x.png
sips -z 128 128   image_name.png --out icon_name.iconset/icon_128x128.png
sips -z 256 256   image_name.png --out icon_name.iconset/icon_128x128@2x.png
sips -z 256 256   image_name.png --out icon_name.iconset/icon_256x256.png
sips -z 512 512   image_name.png --out icon_name.iconset/icon_256x256@2x.png
sips -z 512 512   image_name.png --out icon_name.iconset/icon_512x512.png
cp Icon1024.png icon_name.iconset/icon_512x512@2x.png
iconutil -c icns icon_name.iconset
rm -R icon_name.iconset