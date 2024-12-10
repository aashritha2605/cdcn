#include<bits/stdc++.h>
using namespace std;

set<char> calc_first(char nt,vector<pair<char,vector<string> > > &gram,int n){
	vector<string> prodn;
	for(int i=0;i<n;i++){
	if(gram[i].first==nt){
		prodn=gram[i].second;
		break;
		}
	}
	set<char> ans;
	for(int i=0;i<prodn.size();i++){
		if(!isupper(prodn[i][0])) ans.insert(prodn[i][0]);
		else{
			int ind=0;
			while(ind<(prodn[i].size())){
				bool eps=false;
				set<char> sfirst=calc_first(prodn[i][ind],gram,n);
				set<char>::iterator itr;
				for(itr=sfirst.begin();itr != sfirst.end();itr++){
					if(*itr=='#') eps=true;
					else ans.insert(*itr);
				}
				if(eps && ind==(prodn[i].size()-1)) ans.insert('#');
				if(eps) ind++;
				else break;
			}
			}
	}
	return ans;
}

set<char> calc_follow(char nt,vector<pair<char,vector<string> > > &gram,int n){
	set<char> ans;
	if(nt==gram[0].first) ans.insert('$');
	for(int i=0;i<n;i++){
		vector<string> prodn=gram[i].second;
		for(int j=0;j<prodn.size();j++){
			string pro=prodn[j];
			for(int k=0;k<pro.size();k++){
				if(pro[k]==nt){
					if(k==pro.size()-1 && gram[i].first!=nt){
						set<char> sfollow=calc_follow(gram[i].first,gram,n);
						set<char>::iterator itr;
						for(itr=sfollow.begin();itr != sfollow.end();itr++){
							if(*itr=='#') continue;
							ans.insert(*itr);
						}
					}
					else if(!isupper(pro[k+1]) && pro[k+1]!='#') ans.insert(pro[k+1]);
					else{
						int ind=k+1;
						while(ind<pro.size()){
							bool eps=false;
							set<char>::iterator itr;
							set<char> sfirst=calc_first(pro[ind],gram,n);
							for(itr=sfirst.begin();itr != sfirst.end();itr++){
								if(*itr=='#') eps=true;
								else ans.insert(*itr);
								}
							if(ind==pro.size()-1 && eps && gram[i].first!=nt){
								set<char> sfollow=calc_follow(gram[i].first,gram,n);
								set<char>::iterator itr;
								for(itr=sfollow.begin();itr != sfollow.end();itr++){
									if(*itr=='#') continue;
									ans.insert(*itr);
								}
							}
							if(!isupper(pro[ind+1]) && pro[ind+1]!='#'){
								ans.insert(pro[ind+1]);
								break;
							}
							if(eps) ind++;
							else break;
						}
					}
				}
			}
		}
	}
return ans;
}

void display(map<pair<char,char>,string> &mp){
	cout<<"Predictive Parse Table : "<<endl;
	map<pair<char,char>,string>:: iterator itr;
	for(itr=mp.begin();itr!=mp.end();itr++){
		cout<<"M["<<((itr->first).first)<<","<<((itr->first).second)<<"]="<<(itr->second)<<endl;
	}
}

void build_table(vector<pair<char,vector<string> > > &gram,int n,map<pair<char,char>,string> &parse_table){
	for(int i=0;i<n;i++){
	char nt=gram[i].first;
	vector<string> prodn=gram[i].second;
	for(int j=0;j<prodn.size();j++){
		string pro=prodn[j];
		if(!isupper(pro[0]) && pro[0]!='#'){
			parse_table[{nt,pro[0]}]=pro;
		}
		else{
			set<char> fst=calc_first(pro[0],gram,n);
			set<char>::iterator itr;
			for(itr=fst.begin();itr != fst.end();itr++){
				if(*itr=='#') continue;
				parse_table[{nt,*itr}]=pro;
			}
			if(fst.find('#')!=fst.end() || pro[0]=='#'){
				set<char> flw=calc_follow(nt,gram,n);
				for(itr=flw.begin();itr != flw.end();itr++){
					if(*itr=='\0' || *itr==' ') continue;
					parse_table[{nt,*itr}]='#';
				}
			}
		}
	}
	}
}

bool parse_string(string inp,map<pair<char,char>,string> &parse_table,char start){
	stack<char> st;
	st.push('$');
	st.push(start);
	int i=0;
	while(!st.empty()){
		char top=st.top();
		char cur=inp[i];
	if(top==cur){
		i++;
		st.pop();
	}
	else if(!isupper(top)){
		return false;
	}
	else if(parse_table.find({top,cur})!=parse_table.end()){
		st.pop();
		string pro=parse_table[{top,cur}];
		if(pro!="#"){
			for(int j=pro.size()-1;j>=0;j--){
				st.push(pro[j]);
			}
		}
	}
	else return false;
	}
	return (i==inp.size());
}

int main(){
	int n;
	cout<<"Enter the no.of lines of the grammar: ";
		cin>>n;
	vector<pair<char,vector<string> > > gram;
	cout<<"Enter the lines of grammar(put epsilon as #):\n";
	for(int i=0;i<n;i++){
		string ln;
		cin>>ln;
		string prod="";
		gram.push_back({ln[0],{}});
		for(int j=3;j<ln.size();j++){
		if(ln[j]=='/'){
			gram[i].second.push_back(prod);
			prod="";
		}
		else prod+=ln[j];
		}
		gram[i].second.push_back(prod);
	}
	vector<pair<char,vector<char> > > first(n),follow(n);
	for(int i=0;i<n;i++){
		first[i]={gram[i].first,{}};
		follow[i]={gram[i].first,{}};
	}
	for(int i=0;i<n;i++){
		set<char> st=calc_first(gram[i].first,gram,n);
		vector<char> tmp;
		set<char>::iterator itr;
		for(itr=st.begin();itr!=st.end();itr++) tmp.push_back(*itr);
			first[i].second=tmp;
	}
	for(int i=0;i<n;i++){
		cout<<"First of "<<first[i].first<<" : {"<<first[i].second[0];
		for(int j=1;j<first[i].second.size();j++) cout<<","<<first[i].second[j];
			cout<<"}"<<endl;
	}
	for(int i=0;i<n;i++){
		set<char> st=calc_follow(gram[i].first,gram,n);
		vector<char> tmp;
		set<char>::iterator itr;
		for(itr=st.begin();itr!=st.end();itr++) tmp.push_back(*itr);
			follow[i].second=tmp;
	}
	for(int i=0;i<n;i++){
		cout<<"Follow of "<<follow[i].first<<" : {"<<follow[i].second[0];
		for(int j=1;j<follow[i].second.size();j++) cout<<","<<follow[i].second[j];
			cout<<"}"<<endl;
	}
	map<pair<char,char>,string> parse_table;
	build_table(gram,n,parse_table);
	display(parse_table);
	string inp;
	cout<<"Enter the string to parse : ";
	cin>>inp;
	bool f=parse_string(inp,parse_table,gram[0].first);
	if(f) cout<<"String "<<inp<<" is a valid string"<<endl;
	else cout<<"String "<<inp<<" is not a valid string"<<endl;
	return 0;
}
