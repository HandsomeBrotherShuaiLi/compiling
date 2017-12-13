#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<Qstring>
#include<QVector>
#include<dialog1.h>
#include<dialog2.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();//判断是不是算符优先文法

    void on_pushButton_2_clicked();//显示优先表

    void on_pushButton_6_clicked();//确认输入键

    void on_pushButton_7_clicked();//显示FIRSTVT && LASTVT



    void on_pushButton_3_clicked();//确定输入字符串

    void on_pushButton_4_clicked();//显示分析过程

    void on_pushButton_5_clicked();//显示树

private:
    Ui::MainWindow *ui;
    Dialog1 w;
    Dialog2 u;
};
//存储一行文法语句
class wenfa
{
public:
    QChar left;
    QVector<QString> right;
    wenfa(const QChar& str)
    {
        left=str;
    }
    wenfa()
    {

    }

    void insert(QString st)
    {
        right.push_back(st);
    }

};

#endif // MAINWINDOW_H
