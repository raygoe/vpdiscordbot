const Discord = require('discord.js');
const WebSocket = require('ws');
const fs = require('fs');

let client = new Discord.Client();
const wss = new WebSocket.Server({ port: 7414 });
let ws_client = undefined;

let guild = undefined;
let channel = undefined;

const token = JSON.parse(fs.readFileSync('../Configuration/bot-configuration.json', 'utf8')).discord.token;

async function getWebhook(guild, channel) {
    let hook_collection = await guild.fetchWebhooks();
    let hook = hook_collection.find(h => h.name === channel.name);

    if (hook !== null) {
        return Promise.resolve(hook);
    } else {
        return channel.createWebhook(channel.name, "https://i.imgur.com/BuLE1VA.png");
    }
}

let avatar = "";

client.on('ready', () => {
    console.log('Connected to Discord API... :o');
});

client.on('message', message => {
    console.log(message.content);
    if (ws_client != undefined && !message.author.bot) {
        guild = message.guild;
        channel = message.channel;

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

wss.on('connection', function connection(ws) {
    ws_client = ws;
    console.log("Connected with C++ API...");

    client.login(token);

    ws.send("connected");

    ws.on('message', function incoming(message){
        console.log("From VP: " + message);

        let msg_decoded = JSON.parse(message);
        if (guild == undefined || channel == undefined) {
            return;
        }

        getWebhook(guild, channel)
            .then(webhook => webhook.edit(msg_decoded.name, "https://i.imgur.com/BuLE1VA.png"))
            .then(webhook => {webhook.sendMessage(msg_decoded.message);
                              webhook.edit(channel.name, "https://i.imgur.com/BuLE1VA.png");
                             })
            .catch(console.error);
    });
})