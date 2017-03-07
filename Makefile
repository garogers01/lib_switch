## Copyright (c) 2005-2015, Intel Corporation
## Redistribution  and  use  in source  and  binary  forms,  with  or  without
## * Redistributions of  source code  must retain the  above copyright notice,
##   this list of conditions and the following disclaimer.
## * Redistributions in binary form must reproduce the above copyright notice,
##   this list of conditions and the following disclaimer in the documentation
##   and/or other materials provided with the distribution.
## * Neither the name  of Intel Corporation  nor the names of its contributors
##   may be used to  endorse or promote  products derived  from this  software
##   without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
## IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
## ARE DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT OWNER  OR CONTRIBUTORS BE
## LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
## CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT LIMITED  TO,  PROCUREMENT  OF
## SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,  DATA, OR PROFITS;  OR BUSINESS
## INTERRUPTION)  HOWEVER CAUSED  AND ON ANY THEORY  OF LIABILITY,  WHETHER IN
## CONTRACT,  STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE  OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.
##****************************************************************************
##     Intel(R) Accelerator Abstraction Layer Library Software Developer
##        Kit (SDK)
##  Content:
##     samples/Hello_AAL/Makefile
##  Author:
##     Tim Whisonant, Intel Corporation
##     Joseph Grecco, Intel Corporation
##  History:
##     04/10/2015    JG   Initial version based off of canonical sample makefile
##******************************************************************************
CPPFLAGS ?=
CXX      ?= g++
LDFLAGS  ?=

CC=gcc
CCFLAGS= -g -O2 -fPIC
CCLDFLAGS= -g -O2 -fPIC -shared -ldl

BUILD=./build
RM=rm

all : sw lib_sock_comm.so lib_cli_comm.so lib_bridge.so lib_asic.so lib_flow.so lib_flow_stub.so mcpsub nsnsub

mcpsub : mcp
	make -C mcp

nsnsub : nsn
	make -C nsn

#test main application
sw : main.o
	$(CC) $(CCFLAGS) -o sw $(BUILD)/main.o -ldl

main.o : main.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/main.o main.c

#test asic bridge stub
lib_asic.so: lib_asic.o lib_asic_bridge.o
	$(CC) $(CCLDFLAGS) -o lib_asic.so $(BUILD)/lib_asic.o $(BUILD)/lib_asic_bridge.o

lib_asic.o : lib_asic.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/lib_asic.o lib_asic.c

lib_asic_bridge.o : lib_asic_bridge.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/lib_asic_bridge.o lib_asic_bridge.c


# lib flow protocol
lib_flow_dbg.o : lib_flow_dbg.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/lib_flow_dbg.o lib_flow_dbg.c

lib_flow.o : lib_flow.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/lib_flow.o lib_flow.c

lib_flow.so: lib_flow.o lib_flow_dbg.o
	$(CC) $(CCLDFLAGS) -o lib_flow.so $(BUILD)/lib_flow.o $(BUILD)/lib_flow_dbg.o

# lib flow stub implementation
lib_flow_stub.so: lib_flow_stub.o lib_flow_stub_main.o
	$(CC) $(CCLDFLAGS) -o lib_flow_stub.so $(BUILD)/lib_flow_stub.o $(BUILD)/lib_flow_stub_main.o

lib_flow_stub.o : lib_flow_stub.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/lib_flow_stub.o lib_flow_stub.c

lib_flow_stub_main.o : lib_flow_stub_main.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/lib_flow_stub_main.o lib_flow_stub_main.c

# lib bridge implementation
lib_bridge.so: lib_bridge.o 
	$(CC) $(CCLDFLAGS) -o lib_bridge.so $(BUILD)/lib_bridge.o

lib_bridge.o : lib_bridge.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/lib_bridge.o lib_bridge.c

# lib cli mgmt implementation
lib_cli_comm.so: lib_cli_comm.o 
	$(CC) $(CCLDFLAGS) -o lib_cli_comm.so $(BUILD)/lib_cli_comm.o

lib_cli_comm.o : lib_cli_comm.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/lib_cli_comm.o lib_cli_comm.c

# lib socket mgmt implementation
lib_sock_comm.so: lib_sock_comm.o 
	$(CC) $(CCLDFLAGS) -o lib_sock_comm.so $(BUILD)/lib_sock_comm.o

lib_sock_comm.o : lib_sock_comm.c
	$(CC) $(CCFLAGS) -c -o $(BUILD)/lib_sock_comm.o lib_sock_comm.c

clean:
	$(RM) lib_cli_comm.so lib_sock_comm.so lib_bridge.so lib_flow_stub.so lib_flow.so lib_asic.so sw $(BUILD)/*

.PHONY:all clean
