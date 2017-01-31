gcc -o sw    main.c -ldl
gcc -c -fPIC lib_flow.c -o lib_flow.o
gcc -shared -fPIC  -o lib_flow.so  lib_flow.o
gcc -c -fPIC lib_asic.c -o lib_asic.o
gcc -c -fPIC lib_asic_bridge.c -o lib_asic_bridge.o
gcc -shared  -fPIC -o lib_asic.so  lib_asic.o lib_asic_bridge.o
#gcc -shared  -o lib_asic.so  lib_asic.o
gcc -c -fPIC lib_bridge.c -o lib_bridge.o
gcc -shared  -fPIC -o lib_bridge.so  lib_bridge.o
gcc -c -fPIC lib_sock_comm.c -o lib_sock_comm.o
gcc -shared  -fPIC -o lib_sock_comm.so  lib_sock_comm.o
