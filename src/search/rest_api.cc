#include "crow_all.h"

crow::json::wvalue search(const std::string& query){
    crow::json::wvalue response;

    response["query"] = query;
    response["results"] = crow::json::wvalue::list({"Example Result1", "Example Result2"});

    return response;
}

int main() {
    crow::SimpleApp api;

    CROW_ROUTE(api, "/").methods(crow::HTTPMethod::GET)([](const crow::request& req){
        auto query = req.url_params.get("q");
        if (!query) {
            return crow::response(400, "Missing query parameter\n");
        }

        return crow::response(search(query));
    });

    api.port(8080).multithreaded().run();
}