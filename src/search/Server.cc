#include "Server.h"
#include "Router.cc"

SearchRPI::Server::start(){
    crow::SimpleApp app;

    Router::setupRoutes(app);

    app.port(8080).multithreaded().run();
}