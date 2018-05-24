#define _CRT_SECURE_NO_WARNINGS
#pragma  once 
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<sstream>
#include "sqlite3.h"
#include "Func.h"
using namespace std;
class Date
{
public:
	static int MonthDay[12];
	static int LeapMonthDay[12];
	int Year;
	int Month;
	int Day;
	void SetFromString(string a);
	void SetBirthday();
	string GetDateString();
};
class BaseUser {
public:

	map<string, int> GlobalFriendMap;//全微X通用的好友列表,参数意义:<Globalid,ProductCode>
	map<string, string> FriendMap;//单个用户的好友列表 参数意义: <Globalid,ProductCode>
	map<string, int> GroupMap;//单个应用的群列表

	static string LastGlobalid;//最后的全局id
	string RECORDid;//本用户在数据库对应的唯一id
	string Name;//昵称
	string id;//分别到各个产品的号 比如QQ号
	int ProductCode;//表示该用户存在的版本(此处主要用途是虚继承)
	Date Birthday;
	
	Date ReGistDate;


	//创建模式  ,  在基类中不执行文件操作 
	BaseUser();
	//从数据库中初始化
	BaseUser(char **Attrs);
	//设置新名字
	virtual int SetName(string NewName)=0;
	//检查登陆
	virtual int LoginCheck()=0;
	//从群中被删除
	virtual int DeledFromGroup()=0;
	//改变群权限
	virtual int PermissionChange()=0;
	//创建好友关系
	int CreatFriendRelationship(string Target_Globalid);


private:
	string Global_id;//全局ID
	Date SignDay;//注册日(用来计算X龄)
};
class WeChatUser :public virtual BaseUser
{
public:
	//static vector<int> FriendProductList;
	static string LastWeChatid;
	//三个存储的共享静态成员
	
	
	//微信注册
	WeChatUser();
	//从数据库读取微信用户
	WeChatUser(char **Attrs);
	//设置新名字
	virtual int SetName(string NewName);
	//检查登陆
	virtual int LoginCheck();
	//从群中被删除
	virtual int DeledFromGroup();
	//改变群权限
	virtual int PermissionChange();
};
class QQUser :public virtual BaseUser
{
public:
	//static vector<int> FriendProductList;
	static string LastQQid;
	//三个存储的共享静态成员
	QQUser();//QQ注册
	QQUser(char **Attrs);
	//设置新名字
	virtual int SetName(string NewName);
	//检查登陆
	virtual int LoginCheck();
	//从群中被删除
	virtual int DeledFromGroup();
	//改变群权限
	virtual int PermissionChange();
};