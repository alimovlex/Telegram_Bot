#include "WeatherForecast.hpp"
#include "network/network.hpp"

#include <iostream>
#include <iomanip>

WeatherForecast::WeatherForecast(const std::string& api) {
    api_ = api;
}

std::string WeatherForecast::get_city() const {
    return parsed_data_.at("name");
}

void WeatherForecast::set_city(const std::string& city) {
    city_ = city;
    std::string city_name;
    for(auto &c : city_) {
        if (std::isspace(c)) {
            city_name.append("%20");
        } else {
            city_name.push_back(c);
        }
    }
    url_ = "http://api.openweathermap.org/data/2.5/weather?q=" + city_name + "&appid=" + api_ + "&units=metric";
}

bool WeatherForecast::check_city() const {
    if (parsed_data_.at("cod").is_number_integer()) {
        return true;
    } else {
        return false;
    }
}

void WeatherForecast::refresh() {
    curl_data_ = get_request(url_);
    parsed_data_ = json::parse(curl_data_);
    //std::cout << parsed_data_;
}

std::string WeatherForecast::get_forecast() const {
    return parsed_data_.at("weather").at(0).at("description");
}

int WeatherForecast::get_temp() const {
    return parsed_data_.at("main").at("temp");
}

std::string WeatherForecast::get_wind() const {
    double wind_speed = parsed_data_.at("wind").at("speed");
    std::string str;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << wind_speed;
    str += ss.str();
    //cout << s << "\n";
    return str;
}
