# pragma  once 
#include "Users.h"
using namespace std;
int Date::MonthDay[12] = { 31,27,31,30,31,30,31,31,30,31,30,31 };
int Date::LeapMonthDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
string BaseUser::LastGlobalid = "888";

int main()
{
	Date a;
	a.SetBirthday();
	cout << a.GetDateString();
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