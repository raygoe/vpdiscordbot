# VP Discord Services Bot

This bot is, effectively, a proof of concept (and therefore pretty rough) implementation of a VPSDK forwarded traffic.

It uses a Discord bot that also serves a websocket server and a C++ bot that uses Boost::Beast for a websocket client.

## Configuring the Bots

Firstly, to configure the bot you need to modify the bot-configuration.json file in the Configuration folder.

You will need a Discord Bot API Token. (Look it up). That bot will need MANAGE_WEBHOOKS permissions.

## Building the VP Services Bot

You will need either Linux or Windows x64. You will also need a very new version of CMake (3.10.1).

After that, be sure to install Boost 1.66.0 (you will need the binaries, so either build it or download it pre-built).

Then, be sure to set your BOOST_ROOT environment variable to that directory.

Simply use CMAKE and then use the ALL target and then use the INSTALL target.

After that you will have a bin/ folder in hte VPBot/ directory containing the VP Bot.

You will need to run the Discord bot first, however.

## Running the Discord Bot

First, you will need to install npm and node. After that, you should go to the DiscordBot directory and run npm install.

After that is installed, you should be able to run it by running: node discordbot.js

## Running the VP Bot

Simply go to the VPBot/bin directory and run the binary there.

You should see messages now sync'd between the two. There are several issues (as this is a simple proof of concept implementation.)
However, they are relatively minor ones.