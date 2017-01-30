# lib_switch

This application is a library based switch.  It uses dynamically linked libraries to build a flexible switch.

It is composed of three main constructs :

Management :

This is the management layer, and would be akin to a CLI.  This layer will be the access to managing the switch infrastructure provided by other components.   It is not defined what the interface used to manage the switch modules would be, this is up to the management interface itself.  Examples could be sockets, netlink sockets, OVSDB, Command Line Shell, or whichever interface the module would choose to offer to the lower modules.

Protocol :

Protocol is a protocol handler.  This is akin to OSPF, RIP, BGP, etc. for non-human intervention for managing the switch modules.  The protocol management resides on top of the switching modules to control the operation of the switch itself.

Switch modules :

A switch module registers to provide an interface that would manage a switch that forwards packets.  This switch could be a simple L2 switch, L3 Router, or Openflow switch.  Standard interfaces would be provided for these layers.

Status : February 2017

Initial Commit of shell, demonstrating the usage of such a system.


Directory structure 

lib_switch /  -- High level switching infrastructure with main.c loading all the libraries.

client / -- A sample client application that connects through the lib_sock_comm to query the status of the dynamic switching.


Compile :

lib_switch -- Type ./compile.sh
This will build the main application sw, which loads all the dynamic libraries.  It will also build all the dynamic libraries, which are just stubs at the moment.

move into the client directory and build by typing ./compile.sh


Usage :

once compiled, 

set your LD_LIBRARY_PATH by typing : export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./.

just type ./sw   to start the applicaiton, which will search the current directory and pull in all dynamic libraries to initialize the system.

To test the socket communcation go to the client directory, export the LD_LIBRARY_PATH as above, and type ./sw to start and send a query to the main applicaiton.

