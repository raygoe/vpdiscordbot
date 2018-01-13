const Discord = require('discord.js');
const WebSocket = require('ws');
const fs = require('fs');

let client = new Discord.Client();
const wss = new WebSocket.Server({ port: 7414 });
let ws_client = undefined;

let guild = undefined;
let channel_id = undefined;
let webhook = undefined;

const discord_config = JSON.parse(fs.readFileSync('../Configuration/bot-configuration.json', 'utf8')).discord;

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
        console.log("Found webhook w/ ID: " + id);
        console.log("Found channel w/ ID: " + channel_id);
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
    if (ws_client != undefined && !message.author.bot && channel_id == message.channel.id) {
        if (message.content.length > 250) {
            message.content = message.content.substring(0, 250);
        }

        //lolno
        message.content = message.content.replace(/^@[^ ]+/, "");
        message.content = message.content.replace(/\ @[^ ]+/g, "");
        message.author.username = "[d-" + message.author.username + "]";

        let msg_to_send = { "name": message.author.username, "message": message.content };
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

        //lolno
        msg_decoded.message = msg_decoded.message.replace(/^@[^ ]+/, "");
        msg_decoded.message = msg_decoded.message.replace(/\ @[^ ]+/g, "");
        
        webhook.edit(msg_decoded.name, "https://i.imgur.com/a2KuqGe.png")
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