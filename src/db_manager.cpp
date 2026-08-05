#include "db_manager.h"
#include <stdexcept>
#include <cmath>
#include <random>
#include <algorithm>

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
    load_all_cities(); 
}

// void db_manager::exec(const char *sql) {
//     char *error_msg = nullptr; 
//     if(sqlite3_exec(db.get(), sql, nullptr, nullptr, &error_msg) != SQLITE_OK) {
//         std::string s = (error_msg ? error_msg : "unknow error");
//         sqlite3_free(error_msg);
//         throw std::runtime_error("SQL Error: " + s); 
//     }
// }

void db_manager::load_all_cities() {
    const char* sql = "SELECT * FROM cities;";
    sqlite3_stmt* raw_stmt = nullptr;
    auto stmt_deleter = [](sqlite3_stmt* s) { sqlite3_finalize(s); };
    if (sqlite3_prepare_v2(db.get(), sql, -1, &raw_stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to read all cities: " + *sqlite3_errmsg(db.get()));
    }
    std::unique_ptr<sqlite3_stmt, decltype(stmt_deleter)> stmt(raw_stmt, stmt_deleter);
    
    while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
        city city_; 
        city_.id = sqlite3_column_int64(stmt.get(), 0);
        city_.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
        city_.latitude = sqlite3_column_double(stmt.get(), 2);
        city_.longitude = sqlite3_column_double(stmt.get(), 3);
        city_.country = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 4));
        cities.push_back(city_);
        std::string name_ = city_.name; 
        std::transform(name_.begin(), name_.end(), name_.begin(), ::tolower); 
        name_to_city[name_] = city_; 
    }
}

city db_manager::get_random_city() {
    if(cities.empty()) {
        throw std::runtime_error("Cities table is empty"); 
    }
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distrib(1, 9999999); 
    return cities[distrib(gen) % (int)cities.size()]; 

    // const char *sql = "SELECT * FROM cities ORDER BY RANDOM() LIMIT 1;";
    // city city_; 

    // sqlite3_stmt* raw_stmt = nullptr;
    // auto stmt_deleter = [](sqlite3_stmt* s) { sqlite3_finalize(s); };
    // if (sqlite3_prepare_v2(db.get(), sql, -1, &raw_stmt, nullptr) != SQLITE_OK) {
    //     throw std::runtime_error("Failed to get random city: " + *sqlite3_errmsg(db.get()));
    // }
    // std::unique_ptr<sqlite3_stmt, decltype(stmt_deleter)> stmt(raw_stmt, stmt_deleter);

    // if(sqlite3_step(stmt.get()) == SQLITE_OK) {
    //     city_.id = sqlite3_column_int64(stmt.get(), 0);
    //     city_.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
    //     city_.latitude = sqlite3_column_double(stmt.get(), 2);
    //     city_.longitude = sqlite3_column_double(stmt.get(), 3);
    //     city_.country = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 4));
    // }
    // else {
    //     sqlite3_finalize(stmt.get());
    //     throw std::runtime_error("City table is empty"); 
    // }
    // sqlite3_finalize(stmt.get());
    // return city_;
}

std::optional<city> db_manager::get_city_by_name(const std::string &name) {
    std::string name_ = name; 
    std::transform(name_.begin(), name_.end(), name_.begin(), ::tolower); 
    if(name_to_city.contains(name)) {
        return name_to_city[name];
    }
    return std::nullopt;
    // const char *sql = "SELECT * FROM cities WHERE LOWER(name) = LOWER(?) LIMIT 1;";
    // city city_; 

    // sqlite3_stmt* raw_stmt = nullptr;
    // auto stmt_deleter = [](sqlite3_stmt* s) { sqlite3_finalize(s); };
    // if (sqlite3_prepare_v2(db.get(), sql, -1, &raw_stmt, nullptr) != SQLITE_OK) {
    //     throw std::runtime_error("Failed to get city by name: " + *sqlite3_errmsg(db.get()));
    // }
    // std::unique_ptr<sqlite3_stmt, decltype(stmt_deleter)> stmt(raw_stmt, stmt_deleter);

    // if(sqlite3_step(stmt.get()) == SQLITE_OK) {
    //     city_.id = sqlite3_column_int64(stmt.get(), 0);
    //     city_.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
    //     city_.latitude = sqlite3_column_double(stmt.get(), 2);
    //     city_.longitude = sqlite3_column_double(stmt.get(), 3);
    //     city_.country = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 4));
    // }
    // else {
    //     sqlite3_finalize(stmt.get());
    //     throw std::runtime_error("City table is empty"); 
    // }
    // sqlite3_finalize(stmt.get());
    // return city_;    
}

static inline double deg_to_rad(double x) {
    return x * M_PI / 180.0; 
}

static inline double rad_to_deg(double x) {
    return x * 180.0 / M_PI; 
}

const db_manager::vector_geo db_manager::count_vector(const city &from, const city &to) {
    double lat1 = deg_to_rad(from.latitude);
    double lat2 = deg_to_rad(to.latitude);
    double lon1 = deg_to_rad(from.longitude);
    double lon2 = deg_to_rad(to.longitude);

    double dlon = lon2 - lon1; 
    double y = std::sin(dlon) * std::cos(lat2);
    double x = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(dlon);
    double angle = rad_to_deg(std::atan2(y, x));
    if(angle < 0.0) angle += 360.0;

    double haversine = std::sin((lat2 - lat1) / 2) * std::sin((lat2 - lat1) / 2) + 
                    std::cos(lat1) * std::cos(lat2) * std::sin(dlon / 2) * std::sin(dlon / 2);
    double distance = 2.0 * 6371.0 * std::asin(std::sqrt(haversine));

    return {distance, angle};
}