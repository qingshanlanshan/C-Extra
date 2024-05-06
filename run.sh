python translator.py $1
gcc src/*.c $1.c -o test.exe
./test.exe