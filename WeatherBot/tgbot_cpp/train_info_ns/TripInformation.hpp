#ifndef TGBOT_TRIPINFORMATION_HPP
#define TGBOT_TRIPINFORMATION_HPP

#include <string>
#include <nlohmann/json.hpp>

using nlohmann::json;

class TripInformation {
public:
    /*
     * Methods list:
     * set_starting_point
     * set_destination
     */

private:
    std::string curl_data_;
    json::value_type parsed_data_;
    std::string api_;
    std::string url_;
    std::string city_;
     //NS GET parameter list
    std::string lang; //not required. Language to use for localizing the travel advice. Only a small subset of text is translated, mainly notes. Defaults to Dutch.
    std::string fromStation; //not required. NS station code of the origin station.
    std::string originUicCode; //not required. UIC code of the origin station.
    double originLat; //not required. Latitude of the origin location. Should be used together with originLng. If the origin is a station, just provide the uicCode instead of the lat/lng.
    double originLng; //not required. Longitude of the origin location. Should be used together with originLat. If the origin is a station, just provide the uicCode instead of the lat/lng.


};


#endif //TGBOT_TRIPINFORMATION_HPP
