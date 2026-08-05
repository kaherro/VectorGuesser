#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite3.h>
#include <memory>

struct city {
    long long id{};
    std::string name; 
    double latitude{};
    double longitude{};
    std::string country; 
};

class db_manager {
public:
    explicit db_manager(const std::string &db_path);
    ~db_manager();

    city get_random_city(); 
    std::optional<city> get_city_by_name(const std::string &name); 
    
    struct vector_geo {
        double distance_km;
        double angle_deg; 
    };
    vector_geo count_vector(const city &from, const city &to);
private:
    struct sqlite3_deleter {
        void operator()(sqlite3* db) const {
            sqlite3_close(db);
        }
    };
    std::unique_ptr<sqlite3, sqlite3_deleter> db;
    void exec(const char *sql); 
};