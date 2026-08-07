#include <crow.h>
#include <iostream>
#include <db_manager.h>
#include <cmath> 
#include <algorithm>

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
    int attempts_left = 5; 
};
std::mutex sessions_mutex;
std::unordered_map<std::string, session> sessions; 

std::string new_session_id() {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distrib(1, 9999999); 
    return std::to_string(distrib(gen)); 
}

int main() {
    std::unique_ptr<db_manager> db_ptr;
    try {
        db_ptr = std::make_unique<db_manager>("cities.db");
    } 
    catch (const std::exception &e) {
        std::cerr << "Failed to initialize database: " << e.what() << '\n';
        return 1;
    }
    db_manager &db = *db_ptr;

    crow::SimpleApp app;
    CROW_ROUTE(app, "/")
    ([](){
        return "VectorGuesser backend is running!";
    });

    // CROW_ROUTE(app, "/get_city_by_name/").methods(
    //     crow::HTTPMethod::GET)([&db](const crow::request &req){
    //     auto name_ = req.url_params.get("name");
    //     if (!name_) {
    //         return crow::response(400, "Missing 'name' parameter");
    //     }
    //     std::string name(name_);
    //     auto city_opt = db.get_city_by_name(name);
    //     if(!city_opt.has_value()) {
    //         return crow::response(404, "City " + name + " not found in database");
    //     }
    //     auto &city_ = city_opt.value();
    //     crow::json::wvalue json_res;
    //     json_res["id"] = city_.id;
    //     json_res["name"] = city_.name;
    //     json_res["latitude"] = round_to_4(city_.latitude);
    //     json_res["longitude"] = round_to_4(city_.longitude);
    //     json_res["country"] = city_.country;
    //     return crow::response(json_res);
    // });

    CROW_ROUTE(app, "/api/start").methods(
    crow::HTTPMethod::GET)([&db](const crow::request &req){
        try {    
            auto city_from = db.get_random_city();
            crow::json::wvalue json_res;
            json_res["city"]["id"] = city_from.id;
            json_res["city"]["name"] = city_from.name;
            json_res["city"]["latitude"] = round_to_4(city_from.latitude);
            json_res["city"]["longitude"] = round_to_4(city_from.longitude);
            json_res["city"]["country"] = city_from.country;
            std::string session_id = new_session_id(); 
            json_res["session_id"] = session_id; 
            auto city_to = db.get_random_city(); 
            {
                std::lock_guard<std::mutex> lock(sessions_mutex);
                sessions[session_id].target = city_to;
            }
            auto vec = db.count_vector(city_from.name, city_to.name);
            json_res["vector"]["distance_km"] = vec.distance_km; 
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
            return crow::response(400, "Missing 'from' or 'session_id' parameters");
        }
        try {
            std::string from(from_);
            std::string session_id(session_id_);
            std::lock_guard<std::mutex> lock(sessions_mutex);
            if(!sessions.contains(session_id)) {
                return crow::response(404, "Session " + session_id + " doesn't exist or was terminated");
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
                return crow::response(200, "Attempts are over"); 
            }
            db_manager::vector_geo vec = db.count_vector(from, to); 
            crow::json::wvalue json_res;
            json_res["vector"]["distance_km"] = vec.distance_km; 
            json_res["vector"]["angle_deg"] = vec.angle_deg; 
            return crow::response(json_res);
        }
        catch (const std::exception &e) {
            return crow::response(500, crow::json::wvalue{{"error", e.what()}});
        }
    });

    CROW_ROUTE(app, "/api/next_roud").methods(
    crow::HTTPMethod::GET)([&db](const crow::request &req){
        try {    
            auto session_id_ = req.url_params.get("session_id");
            if (!session_id_) {
                return crow::response(400, "Missing 'session_id' parameter");
            }
            std::string session_id = session_id_; 
            if(!sessions.contains(session_id)) {
                return crow::response(404, "Session " + session_id + " doesn't exist or was terminated");
            }
            auto city_from = db.get_random_city();
            crow::json::wvalue json_res;
            json_res["city"]["id"] = city_from.id;
            json_res["city"]["name"] = city_from.name;
            json_res["city"]["latitude"] = round_to_4(city_from.latitude);
            json_res["city"]["longitude"] = round_to_4(city_from.longitude);
            json_res["city"]["country"] = city_from.country;
            json_res["session_id"] = session_id; 
            auto city_to = db.get_random_city(); 
            {
                std::lock_guard<std::mutex> lock(sessions_mutex);
                sessions[session_id].target = city_to;
            }
            auto vec = db.count_vector(city_from.name, city_to.name);
            json_res["vector"]["distance_km"] = vec.distance_km; 
            json_res["vector"]["angle_deg"] = vec.angle_deg; 
            return crow::response(json_res);
        }
        catch (const std::exception &e) {
            return crow::response(500, crow::json::wvalue{{"error", e.what()}});
        }
    });

    // CROW_ROUTE(app, "/get_vector/").methods(
    //     crow::HTTPMethod::GET)([&db](const crow::request &req){
    //     auto from_ = req.url_params.get("from");
    //     auto to_ = req.url_params.get("to");
    //     if (!from_ || !to_) {
    //         return crow::response(400, "Missing 'from' or 'to' parameters");
    //     }
    //     std::string from(from_);
    //     std::string to(to_);
    //     db_manager::vector_geo vec = db.count_vector(from, to); 
    //     crow::json::wvalue json_res;
    //     json_res["distance_km"] = vec.distance_km; 
    //     json_res["angle_deg"] = vec.angle_deg; 
    //     return crow::response(json_res);
    // });

    std::cout << "Server listening on http://0.0.0.0:18080\n";
    app.port(18080).multithreaded().run();
}