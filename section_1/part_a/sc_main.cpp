#include <systemc.h>
#include "producer.h"
#include "consumer.h"

int sc_main(int argc, char* argv[]) {
    // 1. Instantiate modules
    tpg_producer   *producer = new tpg_producer("producer");
    memory_consumer *consumer = new memory_consumer("consumer");

    // 2. Bind sockets (Initiator to Target)
    producer->socket.bind(consumer->socket);

    // 3. Smart Simulation
    cout << "Simulation started..." << endl;
    sc_start();
    cout << "Simulation finished at " << sc_time_stamp() << endl;

    // 4. Verification: Save the image from the consumer's memory
    consumer->save_image("output_frame.ppm");

    // Cleanup
    delete producer;
    delete consumer;

    return 0;

}
