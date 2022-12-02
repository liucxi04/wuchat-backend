//
// Created by liucxi on 2022/12/1.
//

#include <luwu/reactor.h>
#include <luwu/address.h>
#include <luwu/http/ws_server.h>
#include "json.hpp"
#include "chat_servlet.h"

using namespace luwu;
using json = nlohmann::json;

void chat() {
    http::WSServer::ptr server(new http::WSServer);
    Address::ptr addr = IPv4Address::Create("0.0.0.0", 12345);
    server->bind(addr);
    server->getDispatch()->addExactServlet("/chat", std::make_shared<ChatServlet>());
    server->start();
}

int main() {
    Reactor r("wuchat");
    r.addTask(chat);
    return 0;
}