#pragma once
#include "Users.h"
#include "Func.h"
extern string LastRECORDid;
extern map<int, map<string, BaseUser*> > UserList;


//---------------------日期类-------------------------------
void Date::SetFromString(string a)
{
	Year = atoi(a.substr(0, 4).c_str());
	Month = atoi(a.substr(4, 2).c_str());
	Day = atoi(a.substr(6, 2).c_str());
}
void Date::SetBirthday()
{
	int *MonthDayLimit;
	do
	{
		cout << "输入出生年份:" << endl;
		cin >> Year;
		cout << "输入出生月份:" << endl;
		cin >> Month;
		cout << "输入出生日:" << endl;
		cin >> Day;
		if (Year % 100 != 0 && Year % 4 == 0)MonthDayLimit = LeapMonthDay;//年份是闰年
		else MonthDayLimit = MonthDay;
		if (!(Year > 0 && Year <= 2018 && Month > 0 && Month <= 12 && Day > 0 && Day <= MonthDayLimit[Month - 1]))
		{
			cout << "输入有误,请重新输入" << endl;
			fflush(stdin);
		}
		else
			return;
	} while (!(Year > 0 && Year <= 2018 && Month > 0 && Month <= 12 && Day>0 && Day <= MonthDayLimit[Month - 1]));
}
string Date::GetDateString()
{
	char Res[9];
	sprintf(Res, "%04d%02d%02d",Year, Month, Day);
	return Res;
}


//---------------------基类---------------------------------
//通用部分的注册
BaseUser::BaseUser()
{
	RECORDid = LastRECORDid;
	String_Add(&LastRECORDid);
	Global_id = LastGlobalid;
	String_Add(&LastGlobalid);
	cout << "输入昵称";
	cin >> Name;
	Birthday.SetBirthday();
	ReGistDate.SetBirthday();
	string Sqlstr = "INSERT INTO USERS (RECORDid,GLOBAL_ID,NAME,BIRTHDAY,REGIST_DATE)"\
		"VALUES('" + RECORDid + "','" + Global_id + "','" + Name + "','" + Birthday.GetDateString() + "','" + ReGistDate.GetDateString() + "');";
	Exe(Sqlstr);
}
//从文件读取通用部分
BaseUser::BaseUser(char **Attrs)//从数据库中初始化
{
	RECORDid = Attrs[0];
	Global_id = Attrs[1];
	Name = Attrs[2];
	Birthday.SetFromString(Attrs[3]);
	ReGistDate.SetFromString(Attrs[4]);
}
//创建好友关系
int BaseUser::CreatFriendRelationship(string Target_Globalid)
{
	string ScureSqt = "SELECT * FROM FRIEND WHERE FROMGB='"+Global_id+"' AND TOGB='"+Target_Globalid+"';";
	if (Exe(ScureSqt) == 1)
	{
		printf("好友关系已存在!\n");
		return 0;
	}
	string SqlStr = "INSERT INTO FRIEND(FROMGB,TOGB,PRODUCTCODE)"\
		"VALUES('" + Global_id + "', '" + Target_Globalid + "', '" + to_string(ProductCode) + "'); ";
	Exe(SqlStr);
	GlobalFriendMap[ProductCode].insert(pair<string, BaseUser*>(Target_Globalid, UserList[ProductCode][Target_Globalid]));
	return 0;
}



//---------------------微信---------------------------------
//微信用户注册
WeChatUser::WeChatUser() :BaseUser()//QQ注册
{
	id = LastWeChatid;
	String_Add(&id);
	//UserList.insert(pair<string,QQUser*>(QQid, this));//加入QQ全局用户
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
}
//从本地读取微信用户
WeChatUser::WeChatUser(char **Attrs):BaseUser(Attrs)
{
	ProductCode = 2;
	id = Attrs[7];
}
//设置新名字
int WeChatUser::SetName(string NewName)
{
	Name = NewName;
	string Sqlstr = "UPDATE USERS SET NAME = '" + NewName + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
	return 0;
}
//检查登陆
int WeChatUser::LoginCheck() { return 0; }
//从群中被删除
int WeChatUser::DeledFromGroup() { return 0; }
//改变群权限
int WeChatUser::PermissionChange() { return 0; }



//---------------------QQ---------------------------------
//QQ注册
QQUser::QQUser() :BaseUser()
{
	ProductCode = 1;
	id = LastQQid;
	String_Add(&id);
	//UserList.insert(pair<string,QQUser*>(QQid, this));//加入QQ全局用户
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";" + "UPDATE USERS SET PRODUCTCODE = '" + to_string(ProductCode) + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
}
//从文件读取QQ用户
QQUser::QQUser(char **Attrs) :BaseUser(Attrs)
{
	ProductCode = 1;
	id = Attrs[7];
}
//设置新名字
int QQUser::SetName(string NewName) {
	Name = NewName;
	string Sqlstr = "UPDATE USERS SET NAME = '" + NewName + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
	return 0;
}
//检查登陆
int QQUser::LoginCheck() { return 0; }
//从群中被删除
int QQUser::DeledFromGroup() { return 0; }
//改变群权限
int QQUser::PermissionChange() { return 0; }