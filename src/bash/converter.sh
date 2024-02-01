#!/bin/bash
type=$1
for oldFileName in *.tga; do
    [ -f "$oldFileName" ] || break
    newFileName=${oldFileName%.tga}.$type
    convert "$oldFileName" -flip "$newFileName"
    rm "$oldFileName"
    echo "Converted $oldFileName to $newFileName"
done