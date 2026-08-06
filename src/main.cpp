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

    CROW_ROUTE(app, "/get_city_by_name/").methods(
        crow::HTTPMethod::GET)([&db](const crow::request &req){
        auto name_ = req.url_params.get("name");
        if (!name_) {
            return crow::response(400, "Missing 'name' parameter");
        }
        std::string name(name_);
        auto city_opt = db.get_city_by_name(name);
        if(!city_opt.has_value()) {
            return crow::response(404, "City " + name + " not found in database");
        }
        auto &city_ = city_opt.value();
        crow::json::wvalue json_res;
        json_res["id"] = city_.id;
        json_res["name"] = city_.name;
        json_res["latitude"] = round_to_4(city_.latitude);
        json_res["longitude"] = round_to_4(city_.longitude);
        json_res["country"] = city_.country;
        return crow::response(json_res);
    });

    CROW_ROUTE(app, "/get_random_city").methods(
        crow::HTTPMethod::GET)([&db](const crow::request &req){
        auto city_ = db.get_random_city();
        crow::json::wvalue json_res;
        json_res["id"] = city_.id;
        json_res["name"] = city_.name;
        json_res["latitude"] = round_to_4(city_.latitude);
        json_res["longitude"] = round_to_4(city_.longitude);
        json_res["country"] = city_.country;
        return crow::response(json_res);
    });

    CROW_ROUTE(app, "/get_vector/").methods(
        crow::HTTPMethod::GET)([&db](const crow::request &req){
        auto from_ = req.url_params.get("from");
        auto to_ = req.url_params.get("to");
        if (!from_ || !to_) {
            return crow::response(400, "Missing 'from' or 'to' parameters");
        }
        std::string from(from_);
        std::string to(to_);
        db_manager::vector_geo vec = db.count_vector(from, to); 
        crow::json::wvalue json_res;
        json_res["distance_km"] = vec.distance_km; 
        json_res["angle_deg"] = vec.angle_deg; 
        return crow::response(json_res);
    });

    std::cout << "Server listening on http://0.0.0.0:18080\n";
    app.port(18080).multithreaded().run();
}