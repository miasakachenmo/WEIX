#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include "sqlite3.h"
#include "Users.h"
using namespace std;
//SQL�ص�����
int callback(void *NotUsed, int argc, char **argv, char **azColName);
//��ȡʱʹ�õ�SQL��ѯ����
int CreatCallBack(void *NotUsed, int argc, char **argv, char **azColName);
//��һ�������ַ����Լ�1
void String_Add(string *a);
//��Ĭ�����ݿ�
sqlite3* OpenDb();
//ִ��ָ��SQLITE���
int Exe(string SqlStr, int(*callbackfunc)(void *, int, char **, char **) = callback);
//�����û��˵�
int CreatUserView();