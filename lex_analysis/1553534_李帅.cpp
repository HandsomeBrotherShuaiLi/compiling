#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<iomanip>
#include<ctype.h>
#include<fstream>
using namespace std;
typedef pair<int,int> mp;//��������͵��������ֵ
//�ؼ���
const string key_word[]={"short", "int", "long", "float", "double", "char"
, "struct", "union", "enum", "typedef", "const", "unsigned", "signed", "extern"
, "static", "void", "if", "else", "switch", "case", "for", "do", "while",
"goto", "continue", "break", "default", "sizeof", "return", "true", "false" ,"main","using","namespace","std","cin","cout"};
//�����
const string operators[]={"+","-","*","/","=","+=","-=","*=","/=","%","%=","^","**",">","<",">=","<=","==",">>","<<"};
//����
const char jiefu[]={',',';','(',')','{','}','[',']','"'};
map<string,mp> flag_table;//��ʶ����� ������
map<string,mp>num_table;//���ֱ� ������
map<string,mp>str_table;//�ַ�����,3
map<string,mp>head_table;//ͷ�ļ���,4
map<string,mp>char_table;//�ַ���,5
map<char,mp>fenjiefu_table;//�ֽ����,6 
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
//�ж��ǲ��ǹؼ���
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
//��ùؼ��ֻ��߱�ʶ��
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
	if(h>=0)//�ǹؼ���
	{
		if(!keymark[h])
			keymark[h]=1;
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"�ؼ���,��"<<h+1<<"λ"<<setw(25)<<">"<<endl;
	}
	else//��ʶ��
	{
		mp tmp;
		if(flag_table.find(token)==flag_table.end())//ո�µı�Ƿ�
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
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"��ʶ��"<<setw(20)<<"���λ����"<<setw(10)<<tmp.second<<">"<<endl;
	}
}

//��ȡ����
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
	if(num_table.find(token)==num_table.end())//ո�µ�����
	{
		tmp=make_pair(2,num_table.size()+1);//2����������
		num_table[token]=tmp;
	}
	else
	{
		map<string,mp>::iterator it;
		it=num_table.find(token);
		tmp=it->second;
	}
	cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"����"<<setw(20)<<"���λ����"<<setw(10)<<tmp.second<<">"<<endl;
}
//����ַ���
void get_string(char* ptr,FILE* f)
{
	string token;
	
	*(++ptr)=fgetc(f);
	token+=*(ptr);
	while(1)//���ŵ�ƥ���ģ��
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
		if(it==str_table.end())//ո�µ��ַ���
		{
			
			tmp=make_pair(3,str_table.size()+1);
			str_table[token]=tmp;
		}
		else
		{
			tmp=it->second;
		}
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(5)<<"�ַ���"<<setw(20)<<"���λ����"<<setw(10)<<tmp.second<<">"<<endl;
	}
	//�Ҳ���ƥ����ַ���
	else
	{
		cout<<"����������Ǵ�\""<<endl;
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
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"˫Ŀ�����"<<p<<">"<<endl;
	}
	else
	{
		ungetc(*ptr,f);
		p=isope(tmp);
		operatormark[p]=1;
		cout<<setw(5)<<"<"<<setw(10)<<tmp<<setw(20)<<"��Ŀ�����"<<p<<">"<<endl;
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
		cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"��Ŀ�����"<<index<<">"<<endl;
	}
	else
	{
		if(*ptr<0||*ptr>127)
			cout<<"����Ĳ���ascii��"<<endl;
	}

}
//����һ��������ֱ���ж��ǲ��Ƿֽ��
//����# ptr һ��Ҫnew char[1000];
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
	   cout<<setw(5)<<token<<setw(5)<<"�궨��"<<setw(5)<<">"<<endl;
   }
   if(token=="#include")
   {
	   cout<<setw(5)<<"<"<<setw(10)<<token<<setw(20)<<"Ԥ����"<<setw(20)<<">"<<endl;
	   *ptr=fgetc(f);
	   cout<<setw(5)<<"<"<<setw(10)<<*ptr<<setw(20)<<"�ֽ��"<<setw(20)<<">"<<endl;
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
	   cout<<setw(5)<<"<"<<setw(10)<<tmp<<setw(10)<<p.first<<setw(10)<<"ͷ�ļ�"<<setw(10)<<p.second<<">"<<endl;
	   cout<<setw(5)<<"<"<<setw(10)<<*ptr<<setw(20)<<"�ֽ��"<<setw(20)<<">"<<endl;

   }
}
void handlechar(char ch,FILE* f)
{
	char* ptr=new char[1000];
	*ptr=ch;
	if(ischar(*ptr)==1)//����ĸ��ͷ��--�ؼ��ֻ��߱�ʶ��
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
	else if(ch=='/')//����ע��
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
			cout<<setw(5)<<"<"<<setw(5)<<"//"<<setw(20)<<"ע�ͷ�"<<setw(20)<<">"<<endl;
			cout<<setw(5)<<"<"<<setw(5)<<s<<setw(20)<<"ע������"<<setw(20)<<">"<<endl;
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
			cout<<setw(5)<<"<"<<setw(5)<<"/*"<<setw(20)<<"ע�ͷ�"<<setw(20)<<">"<<endl;
			cout<<setw(5)<<"<"<<setw(5)<<s<<setw(20)<<"ע������"<<setw(20)<<">"<<endl;
			cout<<setw(5)<<"<"<<setw(5)<<"*"<<"/"<<setw(20)<<"ע�ͷ�"<<setw(20)<<">"<<endl;

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
		
		
		 cout<<setw(5)<<"<"<<setw(20)<<*ptr<<setw(20)<<"�ֽ��"<<setw(20)<<"���λ����"<<tmp.second<<">"<<endl;
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
		cout<<"�����ļ�����"<<endl;
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