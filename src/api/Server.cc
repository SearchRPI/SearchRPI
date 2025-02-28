#include "api/Server.h"
#include "api/Router.h"

namespace SearchRPI {
    void Server::start(){
#ifndef TEST_MODE  // Only run in production, not in tests
        crow::SimpleApp app;
        Router::setupRoutes(app);
        app.port(8080).multithreaded().run();
#else
        std::cout << "[TEST_MODE] Server start skipped." << std::endl;
#endif
    }   
}