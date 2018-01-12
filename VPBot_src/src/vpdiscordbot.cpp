#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>


using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

#include <VPSDK/VP.h>

#include "botsettings.hpp"

using namespace std;

void event_avatar_add(VPInstance sdk);
void event_chat(VPInstance sdk);

struct websocket_client {
    boost::asio::io_context ioc;
    tcp::resolver resolver;
    websocket::stream<tcp::socket> ws;
	std::mutex read_mtx;

	std::vector<vpdiscordbot::Message> messages_to_send;

	bool shouldContinue = true;
	bool connected = true;

	websocket_client() : ioc{}, resolver{ ioc }, ws{ ioc }, messages_to_send{} {
        auto const results = resolver.resolve("localhost", "7414");
		bool connected = false;
		bool upgraded = false;

		while (!connected) {
			try {
				boost::asio::connect(ws.next_layer(), results.begin(), results.end());
				connected = true;
			}
			catch (std::exception& error) {}
		}

		while (!upgraded) {
			ws.handshake("localhost", "/");
			boost::beast::multi_buffer buffer;
			ws.read(buffer);
			std::stringstream ss;
			ss << boost::beast::buffers(buffer.data());
			std::string data = ss.str();
			if (data == std::string("connected")) {
				upgraded = true;
			}
		}
    }

	void close() {
		if (connected) {
			ws.close(websocket::close_code::normal);
			connected = false;
		}
	}

    ~websocket_client() {
		if (connected) {
			close();
		}
    }
};

websocket_client * client = nullptr;

static void websocket_service() {
	while (client->shouldContinue) {
		// Parse websocket messages
		boost::beast::multi_buffer buffer;
		client->ws.read(buffer);

		cout << "From Discord: " << boost::beast::buffers(buffer.data()) << endl;
		std::stringstream ss;
        ss << boost::beast::buffers(buffer.data());
		std::string data = ss.str();
		vpdiscordbot::Message message = vpdiscordbot::GetMessageFromJson(data);
        if (message.Malformed) {
            cout << "Malformed message. Skipping..." << endl;
        } else {
			std::stringstream ss;
			ss << "dcd-" << message.name;
			message.name = ss.str();
			std::lock_guard<std::mutex> lock{client->read_mtx};
			client->messages_to_send.push_back(message);
        }
	}
}

int main(int argc, char ** argv)
{
    vpdiscordbot::Settings settings;
    websocket_client websocket;

    client = &websocket;

	thread ws_service(websocket_service);

	vpdiscordbot::GetMessageFromJson("{\"name\":\"Bob\",\"message\":\"Test!\"}");

    try {
        settings = vpdiscordbot::GetSettingsFromFile("../../Configuration/bot-configuration.json");
    } catch (exception& err) {
        cerr << "ERROR: " << err.what() << endl;
        return -1;
    }

    int err = 0;
    VPInstance sdk;

    if (err = vp_init(VPSDK_VERSION))
    {
        cerr << "ERROR: Couldn't initialize VP API (reason " << err << ")" << endl;
        return -err;
    }

    sdk = vp_create(nullptr);

    if (err = vp_connect_universe(sdk, "universe.virtualparadise.org", 57000))
    {
        cerr << "ERROR: Couldn't connect to universe (reason " << err << ")" << endl;
        return -err;
    }

    if (err = vp_login(sdk, settings.auth.username.c_str(), settings.auth.password.c_str(), settings.bot.name.c_str()))
    {
        cerr << "ERROR: Couldn't login (reason " << err << ")" << endl;
        return -err;
    }

    //if (err = vp_world_setting_set(sdk))

    if (err = vp_enter(sdk, settings.bot.world.c_str()))
    {
        cerr << "ERROR: Couldn't enter " << settings.bot.world << " (reason " << err << ")" << endl;
        return -err;
    } else {
        cout << "Entered world..." << endl;
    }

    vp_event_set(sdk, VP_EVENT_AVATAR_ADD, event_avatar_add);
	vp_event_set(sdk, VP_EVENT_CHAT, event_chat);
    vp_state_change(sdk);

    while (vp_wait(sdk, 100) == 0) {
		std::lock_guard<std::mutex> lock{ websocket.read_mtx };
		for (auto message : websocket.messages_to_send) {
			vp_console_message(sdk, 0, message.name.c_str(), message.message.c_str(), 0, 0, 0, 0);
		}
		websocket.messages_to_send.clear();
	}
	websocket.shouldContinue = false;
	websocket.close();
	ws_service.join();

    return 0;
}

void event_avatar_add(VPInstance sdk)
{
    stringstream ss;
    std::string name(vp_string(sdk, VP_AVATAR_NAME));
	ss << "{ \"name\" : \"" << name << "\", \"message\": \"joined #blizzard.\" }";
	cout << ss.str() << endl;
    client->ws.write(boost::asio::buffer(ss.str()));
}

void event_chat(VPInstance sdk)
{
	stringstream ss;
	std::string name(vp_string(sdk, VP_AVATAR_NAME));
	std::string message(vp_string(sdk, VP_CHAT_MESSAGE));

	if (name.substr(0, 4) == std::string("dcd-")) {
		return; // Ignore discord messages.
	}

	ss << "{ \"name\" : \"" << name << "\", \"message\": \"" << message << "\" }";
	cout << ss.str() << endl;
	client->ws.write(boost::asio::buffer(ss.str()));
}