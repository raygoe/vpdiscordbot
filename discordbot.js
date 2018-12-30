const vpsdk = require('node-vpsdk');
const settings = require('./Configuration/bot-configuration.json');
const Discord = require('discord.js');

/////////////////////////////////////////////////////////////////////
// Discord Side
/////////////////////////////////////////////////////////////////////

let av_list =  require('./Configuration/avs-db.json');

let client = new Discord.Client();

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
        if (h.id == settings.discord.webhook_id) {
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
        console.log("Could not find webhook with ID: " + settings.discord.webhook_id);
        process.exit(-1);
    }
}

client.on('ready', () => {
    console.log('Connected to Discord API... :o');
    console.log('Searching for guild id: ' + settings.discord.guild_id);

    client.user.setPresence({
        game: {
            name: 'Blizzard Chat!',
            url: 'https://github.com/raygoe/vpdiscordbot',
            type: "LISTENING"
        },
        status: 'online'
    });
 
    let guildList = client.guilds.array();

    guildList.forEach( g => {
        if (g.id == settings.discord.guild_id) {
            guild = g;
        }
    });
    
    if (guild != undefined) {
        console.log("Found guild. Name: " + guild.name);
    } else {
        console.log("Could not join guild with id: " + settings.discord.guild_id);
        process.exit(-1);
        return;
    }

    getWebhook().then( w => webhook = w);
});

client.on('message', message => {
    let username = message.author.username;
    if (!message.author.bot && (channel_id == message.channel.id || message.guild === null)) {
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

        console.log("[@DISCORD] " + username + ": " + message.content );

        // Oh, and lets delete it if it says "!online".
        if (message.content.substr(0, 7) == "!online") {
            message.delete(1000);
        }
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

client.login(settings.discord.token);

/////////////////////////////////////////////////////////////////////
// VP Side
/////////////////////////////////////////////////////////////////////

async function vpbot() {
    let bots = [];
    bots.push(new vpsdk.Instance());
    bots.push(new vpsdk.Instance());
    bots.push(new vpsdk.Instance());
    bots.push(new vpsdk.Instance());
    bots.push(new vpsdk.Instance());

    let id = 1;
    for (const bot of bots) {
        let init = async id => {
            bot.on("chat", data => {
                console.log("[@VP #" + id + "] " + data.name + ": " + data.message);
            });
        
            bot.on("avatarAdd", data => {
                console.log("[@VP #" + id + "] " + data.name + " entered.");
            });
        
            console.log("+Connect @ " + id);
            await bot.connect("universe.virtualparadise.org", 57000);
            console.log("-Connect @ " + id);
            console.log("+Login @ " + id);
            await bot.login(settings.vp.auth.username, settings.vp.auth.password, settings.vp.bot.name + "#" + id);
            console.log("-Login @ " + id);
            console.log("+Enter @ " + id);
            await bot.enter(settings.vp.bot.world);
            console.log("-Enter @ " + id);
            console.log("+SetAvatar @ " + id);
            bot.setAvatar([0, 0, 0], 0, 0, 0);
            console.log("-SetAvatar @ " + id);
        };
        
        await init(id);
        id += 1;
    }
}

vpbot().catch(error => {
    console.error("[@VP #1] Error: " + error);
})