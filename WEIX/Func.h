#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include "sqlite3.h"
#include "Users.h"
using namespace std;
//SQL回调函数
int callback(void *NotUsed, int argc, char **argv, char **azColName);
//读取时使用的SQL查询参数
int CreatCallBack(void *NotUsed, int argc, char **argv, char **azColName);
//让一个数字字符串自加1
void String_Add(string *a);
//打开默认数据库
sqlite3* OpenDb();
//执行指定SQLITE语句
int Exe(string SqlStr, int(*callbackfunc)(void *, int, char **, char **) = callback);
//创建用户菜单
int CreatUserView();