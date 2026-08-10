#include <crow.h>
#include <iostream>
#include <db_manager.h>
#include <cmath> 
#include <algorithm>
#include <mutex>
#include <random>
#include <fstream>
#include <sstream>
#include <string>

static inline std::string round_to_4(double x) {
    std::string s = std::to_string(x); 
    std::string ans = ""; 
    int i = 0; 
    while(i < s.length() && s[i] != '.') {
        ans += s[i]; 
        i++; 
    }
    ans += s[i]; 
    i++;
    int j = i; 
    while(j < s.length() && j < std::min(i + 4, (int)s.length())) {
        ans += s[j]; 
        j++; 
    }
    return ans; 
}

struct session {
    city target; 
    int attempts_left = 10; 
    std::string difficulty = "easy"; 
    bool capitals = false; 
};
std::mutex sessions_mutex;
std::unordered_map<std::string, session> sessions; 

std::string new_session_id() {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distrib(1, 9999999); 
    return std::to_string(distrib(gen)); 
}

std::string readFile(const std::string& basePath, const std::string& requestedPath) {
    if (requestedPath.find("..") != std::string::npos) {
        return "";
    }
    std::string fullPath = basePath + "/" + requestedPath;
    std::ifstream file(fullPath, std::ios::in | std::ios::binary);
    if (!file) {
        return "";
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

int main() {
    std::unique_ptr<db_manager> db_ptr;
    try {
        db_ptr = std::make_unique<db_manager>("../data/cities.db");
    } 
    catch (const std::exception &e) {
        std::cerr << "Failed to initialize database: " << e.what() << '\n';
        return 1;
    }
    db_manager &db = *db_ptr;

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([](){
        crow::response res(302); 
        res.set_header("Location", "/index.html");
        return res; 
    });

    CROW_ROUTE(app, "/<string>")
    ([](const crow::request& req, std::string path){
        if (path.empty()) path = "index.html";
        std::string content = readFile("../web", path);
        if (!content.empty()) {
            crow::response res(content);
            return res;
        }
        return crow::response(404);
    });

    CROW_ROUTE(app, "/api/start").methods(
    crow::HTTPMethod::GET)([&db](const crow::request &req){
        try {    
            auto difficulty_ = req.url_params.get("difficulty");
            if(!difficulty_) {
                return crow::response(400, crow::json::wvalue{{"error", "Missing 'difficulty' parameter"}});
            }
            std::string difficulty(difficulty_); 
            if (!difficulty_validation(difficulty)) {
                return crow::response(400, crow::json::wvalue{{"error", "Incorrect 'difficulty' parameter value"}});
            }
            auto capitals_only_ = req.url_params.get("capitals_only");
            if(!capitals_only_) {
                return crow::response(400, crow::json::wvalue{{"error", "Missing 'capitals_only' parameter"}});
            }
            bool capitals_only(std::string(capitals_only_) == "true"); 
            auto city_from = db.get_random_city(difficulty, capitals_only);
            crow::json::wvalue json_res;
            json_res["city"]["id"] = city_from.id;
            json_res["city"]["name"] = city_from.name;
            json_res["city"]["latitude"] = round_to_4(city_from.latitude);
            json_res["city"]["longitude"] = round_to_4(city_from.longitude);
            json_res["city"]["country"] = city_from.country;
            std::string session_id = new_session_id(); 
            json_res["session_id"] = session_id; 
            auto city_to = db.get_random_city(difficulty, capitals_only); 
            while(city_from.name == city_to.name) {
                city_to = db.get_random_city(difficulty, capitals_only); 
            }
            {
                std::lock_guard<std::mutex> lock(sessions_mutex);
                sessions[session_id].target = city_to;
                sessions[session_id].difficulty = difficulty;
                sessions[session_id].capitals = capitals_only;
                json_res["attempts_left"] = sessions[session_id].attempts_left;
            }
            auto vec = db.count_vector(city_from.name, city_to.name);
            json_res["vector"]["distance_km"] = int(vec.distance_km); 
            json_res["vector"]["angle_deg"] = vec.angle_deg; 
            return crow::response(json_res);
        }
        catch (const std::exception &e) {
            return crow::response(500, crow::json::wvalue{{"error", e.what()}});
        }
    });

    CROW_ROUTE(app, "/api/guess").methods(
    crow::HTTPMethod::GET)([&db](const crow::request &req){
        auto from_ = req.url_params.get("from");
        auto session_id_ = req.url_params.get("session_id");
        if (!from_ || !session_id_) {
            return crow::response(400, crow::json::wvalue{{"error", "Missing 'from' or 'session_id' parameters"}});
        }
        try {
            std::string from(from_);
            auto from_opt = db.get_city_by_name(from); 
            if(!from_opt.has_value()) {
                return crow::response(404, crow::json::wvalue{{"error", "City " + from + " not found"}});
            }
            std::string session_id(session_id_);
            std::lock_guard<std::mutex> lock(sessions_mutex);
            if(!sessions.contains(session_id)) {
                return crow::response(404, crow::json::wvalue{{"error", "Session " + session_id + " doesn't exist or was terminated"}});
            }
            std::string to(sessions[session_id].target.name); 
            std::transform(from.begin(), from.end(), from.begin(), ::tolower); 
            std::transform(to.begin(), to.end(), to.begin(), ::tolower); 
            if(from == to) {
                return crow::response(200, "Correct"); 
            }
            sessions[session_id].attempts_left--; 
            if(sessions[session_id].attempts_left <= 0) {
                sessions.erase(session_id);
                if(to.size() > 0 && to[0] >= 'a' && to[0] <= 'z') {
                    to[0] += 'A' - 'a'; 
                } 
                return crow::response(200, "No attempts left. Game over. Target city was: " + to); 
            }
            db_manager::vector_geo vec = db.count_vector(from, to); 
            crow::json::wvalue json_res;

            auto city_opt = db.get_city_by_name(from);
            if(!city_opt.has_value()) {
                throw std::runtime_error(
                    "City " + from + " not found"
                );
            }
            city &city_from = city_opt.value(); 
            json_res["city"]["id"] = city_from.id;
            json_res["city"]["name"] = city_from.name;
            json_res["city"]["latitude"] = round_to_4(city_from.latitude);
            json_res["city"]["longitude"] = round_to_4(city_from.longitude);
            json_res["city"]["country"] = city_from.country;

            json_res["vector"]["distance_km"] = int(vec.distance_km); 
            json_res["vector"]["angle_deg"] = vec.angle_deg; 
            json_res["attempts_left"] = sessions[session_id].attempts_left;
            return crow::response(json_res);
        }
        catch (const std::exception &e) {
            return crow::response(500, crow::json::wvalue{{"error", e.what()}});
        }
    });

    CROW_ROUTE(app, "/api/next_round").methods(
    crow::HTTPMethod::GET)([&db](const crow::request &req){
        try {    
            auto session_id_ = req.url_params.get("session_id");
            if (!session_id_) {
            return crow::response(400, crow::json::wvalue{{"error", "Missing session_id' parameter"}});
            }
            std::string session_id = session_id_; 
            std::string difficulty; 
            bool capitals_only = false; 
            {
                std::lock_guard<std::mutex> lock(sessions_mutex);
                if(!sessions.contains(session_id)) {
                    return crow::response(404, crow::json::wvalue{{"error", "Session " + session_id + " doesn't exist or was terminated"}});
                }
                difficulty = sessions[session_id].difficulty; 
                capitals_only = sessions[session_id].capitals; 
            }
            auto city_from = db.get_random_city(difficulty, capitals_only);
            crow::json::wvalue json_res;
            json_res["city"]["id"] = city_from.id;
            json_res["city"]["name"] = city_from.name;
            json_res["city"]["latitude"] = round_to_4(city_from.latitude);
            json_res["city"]["longitude"] = round_to_4(city_from.longitude);
            json_res["city"]["country"] = city_from.country;
            json_res["session_id"] = session_id; 
            auto city_to = db.get_random_city(difficulty, capitals_only); 
            while(city_from.name == city_to.name) {
                city_to = db.get_random_city(difficulty, capitals_only); 
            }
            {
                std::lock_guard<std::mutex> lock(sessions_mutex);
                sessions[session_id].target = city_to;
                json_res["attempts_left"] = sessions[session_id].attempts_left;
            }
            auto vec = db.count_vector(city_from.name, city_to.name);
            json_res["vector"]["distance_km"] = int(vec.distance_km); 
            json_res["vector"]["angle_deg"] = vec.angle_deg; 
            return crow::response(json_res);
        }
        catch (const std::exception &e) {
            return crow::response(500, crow::json::wvalue{{"error", e.what()}});
        }
    });

    std::cout << "Server listening on http://0.0.0.0:18080\n";
    app.port(18080).bindaddr("0.0.0.0").multithreaded().run();
}