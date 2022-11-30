cls
g++ main.cpp -o falcon -static-libstdc++ -static -lws2_32 -D DEBUG -fno-gcse

gdb falcon.exe