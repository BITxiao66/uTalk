## Fianl UI

- get search text OK
- history text view OK
- search friend margin OK
- text style 
- save file

- group chat
- trans avator
- stiker
- name margin

- signin
- test module

- Second open


## 功能

- 加载后立即做的事情 OK
    - 加载好友列表和离线消息 OK
- 点击某个好友后 OK
    - 如果现在就是他，则忽略 全局变量 OK
    - 切换当前聊天好友，设置label，清屏 OK
- 收到消息　OK
    - 如果是当前好友，则显示 OK
    - 在好友列表中找到，提示消息 OK
- 滚动条 OK
- 发送按钮 OK

- 整体布局
    - right_paned 向上调整 OK
    - 用户头像间隔 OK
    - 好友列表间隔 OK
- 菜单 OK

- 更新好友列表bug OK
- 加好友接口 OK
- 加好友界面更新


- textviev文字样式
- label文字样式

- ui log g_print

- 表情
- 传文件
- 消息记录
- 个人设置
    - 换头像
    - 换密码

## 主界面接口
- 调用别人的
    - 加载好友列表和离线消息 gboolean request_friends_list_to_server (const gchar *username, gint *friends_num, const gchar *friends_list[], gchar *msg_list[]);
    - 选定好友，加载消息　gboolean request_friend_msg_to_server (const gchar *friend_name, const gint *msg_num, const gint *from[], const gchar *msg[])
    - 发送消息　gboolean send_msg_to_server (const gchar *friend_name, const gchar *msg);

    - 搜索好友　gboolean search_friends_to_server (const char *search_string, const gint *friends_num, const gchar *friends_name[]);
    - 发出好友请求　gboolean send_friend_request_to_server(const_gchar *friend_name);

- 别人调用自己的
    - 启动主界面 gboolean chat_ui(const gchar *username);
    - 收到消息　void receive_msg_from_server(const gchar *friend_name, const gchar *msg);
    - 收到好友请求 gboolean receive_friend_request_from_server(const gchar *friend_name);
    - 发出的好友申请被处理 receive_friend_request_response_from_server(gboolean result);

## Monday

- morning
    - 加好友
    - 收发消息
- afternoon
    - 加载好友列表
    - 传送头像（图片）
- evening
    - 传文件
    - 加载最近消息
    - 加载聊天记录

## Tuesday

- 群聊 今晚进一步细化

## Wedesday

- morning
    - 把服务端架在远程服务器
- afternoon
    - 每个人再次整理代码，整理出思维导图
    - 准备答辩演示流程，做PPT
- evening
    - 反复演练答辩流程