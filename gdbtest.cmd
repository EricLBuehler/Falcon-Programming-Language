cls
g++ main.cpp -o fpl -static-libstdc++ -static -lws2_32 -D DEBUG

gdb fpl.exe