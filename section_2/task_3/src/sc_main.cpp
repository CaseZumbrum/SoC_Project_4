#include <systemc.h>
#include "producer.h"
#include "consumer.h"
#include "memory.h"

int sc_main(int argc, char* argv[]) {
    // 1. Instantiate modules
    tpg_producer   *producer = new tpg_producer("producer");
    memory_consumer *consumer = new memory_consumer("consumer");
    Memory *ram = new Memory("ram");

    // 2. Bind sockets (Initiator to Target)
    producer->socket.bind(consumer->socket);

    producer->ram_socket.bind(ram->socket);
    consumer->ram_socket.bind(ram->socket);

    // 3. Smart Simulation
    cout << "Simulation started..." << endl;
    sc_start();
    cout << "Simulation finished at " << sc_time_stamp() << endl;

    // 4. Verification: Save the image from the consumer's memory
    consumer->save_image("out.png");

    // Cleanup
    delete producer;
    delete consumer;

    return 0;

}
