#include <systemc.h>
#include "producer.h"
#include "consumer.h"
#include "router.h"

int sc_main(int argc, char* argv[]) {
    // 1. Instantiate modules
    tpg_producer   *producer = new tpg_producer("producer");
    memory_consumer *consumer_one = new memory_consumer("consumer_one");
    memory_consumer *consumer_two = new memory_consumer("consumer_two");
    memory_router *router = new memory_router("memory_router");


    // 2. Bind sockets (Initiator to Target)
    producer->socket.bind(router->in_socket);

    router->out_socket_one.bind(consumer_one->socket);
    router->out_socket_two.bind(consumer_two->socket);

    // 3. Smart Simulation
    cout << "Simulation started..." << endl;
    sc_start();
    cout << "Simulation finished at " << sc_time_stamp() << endl;

    // 4. Verification: Save the image from the consumer's memory
    consumer_one->save_image("output_frame_one.ppm");
    consumer_two->save_image("output_frame_two.ppm");

    // Cleanup
    delete producer;
    delete consumer_one;
    delete consumer_two;
    delete router;

    return 0;

}
