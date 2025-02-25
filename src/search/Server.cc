#include "../../include/search/Server.h"
#include "../../include/search/Router.h"

namespace SearchRPI {
    void Server::start(){
        crow::SimpleApp app;
        Router::setupRoutes(app);
        app.port(8080).multithreaded().run();
    }
}