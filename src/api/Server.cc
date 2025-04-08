#include "../../include/api/Server.h"
#include "../../include/api/Router.h"

namespace SearchRPI {
void Server::start() {
  crow::SimpleApp app;
  Router::setupRoutes(app);
  app.port(8080).multithreaded().run();
}
} // namespace SearchRPI
