#ifndef WEATHERFORECAST_CPP
#define WEATHERFORECAST_CPP

#include <nlohmann/json.hpp>
#include <string>

using nlohmann::json;

class WeatherForecast {
   public:
    explicit WeatherForecast(const std::string& api);

    std::string get_city() const;
    void set_city(const std::string& city);
    bool check_city() const;

    void refresh();

    std::string get_forecast() const;
    int get_temp() const;
    std::string get_wind() const;

   private:
    std::string curl_data_;
    json::value_type parsed_data_;
    std::string api_;
    std::string url_;
    std::string city_;
};

#endif
