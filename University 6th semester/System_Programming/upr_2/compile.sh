gcc main.c read_convert_file.c grammar_correction.c -o main

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
else
    echo "Compilation failed!"
    exit 1
fi