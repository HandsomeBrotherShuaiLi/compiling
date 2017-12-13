#include "dialog1.h"
#include "ui_dialog1.h"
#include<QPainter>
QVector<QString> output;
Dialog1::Dialog1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog1)
{
    ui->setupUi(this);
    this->setWindowTitle("分析过程");
    this->resize(700,400);
}

Dialog1::~Dialog1()
{
    delete ui;
}
void Dialog1::paintEvent(QPaintEvent *)
{
     QPainter painter(this);
     for(int i=0;i<output.size();i++)
         painter.drawText(10,30*i+40,output[i]);
}
