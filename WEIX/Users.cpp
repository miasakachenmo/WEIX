#pragma once
#include "Users.h"
#include "Func.h"
extern string LastRECORDid;
extern map<int, map<string, BaseUserZYS*> > UserList;
extern vector<string> Products;

//---------------------日期类-------------------------------
DateZYS::DateZYS()
{
	DateName = "";
}
DateZYS::DateZYS(string Name)
{
	DateName = Name;
}
void DateZYS::SetFromString(string a)
{
	Year = atoi(a.substr(0, 4).c_str());
	Month = atoi(a.substr(4, 2).c_str());
	Day = atoi(a.substr(6, 2).c_str());
}
void DateZYS::SetBirthday()
{
	int *MonthDayLimit;
	do
	{
		cout << "输入"+DateName+"年份:" << endl;
		cin >> Year;
		cout << "输入"+DateName+"月份:" << endl;
		cin >> Month;
		cout << "输入"+DateName+"日:" << endl;
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
string DateZYS::GetDateString()
{
	char Res[9];
	sprintf(Res, "%04d%02d%02d",Year, Month, Day);
	return Res;
}
//列表类----------------------------------------------------
int FriendList::CreatRelationShip(BaseUserZYS* Master, string Target_Globalid)
{
	//验证是否已经存在
	string ScureSqt = "SELECT * FROM FRIEND WHERE FROMGB='" + Master->GetGlobalid() + "' AND TOGB='" + Target_Globalid + "';";
	if (Exe(ScureSqt) == 1)
	{
		printf("好友关系已存在!\n");
		return 0;
	}

	//若不存在则调用数据库存进去
	string SqlStr = "INSERT INTO FRIEND(FROMGB,TOGB,PRODUCTCODE)"\
		"VALUES('" + Master->GetGlobalid() + "', '" + Target_Globalid + "', '" + to_string(Master->ProductCode) + "'); ";
	Exe(SqlStr);

	//写入内存
	List[Master->ProductCode].insert(pair<string, string>(Target_Globalid, UserList[Master->ProductCode][Target_Globalid]->Name));
	return 0;
}
int FriendList::GetRelationShip(BaseUserZYS* Master, string Target_Globalid)
{
	//写入内存
	List[Master->ProductCode].insert(pair<string, string>(Target_Globalid, UserList[Master->ProductCode][Target_Globalid]->Name));
	return 0;
}
int FriendList::ShowList(BaseUserZYS* Master, string Target_Globalid)
{
	//UNDONE SHOWLIST
	map<string, string>::iterator iter;
	printf("账号      密码");
	for (iter = List[Master->ProductCode].begin(); iter != List[Master->ProductCode].end(); iter++)
	{
		printf("%s      %s\n",UserList[Master->ProductCode][iter->first]->id.c_str(),iter->second.c_str());
	}
	return 0;
}

//---------------------基类---------------------------------
//通用部分的注册
BaseUserZYS::BaseUserZYS():Birthday("生日"),ReGistDate("注册日")
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
		"VALUES('" + RECORDid + "','" + Global_id + "','" + GBKToUTF8(Name.c_str()) + "','" + Birthday.GetDateString() + "','" + ReGistDate.GetDateString() + "');";
	Exe(Sqlstr);
}
//从文件读取通用部分
BaseUserZYS::BaseUserZYS(char **Attrs) :Birthday("生日"), ReGistDate("注册日") //从数据库中初始化
{
	RECORDid = Attrs[0];
	Global_id = Attrs[1];
	Name = Attrs[2];
	Birthday.SetFromString(Attrs[3]);
	ReGistDate.SetFromString(Attrs[4]);
}
//创建好友关系
int BaseUserZYS::CreatFriendRelationship(string Target_Globalid)
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
	GlobalFriendMap[ProductCode].insert(pair<string, BaseUserZYS*>(Target_Globalid, UserList[ProductCode][Target_Globalid]));
	return 0;
}
//改名
int BaseUserZYS::SetName(string NewName)
{
	Name = NewName;
	string Sqlstr = "UPDATE USERS SET NAME = '" + NewName + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
	return 0;
}
//打印基本信息
int BaseUserZYS::PrintMessage()
{
	printf("%s用户\n昵称:%s\n账号:%s\n生日:%s\n注册日期:%s\n",Products[ProductCode-1].c_str(),Name.c_str(),id.c_str(),Birthday.GetDateString().c_str(),ReGistDate.GetDateString().c_str());
	return 0;
}
//得到全局信息
string BaseUserZYS::GetGlobalid()
{
	return Global_id;
}


//---------------------微信---------------------------------
//微信用户注册
WeChatUserZYS::WeChatUserZYS() :BaseUserZYS()//QQ注册
{
	id = LastWeChatid;
	String_Add(&id);
	//UserList.insert(pair<string,QQUserZYS*>(QQid, this));//加入QQ全局用户
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
}
//从本地读取微信用户
WeChatUserZYS::WeChatUserZYS(char **Attrs):BaseUserZYS(Attrs)
{
	ProductCode = 2;
	id = Attrs[7];
}
//检查登陆
int WeChatUserZYS::LoginCheck() { return 0; }
//从群中被删除
int WeChatUserZYS::DeledFromGroup() { return 0; }
//改变群权限
int WeChatUserZYS::PermissionChange() { return 0; }


//---------------------QQ---------------------------------
//QQ注册
QQUserZYS::QQUserZYS() :BaseUserZYS()
{
	ProductCode = 1;
	id = LastQQid;
	String_Add(&id);
	//UserList.insert(pair<string,QQUserZYS*>(QQid, this));//加入QQ全局用户
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";" + "UPDATE USERS SET PRODUCTCODE = '" + to_string(ProductCode) + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
}
//从文件读取QQ用户
QQUserZYS::QQUserZYS(char **Attrs) :BaseUserZYS(Attrs)
{
	ProductCode = 1;
	id = Attrs[7];
}
//检查登陆
int QQUserZYS::LoginCheck() { return 0; }
//从群中被删除
int QQUserZYS::DeledFromGroup() { return 0; }
//改变群权限
int QQUserZYS::PermissionChange() { return 0; }