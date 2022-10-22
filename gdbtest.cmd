cls
g++ main.cpp -o fpl -static-libstdc++ -static -lws2_32 -O3 -D DEBUG

gdb fpl.exe