BUILD_DIR=./build
DPDK_DIR=/home/grogers1/dpdk/dpdk-16.07/x86_64-ivshmem-linuxapp-gcc/
aal_prefix=/usr/local/include
INC_DIRS="-I/home/grogers1/new_switch/lib_switch/ -I`pwd` -I$DPDK_DIR/include -I$aal_prefix/include -I/usr/local/include"
#LDFLAGS=-L$aal_prefix/lib -Wl -rpath-link -Wl $aal_prefix/lib -Wl,-rpath -Wl,$aal_prefix/lib \
            #-L$aal_prefix/lib64 -Wl,-rpath-link -Wl,$aal_prefix/lib64 -Wl,-rpath -Wl,$aal_prefix/lib64

echo ""
echo ""
echo ""
echo ""
echo ""
echo ""
echo ""
echo ""
echo ""
# Main application 
gcc -fPIC  -g -o sw  main.c -ldl

#stub implementation of a bridge
gcc -g -c -fPIC  lib_asic.c -o $BUILD_DIR/lib_asic.o
gcc -g -c -fPIC  lib_asic_bridge.c -o $BUILD_DIR/lib_asic_bridge.o
gcc -shared  -fPIC  -o lib_asic.so  $BUILD_DIR/lib_asic.o $BUILD_DIR/lib_asic_bridge.o

#Flow sample switch
gcc -g -c -fPIC  lib_flow.c -o $BUILD_DIR/lib_flow.o
gcc -g -c -fPIC  lib_flow_dbg.c -o $BUILD_DIR/lib_flow_dbg.o
gcc -shared -fPIC   -o lib_flow.so $BUILD_DIR/lib_flow.o $BUILD_DIR/lib_flow_dbg.o

#stub implementation of a flow
gcc -g -c -fPIC  lib_flow_stub.c -o $BUILD_DIR/lib_flow_stub.o
gcc -g -c -fPIC  lib_flow_stub_main.c -o $BUILD_DIR/lib_flow_stub_main.o
gcc -shared  -fPIC  -o lib_flow_stub.so  $BUILD_DIR/lib_flow_stub_main.o $BUILD_DIR/lib_flow_stub.o $BUILD_DIR/lib_flow_dbg.o


#mcp implementation of a flow
gcc $INC_DIRS -g -c -fPIC  mcp/lib_flow_mcp.c -o $BUILD_DIR/lib_flow_mcp.o
gcc $INC_DIRS -g -c -fPIC  mcp/lib_flow_mcp_main.c -o $BUILD_DIR/lib_flow_mcp_main.o
g++ $INC_DIRS -g -c -fPIC  mcp/mcp_aal.cpp -o $BUILD_DIR/mcp_aal.o
gcc $INC_DIRS -g -c -fPIC  mcp/mcp_dpdk.c -o $BUILD_DIR/mcp_dpdk.o
gcc -march=native $INC_DIRS -g -c -fPIC  mcp/virtio_hdlr.c -o $BUILD_DIR/virtio_hdlr.o
gcc -L/usr/local/lib -L$DPDK_DIR/lib $INC_DIRS -shared -fPIC  -o lib_flow_mcp.so $BUILD_DIR/lib_flow_mcp_main.o $BUILD_DIR/lib_flow_mcp.o $BUILD_DIR/lib_flow_dbg.o $BUILD_DIR/mcp_aal.o $BUILD_DIR/mcp_dpdk.o $BUILD_DIR/virtio_hdlr.o -ldpdk -lrte_vhost /usr/local/lib/libAAS.so /usr/local/lib/libaalrt.so 

#software implementation of a bridge
gcc -g -c -fPIC  lib_bridge.c -o $BUILD_DIR/lib_bridge.o
gcc -shared  -fPIC  -o lib_bridge.so  $BUILD_DIR/lib_bridge.o


#cli level configuration interface
gcc -g -c -fPIC  lib_cli_comm.c -o $BUILD_DIR/lib_cli_comm.o
gcc -shared  -fPIC  -o lib_cli_comm.so  $BUILD_DIR/lib_cli_comm.o

#socket level configuration interface
gcc -g -c -fPIC  lib_sock_comm.c -o $BUILD_DIR/lib_sock_comm.o
gcc -shared  -fPIC  -o lib_sock_comm.so  $BUILD_DIR/lib_sock_comm.o

# Netlink listener pulled form NSN https://github.com/M-a-z/nsn 
# modified to work within the framework
gcc $INC_DIRS  -g -c -fPIC  nsn/src/netstatenotifier_lib.c  -o $BUILD_DIR/netstatenotifier_lib.o  
gcc $INC_DIRS  -g -c -fPIC  nsn/src/nsn_commandlauncher.c  -o $BUILD_DIR/nsn_commandlauncher.o  
gcc $INC_DIRS  -g -c -fPIC  nsn/src/nsn_printfuncs.c  -o $BUILD_DIR/nsn_printfuncs.o  
gcc -shared -fPIC  -o lib_net_listener.so $BUILD_DIR/netstatenotifier_lib.o $BUILD_DIR/nsn_commandlauncher.o $BUILD_DIR/nsn_printfuncs.o  
