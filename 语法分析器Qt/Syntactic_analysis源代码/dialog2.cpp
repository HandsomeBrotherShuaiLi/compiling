#include "dialog2.h"
#include "ui_dialog2.h"
#include"mainwindow.h"
QVector<wenfa> VN_input;
QVector<QString> tree;
QVector<QPoint> point[10];
Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
    this->setWindowTitle("算符优先语法树");
    this->setGeometry(700,300,600,600);
}

Dialog2::~Dialog2()
{
    delete ui;
}
void Dialog2::paintEvent(QPaintEvent *)
{
    //tree的tree[0]最低一层，tree[1]倒数第二层
            QPainter painter(this);

           // 反走样
            int s=tree.size();//
            painter.setRenderHint(QPainter::Antialiasing, true);

            //设置画笔颜色、宽度
           painter.setPen(QPen(QColor(0, 160, 230), 2));

           painter.setBrush(QColor(255, 160, 90));

                   // 绘制圆
           painter.drawEllipse(QPointF(250, 40), 30, 30);//坐标（200,40）
           painter.setPen(QColor(0, 160, 230));
              QFont font;
              font.setFamily("Microsoft YaHei");
              font.setPointSize(10);
               painter.setFont(font);
               QString w;
               w+=tree[s-1][1];
               painter.drawText(240,40,w);//坐标（190,40）
               point[0].push_back(QPoint(240,40));
               for(int i=2;i<=s;i++)
               {
                   for(int j=1;j<=tree[s-i].length()-2;j++)
                   {
                       QString tmp;
                       tmp+=tree[s-i][j];
                       painter.drawEllipse(QPoint(10+120*j,40+100*(i-1)),30,30);
                       painter.drawText(120*j,40+100*(i-1),tmp);
                       point[i-1].push_back(QPoint(120*j,40+100*(i-1)));
                   }
               }
               for(int i=0;i<point[1].size();i++)
                   painter.drawLine(point[0][0],point[1][i]);
               for(int i=1;i<tree.size()-1;i++)
               {
                   for(int j=0;j<point[i].size();j++)
                       painter.drawLine(point[i][j],point[i+1][j]);
               }
}
