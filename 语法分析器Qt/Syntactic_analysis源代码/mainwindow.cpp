#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QPushButton>
#include<QLabel>
#include<QTextEdit>
#include<QMessageBox>
#include<QString>
#include<QVector>
#include<QMap>
#include<QSet>

#define Max 300
//最多300个终结符
struct node{

    QChar x;
    QChar y;
    bool operator <(const node &a)const
    {
        if(x!=a.x)
            return x<a.x;
        return y<a.y;
    }
};
QMap<node,QChar> ans;//优先关系表

 QVector<QChar> VT;//算符
extern QVector<wenfa> VN_input;//输入的几行文法
 QMap<QChar,int> VN_map;//记录左端大写字符的位置,从1
 QSet<QChar> FIRSTVT[Max];
 QSet<QChar> LASTVT[Max];
QMap<QChar,int>used;

int vis[Max];//dfs

QVector<QString> hang;

bool flag1=true;//判断是不是算符文法
bool flag2=true;//判断是不是算符优先文法

int isupper(QCharRef c)
{
    if(c>='A'&&c<='Z')
        return 1;
    return 0;
}

void dfs(int dep)
{
  if(vis[dep])
      return;
  vis[dep]=1;

  for(int i=0;i<VN_input[dep].right.size();i++)
  {
      QString rightmp=VN_input[dep].right[i];
      if(isupper(rightmp[0]))
      {
          int y=VN_map[rightmp[0]]-1;//
          if(rightmp.length()>1 && !isupper(rightmp[1]))
              FIRSTVT[dep].insert(rightmp[1]);
          dfs(y);
          //a属于FIR(Q),P->Q.... a属于FIR(P)
          QSet<QChar>::iterator it=FIRSTVT[y].begin();
          for(;it!=FIRSTVT[y].end();it++)
              FIRSTVT[dep].insert(*it);
      }
      //如果第一位就是终结符，那么符合第一条规律
      else
      {
          FIRSTVT[dep].insert(rightmp[0]);
      }
  }
}
void do_FIRSTVT()
{
   memset(vis,0,sizeof(vis));
   for(int i=0;i<VN_input.size();i++)
       if(vis[i])
           continue;
       else
           dfs(i);
 }
void dfs_lastvt(int dep)
{
    if(vis[dep])
        return;
    vis[dep]=1;
    for(int i=0;i<VN_input[dep].right.size();i++)
    {
        QString rightmp=VN_input[dep].right[i];
        int len=rightmp.length();
        //P->.......aQ
        if(isupper(rightmp[len-1]))
        {
            int y=VN_map[rightmp[len-1]]-1;
            if(rightmp.length()>1&& !isupper(rightmp[len-2]))
                LASTVT[dep].insert(rightmp[len-2]);//
            dfs_lastvt(y);
            QSet<QChar>::iterator it=LASTVT[y].begin();
            for(;it!=LASTVT[y].end();it++)
                LASTVT[dep].insert(*it);
         }
        //p->.......a
        else
        {
            LASTVT[dep].insert(rightmp[len-1]);
        }
    }
}
void do_LASTVT()
{
    memset(vis,0,sizeof(vis));
    for(int i=0;i<VN_input.size();i++)
        if(vis[i])
            continue;
        else
            dfs_lastvt(i);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("语法分析器");
    this->resize(600,400);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()//显示是否是算符优先文法
{
    QString tm;
    if(flag1==false)
        tm="不是算符文法，不是算符优先文法";
    else if(flag2==true)
        tm="是算符文法，也是算符优先文法";
    else
        tm="是算符文法，不是算符优先文法";
    QMessageBox::information(this,tr("文法性质"),tm,QMessageBox::Yes);
}

void MainWindow::on_pushButton_2_clicked()//显示优先表
{
//    memset(ans,' ',sizeof(ans));
   for(int i=0;i<VN_input.size();i++)
       for(int j=0;j<VN_input[i].right.size();j++)
       {
           QString str=VN_input[i].right[j];
           for(int k=0;k<str.length()-1;k++)
           {
               if(!isupper(str[k])&&!isupper(str[k+1]))//...ab...
                  {
                   node tmp;
                   tmp.x=str[k];
                   tmp.y=str[k+1];
                   ans[tmp]='=';
               }
               if(!isupper(str[k])&&isupper(str[k+1]))//...aB...
               {
                   int x=VN_map[str[k+1]]-1;
                   QSet<QChar>::iterator it=FIRSTVT[x].begin();
                   for(;it!=FIRSTVT[x].end();it++)
                   {
                       node tmp;
                       tmp.x=str[k];
                       tmp.y=*it;
                       ans[tmp]='<';
                   }


               }
               if(isupper(str[k])&&!isupper(str[k+1]))//....Ab.....
                 {
                   int x=VN_map[str[k]]-1;
                    QSet<QChar>::iterator it=LASTVT[x].begin();
                    for(;it!=LASTVT[x].end();it++)
                    {
//                        node tmp(*it,str[k+1]);
                        node tmp;
                        tmp.x=*it;
                        tmp.y=str[k+1];
                        ans[tmp]='>';
                    }
//                        ans[int(*it)][int(str[k+1])]='>';
               }

               if((k<=str.length()-3)&&!isupper(str[k])&&isupper(str[k+1])&&!isupper(str[k+2]))//...aBc....
               {
//                   node tmp(str[k],str[K+2]);
                   node tmp;
                   tmp.x=str[k];
                   tmp.y=str[k+2];
                   ans[tmp]='=';
               }
//                   ans[int(str[k])][int(str[k+2])]='=';
           }
       }

     //  print
    QString tmp;
    tmp.clear();
    tmp="             优先表\n";
    tmp+="     ";
    for(int i=0;i<VT.size();i++)
    {
        tmp+=VT[i];
        tmp+="     ";
    }
    tmp+='\n';
    for(int i=0;i<VT.size();i++)
    {
        tmp+=VT[i];
        tmp+="   ";
        for(int j=0;j<VT.size();j++)
        {
//            node t;
//            t.x=VT[i];
//            t.y=VT[j];

//            if(ans.contains(t))
//            {
//                QMap<node,QChar>::iterator it=ans.find(t);
//                tmp+=it.value();
//                tmp+="   ";

//            }
//            else
//            {
//                tmp+="    ";
//            }
            bool fl=true;
            QMap<node,QChar>::iterator it=ans.begin();
            for(;it!=ans.end();it++)
            {
                if(it.key().x==VT[i]&&it.key().y==VT[j])
                {
                    tmp+=it.value();
                    tmp+="    ";
                    fl=false;
                    break;
                }
            }
            if(fl==true)
                tmp+="空     ";

        }
        tmp+='\n';
    }
    QMessageBox::information(this,tr("优先表"),tmp,QMessageBox::Yes);
}

void MainWindow::on_pushButton_6_clicked()//确认键，将输入的文本存入字符串数组
{
        hang.clear();
    QString str=ui->textEdit->toPlainText();
//    QMessageBox::information(this,tr("判断"),str,QMessageBox::Yes);
     int len=str.length();

     QString tmp;
     for(int i=0;i<len;i++)
     {

        if(str[i]!='\n')
        {
          tmp+=str[i];
        }
        if(str[i]=='\n')
        {
            hang.push_back(tmp);
            tmp.clear();
        }
        if(i==len-1&&str[i]!='\n')
        {
            hang.push_back(tmp);
            tmp.clear();
        }
     }
   int s=hang.size();

   for(int i=0;i<s;i++)
   {
       if(!isupper(hang[i][0]))
       {
           flag1=false;
           flag2=false;
           break;
       }
       int j=0;
       for(;j<hang[i].length();j++)
       {
          if(hang[i][j]=='-')
              break;
       }
       if(!VN_map[hang[i][0]])//新的左端时
       {
           wenfa ttt(hang[i][0]);
           VN_input.push_back(ttt);
           VN_map[hang[i][0]]=VN_input.size();
       }
       //对后半部分进行|判断,再插入
       int loc=VN_map[hang[i][0]]-1;
       int m=j+2;
       QString tt;

       for(;m<hang[i].length();m++)
       {

           if(hang[i][m]!='|')
           {
               tt+=hang[i][m];
               if(isupper(hang[i][m])&&isupper(hang[i][m+1]))//当出现连续两个大写字符时就不是算符文法
               {
                   flag1=false;
                   flag2=false;
               }

           }
           if(hang[i][m]=='|')
           {
               VN_input[loc].insert(tt);
               tt.clear();

           }
           if(m==hang[i].length()-1)
           {
               VN_input[loc].insert(tt);
               tt.clear();
           }
       }
       for(int k=0;k<j;k++)
       {
           if(!isupper(hang[i][k]))
           {
               if(used[hang[i][k]])
                   continue;
               used[hang[i][k]]=1;
               VT.push_back(hang[i][k]);
           }
       }
       for(int k=j+2;k<hang[i].length();k++)
       {
           if(!isupper(hang[i][k])&&hang[i][k]!='|')
           {
               if(used[hang[i][k]])
                   continue;
               VT.push_back(hang[i][k]);
               used[hang[i][k]]=VT.size();
           }
       }

   }
//   QString hhh;
//   for(int i=0;i<VN_input.size();i++)
//   { hhh+=VN_input[i].right[0];hhh+=' ';}
//  QMessageBox::information(this,tr("判断"),hhh,QMessageBox::Yes);
}

void MainWindow::on_pushButton_7_clicked()//显示FIRSTVT
{
    do_FIRSTVT();
    QString tmp;
    tmp.clear();
    tmp="FIRSTVT表\n";
    for(int i=0;i<VN_input.size();i++)
    {
        tmp+=VN_input[i].left;
        tmp+=":   ";
        QSet<QChar>::iterator it=FIRSTVT[i].begin();
        for(;it!=FIRSTVT[i].end();it++)
        {
            tmp+=*it;
            tmp+="   ";
        }
        tmp+='\n';
    }
    do_LASTVT();
    tmp+="LASTVT表\n";
    for(int i=0;i<VN_input.size();i++)
    {
        tmp+=VN_input[i].left;
        tmp+=":   ";
        QSet<QChar>::iterator it=LASTVT[i].begin();
        for(;it!=LASTVT[i].end();it++)
        {
            tmp+=*it;
            tmp+="   ";
        }
        tmp+='\n';
    }
   QMessageBox::information(this,tr("FIRSTVT&LASTVT"),tmp,QMessageBox::Yes);
}

//E->E+T|T
//T->T*F|F
//F->P~F|P
//P->(E)|i

//***************************************************移进规约************************************************//
QString chuang;//记录字符串
int father[Max];
QString input;
int zhuan(QChar a)
{
   char t=a.unicode();
   char z=' ';
   return t-z;
}

//并查集
int find(int x)//查
{
    while(x!=father[x])
        x=father[x];
    return x;

}
void Union(int x,int y)//并
{
    x=find(x);
    y=find(y);
    father[x]=y;
}
bool judge(int x,int y)//判断是不是同一根节点
{
    if(find(x)==find(y))
        return true;
    return false;
}
void Init()
{
    for(int i=0;i<Max;i++)
        father[i]=i;
    for(int i=0;i<VN_input.size();i++)
    {
        QString lt=VN_input[i].left;
        for(int j=0;j<VN_input[i].right.size();j++)
        {
            QString rt=VN_input[i].right[j];
            if(lt.length()==1&&rt.length()==1)
                Union(zhuan(lt[0]),zhuan(rt[0]));

        }
    }
}
bool check(QString str1,QString str2)
{
    if(str1.length()!=str2.length())
        return false;
    for(int i=0;i<str1.length();i++)
    {
        if(isupper(str1[i]))
        {
           if(!judge(zhuan(str1[i]),zhuan(str2[i])))
               return false;
        }
        else
        {
            if(str1[i]!=str2[i])
                return  false;
        }
    }
    return true;

}
QString reduction(QString s)//规约
{
    for(int i=0;i<VN_input.size();i++)
        for(int j=0;j<VN_input[i].right.size();j++)
        {
            if(check(VN_input[i].right[j],s))
                return VN_input[i].left;
        }
    return "   ";
}
QString get_stack(QVector<QChar> s)
{
    QString tmp;
    for(int i=0;i<s.size();i++)
        tmp+=s[i];
    return tmp;
}

void move_reduction(QString src,QVector<QString>& output,QVector<QString>& tree,bool& flag3)
{
    Init();
    QVector<QChar> stack;//符号栈
    int pc=1;
    src+="#";
    stack.push_back('#');
    output.push_back("                             移进归约过程                            ");
    output.push_back("步骤        栈        优先关系       当前符号     剩余符号      动作           ");
    for(int i=0;i<src.length();i++)
    {
        if(src[i]>='A'&&src[i]<='Z')
        {
            flag3=false;
            break;
        }
        bool tflag=false;
        for(int j=0;j<VT.size();j++)
        {
            if(VT[j]==src[i])
              {
                tflag=true;
                break;
            }
        }
        if(tflag==false)
        {
            flag3=false;
            break;
        }

        int len=stack.size();
        QChar top=stack[len-1];
        for(int j=len-1;j>=0;j--)
        {
            QChar tmpp=stack[j];
            if(tmpp>='A'&&tmpp<='Z')
                continue;
            else
            {
                top=stack[j];
                break;
            }
        }
        node t;
        t.x=top;
        t.y=src[i];

        QMap<node,QChar>::iterator it=ans.find(t);
        if(it==ans.end())
        {
            flag3=false;
            break;
        }
        QChar ch=it.value();
        if(ch=='<'||ch=='=')//移进
        {
            QString tt;
            QString out;
            tt+=QChar(pc+48);
            if(i==src.length()-1)
            {
                out+=tt;
                out+="          ";
                out+=get_stack(stack);
                tt.clear();
                tt+=ch;
                out+="          ";
                out+=tt;
                out+="              ";
                out+=src[i];
                out+="          ";
                out+="空";
                out+="          ";
                out+="移进";
            }
            else
            {
                out+=tt;
                out+="          ";
                out+=get_stack(stack);
                tt.clear();
                tt+=ch;
                out+="          ";
                out+=tt;
                out+="               ";
                out+=src[i];
                out+="          ";
              out+=src.mid(i+1,src.length()-i-1);

                out+="          ";
                out+="移进";
            }
//            tt.clear();
            stack.push_back(src[i]);
            output.push_back(out);
        }
        //规约
        else
        {
            QString tt;
            QString output1;
            tt+=QChar(pc+48);
            if(i==src.length())
            {
                output1+=tt;
                output1+="          ";
                output1+=get_stack(stack);
                tt.clear();
                tt+=ch;
                output1+="          ";
                output1+=tt;
                output1+="               ";
                output1+=src[i];
                output1+="          ";
                output1+="空";
                output1+="          ";
                output1+="归约";
            }
            else
            {
                output1+=tt;
                output1+="          ";
                output1+=get_stack(stack);
                tt.clear();
                tt+=ch;
                output1+="          ";
                output1+=tt;
                output1+="               ";
                output1+=src[i];
                output1+="          ";
                output1+=src.mid(i+1,src.length()-i-1);
                output1+="          ";
                output1+="归约";
            }
            output.push_back(output1);
            tree.push_back(get_stack(stack)+src[i]+src.mid(i+1,src.length()-i-1));
            QString str;
            int l=stack.size()-2;

            while(1)
            {
                if(stack.size()==0)
                    break;

                node tm;
                tm.x=stack[l];
                tm.y=top;
                QMap<node,QChar>::iterator it=ans.find(tm);
//                if(it==ans.end())
//                {
//                    flag3=false;
//                    mflag=false;
//                    break;
//                }
                if(!(stack[l]>='A'&&stack[l]<='Z')&&it!=ans.end()&&it.value()=='<')
                    break;
                l--;
            }
//            if(mflag==false)
//            {
//                break;
//            }
            for(int m=stack.size()-1;m>l;m--)
            {
                str+=stack[m];
                stack.pop_back();
            }
            str=reduction(str);

            for(int k=0;k<str.length();k++)
                stack.push_back(str[k]);
          i--;
        }
       pc++;
    }
    tree.push_back(get_stack(stack));

}
extern QVector<QString> output;
extern QVector<QString> tree;
bool flag3;
void MainWindow::on_pushButton_3_clicked()//确定输入字符串
{
   input=ui->lineEdit->text();
    flag3=true;
   move_reduction(input,output,tree,flag3);
//   QMessageBox::information(this,tr("分析过程"),output,QMessageBox::Yes);

}

void MainWindow::on_pushButton_4_clicked()//显示分析过程
{
    if(flag3)
      w.show();
    else
        QMessageBox::information(this,tr("分析过程"),"输入错误",QMessageBox::Yes);
}

void MainWindow::on_pushButton_5_clicked()//画树
{

  if(flag3)
   u.show();
  else
      QMessageBox::information(this,tr("画图"),"输入错误",QMessageBox::Yes);


}
