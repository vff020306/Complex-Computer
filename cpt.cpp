#include<iostream>
#include<cstdio>
#include<string>
#include<stack>
#include<vector>
#include"complex.h"
#include"equation.h"
using namespace std;

struct node{//type为1时为数，为2时为运算符
	int type;
	Complex num;
	char opr;
};


bool judge(string str);//判断表达式是否合法，不合法则输出错误

bool Prio(char a,char b);//比较运算符优先级
void ReversePolishNotation(string str,vector<node> &rpn);//转化为后缀表达式
Complex cal(vector<node> array);//计算一个后缀表达式的值
string change(string str);//删除所有空格,调整arg,cjg的顺序,调整||的表示

void solve(string str);
void smart_solve(string str);
string addit(string str);//智能模式下纠错

int main(){
	while(1){
		bool flag=true;
		cout<<"1.计算表达式"<<endl;
		cout<<"2.开启智能纠错"<<endl;
		cout<<"3.求解二元一次方程"<<endl;
		cout<<"4.退出"<<endl;
		cout<<"请输入操作前的数字代号"<<endl;

		int mat;
		cin>>mat;

		switch(mat){
			case 1:{
				cout<<"输入表达式："<<endl;
				string S;
				cin.ignore(1024,'\n');
				getline(cin,S);
				solve(S);
				break;
			}

			case 2:{
				cout<<"输入表达式："<<endl;
				string S;
				cin.ignore(1024,'\n');
				getline(cin,S);
				smart_solve(S);
				break;
			}

			case 3:{
				double a,b,c;
				Complex r1,r2;
				cout<<"从高到低依次输入三个系数："<<endl;
				cout<<"二次项系数为："<<endl;
				cin>>a;
				cout<<"一次项系数为："<<endl;
				cin>>b;
				cout<<"常数项系数为："<<endl;
				cin>>c;
				solve_equation(a,b,c,r1,r2);
				cout<<"x1=";r1.out();
				cout<<"  x2=";r2.out();
				cout<<endl;
				break;
			}

			case 4:{
				flag=false;
				cout<<"欢迎下次使用"<<endl;
				break;
			}

			default:{
				cout<<"输入了错误的代号"<<endl;
				break;
			}
		}

		if(!flag) break;
		for(int i=1;i<=30;i++) cout<<"- ";
		cout<<endl;
	}
	return 0;
}


bool is_opr(char ch){
	return ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='^'||ch=='$';
}
bool is_num(char ch){
	return ch>='0'&&ch<='9';
}

bool Prio(char top,char cur){//判断栈顶与当前运算的优先级
	if((top=='+'||top=='-')&&(cur=='+'||cur=='-')) return true;
	if((top=='*'||top=='/')&&(cur=='+'||cur=='-'||cur=='*'||cur=='/')) return true;
	if((top=='^')&&(cur=='^'||cur=='+'||cur=='-'||cur=='*'||cur=='/')) return true;
	if((top=='$')&&(cur=='$'||cur=='^'||cur=='+'||cur=='-'||cur=='*'||cur=='/')) return true;
	return false;
}

bool judge(string str){
//在判断数字、i与|的搭配时仍存在问题
	bool flag=true;
	char ch;
	int len=str.length();

	ch=str[0];
	string sub=str.substr(0,3);
	if((!is_num(ch)) && (ch!='|') && (ch!='-') && (ch!='i') && (ch!='(') && (sub!="arg") && (sub!="cjg") && (sub!="dis") ){
		flag=false;
		cout<<"起始字符错误!"<<endl;
	}

	ch=str[len-1];
	if((!is_num(ch)) && (ch!='|') && (ch!='i') && (ch!=')')){
		flag=false;
		cout<<"结尾字符错误！"<<endl;
	}

	for(int i=1;i<len-1;i++){
		ch=str[i];

		if(is_opr(ch)){
			char ch2=str[i+1];
			if( (ch2==')') || is_opr(ch2) ){
				flag=false;
				cout<<ch<<"与"<<ch2<<"搭配错误!"<<endl;
			}
		}

		if(is_num(ch)){
			char ch2=str[i+1];
			if( (ch2=='(') ){
				flag=false;
				cout<<ch<<"与"<<ch2<<"搭配错误!"<<endl;
			}
		}

		if(ch=='i'){
			char ch2=str[i+1];
			if( (ch2=='(') || (ch2=='i') || (is_num(ch2)) ){
				flag=false;
				cout<<ch<<"与"<<ch2<<"搭配错误!"<<endl;
			}
		}
		
		if(ch=='('){
			char ch2=str[i+1];
			if( (ch2==')') || (is_opr(ch2)&&ch2!='-') ){
				flag=false;
				cout<<ch<<"与"<<ch2<<"搭配错误!"<<endl;
			}
		}

		if(ch==')'){
			char ch2=str[i+1];
			if( (ch2=='(') || is_num(ch2) ){
				flag=false;
				cout<<ch<<"与"<<ch2<<"搭配错误!"<<endl;
			}
		}

		if(ch==' '){
			if(is_num(str[i-1])&&is_num(str[i+1])){
				flag=false;
				cout<<"出现错误空格!"<<endl;
			}
		}
	}

	int cnt=0;
	for(int i=0;i<len;i++){
		if(str[i]=='(') cnt--;
		if(str[i]==')') cnt++;
		if(cnt>0){
			flag=false;
			cout<<"右括号匹配错误!"<<endl;
		}
	}
	if(cnt!=0){
		flag=false;
		cout<<"括号匹配错误!"<<endl;
	}

	return flag;
}


string change(string str){
	string res="";
	int len=str.length();
	for(int i=0;i<len;i++){
		if((i<=len-3)&&(str.substr(i,3)=="arg")){
			res+='@';
			i+=2;
			continue;
		}
		if((i<=len-3)&&(str.substr(i,3)=="cjg")){
			res+='!';
			i+=2;
			continue;
		}
		if((i<=len-3)&&(str.substr(i,3)=="dis")){
			i+=2;
			continue;
		}
		if(str[i]==','){
			res+=")$(";
			continue;
		}
		if(str[i]==' ') continue;
		if(str[i]=='|'){
			if(i==0){res+="&(";continue;}
			if(i==len-1){res+=')';continue;}
			if(is_opr(str[i-1]) || str[i-1]=='(') res+="&(";
			else res+=')';
			continue;
		}
		if(str[i]=='-'){
			if(i>0&&str[i-1]=='('){
				res+="0-";
				continue;
			}
			else if(i==0){
				res+="0-";
				continue;
			}
		}
		if(str[i]==')'&&str[i+1]=='i'){
			res+=")*";
			continue;
		}

		res+=str[i];
	}
	return res;
}

void ReversePolishNotation(string str,vector<node> &rpn){
	rpn.clear();
	stack<char> q;

	int len=str.length();
	for(int i=0;i<len;i++){
		if(is_num(str[i])||str[i]=='i'){
			node now;
			now.type=1;
			if(str[i]=='i') now.num=Complex(0,1);
			else{
				int j=i,k=i;
				double cnt=0;
				while(is_num(str[j])&&j<len){
					cnt=cnt*10.0+(double)(str[j]-'0');
					j++;
				}
				while((is_num(str[k])||str[k]=='.')&&k<len) k++;

				double ff=0;
				if(j==k) ff=0;
				else{
					j++;
					double e=0.1;
					while(is_num(str[j])){
						ff+=e*(double)(str[j]-'0');
						j++;
						e/=10.0;
					}
				}

				if(str[j]=='i'){
					j++;
					now.num=Complex(0,cnt+ff);
				}
				else now.num=Complex(cnt+ff,0);
				i=j-1;
			}

			rpn.push_back(now);
		}
		else{
			node now;
			now.type=2;
			now.opr=str[i];
			if(q.empty()||str[i]=='('||str[i]=='&'||str[i]=='@'||str[i]=='!') q.push(str[i]);
			else if(str[i]==')'){
				while(q.top()!='('){
					now.opr=q.top();
					rpn.push_back(now);
					q.pop();
				}
				q.pop();
				if((!q.empty())&&(q.top()=='@'||q.top()=='!'||q.top()=='&')){
					now.opr=q.top();
					rpn.push_back(now);
					q.pop();
				}
			}
			else {
				while(Prio(q.top(),str[i])){
					now.opr=q.top();
					rpn.push_back(now);
					q.pop();
					if(q.empty()) break;
				}
				q.push(str[i]);
			}
		}

	}
	node now;
	now.type=2;
	while(!q.empty()){
		now.opr=q.top();
		rpn.push_back(now);
		q.pop();
	}

	return;
}

Complex cal(vector<node> array){
	stack<Complex> q;
	for(int i=0;i<array.size();i++){
		if(array[i].type==1) q.push(array[i].num);
		if(array[i].type==2){
			char ch=array[i].opr;
			if(is_opr(ch)){
				Complex a=q.top();
				q.pop();
				Complex b=q.top();
				q.pop();
				switch(ch){
					case('+'):{ q.push(a+b);break;}
					case('-'):{ q.push(b-a);break;}
					case('*'):{ q.push(a*b);break;}
					case('/'):{ q.push(b/a);break;}
					case('^'):{ q.push(b.pow((int)a.x()));break;}
					case('$'):{ q.push(dis(a,b));break;}
				}

			}
			else{
				Complex a=q.top();
				q.pop();
				switch(ch){
					case('@'):{ q.push(a.arg());break;}
					case('!'):{ q.push(a.cjg());break;}
					case('&'):{ q.push(a.mag());break;}
				}
			}
		}
	}
	return q.top();
}

void solve(string S){
	if(judge(S)){
		S=change(S);
		vector<node> arr;
		ReversePolishNotation(S,arr);
		Complex ans=cal(arr);
		cout<<"结果为：";
		ans.out();
		cout<<endl;
	}
}

void smart_solve(string S){
	S=addit(S);
	cout<<"修改为："<<S<<endl;
	if(judge(S)){
		S=change(S);
		vector<node> arr;
		ReversePolishNotation(S,arr);
		Complex ans=cal(arr);
		cout<<"计算结果为：";
		ans.out();
		cout<<endl;
	}
}

bool near(string a,string b){
	int la=a.length(),lb=b.length();
	int cnt=0;
	if(la!=lb) return false;
	for(int i=0;i<la;i++){
		if(a[i]!=b[i]) cnt++;
	}
	return cnt<=1;
}

string addit(string str){
	string res="";
	int len=str.length();
	char ch,ch2;

	for(int i=0;i<len-1;i++){
		ch=str[i];
		ch2=str[i+1];
		res+=ch;

		if(is_num(ch)&&ch2=='('){
			res+='*';
		}

		if(ch=='i'){
			if( (ch2=='(') || (ch2=='i') || (is_num(ch2)) ){
				res+='*';
			}
		}

		if(ch=='('&&ch2==')'){
			res.erase(prev(res.end()));
			i++;
		}

		if( (ch==')') && (ch2=='('||is_num(ch2)) ){
			res+='*';
		}

		if(ch==' '){
			res.erase(prev(res.end()));
		}

		if((ch>='a')&&(ch<='z')&&(i<len-2)){
			string sub=str.substr(i,3);
			if(near(sub,"cjg")){
				res.erase(prev(res.end()));
				res+="cjg";
				i+=2;
			}
			else if(near(sub,"arg")){
				res.erase(prev(res.end()));
				res+="arg";
				i+=2;
			}
			else if(near(sub,"dis")){
				res.erase(prev(res.end()));
				res+="dis";
				i+=2;
			}
		}
	}

	res+=str[len-1];
	return res;
}
