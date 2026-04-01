#define int64 opencv_broken_int
#define uint64 opencv_broken_uint
#include <opencv2/opencv.hpp>
#undef int64
#undef uint64


#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>


using namespace cv;

SC_MODULE(tpg_producer) {
    tlm_utils::simple_initiator_socket<tpg_producer> socket;
    tlm_utils::simple_initiator_socket<tpg_producer> ram_socket;

    SC_HAS_PROCESS(tpg_producer);

    tpg_producer(sc_module_name name) : sc_module(name), socket("socket"), ram_socket("ram_socket") {
        SC_THREAD(generate_traffic);
    }

    void generate_traffic() {
        tlm::tlm_generic_payload payload;
        sc_time delay = SC_ZERO_TIME;
        delay += sc_time(10, SC_NS);
        wait(sc_time(10, SC_NS));

        Mat input = imread("images/car.png", IMREAD_COLOR);

        uint16_t width = input.cols;
        int16_t height = input.rows;

        payload.set_command(tlm::TLM_WRITE_COMMAND);
        payload.set_address(0); // Byte address
        payload.set_data_ptr(reinterpret_cast<unsigned char*>(&width));
        payload.set_data_length(2);
        payload.set_streaming_width(2);
        payload.set_byte_enable_ptr(0);
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        // Send transaction
        socket->b_transport(payload, delay);

        if (payload.get_response_status() != tlm::TLM_OK_RESPONSE)
            SC_REPORT_ERROR("TLM", "TPG transaction failed!");

        payload.set_command(tlm::TLM_WRITE_COMMAND);
        payload.set_address(2); // Byte address
        payload.set_data_ptr(reinterpret_cast<unsigned char*>(&height));
        payload.set_data_length(2);
        payload.set_streaming_width(2);
        payload.set_byte_enable_ptr(0);
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        // Send transaction
        socket->b_transport(payload, delay);

        if (payload.get_response_status() != tlm::TLM_OK_RESPONSE)
            SC_REPORT_ERROR("TLM", "TPG transaction failed!");

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Generate a simple RGB pattern (32-bit: 00RRGGBB)
                payload.set_command(tlm::TLM_WRITE_COMMAND);
                payload.set_address((y * width + x) * 3); // Byte address
                payload.set_data_ptr(reinterpret_cast<unsigned char*>(&(input.at<cv::Vec3b>(y,x))));
                payload.set_data_length(3);
                payload.set_streaming_width(3);
                payload.set_byte_enable_ptr(0);
                payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

                // Send transaction
                ram_socket->b_transport(payload, delay);

                if (payload.get_response_status() != tlm::TLM_OK_RESPONSE)
                    SC_REPORT_ERROR("TLM", "TPG transaction failed!");
            }
        }

        // send command to go!!!
        payload.set_command(tlm::TLM_WRITE_COMMAND);
        payload.set_address(4); // Byte address
        payload.set_data_ptr(reinterpret_cast<unsigned char*>(&height));
        payload.set_data_length(2);
        payload.set_streaming_width(2);
        payload.set_byte_enable_ptr(0);
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        // Send transaction
        socket->b_transport(payload, delay);

        sc_stop(); // Explicitly tells the kernel to finish
        cout << "TPG: Frame transfer complete at " << sc_time_stamp() << endl;
        
        cout << "All pixels sent. Stopping simulation." << endl;
    }
};








