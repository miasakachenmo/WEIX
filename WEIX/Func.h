#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include "sqlite3.h"
#include "Users.h"
using namespace std;

#pragma region SQL����
//SQL�ص�����
int callback(void *NotUsed, int argc, char **argv, char **azColName);
//��ȡʱʹ�õ�SQL��ѯ����
int CreatCallBack(void *NotUsed, int argc, char **argv, char **azColName);
//��ȡ����ʹ�õ�SQL�ص�
int GetFriendCallBack(void *NotUsed, int argc, char **argv, char **azColName);
//ִ��ָ��SQLITE���
int Exe(string SqlStr, int(*callbackfunc)(void *, int, char **, char **) = callback);
//��Ĭ�����ݿ�
sqlite3* OpenDb();
#pragma endregion

#pragma region ���ܺ���
//��һ�������ַ����Լ�1
void String_Add(string *a);
//��ʼ������!!!!
int init();
//�õ�һ�����Ϸ�Χ��ѡ��
int GetOption(int Min, int Max);
//���������ṩ�Ĳ���ʾЧ��
string InputPwd();

//GBKתUTF8(��Դ:CSDN)
string GBKToUTF8(const char* strGBK);
//UTF8תGBK(��Դ:CSDN)
string UTF8ToGBK(const char* strUTF8);

#pragma endregion

#pragma region �˵���װ
//�����û��˵�
int CreatUserView();
//��½�˵�
int LoginView(int ProductCode = 0);
//ѡ���Ʒ�˵�
int ChooseProductView();
#pragma endregion


