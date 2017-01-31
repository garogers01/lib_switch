gcc -I ../ -o sw main.c -ldl
gcc -fPIC -I ../ -c lib_sock_comm.c -o lib_sock_comm.o
gcc -fPIC -shared  -o lib_sock_comm.so  lib_sock_comm.o
