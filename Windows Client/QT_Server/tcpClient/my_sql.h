#ifndef MY_SQL_H
#define MY_SQL_H
#include <QString>

class my_sql
{
public:
    my_sql();
    my_sql(const QString &dbName);
    insert(int pm2_5, int pm10);
};

#endif // MY_SQL_H
