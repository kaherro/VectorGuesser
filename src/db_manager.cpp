#include "db_manager.h"
#include <stdexcept>

db_manager::db_manager(const std::string &db_path) {
    // open sqlite
}
db_manager::~db_manager() {
    // close sqlite
};

city db_manager::get_random_city() {
    city x; 
    x.id = 1; 
    x.name = "London"; 
    x.latitude = 51.5074;
    x.longitude = -0.1278;
    x.country = "United Kingdom";
    return x;
}

std::optional<city> db_manager::get_city_by_name(const std::string &name) {
    return std::nullopt; 
}

db_manager::vector_geo db_manager::count_vector(const city &from, const city &to) {
    return {0.0, 0.0}; 
}