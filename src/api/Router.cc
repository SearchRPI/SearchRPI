#include "api/Router.h"

namespace SearchRPI {
    void Router::setupRoutes(crow::SimpleApp& app){
        //TODO: returns ranked search results for a query
        CROW_ROUTE(app, "/search").methods(crow::HTTPMethod::GET)([](const crow::request& req){
            auto query = req.url_params.get("q");
            if (!query) {
                return crow::response(400, "Missing query parameter\n");
            }

            return crow::response(200, "Ok");
        });

        //TODO: adds new document to the index
        CROW_ROUTE(app, "/index").methods(crow::HTTPMethod::POST)([](const crow::request& req){
            return crow::response(200, "Ok");
        });

        //TODO: removes a document from the index
        CROW_ROUTE(app, "/delete").methods(crow::HTTPMethod::DELETE)([](const crow::request& req){
            auto query = req.url_params.get("id");
            if (!query){
                return crow::response(400, "Missing query parameter\n");
            }
            return crow::response(200, "Ok");
        });

        //TODO: returns search engine stats
        CROW_ROUTE(app, "/stats").methods(crow::HTTPMethod::GET)([](const crow::request& req){
            return crow::response(200, "Ok");
        });

        //TODO: checks if service is running
        CROW_ROUTE(app, "/health").methods(crow::HTTPMethod::GET)([](const crow::request& req){
            return crow::response(200, "Ok");
        });

    }

    
}
