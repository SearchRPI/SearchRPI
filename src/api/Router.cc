#include "api/Router.h"

namespace SearchRPI {
    void Router::setupRoutes(crow::SimpleApp& app){

        CROW_ROUTE(app, "/search").methods(crow::HTTPMethod::GET)([](const crow::request& req){
            auto query_params = crow::query_string(req.url_params);
            
            std::string query = query_params.get("query") ? query_params.get("query") : "";
            int start = query_params.get("start") ? std::stoi(query_params.get("start")) : 0;
            int end = query_params.get("end") ? std::stoi(query_params.get("end")) : 10;
            
            return crow::response(200);
        });

        // Get the logo for a specific document
        CROW_ROUTE(app, "/favicon/<string>").methods(crow::HTTPMethod::GET)([](const crow::request& req, std::string docid){
            crow::response res;
            res.code = 200;
            res.set_header("Content-Type", "image/png");
            // you'd load and return the favicon image here
            return res;
        });

        // Update document information
        CROW_ROUTE(app, "/backend/update").methods(crow::HTTPMethod::POST)([](const crow::request& req){
            auto body = crow::json::load(req.body);
            if (!body || !body["url"].s() || !body["title"].s() || !body["raw"].s()) return crow::response(400);

            std::string url = body["url"].s();
            std::string title = body["title"].s();
            std::string raw = body["raw"].s();
            int pagerank = body.has("pagerank") ? body["pagerank"].i() : -1;

            return crow::response(200, "docid");
        });

        // Update the favicon for a specific site
        CROW_ROUTE(app, "/backend/update-favicon/<string>").methods(crow::HTTPMethod::PUT)([](const crow::request& req, std::string docid){
            return crow::response(200);
        });

        // Get document ID by URL
        CROW_ROUTE(app, "/backend/getdocid/<string>").methods(crow::HTTPMethod::GET)([](const crow::request& req, std::string url){
            return crow::response(200, "docid");
        });

        // Update PageRank for a document
        CROW_ROUTE(app, "/backend/pagerank/<string>").methods(crow::HTTPMethod::POST)([](const crow::request& req, std::string docid){
            return crow::response(200, "Success");
        });

    }
    
}