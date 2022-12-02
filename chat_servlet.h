//
// Created by liucxi on 2022/12/2.
//

#ifndef WUCHAT_CHAT_SERVLET_H
#define WUCHAT_CHAT_SERVLET_H

#include <luwu/http/servlet.h>
#include <luwu/utils/mutex.h>

using namespace luwu;

class ChatServlet : public http::ServletBase {
public:
    using ptr = std::shared_ptr<ChatServlet>;

    ChatServlet();

    bool connExits(const std::string &id);

    std::string findId(const http::WSConnection::ptr& conn);

    void addConn(const std::string &id, http::WSConnection::ptr conn);

    void addInfo(const std::string &id, const std::string &name, const std::string &avatar);

    void delConn(const std::string &id);

    std::pair<std::string, std::string> getInfo(const std::string &id);

    void notifyConn(const http::WSFrameMessage::ptr& msg, const http::WSConnection::ptr& filter = nullptr);

    int onConnect(http::HttpRequest::ptr req, http::WSConnection::ptr conn) override;

    int handle(http::HttpRequest::ptr req, http::WSFrameMessage::ptr msg, http::WSConnection::ptr conn) override;

    int onClose(http::HttpRequest::ptr req, http::WSConnection::ptr conn) override;

    int handle(http::HttpRequest::ptr req, http::HttpResponse::ptr rsp, http::HttpConnection::ptr conn) override { return 0; }

private:
    RWMutex mutex_;
    std::unordered_map<std::string, http::WSConnection::ptr> conns_;
    std::unordered_map<std::string, std::pair<std::string, std::string>> users_;
};


#endif //WUCHAT_CHAT_SERVLET_H
