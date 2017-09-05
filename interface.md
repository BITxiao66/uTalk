#define MAX_LENGTH 20

## main.c
创建登陆界面，注册界面，主界面

OK extern int init_net (const char *server_ip); // 是否连接成功

## signin.c

OK int signin (const char *username, const char *password); // 是否验证成功

## signup.c
OK int signup (const char *username, const char *password);　// 是否注册成功

## chat.c

OK int load_friends_list (const char *username, char friendname[][MAX_LENGTH + 1]);　// 好友数量

OK int send_msg (const char *friendname, const char *msg); // 是否发送成功
OK void recv_msg (const char *friendname, const char *msg);

// 加好友

OK int search_friends (const char *text, char friendname[][MAX_LENGTH + 1]); // 搜索到的好友数量

OK int add_friend_request (const char *friendname); // 是否发送成功　-1 已经是好友了

OK int recv_friend_request (const char *name) // 是否同意

OK void friend_request_response (const char *name, int response) // 1 同意　0 不同意


// 群聊

int create_group (int num, char friendname[][USERNAME_LENGTH + 1], const char *group_name); // 是否重名

void added_to_group (const cha)
