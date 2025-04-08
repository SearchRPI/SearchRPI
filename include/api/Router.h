#pragma once

#include "../external/crow_all.h"

namespace SearchRPI {
class Router {
public:
  static void setupRoutes(crow::SimpleApp &app);
};
} // namespace SearchRPI
