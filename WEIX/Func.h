#pragma once
#include "Users.h"
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include "sqlite3.h"
using namespace std;
class BaseUserZYS;
#pragma region SQL函数
//SQL回调函数
int callback(void *NotUsed, int argc, char **argv, char **azColName);
//读取时使用的SQL查询参数
int CreatCallBack(void *NotUsed, int argc, char **argv, char **azColName);
//读取好友使用的SQL回调
int GetFriendCallBack(void *NotUsed, int argc, char **argv, char **azColName);
//读取群关系使用的SQL回调函数
int GetGroupCallBack(void *NotUsed, int argc, char **argv, char **azColName);
//用于展示的回调函数
int ShowCallBack(void * NotUsed, int argc, char ** argvs, char ** azColName);

//执行指定SQLITE语句
int Exe(string SqlStr, int(*callbackfunc)(void *, int, char **, char **)=callback);

//打开默认数据库
sqlite3* OpenDb();
#pragma endregion

#pragma region 功能函数

//让一个数字字符串自加1
void String_Add(string *a);
//初始化函数!!!!
int init();
//得到一个符合范围的选项
int GetOption(int Min, int Max);
//得到一个大范围的选项
int GetBigOption(int Min,int Max);
//输入密码提供的不显示效果
string InputPwd();
//将id转换为gb
string idToGlobalid(int ProductCode, string id);
//获取同一GB的用户列表
int GetBindUsers(BaseUserZYS  * User,vector<BaseUserZYS*>  &BindUserList);
//验证群是否存在
string GetCorrectGroup(int ProductCode);

//GBK转UTF8(来源:CSDN)
string GBKToUTF8(const char* strGBK);
//UTF8转GBK(来源:CSDN)
string UTF8ToGBK(const char* strUTF8);

#pragma endregion

#pragma region 菜单封装
//创建用户菜单
int CreatUserView();
//登陆菜单
int LoginView(int ProductCode = 0);
//选择产品菜单
int ChooseProductView();
#pragma endregion


