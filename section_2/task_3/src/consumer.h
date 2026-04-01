#define int64 opencv_broken_int
#define uint64 opencv_broken_uint
#include <opencv2/opencv.hpp>
#undef int64
#undef uint64

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <memory.h>
#include <vector>
#include <fstream>

SC_MODULE(memory_consumer) {
    tlm_utils::simple_target_socket<memory_consumer> socket;
    tlm_utils::simple_initiator_socket<memory_consumer> ram_socket;

    // Internal memory buffer for 780x610 pixels
    std::vector<unsigned char> mem_buffer;

    uint16_t width = 512; 
    uint16_t height = 512;

    SC_HAS_PROCESS(memory_consumer);

    memory_consumer(sc_module_name name) : sc_module(name), socket("socket"), ram_socket("ram_socket") {

        socket.register_b_transport(this, &memory_consumer::b_transport);
        mem_buffer.resize(height*width*3, 0);
    }

    // The TLM-2.0 Blocking Transport Implementation
    virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    addr = trans.get_address();
        unsigned char* ptr  = trans.get_data_ptr();
        unsigned int   len  = trans.get_data_length();

        if (cmd == tlm::TLM_WRITE_COMMAND) {

            // 50 ns for any write to consumer
            delay += sc_time(50, SC_NS);
            wait(sc_time(50, SC_NS));

            // addr 0 is the image width
            if(addr == 0){
                memcpy(&width, ptr, len);
                cout << "WIDTH SET "<< width << endl;
                mem_buffer.resize(height*width*3, 0);
                trans.set_response_status(tlm::TLM_OK_RESPONSE);
            }
            // addr 2 is the image height
            else if (addr == 2){
                memcpy(&height, ptr, len);
                cout << "HEIGHT SET "<< height << endl;
                mem_buffer.resize(height*width*3, 0);
                trans.set_response_status(tlm::TLM_OK_RESPONSE);
            }
            // addr 4 is set when the image is ready to be read from ram
            else if(addr == 4){

                // read image from ram
                tlm::tlm_generic_payload payload;
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        // read into buffer
                        payload.set_command(tlm::TLM_READ_COMMAND);
                        payload.set_address((y * width + x) * 3); // Byte address
                        payload.set_data_ptr(reinterpret_cast<unsigned char*>(&mem_buffer[(y * width + x) * 3]));
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

                // perform line detection
                // arbitrary 1000ns wait
                delay += sc_time(1000, SC_NS);
                wait(sc_time(1000, SC_NS));

                // use opencv to find lines
                cv::Mat color_image(height, width, CV_8UC3, mem_buffer.data());
                cv::Mat line_detected;
                calculate_lines(color_image, line_detected);
                // store image in buffer
                memcpy(mem_buffer.data(),line_detected.data, width*height*3);
            }
            
        }
    }

    // Helper to save image for verification
    void save_image(const std::string& filename) {
        cv::Mat color_image(height, width, CV_8UC3, mem_buffer.data());
        imwrite(filename, color_image);
        cout << "Consumer: Image saved to " << filename << endl;
    }

    int calculate_lines(cv::Mat& input, cv::Mat& output){
        // based on https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html
        Mat edge_detected;

        // Load image
        if(input.empty()){
            return -1;
        }

        // Edge detection
        Canny(input, edge_detected, 50, 200, 3);
        // Copy edges to the images that will display the results in BGR
        cvtColor(edge_detected, output, COLOR_GRAY2BGR);

        // Probabilistic Line Transform
        std::vector<cv::Vec4i> linesP;
        HoughLinesP(edge_detected, linesP, 1, CV_PI/180, 100, 50, 10 ); // runs the actual detection
        for( size_t i = 0; i < linesP.size(); i++ )
        {
            Vec4i l = linesP[i];
            line( output, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
        }
        return 0;
    }
};


