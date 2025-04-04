#include "api/Router.h" 

namespace SearchRPI {
    void Router::setupRoutes(crow::SimpleApp& app){
        //get search results
        CROW_ROUTE(app, "/search").methods(crow::HTTPMethod::GET)([](const crow::request& req){
            auto query_params = crow::query_string(req.url_params);
            
            std::string query = query_params.get("query") ? query_params.get("query") : "";
            int start = query_params.get("start") ? std::stoi(query_params.get("start")) : 0;
            int end = query_params.get("end") ? std::stoi(query_params.get("end")) : 10;
            
            //dummy response
            crow::json::wvalue response;
            response["total"] = 2;
            
            response["docs"] = crow::json::wvalue::list({
                crow::json::wvalue({
                    {"id", 1234},
                    {"title", "Website Name"},
                    {"url", "https://website.com"},
                    {"snippet", "Text under link that is relevant for query here"}
                }),
                crow::json::wvalue({
                    {"id", 2346},
                    {"title", "Website Name 2"},
                    {"url", "https://website2.com"},
                    {"snippet", "More relevant text from website"}
                })
            });
            
            return crow::response(200, response);
        });
        
        // Get the logo for a specific document
        CROW_ROUTE(app, "/favicon/<int>").methods(crow::HTTPMethod::GET)([](const crow::request& req, int docid){
            crow::response res;
            res.code = 200;
            res.set_header("Content-Type", "image/png");
            // you'd load and return the favicon image here
            return res;
        });
        
        // Update document information
        CROW_ROUTE(app, "/backend/update").methods(crow::HTTPMethod::PUT)([](const crow::request& req){
            auto body = crow::json::load(req.body);
            if (!body || !body.has("url") || !body.has("title") || !body.has("raw")) return crow::response(400);

            std::string url = body["url"].s();
            std::string title = body["title"].s();
            std::string raw = body["raw"].s();
            int pagerank = body.has("pagerank") ? body["pagerank"].i() : -1;

            int doc_id = 100;

            //dummy response
            crow::json::wvalue response;
            response["total"] = 1;
            response["doc_id"] = doc_id;
            
            return crow::response(200, response);
        });
        
        // Update the favicon for a specific site
        CROW_ROUTE(app, "/backend/update-favicon/<int>").methods(crow::HTTPMethod::PUT)([](const crow::request& req, int docid){

            //dummy response
            return crow::response(200);
        });

        // Get document ID by URL
        CROW_ROUTE(app, "/backend/getdocid/<string>").methods(crow::HTTPMethod::GET)([](const crow::request& req, std::string url){
            //dummy response
            crow::json::wvalue response;
            int doc_id = 100;

            response["status"] = "Success";
            response["doc_id"] = doc_id;
            response["message"] = "Document ID was found";

            return crow::response(200, response);
        });

        // Update PageRank for a document
        CROW_ROUTE(app, "/backend/pagerank/<int>").methods(crow::HTTPMethod::PUT)([](const crow::request& req, int docid){

            //dummy response
            crow::json::wvalue response;
            response["status"] = "Success";
            response["updated"] = 1;
            response["message"] = "Pagerank was updated successfully";

            return crow::response(200, response);
        });

    }
    
}
