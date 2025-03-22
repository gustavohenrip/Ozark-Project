#include "stdafx.hpp"
#include "utils/utils.hpp"
#include "networking/sockets.hpp"
#include "networking/handler/handler.hpp"

int main() {
    // Seed rand
    srand((uint32_t)std::time(0));

    // Initialize socket port
    Network::GetServerSocket() = Network::Socket((uint16_t)PORT);

    // Start the client listener
    Utils::CreateThread((void*)Network::Handler::ClientListener, nullptr);

#ifndef NEW_SERVER
    // Start the heartbeat handler
    Utils::CreateThread((void*)Network::Handler::HeartbeatListener, nullptr);

#endif

    // Infinite loop to keep the process running
    while (true) {
        sleep(10);
    }

    return 0;
}