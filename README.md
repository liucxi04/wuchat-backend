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
