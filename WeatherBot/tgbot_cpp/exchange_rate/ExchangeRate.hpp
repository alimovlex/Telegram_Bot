#ifndef EXCHANGERATE_HPP
#define EXCHANGERATE_HPP

#include <string>
#include <nlohmann/json.hpp>

using nlohmann::json;

class ExchangeRate {
   public:
    std::string get_currency();
    void set_currency(const std::string& currency);
    bool check_currency() const;

    void refresh();

    float get_exchange_rate() const;
    inline const static std::string base_currency_ = "EUR";

   private:
    std::string curl_data_;
    json::value_type parsed_data_;
    std::string currency_;

    inline const static std::string url_ =
        "https://v6.exchangerate-api.com/v6/6f6ab4016827eb9dad11f85c/latest/";
    
};

#endif
