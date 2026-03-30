#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include <vector>
#include <fstream>

SC_MODULE(memory_consumer) {
    tlm_utils::simple_target_socket<memory_consumer> socket;

    // Internal memory buffer for 780x610 pixels
    std::vector<uint32_t> mem_buffer;

    SC_HAS_PROCESS(memory_consumer);

    memory_consumer(sc_module_name name) : sc_module(name), socket("socket") {

        socket.register_b_transport(this, &memory_consumer::b_transport);
        mem_buffer.resize(780 * 610, 0);
    }

    // The TLM-2.0 Blocking Transport Implementation
    virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    addr = trans.get_address();
        unsigned char* ptr  = trans.get_data_ptr();
        unsigned int   len  = trans.get_data_length();

        if (cmd == tlm::TLM_WRITE_COMMAND) {
            // Convert byte address to index (assuming 4-byte pixels)
            uint32_t index = addr / 4;
            if (index < mem_buffer.size()) {
                memcpy(&mem_buffer[index], ptr, len);

                // Add an artificial processing delay
                delay += sc_time(10, SC_NS);
                trans.set_response_status(tlm::TLM_OK_RESPONSE);
            } else {
                trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            }
        }
    }

    // Helper to save image for verification
    void save_image(const std::string& filename) {
        std::ofstream ofs(filename, std::ios::binary);
        ofs << "P6\n780 610\n255\n"; // Simple PPM Header
        for (auto pixel : mem_buffer) {
            unsigned char r = (pixel >> 16) & 0xFF;
            unsigned char g = (pixel >> 8) & 0xFF;
            unsigned char b = pixel & 0xFF;
            ofs << r << g << b;
        }
        ofs.close();
        cout << "Consumer: Image saved to " << filename << endl;
    }
};


