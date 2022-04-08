#define N 64
#define READ 1 //查询业主信息
#define CHANGE 2 //修改业主信息
#define DELETE 3 //删除业主信息
#define ADD 4 //添加业主信息
#define LOAD 5 //业主申请登陆
#define QUIT 6 //业主退出时发给服务器通知消息
#define SUCCESS 7 //服务器操作成功
#define FAILED 8 //服务器操作失败
/*用户级别宏*/
#define STAFF 10 //小区业主
#define ADM 11 //物业管理人员

typedef struct{
	int type;//判断是否为物业管理员
	char name[N];//名字
	char passwd[N];//登陆密码
	int no;//业主登记编号
}USER;

typedef struct{
	char name[N];//名字
	char addr[N];//业主小区详细住址
	char time_start[N];//停车费年卡办理时间
	char time_end[N];//年卡有效截止日期
	int location;//车库位置编号
	int no;//身份证号
	int card;//车库年卡费用
	char phone[N];//手机号
	int type;//用户级别
	char car_num[N];//车牌号
}INFO;

typedef struct{
	int sign;//标志符判断操作是否成功
	int type;//判断操作类型
	char name[N];//发送消息的用户
	INFO info;//住户信息结构体
	char passwd[N];//业主密码在第一次登陆使用
	char data[N];//操作失败或成功的消息
}MSG;

/*客户端实现添加新用户信息请求的函数接口*/
int do_adduser(int sockfd, MSG *msg);
/*客户端实现删除用户信息请求的函数接口*/
int do_deluser(int sockfd, MSG *msg);
/*客户端实现修改用户信息请求的函数接口*/
int do_modifyuser(int sockfd, MSG *msg);
/*客户端实现查询用户信息请求的函数接口*/
int do_selectuser(int sockfd, MSG *msg);
