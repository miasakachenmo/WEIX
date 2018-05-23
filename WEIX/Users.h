#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<tchar.h>
#include "sqlite3.h"
using namespace std;
class Date
{
public:
	static int MonthDay[12];
	static int LeapMonthDay[12];
	int Year;
	int Month;
	int Day;
	void SetBirthday()
	{
		int *MonthDayLimit;
		do
		{
			cout << "输入出生年份:"<<endl;
			cin >> Year;
			cout << "输入出生月份:"<<endl;
			cin >> Month;
			cout << "输入出生日:"<<endl;
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
		} while (!(Year > 0 && Year <= 2018 && Month > 0 && Month <= 12&&Day>0&&Day<=MonthDayLimit[Month-1]));
	}
};
class BaseUser {
public:
	string Name;//昵称
	string id;//分别到各个产品的号 比如QQ号
	int ProductCode;//表示该用户存在的版本(此处主要用途是虚继承)
	Date Birthday;

	BaseUser()//创建模式  ,  在基类中不执行文件操作 
	{

		cout << "输入昵称";
		cin >> Name;
		Birthday.SetBirthday();
	}
	map<string, int> GlobalFriendMap;//全微X通用的好友列表
	map<string, string> FriendMap;//单个应用的好友列表
	map<string, int> GroupMap;//单个应用的群列表
	virtual int SetName(string NewName) = 0;//设置新名字
	virtual int LoginCheck() = 0;//检查登陆
	virtual int DeledFromGroup() = 0;//从群中被删除
	virtual int PermissionChange() = 0;//改变群权限
	virtual int WriteToFile()=0;//写入文件
	virtual int ReadFromFile() = 0;//读取文件
private:
	string Global_id;//全局ID
	Date SignDay;//注册日(用来计算X龄)
};
class QQUser:virtual BaseUser
{
public:
	static map<string, QQUser*> UserList;
	static vector<int> FriendProductList;
	static int LastQQid;
	//三个存储的共享静态成员
	string QQid;
	QQUser() :BaseUser()//QQ注册
	{
		QQid = LastQQid;
		LastQQid++;
		UserList.insert(pair<string,QQUser*>(QQid, this));//加入QQ全局用户
	}

};