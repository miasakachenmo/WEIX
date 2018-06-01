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
#pragma region 工具类
class DateZYS
{
public:
	static int MonthDay[12];
	static int LeapMonthDay[12];
	//用于判断日期是否正确的静态成员

	int Year;
	int Month;
	int Day;


	string DateName;//日期的名字 比如生日 注册日
	DateZYS();
	DateZYS(string Name);
	void SetFromString(string a);
	void SetBirthday();
	string GetDateString();
};
class BaseUserZYS;
class GlobalDataZYS
{
public:
	static string LastRECORDid;
	static string LastGlobalid;//最后的全局id
	static string LastQQid;
	static string LastWeChatid;
	static vector<string> Products;
	static map<int, map<string, BaseUserZYS*> > UserList;
	static map<string, BaseUserZYS*> QQUserList;
	static map<string, BaseUserZYS*> WeChatUserList;
};

#pragma endregion

#pragma region 列表接口
//接口函数
class ListOpertion
{
public:
	map<int, map<string, string>> List;
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid) = 0;
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid) = 0;
	virtual int ShowList(BaseUserZYS* Master, string Target_Globalid) = 0;
};
class FriendList :public virtual ListOpertion
{
public:
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int ShowList(BaseUserZYS* Master, string Target_Globalid);
};
class GroupList :public virtual ListOpertion
{
public:
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int ShowList(BaseUserZYS* Master, string Target_Globalid);
};


#pragma endregion

#pragma region 绑定接口
class IBindWithQQUserZYS
{
public:
	virtual int BindTo(BaseUserZYS* Master, string Taget_Globalid) = 0;
};
class IWeakBindWithQQUserZYS :virtual public IBindWithQQUserZYS
{
public:
	virtual int BindTo(BaseUserZYS* Master, string Taget_Globalid);
};
class IStrongBindWithQQUserZYS :virtual public IBindWithQQUserZYS
{
public:
	virtual int BindTo(BaseUserZYS* Master, string Taget_Globalid);
};
#pragma endregion

#pragma region 用户
class BaseUserZYS :virtual public	 IBindWithQQUserZYS
{
public:

	map<int, map<string, BaseUserZYS*>> GlobalFriendMap;//全微X通用的好友列表,参数意义:<productcode,<globaoid>>
	map<string, int> GroupMap;//单个应用的群列表

	string RECORDid;//本用户在数据库对应的唯一id
	string Name;//昵称
	string id;//分别到各个产品的号 比如QQ号

	int ProductCode;//表示该用户存在的版本(此处主要用途是虚继承)
	DateZYS Birthday;
	DateZYS ReGistDate;

	FriendList Friends;

	//创建模式  ,  在基类中不执行文件操作 
	BaseUserZYS();
	//从数据库中初始化
	BaseUserZYS(char **Attrs);
	//检查登陆
	virtual bool LoginCheck() = 0;
	//从群中被删除
	virtual int DeledFromGroup() = 0;
	//改变群权限

	virtual int PermissionChange() = 0;
	//打印信息
	int PrintMessage();
	//创建好友关系
	int CreatFriendRelationship(string Target_Globalid);
	//设置新名字
	int SetName(string NewName);
	//设置密码
	string SetPwd();
	//得到密码
	bool CheckPwd(string InputPwd);
	//得到全局id(对全局id的保护)
	string GetGlobalid();
	//数据更新时
	int OnUpDate();
	//友元函数
	friend void CreatUserView();

	friend class IStrongBindWithQQUserZYS;
	friend class IWeakBindWithQQUserZYS;
private:
	string Global_id;//全局ID
	string Pwd;
	DateZYS SignDay;//注册日(用来计算X龄)
};

class WeChatUserZYS :public virtual BaseUserZYS, virtual public IWeakBindWithQQUserZYS
{
public:
	//static vector<int> FriendProductList;
	//三个存储的共享静态成员


	//微信注册
	WeChatUserZYS();
	//从数据库读取微信用户
	WeChatUserZYS(char **Attrs);

	//检查登陆
	virtual bool LoginCheck();
	//从群中被删除
	virtual int DeledFromGroup();
	//改变群权限
	virtual int PermissionChange();
};
class QQUserZYS :public virtual BaseUserZYS,public virtual IStrongBindWithQQUserZYS
{
public:
	//static vector<int> FriendProductList;
	//三个存储的共享静态成员
	QQUserZYS();//QQ注册
	QQUserZYS(char **Attrs);

	//检查登陆
	virtual bool LoginCheck();
	//从群中被删除
	virtual int DeledFromGroup();
	//改变群权限
	virtual int PermissionChange();
};
#pragma endregion
