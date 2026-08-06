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

    CROW_ROUTE(app, "/get_city_by_name/<string>").methods(
        crow::HTTPMethod::GET)([&db](const crow::request &req, const std::string &name){
        std::string name_ = name;
        std::replace(name_.begin(), name_.end(), '_', ' '); 
        auto city_opt = db.get_city_by_name(name_);
        if(!city_opt.has_value()) {
            return crow::response(404, "City " + name_ + " not found in database");
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

    CROW_ROUTE(app, "/get_vector/<string>/<string>").methods(
        crow::HTTPMethod::GET)([&db](const crow::request &req, const std::string &from, const std::string &to){
        std::string from_ = from;
        std::replace(from_.begin(), from_.end(), '_', ' '); 
        std::string to_ = to; 
        std::replace(to_.begin(), to_.end(), '_', ' '); 
        db_manager::vector_geo vec = db.count_vector(from_, to_); 
        crow::json::wvalue json_res;
        json_res["distance_km"] = vec.distance_km; 
        json_res["angle_deg"] = vec.angle_deg; 
        return crow::response(json_res);
    });

    std::cout << "Server listening on http://0.0.0.0:18080\n";
    app.port(18080).multithreaded().run();
}