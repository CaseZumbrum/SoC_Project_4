#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>

SC_MODULE(tpg_producer) {
    tlm_utils::simple_initiator_socket<tpg_producer> socket;

    SC_HAS_PROCESS(tpg_producer);

    tpg_producer(sc_module_name name) : sc_module(name), socket("socket") {
        SC_THREAD(generate_traffic);
    }

    void generate_traffic() {
        tlm::tlm_generic_payload payload;
        sc_time delay = SC_ZERO_TIME;

        const int width = 780;
        const int height = 610;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Generate a simple RGB pattern (32-bit: 00RRGGBB)
                uint32_t pixel_data = ((((int)((x+y)/5.42)) % 256) << 16) | ((((int)((x+y)/5.42)) % 256) << 8) | (((int)((x+y)/5.42)) % 256);

                payload.set_command(tlm::TLM_WRITE_COMMAND);
                payload.set_address((y * width + x) * 4); // Byte address
                payload.set_data_ptr(reinterpret_cast<unsigned char*>(&pixel_data));
                payload.set_data_length(4);
                payload.set_streaming_width(4);
                payload.set_byte_enable_ptr(0);
                payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

                // Send transaction
                socket->b_transport(payload, delay);

                if (payload.get_response_status() != tlm::TLM_OK_RESPONSE)
                    SC_REPORT_ERROR("TLM", "TPG transaction failed!");
            }
        }
        sc_stop(); // Explicitly tells the kernel to finish
        cout << "TPG: Frame transfer complete at " << sc_time_stamp() << endl;
        
        cout << "All pixels sent. Stopping simulation." << endl;
    }
};








