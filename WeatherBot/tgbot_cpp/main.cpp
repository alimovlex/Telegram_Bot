#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include "fmt/core.h"

#ifndef TGBOT_ENABLE_INLINE_KEYBOARD
#include "bot/bot.hpp"
#else
#include "bot/bot_inline_keyboard/bot_inline_keyboard.hpp"
#endif

#include "exchange_rate/ExchangeRate.hpp"
#include "tgbot/tgbot.h"
#include "weather_forecast/WeatherForecast.hpp"

using namespace std;
using namespace TgBot;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL,"");

    string bot_token_str = "";
    string weather_api_str = "";
#ifndef TGBOT_ENABLE_INLINE_KEYBOARD
    bot bot(bot_token_str, weather_api_str);
#else
    bot_inline_keyboard bot(bot_token_str, weather_api_str);
#endif

    try {
        bot.start();
    } catch (TgBot::TgException& ex) {
        cout << "Error: " << ex.what() << '\n';
    }

    return 0;
}
