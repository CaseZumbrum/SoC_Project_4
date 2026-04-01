#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <vector>
#include <fstream>
#include "bit_ops.h"



SC_MODULE(memory_router) {
    // input
    tlm_utils::simple_target_socket<memory_router> in_socket;
    // output mapped with MSB of addr == 0
    tlm_utils::simple_initiator_socket<memory_router> out_socket_one;
    // output mapped with MSB of addr == 1
    tlm_utils::simple_initiator_socket<memory_router> out_socket_two;


    SC_HAS_PROCESS(memory_router);

    memory_router(sc_module_name name) : sc_module(name), in_socket("in_socket"), out_socket_one("out_socket_one"), out_socket_two("out_socket_two") {
        in_socket.register_b_transport(this, &memory_router::b_transport);
    }


    // The TLM-2.0 Blocking Transport Implementation
    virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    addr = trans.get_address();
        unsigned char* ptr  = trans.get_data_ptr();
        unsigned int   len  = trans.get_data_length();

        if (cmd == tlm::TLM_WRITE_COMMAND) {

            // add delay
            delay += sc_time(10, SC_NS);
            wait(sc_time(10, SC_NS));

            // clear MSB bit of transaction, only used for routing
            trans.set_address(clearBit(addr, 63));
            // if MSB is set, goes to consumer 2
            if(checkBit(addr, 63)){
                out_socket_two->b_transport(trans, delay);
            }else{
                out_socket_one->b_transport(trans, delay);
            }
        }
    }

    
};


