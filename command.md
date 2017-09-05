## 发送
- 登陆: /0:login*username:xuda;password:123456;
- 注册: /1:signup*username:xuda;password:123456;

- 拉取好友列表: /2:xuda*

- 搜索好友: /5:xuda* OK
- 添加好友请求: /3:xuda1* OK
- 回应是否接受: /3:0xuda*    /3:1xuda*

## 接收
- 返回好友列表: /2:shawd\ndada\n

- 返回搜索到的好友列表：　/5:0   /5:-1
- 收到好友请求: /3:xuda*
- 对方是否接受: /3:2xuda1* 拒绝   /3:３xuda１* 同意 