#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<iomanip>
#include<ctype.h>
#include<fstream>
using namespace std;
typedef pair<int,int> mp;//单词种类和单词自身的值
//关键字
const string key_word[]={"short", "int", "long", "float", "double", "char"
, "struct", "union", "enum", "typedef", "const", "unsigned", "signed", "extern"
, "static", "void", "if", "else", "switch", "case", "for", "do", "while",
"goto", "continue", "break", "default", "sizeof", "return", "true", "false" ,"main","using","namespace","std","cin","cout"};
//运算符
const string operators[]={"+","-","*","/","=","+=","-=","*=","/=","%","%=","^","**",">","<",">=","<=","==",">>","<<"};
//定界
const char jiefu[]={',',';','(',')','{','}','[',']','"'};
map<string,mp> flag_table;//标识符表等 １代表
map<string,mp>num_table;//数字表 ２代表
map<string,mp>str_table;//字符串表,3
map<string,mp>head_table;//头文件表,4
map<string,mp>char_table;//字符表,5
map<char,mp>fenjiefu_table;//分界符表,6 
int keymark[40];
int operatormark[40];
//int jiefumark[40];

int isjiefu(char ch)
{
	for(int i=0;i<9;i++)
		if(ch==jiefu[i])
			return 1;
	return 0;
}
//判断是不是关键字
int iskey(string ch)
{   
	int i;
	for(i=0;i<37;i++)
		if(key_word[i]==ch)
			return i;
	return -1;
}
int isope(string ch)
{
	int i;
	for(i=0;i<20;i++)
		if(operators[i]==ch)
			return i;
	return -1;
}

int ischar(char ch)
{
	if(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z')
		return 1;
	return -1;
}
int isnumber(char ch)
{
	if(ch>='0'&&ch<='9')
		return 1;
	return -1;
}
//获得关键字或者标识符
void get_keyflag(char* ptr,FILE* f)
{
	string token;
	token+=*ptr;
	
	while(1)
	{   
		*(++ptr)=fgetc(f);
		if(ischar(*ptr)==-1&&isnumber(*ptr)==-1&&*ptr!='_')
		break;
		token+=*ptr;

	}
	
	ungetc(*ptr,f);
	//*ptr='\0';
	int h=iskey(token);
	if(h>=0)//是关键字
	{
		if(!keymark[h])
			keymark[h]=1;
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"关键字,第"<<h+1<<"位"<<setw(25)<<">"<<endl;
	}
	else//标识符
	{
		mp tmp;
		if(flag_table.find(token)==flag_table.end())//崭新的标记符
		{
			tmp=make_pair(1,flag_table.size()+1);
			flag_table[token]=tmp;
		}
		else
		{
			map<string,mp>::iterator it;
			it=flag_table.find(token);
			tmp=it->second;
		}
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"标识符"<<setw(20)<<"相对位置是"<<setw(10)<<tmp.second<<">"<<endl;
	}
}

//获取数字
void get_num(char* ptr,FILE* f)
{
	string token;
	token+=*ptr;
	while(1)
	{
		*(++ptr)=fgetc(f);
		if(isnumber(*ptr)!=1)
		break;
		token+=*ptr;
	}
	ungetc(*ptr, f);
	
	mp tmp;
	if(num_table.find(token)==num_table.end())//崭新的数字
	{
		tmp=make_pair(2,num_table.size()+1);//2代表着数字
		num_table[token]=tmp;
	}
	else
	{
		map<string,mp>::iterator it;
		it=num_table.find(token);
		tmp=it->second;
	}
	cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"数字"<<setw(20)<<"相对位置是"<<setw(10)<<tmp.second<<">"<<endl;
}
//获得字符串
void get_string(char* ptr,FILE* f)
{
	string token;
	
	*(++ptr)=fgetc(f);
	token+=*(ptr);
	while(1)//引号的匹配很模糊
	{
		*(++ptr)=fgetc(f);
		if(feof(f)||*ptr=='"')
		break;
		token+=*(ptr);
	}
	
	if(*(ptr)=='"')
	{
		map<string,mp>::iterator it;
		it=str_table.find(token);
		mp tmp;
		if(it==str_table.end())//崭新的字符串
		{
			
			tmp=make_pair(3,str_table.size()+1);
			str_table[token]=tmp;
		}
		else
		{
			tmp=it->second;
		}
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(5)<<"字符串"<<setw(20)<<"相对位置是"<<setw(10)<<tmp.second<<">"<<endl;
	}
	//找不到匹配的字符串
	else
	{
		cout<<"程序错误！忘记打\""<<endl;
	}
}
void double_operator(char* ptr,FILE* f)
{
	string token,tmp;
	token+=*ptr;
	tmp+=*ptr;
	*(++ptr)=fgetc(f);
	token+=*ptr;
	int p=isope(token);
	if(p>=0)
	{
		operatormark[p]=1;
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"双目运算符"<<p<<">"<<endl;
	}
	else
	{
		ungetc(*ptr,f);
		p=isope(tmp);
		operatormark[p]=1;
		cout<<setw(5)<<"<"<<setw(10)<<tmp<<setw(20)<<"单目运算符"<<p<<">"<<endl;
	}
	
}
void single_operator(char* ptr,FILE* f)
{
	string token;
	token+=*ptr;
	int index=isope(token);
	if(index>=0)
	{
		operatormark[index]=1;
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"单目运算符"<<index<<">"<<endl;
	}
	else
	{
		if(*ptr<0||*ptr>127)
			cout<<"输入的不是ascii码"<<endl;
	}

}
//在上一级函数中直接判断是不是分解符
//处理# ptr 一定要new char[1000];
void acehandle(char* ptr,FILE* f)//*ptr=="#"
{
   string token;
   token+=*ptr;
//   cout<<token<<endl;
   while(1)
   {
   	 
	   ptr++;
	   *ptr=fgetc(f);
	   if(*ptr==' '||*ptr=='"'||*ptr=='<')
   	    break;
	   token+=*ptr;
   }
   if(*ptr=='<'||*ptr=='"')
   ungetc(*ptr,f);
//   cout<<token<<endl;
   if(token=="#define")
   {
	   cout<<setw(5)<<token<<setw(5)<<"宏定义"<<setw(5)<<">"<<endl;
   }
   if(token=="#include")
   {
	   cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"预处理"<<setw(20)<<">"<<endl;
	   *ptr=fgetc(f);
	   cout<<setw(5)<<"<"<<setw(10)<<*ptr<<setw(20)<<"分界符"<<setw(20)<<">"<<endl;
	   string tmp;
	  
	   *ptr=fgetc(f);
	   tmp+=*ptr;
	   while(1)
	   {
		   

		   *(++ptr)=fgetc(f);
		   if(*ptr=='>'||*ptr=='"')
			   break;
		   tmp+=*ptr;
		   
	   }
	   mp p;
	   if(head_table.find(tmp)==head_table.end())
	   {
		   p=make_pair(4,head_table.size()+1);
		   head_table[tmp]=p;
	   }
	   else
	   {
		   map<string,mp>::iterator it;
		   it=head_table.find(tmp);
		   p=it->second;
	   }
	   cout<<setw(5)<<"<"<<setw(10)<<tmp<<setw(10)<<p.first<<setw(10)<<"头文件"<<setw(10)<<p.second<<">"<<endl;
	   cout<<setw(5)<<"<"<<setw(10)<<*ptr<<setw(20)<<"分界符"<<setw(20)<<">"<<endl;

   }
}
void handlechar(char ch,FILE* f)
{
	char* ptr=new char[1000];
	*ptr=ch;
	if(ischar(*ptr)==1)//以字母开头的--关键字或者标识符
	{
		get_keyflag(ptr,f);
	}
	else if(isnumber(*ptr)==1)
	{
		get_num(ptr,f);
	}
	else if(*ptr=='#')
	{
		acehandle(ptr,f);
	}
	else if(*ptr=='"')
	{
		get_string(ptr,f);

	}
	else if(ch=='/')//处理注释
	{
		char t=fgetc(f);
		if(t=='/')
		{
			string s;
			while(1)
			{
				char de=fgetc(f);
				if(de==' '||de=='\n'||de=='\t')
					break;
				s+=de;
			}
			cout<<setw(5)<<"<"<<setw(5)<<"//"<<setw(20)<<"注释符"<<setw(20)<<">"<<endl;
			cout<<setw(5)<<"<"<<setw(5)<<s<<setw(20)<<"注释内容"<<setw(20)<<">"<<endl;
		}
		if(t=='*')
		{
			string s;
			while(1)
			{
				char de=fgetc(f);
				if(de=='*')
					break;
				s+=de;
			}
			char d=fgetc(f);
			cout<<setw(5)<<"<"<<setw(5)<<"/*"<<setw(20)<<"注释符"<<setw(20)<<">"<<endl;
			cout<<setw(5)<<"<"<<setw(5)<<s<<setw(20)<<"注释内容"<<setw(20)<<">"<<endl;
			cout<<setw(5)<<"<"<<setw(5)<<"*"<<"/"<<setw(20)<<"注释符"<<setw(20)<<">"<<endl;

		}
	}
	else if(*ptr=='+'||*ptr=='-'||*ptr=='*'||*ptr=='/'||*ptr=='='||*ptr=='%'||*ptr=='>'||*ptr=='<')
	{
		double_operator(ptr,f);
	}
	else if(isjiefu(*ptr)==1)
	{
		mp tmp;
		map<char,mp>::iterator it;
		
		if(fenjiefu_table.find(*ptr)==fenjiefu_table.end())
		{
			tmp=make_pair(6,fenjiefu_table.size()+1);
			fenjiefu_table[*ptr]=tmp;
			
		}
		else
		{
			it=fenjiefu_table.find(*ptr);
			tmp=it->second;
		}
		
		
		 cout<<setw(5)<<"<"<<setw(20)<<*ptr<<setw(20)<<"分界符"<<setw(20)<<"相对位置是"<<tmp.second<<">"<<endl;
	}
	
	else
	{
		single_operator(ptr,f);
	}
}
int main()
{ 
	FILE* f;
	f=fopen("cin.txt","r");
   
	if(f==NULL)
	{
		cout<<"读入文件错误"<<endl;
		return 0;
	}
	else
	{
		cout<<"--------------------------------------------------------------------------------------"<<endl;
		char ch=fgetc(f);
		
		while(!feof(f))
		{
			if(ch!=' '&&ch!='\n'&&ch!='\t')
				handlechar(ch,f);
			ch=fgetc(f);
		}
		cout<<"--------------------------------------------------------------------------------------"<<endl;
	}
	fclose(f);
	return 0;
}