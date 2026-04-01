// Based on EEL4930 TLM slide deck

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>


SC_MODULE(Memory)
{
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    
    tlm_utils::multi_passthrough_target_socket<Memory> socket;
    enum { SIZE = 1000*1000*4 };
    char mem[SIZE];
    SC_CTOR(Memory): socket("socket") {
        // Register callback for incoming b_transport interface method call
        socket.register_b_transport(this, &Memory::b_transport);
        // Initialize memory with random data
        for (int i = 0; i < SIZE; i++)
            mem[i] = 0x00;
    }

    // TLM-2 blocking transport method
    virtual void b_transport(int id, tlm::tlm_generic_payload& trans, sc_time& delay ){

        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64 adr = trans.get_address();
        unsigned char* ptr = trans.get_data_ptr();
        unsigned int len = trans.get_data_length();
        unsigned char* byt = trans.get_byte_enable_ptr();
        unsigned int wid = trans.get_streaming_width();
        // Obliged to check address range and check for unsupported features,
        // i.e. byte enables, streaming, and bursts
        // Can ignore DMI hint and extensions
        // Using the SystemC report handler is an acceptable way of signalling an error

        // 100ns wait on memory access
        delay += sc_time(100, SC_NS);
        wait(sc_time(100, SC_NS));

        if (adr >= sc_dt::uint64(SIZE) || len > 4 || wid < len){
            SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");
        }
        
        // Obliged to implement read and write commands
        if ( cmd == tlm::TLM_READ_COMMAND )
            memcpy(ptr, &mem[adr], len);
        else if ( cmd == tlm::TLM_WRITE_COMMAND )
            memcpy(&mem[adr], ptr, len);
        // Obliged to set response status to indicate successful completion
        trans.set_response_status( tlm::TLM_OK_RESPONSE );
    }
};