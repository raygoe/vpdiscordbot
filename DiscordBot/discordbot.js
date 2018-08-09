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
let col_list = JSON.parse(fs.readFileSync('../Configuration/cols-db.json', 'utf8'));

function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

/**
 * Converts an HSL color value to RGB. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes h, s, and l are contained in the set [0, 1] and
 * returns r, g, and b in the set [0, 255].
 *
 * @param   {number}  h       The hue
 * @param   {number}  s       The saturation
 * @param   {number}  l       The lightness
 * @return  {Array}           The RGB representation
 */
function hslToRgb(h, s, l){
    var r, g, b;

    if(s == 0){
        r = g = b = l; // achromatic
    }else{
        var hue2rgb = function hue2rgb(p, q, t){
            if(t < 0) t += 1;
            if(t > 1) t -= 1;
            if(t < 1/6) return p + (q - p) * 6 * t;
            if(t < 1/2) return q;
            if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
            return p;
        }

        var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        var p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3);
    }

    return rgbToHex(Math.round(r * 255), Math.round(g * 255), Math.round(b * 255));
}

function setNewAv(user, url) {
    av_list.av[user] = url;
    fs.writeFileSync('../Configuration/avs-db.json', JSON.stringify(av_list), 'utf8');
}

function setNewCol(user, col) {
    col_list.col[user] = col;
    fs.writeFileSync('../Configuration/cols-db.json', JSON.stringify(col_list), 'utf8');
}

function getAv(user) {
    if (av_list.av[user] == undefined) {
        return "https://i.imgur.com/a2KuqGe.png";
    } else {
        return av_list.av[user];
    }
}

function getCol(user) {
    if (col_list.col[user] == undefined) {
        setNewCol(user, hslToRgb(Math.random(), 1, 0.5));
        return col_list.col[user];
    } else {
        return col_list.col[user];
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
        console.log("Could not find webhook with ID: " + discord_config.webhook_id);
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
    if (ws_client != undefined && !message.author.bot && (channel_id == message.channel.id || message.guild === null)) {
        // Let's format the message.
        if (message.guild === null) {
            message.content = "<:PRIV:TAG:>" + message.content;
        }

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

        if (msg_decoded.message.substr(0, 9) == ".setcol #") {
            setNewCol(msg_decoded.name, "#" + msg_decoded.message.substr(9));
        }

        //lolno
        msg_decoded.av = getAv(msg_decoded.name);
        msg_decoded.col = getCol(msg_decoded.name);
        msg_decoded.message = msg_decoded.message.replace(/^\/me/, "");
        msg_decoded.message = msg_decoded.message.replace(/@[^ ]+/g, "");

        msg_decoded.re = new Discord.RichEmbed();
        msg_decoded.re.setAuthor(msg_decoded.name, msg_decoded.av);
        msg_decoded.re.setDescription(msg_decoded.message);

        if (msg_decoded.col != "none") {
            msg_decoded.re.setColor(msg_decoded.col);
        }

        mqueue.then(webhook => webhook.edit(msg_decoded.name, msg_decoded.av))
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

