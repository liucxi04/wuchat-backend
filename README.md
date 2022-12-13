# wuchat-backend

这是网页聊天室的后端代码，前端见 [wuchat-front](https://github.com/liucxi04/wuchat-front)。

主要使用到的技术栈如下：
 - luwu1.0
 - websocket
 
包含以下功能：
 - 登陆
 - 私聊
 - 群聊
 - 未读消息显示
 
本项目为测试我自己实现的高性能网络服务器框架 [luwu](https://github.com/liucxi04/luwu1.0) 而编写，本质上是一个即时聊天室，用户随机选择一个昵称和头像进入聊天室，只可以和当前时刻在线的用户聊天，关闭页面就会断开连接。

本项目未实现持久化功能，不会保存任何用户的消息。

## 项目运行

### 1. 下载并编译 luwu1.0 框架
```shell
git clone git@github.com:liucxi04/luwu1.0.git
cd luwu1.0
sh autobuild.sh
```
### 2. 下载并运行 wuchat-backend
```shell
git clone git@github.com:liucxi04/wuchat-backend.git
cd wuchat-backend
mkdir build
cd build &&
   cmake .. &&
   make
cd ..
cd bin
./wuchat
```

### 3. 最初设计的通信协议，在实际实现过程中有所改动
客户端
| 序号 | 名称         | 协议内容                                                     |
| ---- | ------------ | ------------------------------------------------------------ |
| 1    | 客户端登陆   | {"type" : "login_request","time" :  "2020-05-07 11:34:56""name" : "liucxi","avatar" : "../data/1001.png"} |
| 2    | 聊天室初始化 | {"type" : "chat_init_request","time" :  "2020-05-07 11:34:56"} |
| 3    | 聊天消息     | {"type" : "chat_request","time" :  "2020-05-07 11:34:56""from" : "dafiulbvao""to" : "dvkpvnjrnvipr""content" : "hello world"} |

服务端
| 序号 | 名称             | 协议内容                                                     |
| ---- | ---------------- | ------------------------------------------------------------ |
| 1    | 客户端登陆结果   | {"type" : "login_response","time" :  "2020-05-07 11:34:56""id" : "axcderbrbbfds""name" : "liucxi","avatar" : "../data/1001.png"} |
| 2    | 聊天室初始化结果 | {"type" : "chat_init_response","time" :  "2020-05-07 11:34:56""data" : [{"id" : "axcderbrbbfds""name" : "liucxi","avatar" : "../data/1001.png"}{...}]} |
| 3    | 聊天消息结果     | {"type" : "chat_response","time" :  "2020-05-07 11:34:56""from" : "dafiulbvao""to" : "dvkpvnjrnvipr""content" : "hello world"} |
| 4    | 用户变动消息     | {"type" : "user_change_response","time" :  "2020-05-07 11:34:56""code" : "1""id" : "axcderbrbbfds""name" : "liucxi","avatar" : "../data/1001.png"}1 新增 / 2 删除 |
