#ifndef BOTSETTINGS_HPP
#define BOTSETTINGS_HPP

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "json.hpp"

namespace vpdiscordbot {

struct AuthSection
{
    std::string username;
    std::string password;
};

struct BotSection
{
    std::string name;
    std::string world;
};

struct Settings
{
    AuthSection auth;
    BotSection bot;
};

struct Message {
    bool Malformed = false;
    std::string name;
    std::string message;
};

void from_json(const nlohmann::json& j, Settings& s)
{
    if (j.count("vp") == 0 ||
        j["vp"].count("auth") == 0 ||
        j["vp"].count("bot") == 0 ||
        j["vp"]["auth"].count("username") == 0 ||
        j["vp"]["auth"].count("password") == 0 ||
        j["vp"]["bot"].count("name") == 0 ||
        j["vp"]["bot"].count("world") == 0 )
    {
        throw std::runtime_error("Malformed settings file!");
    }

	s.auth.username = j["vp"]["auth"]["username"].get<std::string>();
    s.auth.password = j["vp"]["auth"]["password"].get<std::string>();
    s.bot.name = j["vp"]["bot"]["name"].get<std::string>();
    s.bot.world = j["vp"]["bot"]["world"].get<std::string>();
}

void from_json(const nlohmann::json& j, Message& m)
{
	try {
		m.name = j.at("name").get<std::string>();
		m.message = j.at("message").get<std::string>();
	}
	catch (std::exception& err) {
		std::cout << "Error parsing message: " << err.what() << std::endl;
		m.Malformed = true;
	}
}

Settings GetSettingsFromFile(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    nlohmann::json j;
    file >> j;
    Settings settings = j;

    return settings;
}

Message GetMessageFromJson(const std::string& message_str)
{
	nlohmann::json j = nlohmann::json::parse(message_str);
    Message message = j;

    return message;
}

}

#endif /* BOTSETTINGS_HPP */