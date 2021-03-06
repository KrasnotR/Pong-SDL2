//
// Created by krasno on 2020-01-27.
//

#include <discord_rpc.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include "PongRPC.h"
#include "MP.h"

void PongRPC::InitRPC() {
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = HandleDiscordReady;
    handlers.errored = HandleDiscordError;
    handlers.disconnected = HandleDiscordDisconnected;
    handlers.joinGame = HandleDiscordJoinGame;
    handlers.spectateGame = HandleDiscordSpectateGame;
    handlers.joinRequest = HandleDiscordJoinRequest;

    // Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
    Discord_Initialize("671408940243746839", &handlers, 1, "1234");
    First = std::chrono::system_clock::now().time_since_epoch() /
            std::chrono::milliseconds(1);
}

void PongRPC::HandleDiscordReady(const DiscordUser *request) {
    std::cout << "Discord RPC is ready | " << request->username << std::endl;
}

void PongRPC::HandleDiscordError(int errorCode, const char *message) {
    std::cout << "Error mate | " << message << std::endl;
}

void PongRPC::HandleDiscordDisconnected(int errorCode, const char *message) {
    std::cout << "Disconnected mate | " << message << std::endl;
}

void PongRPC::HandleDiscordJoinGame(const char *joinSecret) {
    std::cout << "Someone will join the game | " << joinSecret << std::endl;
}

void PongRPC::HandleDiscordSpectateGame(const char *spectateSecret) {
    std::cout << "Someone wants to spectate the game | " << spectateSecret << std::endl;
}

void PongRPC::HandleDiscordJoinRequest(const DiscordUser *request) {
    std::cout << "Someone wants to join the game | " << request->username << std::endl;
}

void PongRPC::UpdatePresence(int scoreA, int scoreB, MultiplayerMode multiplayerMode)
{
    long now = std::chrono::system_clock::now().time_since_epoch() /
               std::chrono::milliseconds(1);
    if (now - LastUpload > 15000) {
        char buffer[256];
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        if (multiplayerMode == AgainstAI)
            discordPresence.state = "Against AI";
        else if (multiplayerMode == LocalMultiplayer)
            discordPresence.state = "Local Multiplayer";
        else
            discordPresence.state = "Online Multiplayer";
        sprintf(buffer, "Player %d : %d Enemy", scoreA, scoreB);
        discordPresence.details = buffer;
        discordPresence.startTimestamp = First;
        discordPresence.largeImageKey = "cover";
        discordPresence.largeImageText = "Pong";
        discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
        if (multiplayerMode == OnlineMultiplayer)
            discordPresence.partySize = 1;
        else
            discordPresence.partySize = 2;
        discordPresence.partyMax = 2;
        discordPresence.instance = 1;
        Discord_UpdatePresence(&discordPresence);
        LastUpload = now;
    }
}
