#include "db_manager.h"
#include <stdexcept>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <format>

inline void log(const std::string &type, const std::string &msg) {
    auto now = std::chrono::system_clock::now();
    auto local_now = std::chrono::current_zone()->to_local(now);
    auto local_seconds = std::chrono::time_point_cast<std::chrono::seconds>(local_now);
    std::string custom = std::format("{:%H:%M:%S}", local_seconds);
    std::cerr << custom << " [" << type << "]: " << msg << '\n';
}

db_manager::db_manager(const std::string &db_path = "cities.db") {
    sqlite3 *raw_db = nullptr; 
    if (sqlite3_open(db_path.c_str(), &raw_db) != SQLITE_OK) {
        const char* error_msg = "unknown error"; 
        if(raw_db) error_msg = sqlite3_errmsg(raw_db); 
        throw std::runtime_error(
            "Error opening database " + db_path + ": " + error_msg
        );
    }
    db.reset(raw_db);
    log("SQL", "Database " + db_path + " was opened successfully"); 
    load_all_cities(); 
}

void db_manager::load_all_cities() {
    const char* sql = "SELECT * FROM cities;";
    sqlite3_stmt* raw_stmt = nullptr;
    auto stmt_deleter = [](sqlite3_stmt* s) { sqlite3_finalize(s); };
    if (sqlite3_prepare_v2(db.get(), sql, -1, &raw_stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to read all cities: " + std::string(sqlite3_errmsg(db.get())));
    }

    std::unique_ptr<sqlite3_stmt, decltype(stmt_deleter)> stmt(raw_stmt, stmt_deleter);

    cities.clear();
    name_to_city.clear();

    while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
        city city_; 
        city_.id = sqlite3_column_int64(stmt.get(), 0);
        city_.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
        city_.latitude = sqlite3_column_double(stmt.get(), 2);
        city_.longitude = sqlite3_column_double(stmt.get(), 3);
        city_.country = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 4));
        city_.difficulty = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 5));
        city_.capital = (sqlite3_column_int64(stmt.get(), 6) != 0); 
        if(city_.capital) {
            if(city_.difficulty == "easy") {
                cities["easy-capitals"].push_back(city_);
                cities["medium-capitals"].push_back(city_);
                cities["hard-capitals"].push_back(city_);
            }
            else if(city_.difficulty == "medium") {
                cities["medium-capitals"].push_back(city_);
                cities["hard-capitals"].push_back(city_);
            }
            else if(city_.difficulty == "hard") {
                cities["hard-capitals"].push_back(city_);
            }
        }
        if(city_.difficulty == "easy") {
            cities["easy"].push_back(city_);
            cities["medium"].push_back(city_);
            cities["hard"].push_back(city_);
        }
        else if(city_.difficulty == "medium") {
            cities["medium"].push_back(city_);
            cities["hard"].push_back(city_);
        }
        else if(city_.difficulty == "hard") {
            cities["hard"].push_back(city_);
        }
        std::string name_ = city_.name; 
        std::transform(name_.begin(), name_.end(), name_.begin(), ::tolower); 
        name_to_city[name_] = city_; 
    }

    if (cities.empty()) {
        throw std::runtime_error("No cities found in database");
    }

    log("SQL", "Cities were loaded successfully"); 
}

city db_manager::get_random_city(const std::string &difficulty, const bool &capitals_only) {
    if(cities.empty()) {
        throw std::runtime_error("Cities table is empty"); 
    }
    if(!difficulty_validation(difficulty)) {
        throw std::runtime_error("Incorrect 'difficulty' value"); 
    }
    const std::vector<city> *pool = nullptr; 
    if(capitals_only) {
        pool = &cities[difficulty + "-capitals"];
    }
    else {
        pool = &cities[difficulty];
    }
    if (pool->empty()) {
        throw std::runtime_error("No cities with difficulty '" + difficulty + "' found");
    }
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distrib(0, (int)pool->size() - 1); 
    return (*pool)[distrib(gen)];
}

std::optional<city> db_manager::get_city_by_name(const std::string &name) {
    std::string name_ = name; 
    std::transform(name_.begin(), name_.end(), name_.begin(), ::tolower); 
    if(name_to_city.contains(name_)) {
        return name_to_city[name_];
    }
    return std::nullopt;
}

std::vector<std::string> db_manager::get_city_names(const std::string &difficulty, const bool &capitals_only) {
    if(cities.empty()) {
        throw std::runtime_error("Cities table is empty");
    }
    if(!difficulty_validation(difficulty)) {
        throw std::runtime_error("Incorrect 'difficulty' value");
    }
    const std::vector<city> *pool = nullptr;
    if(capitals_only) {
        pool = &cities[difficulty + "-capitals"];
    }
    else {
        pool = &cities[difficulty];
    }
    if (pool->empty()) {
        throw std::runtime_error("No cities with difficulty '" + difficulty + "' found");
    }
    std::vector<std::string> names;
    names.reserve(pool->size());
    for (const auto& c : *pool) {
        names.push_back(c.name);
    }
    return names;
}

static inline double deg_to_rad(double x) {
    return x * M_PI / 180.0; 
}

static inline double rad_to_deg(double x) {
    return x * 180.0 / M_PI; 
}

const db_manager::vector_geo db_manager::count_vector(const std::string &from_str, const std::string &to_str) {
    auto from_opt = db_manager::get_city_by_name(from_str); 
    auto to_opt = db_manager::get_city_by_name(to_str); 
    if(!from_opt.has_value()) {
        throw std::runtime_error(
            "City " + from_str + " not found"
        );
    }
    if(!to_opt.has_value()) {
        throw std::runtime_error(
            "City " + to_str + " not found"
        );
    }
    city &from = from_opt.value(), &to = to_opt.value(); 
    double lat1 = deg_to_rad(from.latitude);
    double lat2 = deg_to_rad(to.latitude);
    double lon1 = deg_to_rad(from.longitude);
    double lon2 = deg_to_rad(to.longitude);
    double dlon = lon2 - lon1; 
    while (dlon > M_PI) dlon -= 2.0 * M_PI;
    while (dlon < -M_PI) dlon += 2.0 * M_PI;
    double y1 = std::log(std::tan(M_PI / 4.0 + lat1 / 2.0));
    double y2 = std::log(std::tan(M_PI / 4.0 + lat2 / 2.0));
    double dy = y2 - y1;
    double dx = dlon; 
    double angle = rad_to_deg(std::atan2(dx, dy));
    if (angle < 0.0) angle += 360.0;
    double haversine = std::sin((lat2 - lat1) / 2) * std::sin((lat2 - lat1) / 2) + 
                    std::cos(lat1) * std::cos(lat2) * std::sin(dlon / 2) * std::sin(dlon / 2);
    double distance = 2.0 * 6371.0 * std::asin(std::sqrt(haversine));

    log("INFO", "Distance between " + from_str + " and " + to_str + " is " + 
        std::to_string(distance) + ", Mercator angle is " + std::to_string(angle)); 
    return {distance, angle};
}