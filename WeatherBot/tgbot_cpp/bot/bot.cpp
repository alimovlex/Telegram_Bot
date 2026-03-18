#include "bot/bot.hpp"
#include "fmt/core.h"
#include "tgbot/TgException.h"
#include "tgbot/types/BotCommand.h"

#include <algorithm>

bot::bot(const std::string& bot_token_str, const std::string& weather_api_str)
    : bot_(bot_token_str), long_poll_(bot_), weather_(weather_api_str) {
    get_weather_city_ = false;
    get_exchange_rate_ = false;

    bot_commands_ = {"start", "help", "weather", "currency"};
    bot_commands_description_ = {"Initialize bot",
                                 "Look what this bot can do",
                                 "Weather forecast by OpenWeatherMap",
                                 "Currency exchange rates"};

    std::vector<TgBot::BotCommand::Ptr> commands;
    std::vector<TgBot::BotCommand::Ptr> vectCmd;

    for (std::size_t x = 0; x != bot_commands_.size(); x++) {
        TgBot::BotCommand::Ptr cmdArray(new TgBot::BotCommand);
        cmdArray->command = bot_commands_[x];
        cmdArray->description = bot_commands_description_[x];
        commands.push_back(cmdArray);
    }

    bot_.getApi().setMyCommands(commands);
    vectCmd = bot_.getApi().getMyCommands();

    for (const auto& cmd : bot_.getApi().getMyCommands()) {
        fmt::print("cmd: {} -> {}\r", cmd->command.c_str(),
                   cmd->description.c_str());
    }
}

void bot::start() {
    start_command();
    help_command();
    weather_command();
    currency_command();
    check_input();

    fmt::print("Bot username: {}\n", bot_.getApi().getMe()->username.c_str());
    bot_.getApi().deleteWebhook();
    while (true) {
        fmt::print("Long poll started\n");
        long_poll_.start();
    }
}

void bot::start_command() {
    bot_.getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(message->chat->id,"Hi! Call /help for more info about commands.");
    });
}

void bot::help_command() {
    bot_.getEvents().onCommand("help", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(
            message->chat->id,
            "I can show weather forecast for your city. Just write /weather"
            "\nI can show the currency exchange rate. Just type /currency");
    });
}

void bot::weather_command() {
    bot_.getEvents().onCommand("weather", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(message->chat->id, "Please enter a city name");

        get_weather_city_ = true;
        long_poll_.start();
        weather_.refresh();

        if (weather_.check_city()) {
            bot_.getApi().sendMessage(
                message->chat->id,
                "Weather in the following town: " + weather_.get_city() + '\n' +
                        weather_.get_forecast() + "\nTemperature " +
                std::to_string(weather_.get_temp()) + "°C\nWind speed: " +
                    weather_.get_wind() + " m/s");
            get_weather_city_ = false;
        } else {
            bot_.getApi().sendMessage(message->chat->id,"Invalid city name entered");
            get_weather_city_ = false;
        }
    });
}

void bot::currency_command() {
    bot_.getEvents().onCommand("currency", [&](TgBot::Message::Ptr message) {
        bot_.getApi().sendMessage(message->chat->id,"Please enter quote currency code relatively base currency EUR.\nFor example: chf. The result is eur/chf");
        get_exchange_rate_ = true;
        long_poll_.start();
        exchange_rate_.refresh();

        if (exchange_rate_.check_currency()) {
            bot_.getApi().sendMessage(message->chat->id, std::to_string(exchange_rate_.get_exchange_rate()));
            get_exchange_rate_ = false;
        } else {
            bot_.getApi().sendMessage(message->chat->id, "Invalid currency code entered");
            get_exchange_rate_ = false;
        }

    });
}

void bot::check_input() {
    bot_.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        if (get_weather_city_) {
            weather_.set_city(message->text);
            return;
        }

        if (get_exchange_rate_) {
            std::transform(message->text.begin(), message->text.end(), message->text.begin(), ::toupper);
            exchange_rate_.set_currency(message->text);
            return;
        }

        for (const auto& command : bot_commands_) {
            if ("/" + command == message->text) {
                return;
            }
        }

        bot_.getApi().sendMessage(message->chat->id,"Invalid command provided. Please enter /help for more info.");
    });
}
