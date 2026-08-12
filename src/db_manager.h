#pragma once
#include <string>
#include <vector>
#include <optional>
#include <sqlite3.h>
#include <memory>
#include <unordered_map>

static inline bool difficulty_validation(const std::string &difficulty) {
    return difficulty == "easy" || difficulty == "medium" || difficulty == "hard";
}

struct city {
    long long id{};
    std::string name; 
    double latitude{};
    double longitude{};
    std::string country; 
    std::string difficulty; 
    bool capital; 
};

class db_manager {
public:
    explicit db_manager(const std::string &db_path);

    city get_random_city(const std::string &difficulty, const bool &capitals_only); 
    std::optional<city> get_city_by_name(const std::string &name); 
    std::vector<std::string> get_city_names(const std::string &difficulty, const bool &capitals_only);

    struct vector_geo {
        double distance_km;
        double angle_deg; 
    };
    const vector_geo count_vector(const std::string &from_str, const std::string &to_str);

private:
    struct sqlite3_deleter {
        void operator()(sqlite3* db) const {
            sqlite3_close(db);
        }
    };
    std::unique_ptr<sqlite3, sqlite3_deleter> db;
    void load_all_cities();
    std::unordered_map<std::string, city> name_to_city; 
    std::unordered_map<std::string, std::vector<city>> cities; 
};