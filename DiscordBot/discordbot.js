const Discord = require('discord.js');
const WebSocket = require('ws');
const fs = require('fs');

let client = new Discord.Client();
const wss = new WebSocket.Server({ port: 7414 });
let ws_client = undefined;

let guild = undefined;
let channel_id = undefined;
let webhook = undefined;
let mqueue = undefined; // Empty queue.

const discord_config = JSON.parse(fs.readFileSync('../Configuration/bot-configuration.json', 'utf8')).discord;
let av_list = JSON.parse(fs.readFileSync('../Configuration/avs-db.json', 'utf8'));

function setNewAv(user, url) {
    av_list.av[user] = url;
    fs.writeFileSync('../Configuration/avs-db.json', JSON.stringify(av_list), 'utf8');
}

function getAv(user) {
    if (av_list.av[user] == undefined) {
        return "https://i.imgur.com/a2KuqGe.png";
    } else {
        return av_list.av[user];
    }
}

async function getWebhook() {
    let hook_collection = await guild.fetchWebhooks();
    let hook_array = hook_collection.array();
    hook_array.forEach(h => {
        if (h.id == discord_config.webhook_id) {
            webhook = h;
        }
    });

    if (webhook != undefined) {
        channel_id = webhook.channelID;
        console.log("Found webhook w/ ID: " + webhook.id);
        console.log("Found channel w/ ID: " + channel_id);
        mqueue = Promise.resolve(webhook);
        return Promise.resolve(webhook);
    } else {
        console.log("Could not find webhook w/ ID: " + discord_config.webhook_id);
        process.exit(-1);
    }
}

let avatar = "";

client.on('ready', () => {
    console.log('Connected to Discord API... :o');
    console.log('Searching for guild id: ' + discord_config.guild_id);
    
    let guildList = client.guilds.array();

    guildList.forEach( g => {
        if (g.id == discord_config.guild_id) {
            guild = g;
        }
    });
    
    if (guild != undefined) {
        console.log("Found guild. Name: " + guild.name);
    } else {
        console.log("Could not join guild with id: " + discord_config.guild_id);
        process.exit(-1);
        return;
    }

    getWebhook().then( w => webhook = w);
});

client.on('message', message => {
    console.log(message.content);
    let username = message.author.username;
    if (ws_client != undefined && !message.author.bot && channel_id == message.channel.id) {
        // Let's format the message.

        // First, let's extract message embeds.
        for (var attach of message.attachments) {
            if (message.content.length) {
                message.content += " " + attach[1].url;
            } else {
                message.content += "" + attach[1].url;
            }
        }

        // match <@!?123> for users.
        let result, regEx = /<@!?([0-9]+)>/g;
        while (result = regEx.exec(message.content)) {
            let id = result[1], id_match = result[0];
            if (guild.members.get(id) != undefined) {
                message.content = message.content.replace(id_match, "@" + guild.members.get(id).displayName);
            } else {
                message.content = message.content.replace(id_match, "@...");
            }
        }

        // match <#123> for channels.
        regEx = /<#([0-9]+)>/g;
        while (result = regEx.exec(message.content)) {
            let id = result[1], id_match = result[0];
            if (guild.channels.get(id) != undefined) {
                message.content = message.content.replace(id_match, "#" + guild.channels.get(id).name);
            } else {
                message.content = message.content.replace(id_match, "#deleted_channel");
            }
        }

        // match <@&123> for roles.
        regEx = /<@&([0-9]+)>/g;
        while (result = regEx.exec(message.content)) {
            let id = result[1], id_match = result[0];
            if (guild.roles.get(id) != undefined) {
                message.content = message.content.replace(id_match, "@" + guild.roles.get(id).name);
            } else {
                message.content = message.content.replace(id_match, "@unknown_role");
            }
        }

        // match <:.*:123> for custom emoji.
        regEx = /<:([^:]*):[0-9]+>/g;
        while (result = regEx.exec(message.content)) {
            let emoji = result[1], id_match = result[0];
            message.content = message.content.replace(id_match, ":" + emoji + ":");
        }
        username = "[d-" + username + "]";

        // Trim the message for VP.
        if (message.content.length > 253) {
            message.content = message.content.substring(0, 253);
        }

        let msg_to_send = { "name": username, "message": message.content };
        ws_client.send(JSON.stringify(msg_to_send));
    }
});

/**
 * This is super cludge-y, oh well.
 */
client.on('error', err => {
    client.destroy();
    client = new Discord.Client();
    client.login(token);
});

function heartbeat() {
    console.log("[WS] +PONG. :: " + this.isAlive);
    this.isAlive = true;
    console.log("[WS] -PONG. :: " + this.isAlive);
}

wss.on('connection', function connection(ws) {
    ws_client = ws;
    console.log("Connected with C++ API...");

    client.login(discord_config.token);

    ws.send("connected");

    ws.on('pong', heartbeat);

    ws.on('message', function incoming(message){
        console.log("From VP: " + message);

        if (webhook == undefined) {
            return;
        }

        let msg_decoded = {};
        try {
            msg_decoded = JSON.parse(message);
        } catch (err) {
            console.log("Skipping bad mesg");
            return;
        }
        if (mqueue == undefined) {
            console.log("!!!!!");
        }

        // parse set av
        if (msg_decoded.message.substr(0, 11) == ".setav http") {
            setNewAv(msg_decoded.name, "http" + msg_decoded.message.substr(11));
        }

        //lolno
        msg_decoded.message = msg_decoded.message.replace(/^\/me/, "");
        msg_decoded.message = msg_decoded.message.replace(/@[^ ]+/g, "");
        mqueue.then(webhook => webhook.edit(msg_decoded.name, getAv(msg_decoded.name)))
              .then(webhook => webhook.sendMessage(msg_decoded.message)).catch(console.error);
    });
});

const interval = setInterval(() => {
    wss.clients.forEach(ws => {
        console.log("[WS] +PING. :: " + ws.isAlive);
        if (ws.isAlive === false) {
            // Now to terminate them.
            console.log("Lost connection to C++ API...!");
            client.destroy();
            client = new Discord.Client();
            return ws.terminate();
        }

        ws.isAlive = false;
        ws.ping(() => {});
        console.log("[WS] -PING. :: " + ws.isAlive);
    });
}, 30000);
