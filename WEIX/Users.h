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
class DateZYS
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
class BaseUserZYS {
public:

	map<int,map<string, BaseUserZYS*>> GlobalFriendMap;//全微X通用的好友列表,参数意义:<Globalid,ProductCode>
	map<string, int> GroupMap;//单个应用的群列表

	static string LastGlobalid;//最后的全局id
	string RECORDid;//本用户在数据库对应的唯一id
	string Name;//昵称
	string id;//分别到各个产品的号 比如QQ号
	int ProductCode;//表示该用户存在的版本(此处主要用途是虚继承)
	DateZYS Birthday;
	
	DateZYS ReGistDate;


	//创建模式  ,  在基类中不执行文件操作 
	BaseUserZYS();
	//从数据库中初始化
	BaseUserZYS(char **Attrs);
	//检查登陆
	virtual int LoginCheck()=0;
	//从群中被删除
	virtual int DeledFromGroup()=0;
	//改变群权限
	virtual int PermissionChange()=0;
	//打印信息
	int PrintMessage();
	//创建好友关系
	int CreatFriendRelationship(string Target_Globalid);
	//设置新名字
	int SetName(string NewName);

	//友元函数
	friend void CreatUserView();
private:
	string Global_id;//全局ID
	DateZYS SignDay;//注册日(用来计算X龄)
};
class WeChatUserZYS :public virtual BaseUserZYS
{
public:
	//static vector<int> FriendProductList;
	static string LastWeChatid;
	//三个存储的共享静态成员
	
	
	//微信注册
	WeChatUserZYS();
	//从数据库读取微信用户
	WeChatUserZYS(char **Attrs);

	//检查登陆
	virtual int LoginCheck();
	//从群中被删除
	virtual int DeledFromGroup();
	//改变群权限
	virtual int PermissionChange();
};
class QQUserZYS :public virtual BaseUserZYS
{
public:
	//static vector<int> FriendProductList;
	static string LastQQid;
	//三个存储的共享静态成员
	QQUserZYS();//QQ注册
	QQUserZYS(char **Attrs);

	//检查登陆
	virtual int LoginCheck();
	//从群中被删除
	virtual int DeledFromGroup();
	//改变群权限
	virtual int PermissionChange();
	//打印信息
};