#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <nlohmann/json.hpp>
#include <netinet/in.h>
#include <unistd.h>

#include "Database.h"
#include "index/IDatabase.h"
#include "query-processing/stemmer.h"

using json = nlohmann::json;

constexpr int PORT = 9002;
const std::string DB_PATH = "./testdb";

// Tag to priority mapping
int tag_priority(const std::string& tag) {
    if (tag == "title") return 100;
    if (tag == "h1")    return 80;
    if (tag == "h2")    return 60;
    if (tag == "h3")    return 50;
    if (tag == "p")     return 40;
    if (tag == "div")   return 20;
    if (tag == "span")  return 10;
    return 5;
}

std::mutex db_mutex;
std::mutex docid_mutex;
std::unordered_map<std::string, int> url_to_docId;
int next_docId = 1;

void handle_connection(int client_sock, Database& db) {
    try {
        std::string buffer;
        char chunk[4096];
        ssize_t len;

        while ((len = read(client_sock, chunk, sizeof(chunk))) > 0) {
            buffer.append(chunk, len);
            if (buffer.find('\n') != std::string::npos) break;
        }

        auto parsed = json::parse(buffer);
        std::string url = parsed["url"];
        auto words = parsed["words"];

        // Assign a unique docId per URL
        int docId;
        {
            std::lock_guard<std::mutex> lock(docid_mutex);
            if (url_to_docId.count(url)) {
                docId = url_to_docId[url];
            } else {
                docId = next_docId++;
                url_to_docId[url] = docId;
            }
        }

        std::unordered_set<std::string> seen;

        for (auto& [word, data] : words.items()) {
            std::string stemmed = stemmer::stem(word);
            if (seen.count(stemmed)) continue;
            seen.insert(stemmed);

            std::string tag = data["tag"];
            int priority = tag_priority(tag);

            Data entry = { priority, docId };
            {
                std::lock_guard<std::mutex> lock(db_mutex);
                db.add(stemmed, entry);
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "[!] Error processing connection: " << e.what() << std::endl;
    }

    close(client_sock);
}

int main() {
    Database db(DB_PATH);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "[!] Failed to create socket\n";
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "[!] Failed to bind\n";
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "[!] Failed to listen\n";
        return 1;
    }

    std::cout << "[✓] Stemmer/Indexer listening on port " << PORT << "\n";

    while (true) {
        int client_sock = accept(server_fd, nullptr, nullptr);
        if (client_sock < 0) {
            std::cerr << "[!] Failed to accept\n";
            continue;
        }

        std::thread(handle_connection, client_sock, std::ref(db)).detach();
    }

    close(server_fd);
    return 0;
}

void handle_connection_for_tests(int client_sock, IDatabase& db) {
    try {
        std::string buffer;
        char chunk[4096];
        ssize_t len;

        while ((len = read(client_sock, chunk, sizeof(chunk))) > 0) {
            buffer.append(chunk, len);
            if (buffer.find('\n') != std::string::npos) break;
        }

        auto parsed = json::parse(buffer);
        std::string url = parsed["url"];
        auto words = parsed["words"];

        // Assign a unique docId per URL
        int docId;
        {
            std::lock_guard<std::mutex> lock(docid_mutex);
            if (url_to_docId.count(url)) {
                docId = url_to_docId[url];
            } else {
                docId = next_docId++;
                url_to_docId[url] = docId;
            }
        }

        std::unordered_set<std::string> seen;

        for (auto& [word, data] : words.items()) {
            std::string stemmed = stemmer::stem(word);
            if (seen.count(stemmed)) continue;
            seen.insert(stemmed);

            std::string tag = data["tag"];
            int priority = tag_priority(tag);

            SearchRPI::Data entry = { priority, docId };
            {
                std::lock_guard<std::mutex> lock(db_mutex);
                db.add(stemmed, entry);
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "[!] Error processing connection: " << e.what() << std::endl;
    }

    close(client_sock);
}
