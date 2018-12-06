#include "my_sql.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
my_sql::my_sql()
{

}
my_sql::my_sql(const QString &dbName)
{
  QSqlDatabase database;
  if (QSqlDatabase::contains("qt_sql_default_connection"))
  {
      database = QSqlDatabase::database("qt_sql_default_connection");
  }
  else
  {
      QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
      database.setDatabaseName(dbName);
      database.setUserName("root");
      database.setPassword("123456");
      if (!database.open()) {
            qDebug() << "Database Error!";

        }else{
         QSqlQuery sql_query;

         QString create_sql = "create table student (time int primary key, PM2_5 int, PM10 int)";

         QString update_sql = "update student set name = :name where id = :id";
         QString select_sql = "select id, name from student";
         QString select_all_sql = "select * from student";
         QString delete_sql = "delete from student where id = ?";
         QString clear_sql = "delete from student";
        //创建
         sql_query.prepare(create_sql);
         if(!sql_query.exec())
         {
           qDebug()<<sql_query.lastError();
         }
         else
         {
           qDebug()<<"table created!";
         }
          }
  }
}
my_sql::insert(int pm2_5, int pm10)
{
  QSqlQuery sql_query;
  QString select_max_sql = "select max(id) from student";
  QString insert_sql = "insert into student values (?,?,?)";

  //查询最大id
  int max_id = 0;
  sql_query.prepare(select_max_sql);
  if(!sql_query.exec())
  {
     qDebug()<<sql_query.lastError();
  }else{
    while(sql_query.next())
    {
      max_id = sql_query.value(0).toInt();
      qDebug()<<QString("max id:%1").arg(max_id);
    }
  }

  //插入数据
  sql_query.prepare(insert_sql);
  sql_query.addBindValue(max_id+1);
  sql_query.addBindValue(pm2_5);
  sql_query.addBindValue(pm10);
  if(!sql_query.exec())
  {
  qDebug()<<sql_query.lastError();
  }
  else
  {
  qDebug()<<"inserted!";
  }


}
