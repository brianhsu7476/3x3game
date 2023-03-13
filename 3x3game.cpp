#include <bits/stdc++.h>
#include <sys/time.h>
#define rep(i,n) for(int i=0; i<(n); ++i)
#define fup(i,a,b) for(int i=(a); i<(b); ++i)
using namespace std;
#define kN 3600

void printBoard(char a[][3]){
	rep(i, 3){
		cout<<"+-+-+-+\n|";
		rep(j, 3)cout<<a[i][j]<<'|';
		cout<<endl;
	}
	cout<<"+-+-+-+\n";
}

struct stat{
	int a[3][3]={0}; bool b=0;
	bool operator<(const stat r)const{
		rep(i, 3)rep(j, 3){
			if(a[i][j]<r.a[i][j])return 1;
			if(a[i][j]>r.a[i][j])return 0;
		}
		return !b&&r.b;
	}
	stat operator!()const{
		stat r=*this;
		return r.b=!b, r;
	}
	bool operator==(const stat r)const{
		rep(i, 3)rep(j, 3)if(a[i][j]!=r.a[i][j])return 0;
		return b==r.b;
	}
	void print(){
		char c[3][3];
		rep(i, 3)rep(j, 3)c[i][j]=a[i][j]==1?'x':a[i][j]==-1?'o':' ';
		printBoard(c);
	}
	int score(){
		int r=0;
		rep(j, 3)if(a[j][0]==a[j][1]&&a[j][1]==a[j][2])r+=a[j][0];
		rep(j, 3)if(a[0][j]==a[1][j]&&a[1][j]==a[2][j])r+=a[0][j];
		if(a[0][0]==a[1][1]&&a[1][1]==a[2][2])r+=a[0][0];
		if(a[2][0]==a[1][1]&&a[1][1]==a[0][2])r+=a[2][0];
		return r;
	}
};

stat a[kN]; int n, b[kN]; map<stat, int> p;
vector<int> G[kN];

signed main(){
	srand(time(0));
	int per[9];
	rep(i, 9)per[i]=i;
	do{
		stat tmp;
		rep(i, 3)rep(j, 3)tmp.a[i][j]=per[i*3+j]/3-1;
		p[tmp]=p[!tmp]=0;
	}while(next_permutation(per, per+9));
	for(auto &i:p)a[i.second=n++]=i.first;
	rep(i, n)rep(j, n)if(a[i].b^a[j].b){
		int cnt=0, cnt2=0;
		rep(k, 3)rep(l, 3)if(a[i].a[k][l]!=a[j].a[k][l])++cnt, cnt2+=a[i].a[k][l]+a[j].a[k][l];
		if(cnt==2&&(a[i].b?cnt2==2:cnt2==-2))G[i].push_back(j);
	}
	rep(i, n)b[i]=a[i].score();
	while(1){
		int cnt=0;
		rep(i, n)if(!b[i]){
			int k=a[i].b?1:-1;
			b[i]=-k;
			for(auto j:G[i])if(b[j]!=-k){b[i]=0; break;}
			for(auto j:G[i])if(b[j]==k){b[i]=k; break;}
			if(b[i])++cnt;
		}
		if(!cnt)break;
	}
	stat now;
	now.a[0][0]=now.a[1][2]=now.a[2][0]=-1,
	now.a[0][2]=now.a[1][0]=now.a[2][2]=1;
	cout<<"Initial board:\n", now.print(), cout<<"You and me take turn play this game. In every steps, the player that plays this step should move one of his or her token to any other empty block. The first player such that his or her 3 tokens are collinear will win the game. Have fun!\n\n";
	char key[3][3]={"qw", "as", "zx"}; key[0][2]='e', key[1][2]='d', key[2][2]='c';
	cout<<"Control:\n", printBoard(key), cout<<"For example, \"ac\" means to move the token on a to c\n\n";
	bool fp=0;
	int xkey[256], ykey[256];
	for(int i=0; i<256; ++i)xkey[i]=ykey[i]=-1;
	rep(i, 3)rep(j, 3)xkey[key[i][j]]=i, ykey[key[i][j]]=j;
	cout<<"The first player plays 'o'.\nDo you want to go first (y/n)? ";
	char c; cin>>c, fp=c=='y', cin.get();
	if(fp)cout<<"You play 'o'.\n";
	else cout<<"You play 'x'.\n";
	long long ta=20000000, tb=3000000;
	for(int t=0; 1; ++t){
		cout<<t<<" rounds\n";
		now.print();
		if((t&1)^fp){
			char c, d;
			cout<<"You have "<<(double)ta/1000000<<"+3 seconds.\n";
			timeval st, ed; gettimeofday(&st, NULL);
			while(1){
				string s;
				cout<<"Enter your move: ", getline(cin, s);
				if(s.size()==2&&(now.a[xkey[c=s[0]]][ykey[c]]==1)^fp&&now.a[xkey[d=s[1]]][ykey[d]]==0)break;
				cout<<"Invalid input.\n";
			}
			gettimeofday(&ed, NULL), ed.tv_sec-=st.tv_sec, ed.tv_usec-=st.tv_usec;
			if(ed.tv_usec<0)--ed.tv_sec, ed.tv_usec+=1000000;
			if(ed.tv_sec>=3)ta-=ed.tv_sec*1000000+ed.tv_usec;
			if(ta<0)cout<<"You don't have time. You lose, haha.\n", exit(0);
			swap(now.a[xkey[c]][ykey[c]], now.a[xkey[d]][ykey[d]]), now=!now;
		}
		else{
			int mx=0, id=p[now];
			for(int i=0; i<G[id].size(); ++i)if(fp^(mx>b[G[id][i]]))mx=b[G[id][i]];
			vector<int> tmp;
			for(int i=0; i<G[id].size(); ++i)if(b[G[id][i]]==mx)tmp.push_back(G[id][i]);
			now=a[tmp[rand()%tmp.size()]];
			if(mx)for(int i=0; i<G[id].size(); ++i)if(a[G[id][i]].score()==mx){now=a[G[id][i]]; break;}
			if(mx&&now.score()==mx)now.print(), cout<<"Haha, you lose.\n", exit(0);
		}
	}
}
