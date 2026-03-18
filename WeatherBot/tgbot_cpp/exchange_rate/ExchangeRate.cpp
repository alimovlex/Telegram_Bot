#include "ExchangeRate.hpp"
#include "network/network.hpp"

#include <iostream>
#include <map>
#include <set>

std::string ExchangeRate::get_currency() {
    return currency_;
}

void ExchangeRate::set_currency(const std::string& currency) {
    currency_ = currency;
}

bool ExchangeRate::check_currency() const {
    try {
        std::set<std::string> currencies;
        std::map<std::string, double> dictionary = std::map<std::string, double>(parsed_data_.at("conversion_rates"));

        for( const auto& i : dictionary ) {
            //std::cout << "key:" << i.first << "->"  << "value:"<< i.second << std::endl;
            currencies.insert(i.first);
        }

        if (currencies.find(currency_) != currencies.end()) {
            return true;
        } else {
            return false;
        }

    } catch (const std::exception&) {
        return false;
    }
}

void ExchangeRate::refresh() {
    curl_data_ = get_request(url_ + base_currency_);
    parsed_data_ = json::parse(curl_data_);
}

float ExchangeRate::get_exchange_rate() const {
    return parsed_data_.at("conversion_rates").at(currency_);
}
