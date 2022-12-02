//
// Created by liucxi on 2022/12/2.
//

#include "chat_servlet.h"
#include "json.hpp"
#include <utility>
#include <string>
#include <random>

ChatServlet::ChatServlet() : http::ServletBase("ChatServlet") {
    users_["group"] = std::make_pair("聊天室", "./static/avatar/group.png");
}

bool ChatServlet::connExits(const std::string &id) {
    RWMutex::ReadLock lock(mutex_);
    return conns_.find(id) != conns_.end();
}

std::string ChatServlet::findId(const http::WSConnection::ptr& conn) {
    RWMutex::ReadLock lock(mutex_);
    for (auto & c : conns_) {
        if (c.second == conn) {
            return c.first;
        }
    }
    return "";
}

void ChatServlet::addConn(const std::string &id, http::WSConnection::ptr conn) {
    RWMutex::WriteLock lock(mutex_);
    conns_[id] = std::move(conn);
}

void ChatServlet::addInfo(const std::string &id, const std::string &name, const std::string &avatar) {
    RWMutex::WriteLock lock(mutex_);
    users_[id] = std::make_pair(name, avatar);
}

void ChatServlet::delConn(const std::string &id) {
    RWMutex::WriteLock lock(mutex_);
    conns_.erase(id);
    users_.erase(id);
}

std::pair<std::string, std::string> ChatServlet::getInfo(const std::string &id) {
    RWMutex::ReadLock lock(mutex_);
    auto it = users_.find(id);
    return it == users_.end() ? std::make_pair("", "") : it->second;
}

void ChatServlet::notifyConn(const http::WSFrameMessage::ptr& msg, const http::WSConnection::ptr& filter) {
    RWMutex::ReadLock lock(mutex_);
    for (auto &conn : conns_) {
        if (conn.second == filter) {
            continue;
        }
        conn.second->sendMessage(msg);
    }
}

using std::string;
using std::random_device;
using std::default_random_engine;

string strRand(int length) {			// length: 产生字符串的长度
    char tmp;							// tmp: 暂存一个随机数
    string buffer;						// buffer: 保存返回值

    // 下面这两行比较重要:
    random_device rd;					// 产生一个 std::random_device 对象 rd
    default_random_engine random(rd());	// 用 rd 初始化一个随机数发生器 random

    for (int i = 0; i < length; i++) {
        tmp = (char)(random() % 36);	// 随机一个小于 36 的整数，0-9、A-Z 共 36 种字符
        if (tmp < 10) {			// 如果随机数小于 10，变换成一个阿拉伯数字的 ASCII
            tmp += '0';
        } else {				// 否则，变换成一个大写字母的 ASCII
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return buffer;
}

int ChatServlet::handle(http::HttpRequest::ptr req, http::WSFrameMessage::ptr msg, http::WSConnection::ptr conn) {
    auto data = nlohmann::json::parse(msg->getData().c_str());
    std::string id = findId(conn);

    // 处理异常
    if (data.empty()) {
        if (!id.empty()) {
            delConn(id);
        }
        return 1;
    }

    std::string type = data.at("type");
    if (type == "login_request") {
        nlohmann::json rsp;
        rsp["type"] = "login_response";
        rsp["time"] = std::to_string(time(nullptr));
        rsp["id"] = id;
        std::string name = data.at("name"), avatar = data.at("avatar");
        rsp["name"] = name;
        rsp["avatar"] = avatar;
        addInfo(id, name, avatar);
        conn->sendMessage(std::make_shared<http::WSFrameMessage>(msg->getOpcode(), rsp.dump()));
    } else if (type == "chat_init_request") {
        nlohmann::json rsp;
        rsp["type"] = "chat_init_response";
        rsp["time"] = std::to_string(time(nullptr));
        for (const auto& info : users_) {
            nlohmann::json user;
            user["id"] = info.first;
            user["name"] = info.second.first;
            user["avatar"] = info.second.second;
            rsp["data"].push_back(user);
        }
        conn->sendMessage(std::make_shared<http::WSFrameMessage>(msg->getOpcode(), rsp.dump()));

        auto info = getInfo(id);
        nlohmann::json json;
        json["type"] = "user_change_response";
        json["time"] = std::to_string(time(nullptr));
        json["code"] = "1";
        json["id"] = id;
        json["name"] = info.first;
        json["avatar"] = info.second;
        notifyConn(std::make_shared<http::WSFrameMessage>(http::WSFrameHead::TEXT_FRAME, json.dump()), conn);
    } else if (type == "chat_request") {
        nlohmann::json rsp;
        rsp["type"] = "chat_response";
        rsp["time"] = std::to_string(time(nullptr));
        rsp["from"] = data.at("from");
        rsp["to"] = data.at("to");
        rsp["content"] = data.at("content");
        auto to_conn = conns_[rsp.at("to")];
        to_conn->sendMessage(std::make_shared<http::WSFrameMessage>(msg->getOpcode(), rsp.dump()));
    }
    return 0;
}

int ChatServlet::onConnect(http::HttpRequest::ptr req, http::WSConnection::ptr conn) {
    addConn(strRand(20), conn);
    return 0;
}

int ChatServlet::onClose(http::HttpRequest::ptr req, http::WSConnection::ptr conn) {
    std::string id = findId(conn);
    auto info = getInfo(id);
    delConn(id);
    nlohmann::json json;
    json["type"] = "user_change_response";
    json["time"] = std::to_string(time(nullptr));
    json["code"] = "2";
    json["id"] = id;
    json["name"] = info.first;
    json["avatar"] = info.second;
    notifyConn(std::make_shared<http::WSFrameMessage>(http::WSFrameHead::TEXT_FRAME, json.dump()), conn);
    return 0;
}
