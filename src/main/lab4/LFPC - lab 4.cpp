#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <sstream>
#include <ctype.h>
using namespace std;

class CNF {

protected:

	list <string> pars;
	list <string> acc;
	list <string> prod;
	list <char> term;
	list <string> nonterm;

	struct Rules {
		string sn;
		list <string> vars;
	};

	void checkEmpty(CNF& temp, Rules r) {
		temp.haveEmpty = false;
		for (auto& i : temp.v) {
			for (auto& j : i.vars) {
				for (int l = 0; l < j.size(); l++) {
					if (j[l] == r.sn[0]) {
						substring(j, j[l], 0, "", temp);
					}
				}
			}
			temp.pars.sort();
			temp.pars.unique();
			i.vars.splice(i.vars.end(), temp.pars);
			temp.pars.clear();
		}
		for (auto& i : temp.v) {
			i.vars.sort();
			i.vars.unique();
		}
	}

	void substring(string s, char t, int init,string answer,CNF &temp) {
		string ans=answer;
		string ans1 = answer;
		for (int i = init; i < s.size(); i++) {
			if (s[i] == t) { 
				ans1 = ans1 + s[i];
				substring(s.substr(0, i) + s.substr(i + 1, s.size() - i - 1), t, i, ans, temp);
				substring(s.substr(0, i) + s.substr(i + 1, s.size() - i - 1), t, i, ans1, temp);
			}
			else {
				ans = ans + s[i];
				ans1 = ans1 + s[i];
			}
		}

		//if (ans == "")cout << "empty" << endl; else cout << ans << endl;
		//if (ans1 == "")cout << "empty" << endl; else cout << ans1 << endl;
		if (ans == "") { temp.pars.push_back("empty"); haveEmpty = true; }
		else temp.pars.push_back(ans);
		if (ans1 == "") { temp.pars.push_back("empty"); haveEmpty = true; }
		else temp.pars.push_back(ans1);

	}

	void unitRemain(CNF& temp, string s, Rules &r) {
		for (auto& i : temp.v) {
			if (i.sn == s) {
				r.vars.insert(r.vars.end(), i.vars.begin(), i.vars.end());
			}
		}


	}

	void inaccesible(CNF& temp, Rules& r) {
		for (auto& j : r.vars) {
			for (int i = 0; i < j.size(); ++i) {
					string stemp;
					stemp.append(1,j[i]); 
				if (isupper(j[i]) && find(temp.acc.begin(),temp.acc.end(),stemp) == temp.acc.end()) {
					temp.acc.push_back(stemp);
					for (auto& k : temp.v) {
						if (k.sn == stemp) {
							inaccesible(temp, k);
						}
					}
				}
			}
		}
	}

	void unproduct(CNF& temp) {
		for (auto& i : temp.v) {
			int ind = 0;
			for (auto& j : i.vars) {
			bool b = true;
				for (int l = 0; l < j.size(); l++) {
					if (isupper(j[l]) && find(temp.prod.begin(),temp.prod.end(),""+j[l])==temp.prod.end() ) {
						b = false;
					}
				}
				if (b == false) { ind++; }
			}
			if (ind != i.vars.size()) { temp.prod.push_back(i.sn);
			}
		}
	}

	void toNet(CNF& temp , string s) {
		for (int l = 0; l < s.size(); ++l) {
			if (islower(s[l]) &&  find(temp.term.begin(),temp.term.end(),s[l]) == temp.term.end()) {
				temp.term.push_back(s[l]);
				Rules rtemp;
				rtemp.sn = "X" + to_string(temp.term.size());
				stringstream ss;
				ss << s[l];
				rtemp.vars.push_back(ss.str());
				temp.v.push_back(rtemp);
			}
		}
	}

	void rewriteTerm(CNF& temp) {
		for (auto& i : temp.v) {
			for (auto& j : i.vars) {
				if (j.size() > 1) {
					for (int l = 0; l < j.size(); l++) {
						if (islower(j[l]) && find(temp.term.begin(), temp.term.end(), j[l]) != temp.term.end()) {
							for (int o = 0; o < temp.v.size(); o++) {
								if (temp.v[o].vars.size() == 1) {
									string stt{ j[l] };
									if (find(temp.v[o].vars.begin(),temp.v[o].vars.end(),stt) != temp.v[o].vars.end()) {
										j.replace(l, 1, temp.v[o].sn);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	bool doublesCh(string s) {
		int up = 0;
		for (int l = 0; l < s.size(); ++l) {
			if (isupper(s[l])) {
				up++;
			}
		}
		if (up == 2)return true;
		else return false;
	}

	void checkDoubl(CNF& temp,Rules& r,vector <Rules> &temporaryRules) {
		for (auto& j : r.vars) {
			int up = 0;
			for (int l = 0; l < j.size(); ++l) {
				if (isupper(j[l])) {
					up++;
				}
			}


			if (up > 2) {
				int ind=0,length=0;
				string s = "";
				for (int l = 0; l < j.size(); ++l) {
					if (isupper(j[l])) {
						length++;
						if (isdigit(j[l + 1])) { s = s + j[l] + j[l + 1]; length++; }
						else s = s + j[l];
						ind++;
						if (ind == 2) {
							if (find(temp.nonterm.begin(), temp.nonterm.end(), s) == temp.nonterm.end()) {
							temp.nonterm.push_back(s);
							Rules rtemp;
							rtemp.sn = "X"+to_string(temp.term.size()+temp.nonterm.size());
							rtemp.vars.push_back(s);
							j.replace(0, length, rtemp.sn);
							temporaryRules.push_back(rtemp);
							s = "";
							}
							else {
								for (auto& k : temp.v) {
									if (k.vars.front() == s) {
										j.replace(0, length, k.sn);
										s = "";
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void replaceDoubles(CNF &temp) {

	}

public:

	bool haveEmpty = false;

	vector <CNF::Rules> v;

	void citire(string s, CNF &temp);

	void output(CNF &temp);

	void deleteEmpty(CNF& temp);

	void isEmpty(CNF &temp);

	void unitProduction(CNF& temp);

	void inaccesibleSymb(CNF& temp);

	void unproductSymb(CNF& temp);

	void cnfForm(CNF& temp);
};

void CNF::citire(string s, CNF &temp) {
	stringstream ss;
	ss << s;
	string tt;
	Rules rtemp;
	int ind=0;
	bool b=false,c=false;
	while (ss >> tt) {
		if (isupper(tt[0]) && c == false) {
			for (const Rules& i : temp.v) {
				if (i.sn == tt) {
					b = true;
					break;
				}
				ind++;
			}

			if (b == false) {
				rtemp.sn = tt;
				temp.v.push_back(rtemp);
			}

		}
		else if (tt != "->") {
			if (tt == "empty")temp.haveEmpty = true;
			temp.v[ind].vars.push_back(tt);
		}
		else c = true;
	}

}

void CNF::output(CNF &temp) {
	for (const Rules &i : temp.v) {
		cout << i.sn << " -> ";
		for (const auto &j : i.vars) {
			cout << j << " | ";
		}
		cout << endl;
	}
}

void CNF::deleteEmpty(CNF &temp) {
	for (auto &i : temp.v) {
		for (const auto &j : i.vars) {
			if (j == "empty") {
				checkEmpty(temp, i);
			}
		}
	i.vars.remove("empty");
	}
}

void CNF::unitProduction(CNF& temp) {
	list <string> rem;
	for (auto& i : temp.v) {
		for (auto& j : i.vars) {
			if (j.size()==1 && i.sn != j && isupper(j[0])) {
				unitRemain(temp, j, i);
				rem.push_back(j);
			}
			else if (i.sn == j) {
				rem.push_back(j);
			}
			
		}
	}
	for (auto& i : temp.v) {
		for (auto& j : rem) {
			i.vars.remove(j);
		}
		i.vars.sort();
		i.vars.unique();
	}

}

void CNF::inaccesibleSymb(CNF& temp) {
	Rules s = temp.v[0];
	for(int i=0;i<temp.v.size();i++)
	inaccesible(temp, s);
	temp.acc.sort();
	temp.acc.unique();
	bool b = false;
	int ind = 0;
	vector <int> vec;
	for (auto& i : temp.v) {
		b = false;
		for (auto& j : temp.acc) {
			if (i.sn == j)b = true;
		}

		if (b == false)vec.push_back(ind);

		ind++;
	}

	ind = 0;

	for (int i = 0; i < vec.size(); ++i) {
		temp.v.erase(temp.v.begin() + vec[i]-ind);
		ind++;
	}
}

void CNF::unproductSymb(CNF& temp) {
	for (int l = 0; l < temp.acc.size(); l++)unproduct(temp);
	temp.prod.sort();
	temp.prod.unique();
	bool b = false;
	int ind = 0;
	vector <int> vec;
	for (auto& i : temp.v) {
		b = false;
		for (auto& j : temp.prod) {
			if (i.sn == j)b = true;
		}

		if (b == false) {
			vec.push_back(ind);
			for (auto& k : temp.v) {
				Rules rtemp = k;
				for (auto& j : rtemp.vars) {
					for (int sz = 0; sz < j.size(); sz++) {
						//cout << k.sn << " " << j << " " << i.sn[0] << endl;
						if (j[sz] == i.sn[0]) {
						//	cout << j << endl;
							k.vars.remove(j);
						}
					}
				}
			}
		}

		ind++;
	}

	ind = 0;

	for (int l = 0; l < vec.size(); ++l) {
		temp.v.erase(temp.v.begin() + vec[l] - ind);
		ind++;
	}
}

void CNF::cnfForm(CNF& temp) {
	vector<Rules> dbug;
	dbug = temp.v;
	for (auto& i : dbug) {
		for (auto& j : i.vars) {
			int k=j.size();
			if (k > 1) {
				toNet(temp, j);
			}

		}
	}

	rewriteTerm(temp);

	vector <Rules> temporaryRules;
	int ii = temp.v.size();

	for (int chk = 0; chk <= ii; chk++) {
		for (auto& i : temp.v) {
			checkDoubl(temp, i, temporaryRules);
		}
		temp.v.insert(temp.v.end(), temporaryRules.begin(), temporaryRules.end());
		temporaryRules.clear();
	}

}

void CNF::isEmpty(CNF &temp) {

}

class GNF :private CNF{


public:
	void leftRecursion(GNF& temp);

	void createInitGNF(GNF& temp);

	void output(GNF& temp);


};

void GNF::leftRecursion(GNF& temp)
{
	vector <string> vs;
	vector <string> del;
	vector <Rules> vr;
    Rules rt;
	vector<Rules>::iterator i = temp.v.begin();
	while (i != temp.v.end()) {
		vs.clear();
		rt = (*i);
		list<string>::iterator j = (*i).vars.begin();
		list<string>::iterator jtemp = (*i).vars.begin();
		while (j!=(*i).vars.end()) {
			if (( * i).sn[0] == ( * j)[0]) {
				string stemp = (*j).substr(1, (*j).size() - 1);
				vs.push_back(stemp);
				jtemp = j++;
				(*i).vars.remove((*j));

				temp.output(temp);
				cout << endl;
			}
			else {
				(*j) = (*j) + (*i).sn + "'";
			j++;
			}
			j = jtemp;

			jtemp++;
		}
		if (vs.size() > 0) {
			Rules rr;
			rr.sn = (*i).sn + "'";
			for (auto& j : vs) {
				rr.vars.push_back(j);
			}
			temp.v.push_back(rr);
		}
		++i;
	}

}

void GNF::createInitGNF(GNF& temp) {
	CNF::Rules r1;
	CNF::Rules r2;
	r1.sn = "A";
	r2.sn = "B";
	r1.vars.push_back("ABd");
	r1.vars.push_back("Aa");
	r1.vars.push_back("a");
	r2.vars.push_back("Be");
	r2.vars.push_back("b");

	temp.v.push_back(r1);
	temp.v.push_back(r2);

}

void GNF::output(GNF& temp) {
	for (const Rules& i : temp.v) {
		cout << i.sn << " -> ";
		for (const auto& j : i.vars) {
			cout << j << " | ";
		}
		cout << endl;
	}
}

int main(){

	string line;
	ifstream MyReadFile("Source.txt");

	CNF g;
	GNF gg;
	//g.index = 0;

	while (getline(MyReadFile, line)) {

		g.citire(line, g); // citirea din fisier si controlul daca e empty ( inclusiv inscrierea in vector )

	}

	cout << "First Step:" << endl;

	g.output(g);

	cout << endl << endl;

	cout << "Empty delete Step:" << endl;

	while (g.haveEmpty == true) {

		g.deleteEmpty(g); //daca e gasit un rule cu empty se face check si eliminarea acestora

	}

	g.output(g);
	
	cout << endl << endl;

	cout << "UnitProd. delete Step:" << endl;

	g.unitProduction(g);

	g.output(g);

	cout << endl << endl;

	cout << "Innacc. symbl delete Step:" << endl;

	g.inaccesibleSymb(g);

	g.output(g);

	cout << endl << endl;

	cout << "Unproduct. symbl delete Step:" << endl;

	g.unproductSymb(g);

	g.output(g);

	cout << endl << endl;

	cout << "Chomsky Form :" << endl;

	g.cnfForm(g);

	g.output(g);

	cout << endl << endl;



	//gg.v = g.v ;

	cout << "Greibah Form :" << endl;

	gg.createInitGNF(gg);

	gg.output(gg);

	gg.leftRecursion(gg);

	gg.output(gg);


	MyReadFile.close();
	return 0;
}

