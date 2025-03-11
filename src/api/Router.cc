#include "api/Router.h"

namespace SearchRPI {
    void Router::setupRoutes(crow::SimpleApp& app){

        CROW_ROUTE(app, "/search").methods(crow::HTTPMethod::GET)([](const crow::request& req){
            auto body = crow::json::load(req.body);
            if (!body || !body["query"].s() || !body["start"].s() || !body["end"].s()) {
                return crow::response(400, "Invalid input: query, start, end are required");
            }

            return crow::response(200, "Ok");
        });

        CROW_ROUTE(app, "/favicon/<docid>").methods(crow::HTTPMethod::GET)([](const crow::request& req){
            return crow::response(200, "Ok");
        });

        CROW_ROUTE(app, "/backend/update").methods(crow::HTTPMethod::POST)([](const crow::request& req){
            auto body = crow::json::load(req.body);
            if (!body || !body["url"].s() || !body["title"].s() || !body["raw"].s()) {
                return crow::response(400, "Invalid input: url, title, and raw are required");
            }

            return crow::response(200, "Ok");
        });

        CROW_ROUTE(app, "/backend/update-favicon/<docid>").methods(crow::HTTPMethod::PUT)([](const crow::request& req){
            auto body = crow::json::load(req.body);
            if (!body || !body["favicon"].s()) {
                return crow::response(400, "Invalid input: favicon is required");
            }

            return crow::response(200, "Ok");
        });

        CROW_ROUTE(app, "/backend/getdocid/<url>").methods(crow::HTTPMethod::GET)([](const crow::request& req){
            return crow::response(200, "Ok");
        });

        CROW_ROUTE(app, "/backend/pagerank/<docid>").methods(crow::HTTPMethod::GET)([](const crow::request& req){
            auto body = crow::json::load(req.body);
            if (!body || !body["PageRankScore"].s()) {
                return crow::response(400, "Invalid input: PageRankScore is required");
            }

            return crow::response(200, "Ok");
        });

    }
    
}