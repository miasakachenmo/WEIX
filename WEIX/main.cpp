#pragma  once 
#include "Users.h"
#include<conio.h>
#include <stdio.h>
#include "Func.h"
using namespace std;
//全局/静态变量区
int Date::MonthDay[12] = { 31,27,31,30,31,30,31,31,30,31,30,31 };
int Date::LeapMonthDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
string BaseUser::LastGlobalid = "888";
string QQUser::LastQQid = "888";
string LastRECORDid ="1";
string WeChatUser::LastWeChatid = "999";
vector<BaseUser*>  UserList;



int main()
{
	string SqlString = "SELECT * FROM USERS";
	Exe(SqlString);
	Exe(SqlString, CreatCallBack);
	
	system("pause");
	while (1)
	{
		char Option;
		printf("1.创建一个QQ用户\n2.创建一个微信用户\n3.退出\n");
		Option = _getch();
		system("cls");
		switch (Option)
		{
		case '1':
		{
			QQUser a;
			break;
		}
		case '2':
		{
			WeChatUser b;
			break;
		}
		case '3':
			goto there;
		default:
			break;
		}
		system("cls");
	}
there:
	system("pause");
	return 0;
}
/*
sqlite3 *db;
char *zErrMsg = 0;
int rc;
string sql;
rc = sqlite3_open("USERS.db", &db);
if (rc)
{
printf("cant open database\n");
}
else
{
printf("open successfully!\n");
}
sql = "INSERT INTO USERS (GLOBAL_ID,NAME,AGE,PRODUCT_CODE,REGIST_DATE) "\
"VALUES('123456','CHENMO',18,1,19991027);";
rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
if (rc != SQLITE_OK) {
fprintf(stderr, "SQL error: %s\n", zErrMsg);
sqlite3_free(zErrMsg);
}
else {
fprintf(stdout, "Records created successfully\n");
}
sqlite3_close(db);
system("pause");
*/