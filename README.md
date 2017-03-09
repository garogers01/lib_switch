# lib_switch

This application is a library based switch.  It uses dynamically linked libraries to build a flexible switch.

It is composed of three main constructs :

Management :

This is the management layer, and would be akin to a CLI.  This layer will be the access to managing the switch infrastructure provided by other components.   It is not defined what the interface used to manage the switch modules would be, this is up to the management interface itself.  Examples could be sockets, netlink sockets, OVSDB, Command Line Shell, or whichever interface the module would choose to offer to the lower modules.

Protocol :

Protocol is a protocol handler.  This is akin to OSPF, RIP, BGP, etc. for non-human intervention for managing the switch modules.  The protocol management resides on top of the switching modules to control the operation of the switch itself.

Switch modules :

A switch module registers to provide an interface that would manage a switch that forwards packets.  This switch could be a simple L2 switch, L3 Router, or Openflow switch.  Standard interfaces would be provided for these layers.

Status : March 2017

Added the flow library and changed compiling method to be more traditional with a Makefile.

Currently building modules :

Management :

lib_sock_comm.so -- This is a sample socket communications library that provides for management over a socket connection.

 -- This listens on socket 6123 and waits for messages from a client in the format as specified in the app_msg_hdr.   The module will then relay that message to the respective protocol handler. 

 -- Respective file is lib_sock_comm.c

 -- This module launches a separate thread.

lib_cli_comm.so -- This is a very simple command line parser.   This command line parser r

  -- This opens input and listens for full line entries on the shell.   It is a 
 simply a module to relay cli commands to protocol modules.   Protocol modules will register to handle base commands from the cli module, and extended commands.

 -- Base commands are : help exit and quit.

 -- Once a sub protocol module is loaded and registered with the cli, it will show up in the list of available sub helps as ("#" : "Protocol Name".   To see what a sub module supports type "sec '#';help"  Note, spaces are not processed or removed by cli so please don't add any.

 -- Current protocols supporting CLI commands are bridge and flow see their respective sections for the commands they support.

 -- This module launches a separate thread for processing.

Protocols :

 -- Bridge protocol located in lib_bridge.c

 -- This protocol isn't complete, and is intended as a stub to add in support for a learning bridge.   Currently it has base cli functionality and a defined API for switching modules to register and receive protocol events.   

 -- Current API :

       struct br_device {
               void * dev_hdl;
              /* Minimal Bridge Functions */
               int (*br_add_br) (int br_id);
               int (*br_del_br) (int br_id);
               int (*br_add_br_if) (int br_id, int intf_id);
               int (*br_del_br_if) (int br_id, int intf_id);
               int (*br_set_age) (int br_id, int age);
               int (*br_set_max_age) (int br_id, int max_age);
               int (*br_dump) (int br_id);
               int (*br_add_mac) (int br_id, int intf_id, unsigned char mac[ETHER_ALEN]);
               int (*br_del_mac) (int br_id, int intf_id, unsigned char mac[ETHER_ALEN]); 

  -- The current CLI commands supported :

        help -- This help message
        show -- Dump Bridge info 
        load -- Load bridge config from file 
        add-br dev=x br-id=x  -- Add a bridge br_id is integer 
        del-br dev=x br-id=x  -- Delete a bridge br_id is integer 
        add-br-if dev=x br-id=x if-id=x -- Add interface to a bridge br_id int_id 
        del-br-if dev=x br-id=x if-id=x -- Delete interface to a bridge br_id int_id 
        set-age -- dev=x br-id=x age=x  Set aging timer 
      
  -- Current bridge implementaiton does not spawn a thread, but will in future version.

lib_flow.so 

  -- This is the flow protocol module.  This is not a complete implementation.

  -- This module acts as a flow switch, and is intended to be as an openflow like switching module.

  -- Current API

struct flw_device {
        void *dev_hdl;
        int device_id;
        flow_op_add f_add;
        flow_op_lookup f_lookup;
        flow_op_delete f_delete;
        int active; //this is 1 if added and 0 after the device is deleted
        void * priv; //pointer to private data for each device, where the tuples and actions are stored.
        char * name;

}; 

 -- Current cli commands supported 

     help -- This help message 
     show -- Show all available information
     load -- Load rules from a file 
     filter -- Filter command options an actions below 

     Note Filter command may support all fields, but does not mean underlying switching module will support all the commands.

Switching modules :

lib_asic.so 

   -- This is a stub implementation of the lib_bridge module to see if the API calls are working correctly.   It does no further operation than responding to API calls.

lib_flow_stub.so

   -- This is a stub implemenation of the flow protocol.  As with the lib_asic.so it is used to test the API and verify that commands are processed correctly.  It currently does no further functionality.

Main Module :

main.c 

  -- This is the main main program that loads up all the other sub modules, and will link the management with the protocols and the protocols with the switches. 

Status : February 2017

Initial Commit of shell, demonstrating the usage of such a system.


Directory structure 

lib_switch /  -- High level switching infrastructure with main.c loading all the libraries.

client / -- A sample client application that connects through the lib_sock_comm to query the status of the dynamic switching.


Compile :

type Make 

This will build the main application sw, which loads all the dynamic libraries.  It will also build all the dynamic libraries, which are just stubs at the moment.

move into the client directory and build by typing ./compile.sh


Usage :

once compiled, 

set your LD_LIBRARY_PATH by typing : export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./.

just type ./sw   to start the applicaiton, which will search the current directory and pull in all dynamic libraries to initialize the system.

To test the socket communcation go to the client directory, export the LD_LIBRARY_PATH as above, and type ./sw to start and send a query to the main applicaiton.

