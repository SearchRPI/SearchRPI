#pragma once

#include "crow_all.h"

namespace SearchRPI {
class Router {
public:
    static void setupRoutes(crow::SimpleApp& app);
};
}