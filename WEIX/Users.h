#define _CRT_SECURE_NO_WARNINGS
#pragma  once 
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<sstream>
#include<functional>
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
class BaseGroup;
class GlobalDataZYS
{
public:
	static string LastRECORDid;
	static string LastGlobalid;//最后的全局id
	static string LastQQid;
	static string LastWeChatid;
	static string LastGroupid;
	static vector<string> Products;
	static map<int, map<string, BaseUserZYS*> > UserList;
	static map<string, BaseUserZYS*> QQUserList;
	static map<string, BaseUserZYS*> WeChatUserList;
	static map<int, map<string, BaseGroup*>> Groups;//群们
	static string CurrentUser;
	static string Permissions[];
	static map<string, function<BaseGroup*(char **argv)>> GroupFactory;//GroupType,函数 群工厂
	static map<int, function<int(BaseGroup* Group)>> GroupDecorater;//GroupType,函数 群装饰器
	static string GroupTypeNames[];
};

#pragma endregion

#pragma region 列表接口
//接口函数
class ListOpertion
{

public:
	map<string, string> List;
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid) = 0;
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid) = 0;
	virtual int ShowList(int ProductCode) = 0;
};
class FriendList :public virtual ListOpertion
{
	//含义:<GB,名字>
public:
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int ShowList(int ProductCode);
};
class GroupList :public virtual ListOpertion
{
	//群号 权限
public:
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int ShowList(int ProductCode);
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

#pragma region 菜单操作接口
class MenuInterface
{
public:
	map<string, function<int()>> Menu;//操作名称,函数
	void AddFunc(string FooName,function<int()> Foo);//添加函数
	virtual void CreatMenuMap()=0;
	int ShowFoos();
};
#pragma endregion

#pragma region 用户
class BaseUserZYS :virtual public IBindWithQQUserZYS,virtual public MenuInterface
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
	GroupList Groups;

	//创建模式  ,  在基类中不执行文件操作 
	BaseUserZYS();
	//从数据库中初始化
	BaseUserZYS(char **Attrs);
	//检查登陆 成功返回1 失败返回0
	int LoginCheck();
	//从群中被删除
	virtual int DeledFromGroup() = 0;
	//改变群权限

	//创建群
	virtual void CreatGroup()=0;
	//改
	virtual int PermissionChange() = 0;
	//打印信息
	int PrintMessage();
	//创建好友关系
	int CreatFriendRelationship(string Target_Globalid);
	//设置新名字
	int SetName(string NewName);
	//设置密码,返回值为新密码,返回时已经设置好了
	string SetPwd();
	//得到密码
	bool CheckPwd(string InputPwd);
	//得到全局id(对全局id的保护)
	string GetGlobalid();
	//数据更新时
	int OnUpDate();

	//接口函数
	virtual void CreatMenuMap();

	//友元函数
	friend int CreatUserView();

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
	//创建群的虚函数
	virtual void CreatGroup();



	//从群中被删除
	virtual int DeledFromGroup();
	//改变群权限
	virtual int PermissionChange();
	//菜单
	virtual void CreatMenuMap();

};
class QQUserZYS :public virtual BaseUserZYS,public virtual IStrongBindWithQQUserZYS
{
public:
	//static vector<int> FriendProductList;
	//三个存储的共享静态成员
	QQUserZYS();//QQ注册
	QQUserZYS(char **Attrs);

	virtual void CreatGroup();
	//从群中被删除
	virtual int DeledFromGroup();
	//改变群权限
	virtual int PermissionChange();
	//创建菜单
	virtual void CreatMenuMap();
};
#pragma endregion 

#pragma region 群组
//虚基类
class BaseGroup: virtual public FriendList,virtual public MenuInterface
{
public:
	//list的含义<GB,permissioncode>
	BaseGroup();
	BaseGroup(char **argvs);
	string Groupid;
	string GroupName;
	string GroupType;
	int ProductCode;
	virtual int CreatRelationShip(BaseUserZYS *Target, string PermissionCode);
	virtual int GetRelationShip(BaseUserZYS *Target, string PermissionCode);
	virtual int ShowList(int ProductCode);
	virtual int PermissionCheck(string UserGB,string MinPermission);
	virtual void CreatMenuMap();
	virtual void SetPermissionCode(string TargetGB,string NewCode);
	virtual void SetAdm();
	static BaseGroup* CreatGroup(BaseUserZYS* GroupMaster);
	
};
class  WeChatGroupZYS:virtual public BaseGroup,virtual public MenuInterface
{
public:
	WeChatGroupZYS();
	WeChatGroupZYS(char **argvs);
	virtual void CreatMenuMap();
	//读取群
private:
	
	
};
#pragma endregion 