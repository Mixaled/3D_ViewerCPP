#!/bin/bash
input_folder="gif_make/"
output_folder="gif/"

mkdir -p $output_folder
type=".bmp"
for tga_file in $input_folder*.tga; do
    [ -f "$tga_file" ] || break
    base_name=$(basename -- "$tga_file")
    filename_no_ext="${base_name%.tga}"
    bmp_filename="$output_folder$filename_no_ext$type"
    convert "$tga_file" -flip "$bmp_filename"
    rm "$tga_file"
done

cd $output_folder && convert -delay 100 -loop 0 *.bmp output.gif && rm *.bmp
