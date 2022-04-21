#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <sstream>
#include <ctype.h>
#include <map>
#include "ll1add.h"
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

	void unitRemain(CNF& temp, string s, Rules &r, list<string> &writed) {
		for (auto& i : temp.v) {
			if (i.sn == s) {
				r.vars.insert(r.vars.end(), i.vars.begin(), i.vars.end());
				writed.push_back(s);
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
					string tmp_string;
					if (isupper(j[l])) {
						tmp_string.append(1, j[l]);
					}
					if (isupper(j[l]) && find(temp.prod.begin(),temp.prod.end(),tmp_string)==temp.prod.end() ) {
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
							temp.lengthterm = temp.term.size() + temp.nonterm.size();
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
	int lengthterm;

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
	list <string> writed;
	list <string> rem;
	for (auto& i : temp.v) {
		for (auto& j : i.vars) {
			if (j.size()==1 && i.sn != j && isupper(j[0]) && find(writed.begin(), writed.end(), j) == writed.end()) {
				unitRemain(temp, j, i , writed);
				rem.push_back(j);

			}
			else if (i.sn == j ) {
				rem.push_back(j);

			}
		
		}
		writed.clear();
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
			if (i.sn == j )b = true;
		}

		if (b == false && i.sn!="S")vec.push_back(ind);

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


		//check daca exista productie cu rule scrisa mai devreme
		size_t found;
		for (auto& i : temp.v) {
			for (auto& j : i.vars) {
				for (auto& k : temporaryRules) {
					for (auto& p : k.vars) {
						//while (j.find(p) == string::npos) {
							found = j.find(p);
							if (found != string::npos && (i.sn != k.sn) && j.size()>2) {
								j.replace(found, p.size(), k.sn);
							}
						//}
					}
				}
			}
		}

		temporaryRules.clear();

	}		

}

void CNF::isEmpty(CNF &temp) {

}

class GNF :private CNF{

	list<string> terms;

	list<string> indrec;


	void terminalsGNF(GNF& temp) {
		bool b;
		for (auto& i : temp.v) {
			b = true;
			for (auto& j : i.vars) {
				if (isupper(j[0])) {
					b = false;
					break;
				}
			}
			if (b)temp.terms.push_back(i.sn);
		}
	}
	
	bool leftrec(GNF& temp, char check, string init) {
		Rules rt;
		for (auto& i : temp.v) if (i.sn[0] == check && i.sn.size() == 1) {
			rt = i; 
			break;
		}

		for (auto& j : rt.vars) 
			if (j[0] == init[0])return true;

		for (auto& j : rt.vars) {
			if (isupper(j[0]))
				if (leftrec(temp, j[0], init)) {
					return true;
				}
		}

		return false;
	}


public:
	int lengthterm = 0;

	vector <GNF::Rules> v;

	void leftRecursion(GNF& temp);

	void createInitGNF(GNF& temp);

	void output(GNF& temp);

	void leftProduction(GNF& temp);

	void gnfForm(GNF& temp);

};

void GNF::leftRecursion(GNF& temp)
{
	bool b ;
	vector <Rules> vr;
    Rules rt;
	list <string> lt;
	for (auto& i : temp.v) {
		b = false;
		for (auto& j : i.vars) 
			if (j[0] == i.sn[0]) {
				b = true;
				break;
			}

		for (auto& j : i.vars) {
			if (j[0] == i.sn[0]) {
				rt.sn = i.sn + "'";
				rt.vars.push_back(j.substr(1, j.size() - 1));
				rt.vars.push_back(j.substr(1,j.size()-1) + i.sn+"'");
				j = "0";
			}
			else {
				if (b) {
					string jtemp = j;
					j = j + i.sn + "'";
					lt.push_back(jtemp);
				}
			}
		}
		if (b) {
			i.vars.splice(i.vars.end(), lt);
			vr.push_back(rt);
			rt.vars.clear();
		}
	}

	for (auto& i : temp.v) {
		i.vars.remove("0");
	}
	temp.v.insert(temp.v.end(),vr.begin(),vr.end());

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

void GNF::leftProduction(GNF& temp) {
	vector <Rules> vr;
	Rules rt;
	bool b;
	for (auto& i : temp.v) {
		b = false;

		for (auto& j : i.vars) {
			if (isupper(j[0]) && i.sn.size() == 1)
				if (leftrec(temp, j[0], i.sn)) {
					b = true;
					temp.lengthterm++;
					break;
				}
		}

		for (auto& j : i.vars) {
			if (isupper(j[0]) && i.sn.size() == 1) {
				if (leftrec(temp, j[0], i.sn)) {
					if (b) {
						rt.sn = "X" + to_string(temp.lengthterm);
						rt.vars.push_back(j.substr(1, j.size() - 1) + rt.sn);
						rt.vars.push_back(j.substr(1, j.size() - 1));
						j = "0";
					}
				}
				else {
					if(b)
					j = j + "X" + to_string(temp.lengthterm);
				}
			}
			else {
				if (b)
				j = j + "X" + to_string(temp.lengthterm);
			}
		}
		if (b) {
			vr.push_back(rt);
			rt.vars.clear();
		}
	}

	for (auto& i : temp.v) {
		i.vars.remove("0");
	}
	temp.v.insert(temp.v.end(), vr.begin(), vr.end());
}

void GNF::gnfForm(GNF& temp) {
	vector <Rules> vr;
	Rules rt;
	bool check = true;

	while (check) {
		temp.terminalsGNF(temp);
		temp.terms.sort();
		temp.terms.unique();
		check = false;
		for (auto& i : temp.v) {
			for (auto& j : i.vars) {
				if (isupper(j[0]) && (j[1] == '\'' || isdigit(j[1]))) {
					if (j[2] == '\'' || isdigit(j[2])) {
						for (auto& k : temp.v) {
							string sstemporary = j.substr(0, 3);
							if (k.sn == sstemporary && k.sn.size() == 3 /* && find(temp.terms.begin(), temp.terms.end(), k.sn) != temp.terms.end() */) {
								for (auto& p : k.vars) {
									i.vars.push_back(p + j.substr(3, j.size() - 1));
								}
								j = "0";
								//temp.leftRecursion(temp);
							}
						}
						check = true;
					}
					else {
						for (auto& k : temp.v) {
							string sstemporary = j.substr(0, 2);
							if (k.sn == sstemporary && k.sn.size() == 2 /* && find(temp.terms.begin(), temp.terms.end(), k.sn) != temp.terms.end() */) {
								for (auto& p : k.vars) {
									i.vars.push_back(p + j.substr(2, j.size() - 1));
								}
								j = "0";
								//temp.leftRecursion(temp);
							}
						}
						check = true;
					}
				}else if(isupper(j[0])) {
					for (auto& k : temp.v) {
						if (k.sn[0] == j[0] && k.sn.size() == 1 /* && find(temp.terms.begin(), temp.terms.end(), k.sn) != temp.terms.end() */ ) {
							for (auto& p : k.vars) {
								i.vars.push_back(p + j.substr(1, j.size() - 1));
							}
							j = "0";
							//temp.leftRecursion(temp);
						}
					}
					check = true;
				}


			}

		}

		//temp.output(temp);
		//cout << endl;
	}


	for (auto& i : temp.v) {
		i.vars.remove("0");
		i.vars.remove("00");
		i.vars.sort();
		i.vars.unique();
	}
}

class LLOne:private CNF,private GNF,private ll1add {

	void sort_Uni(LLOne& llo) {
		for (auto& i : llo.follow) {
			i.vars.sort();
			i.vars.unique();
			i.vars.remove("empty");
		}
	}

	struct Nod {
		string f;
		string rule;
	};


public:
	int lengthterm;

	vector <LLOne::Rules> v;

	vector <LLOne::Rules> first;

	vector <LLOne::Rules> follow;

	list <string> emptyRules;

	map <string, list <Nod>> table;

	bool wordb;

	string word;

	void left_fact(LLOne& temp);

	void output(LLOne temp);

	void outputV(std::vector<LLOne::Rules>& v);

	void removedub(vector<LLOne::Rules>& v);

	void writeFirst(LLOne& llo);

	void writeFollow(LLOne& llo);

	void writeTable(LLOne& llo);
	
	void outputTable(LLOne& llo);

	void checkWord(LLOne& llo, string word, string now);
};

void LLOne::left_fact(LLOne& llo) {
	LLOne llt;
	llt = llo;
	list <string> was;
	for (auto& i : llo.v) {
		for (auto& j : i.vars) {
			for (int k = j.size(); k >0; k--) {
				string temp;
				int ind = 0;
				temp = j.substr(0, k);
				for (auto o : i.vars) {
					string check=o.substr(0,k);
					if (check == temp)ind++;
				}
				if (ind > 1 && find(was.begin(),was.end(),temp)==was.end()) {
					llo.lengthterm++;
					Rules rt;
					for (auto& o : i.vars) {
						string check = o.substr(0, k);
						if (check == temp) {
							string ss = o.substr(k, o.size());
							for (auto& it : llt.v) {
								if (it.sn == i.sn) {
									it.vars.push_back(temp + "X" + to_string(llo.lengthterm));
									it.vars.remove(o);
									if (ss == "")rt.vars.push_back("empty");else rt.vars.push_back(ss);
								}
							}
							rt.sn="X" + to_string(llo.lengthterm);
							//llo.output(llo);
							//cout << endl << "after X" << endl;
							//llo.output(llt);
						}
					}
					was.push_back(temp);
					llt.v.push_back(rt);
					//llo.output(llo);
					//cout << endl << "after push" << endl;

					//llo.output(llt);
				}
			}
		}
	}
	for (auto& it : llt.v) {
		it.vars.sort();
		it.vars.unique();
	}
	//llo.output(llt);
	//cout << endl;

	removedub(llt.v);

	//llo.output(llt);
	//cout << endl;
	llt.lengthterm = lengthterm;
	llo = llt;

}

void LLOne::removedub(std::vector<LLOne::Rules>& v)
{
	for (int i = 0; i < v.size(); i++) {
		for (int j = v.size() - 1; j >= 0; j--) {
			if (v[i].sn != v[j].sn && v[i].vars == v[j].vars) {
				for (auto& p : v) {
					for (auto& k : p.vars) {
						for (int o = 0; o < k.size(); o++) {
							size_t found = k.find(v[j].sn);
							if (found != string::npos) {
								k.replace(found, v[j].sn.size(), v[i].sn);
							}
						}
					}
				}
				v.erase(v.begin() + j);

				break;
			}
		}
	}

}

void LLOne::output(LLOne temp) {
	for (const Rules& i : temp.v) {
		cout << i.sn << " -> ";
		for (const auto& j : i.vars) {
			cout << j << " | ";
		}
		cout << endl;
	}
}

void LLOne::outputV(std::vector<LLOne::Rules>& v) {
	for (const Rules& i : v) {
		cout << i.sn << " -> ";
		for (const auto& j : i.vars) {
			cout << j << " | ";
		}
		cout << endl;
	}
}

void LLOne::writeFirst(LLOne& llo) {
	for (auto& i : llo.v) {
		LLOne::Rules rt;
		for (auto& j : i.vars) {
			if (islower(j[0]) ){//&& j != "empty") {
				string ss = j.substr(0, 1);
				if (j == "empty") {
					ss = "empty";
					llo.emptyRules.push_back(i.sn);
				}
				rt.vars.push_back(ss);
			}
		}

		rt.vars.sort();
		rt.vars.unique();

		rt.sn = i.sn;
		llo.first.push_back(rt);
	}
}

void LLOne::writeFollow(LLOne& llo) {
	for (auto& i : llo.v) {
		if (i.sn == "S") {
			LLOne::Rules rt;
			rt.sn = "S";
			rt.vars.push_back("$");
			bool check = true;
			for (auto& j : llo.follow)if (j.sn == rt.sn)check = false;
			if(check)llo.follow.push_back(rt);
			llo.sort_Uni(llo);
		}
		for (auto& j : i.vars) {
			if (j != "empty") {
				for (int o = 0; o < j.size(); o++) {
					if (isupper(j[o]) && (j[o + 1] == '\'' || isdigit(j[o + 1]) && isdigit(j[o+2])) && j.size() == 4) {
						LLOne::Rules rt;
						string sstemp = j.substr(o, 3);
						rt.sn = sstemp;
						bool check = true;
						for (auto& k : llo.follow) {
							if (k.sn == i.sn) {
								rt.vars = k.vars;
							}
							if (k.sn == rt.sn)check = false;
						}

						if (check)llo.follow.push_back(rt); else {
							for (auto& k : llo.follow) {
								list <string> templist = rt.vars;
								if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
							}
						}
						llo.sort_Uni(llo);
					}
					else if (isupper(j[o]) && (j[o + 1] == '\'' || isdigit(j[o + 1])) && j.size() == 3) {
						LLOne::Rules rt;
						string sstemp = j.substr(o, 2);
						rt.sn = sstemp;
						bool check = true;
						for (auto& k : llo.follow) {
							if (k.sn == i.sn) {
								rt.vars = k.vars;
							}
							if (k.sn == rt.sn)check = false;
						}

						if (check)llo.follow.push_back(rt); else {
							for (auto& k : llo.follow) {
								list <string> templist = rt.vars;
								if (k.sn == rt.sn)k.vars.splice(k.vars.end(),templist);
							}
						}
						llo.sort_Uni(llo);
					}
					else if (isupper(j[o]) && j.size() == 2) {
						LLOne::Rules rt;
						string sstemp = j.substr(o, 1);
						rt.sn = sstemp;
						bool check = true;
						for (auto& k : llo.follow) {
							if (k.sn == i.sn) {
								rt.vars = k.vars;

							}
							if (k.sn == rt.sn)check = false;
						}

						if (check)llo.follow.push_back(rt); else {
							for (auto& k : llo.follow) {
								list <string> templist = rt.vars;
								if (k.sn == rt.sn)k.vars.splice(k.vars.end(),templist);
							}
						}
						llo.sort_Uni(llo);
					}

					if (isupper(j[o]) && (j[o + 1] == '\'' || isdigit(j[o + 1])) && isdigit(j[o+1])) {
						LLOne::Rules rt;
						string sstemp = j.substr(o, 3);
						rt.sn = sstemp;

						for (int p = o; p < j.size(); p++) {
							if (islower(j[p])) {
								string sst = j.substr(p, 1);
								rt.vars.push_back(sst);
								bool check = true;
								for (auto& k : llo.follow) {
									if (k.sn == rt.sn)check = false;
								}

								if (check)llo.follow.push_back(rt); else {
									for (auto& k : llo.follow) {
										list <string> templist = rt.vars;
										if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
									}
								}
								llo.sort_Uni(llo);
							}
							else if (isupper(j[p]) && (j[p + 1] == '\'' || isdigit(j[p + 1])) && j[o] + j[o + 1] + j[o + 2] != j[p] + j[p + 1] + j[p + 2] && isdigit(j[p + 2])) {
								string sst = j.substr(p, 3);
								bool check = true;
								if (find(llo.emptyRules.begin(), llo.emptyRules.end(), sst) == llo.emptyRules.end() && p + 3 == j.size()) {
									for (auto& k : llo.follow) {
										if (k.sn == i.sn) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
								else {
									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
							}
							else if (isupper(j[p]) && (j[p + 1] == '\'' || isdigit(j[p + 1])) ) {
								string sst = j.substr(p, 2);
								bool check = true;
								if (find(llo.emptyRules.begin(), llo.emptyRules.end(), sst) == llo.emptyRules.end() && p + 2 == j.size()) {
									for (auto& k : llo.follow) {
										if (k.sn == i.sn) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
								else {
									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
							}
							else if (isupper(j[p])) {
								string sst = j.substr(p, 1);
								bool check = true;
								if (find(llo.emptyRules.begin(), llo.emptyRules.end(), sst) == llo.emptyRules.end() && p + 1 == j.size()) {
									for (auto& k : llo.follow) {
										if (k.sn == i.sn) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
								else {
									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
							}
						}
					}else
					if (isupper(j[o]) && (j[o + 1] == '\'' || isdigit(j[o + 1]))) {
						LLOne::Rules rt;
						string sstemp = j.substr(o, 2);
						rt.sn = sstemp;

						for (int p = o; p < j.size(); p++) {
							if (islower(j[p])) {
								string sst = j.substr(p, 1);
								rt.vars.push_back(sst);
								bool check = true;
								for (auto& k : llo.follow) {
									if (k.sn == rt.sn)check = false;
								}

								if (check)llo.follow.push_back(rt); else {
									for (auto& k : llo.follow) {
										list <string> templist = rt.vars;
										if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
									}
								}
								llo.sort_Uni(llo);
							}
							else if (isupper(j[p]) && (j[p + 1] == '\'' || isdigit(j[p + 1])) && isdigit(j[p + 2])) {
								string sst = j.substr(p, 3);
								bool check = true;
								if (find(llo.emptyRules.begin(), llo.emptyRules.end(), sst) == llo.emptyRules.end() && p + 3 == j.size()) {
									for (auto& k : llo.follow) {
										if (k.sn == i.sn) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
								else {
									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
							}
							else if (isupper(j[p]) && (j[p + 1] == '\'' || isdigit(j[p + 1])) && j[o] + j[o + 1] != j[p] + j[p + 1]) {
								string sst = j.substr(p, 2);
								bool check = true;
								if (find(llo.emptyRules.begin(), llo.emptyRules.end(), sst) == llo.emptyRules.end() && p + 2 == j.size()) {
									for (auto& k : llo.follow) {
										if (k.sn == i.sn) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
								else {
									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
							}
							else if (isupper(j[p]) && j[o]!=j[p]) {
								string sst = j.substr(p, 1);
								bool check = true;
								if (find(llo.emptyRules.begin(), llo.emptyRules.end(), sst) == llo.emptyRules.end() && p + 1 == j.size()) {
									for (auto& k : llo.follow) {
										if (k.sn == i.sn) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
								else {
									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
							}
						}
					}
					else if (isupper(j[o])) {
						LLOne::Rules rt;
						string sstemp = j.substr(o, 1);
						rt.sn = sstemp;
						for (int p = o; p < j.size(); p++) {
							if (islower(j[p])) {
								string sst = j.substr(p, 1);
								rt.vars.push_back(sst);
								bool check = true;
								for (auto& k : llo.follow) {
									if (k.sn == rt.sn)check = false;
								}

								if (check)llo.follow.push_back(rt); else {
									for (auto& k : llo.follow) {
										list <string> templist = rt.vars;
										if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
									}
								}
								llo.sort_Uni(llo);
							}
							else if (isupper(j[p]) && (j[p + 1] == '\'' || isdigit(j[p + 1])) && isdigit(j[p + 2])) {
								string sst = j.substr(p, 3);
								bool check = true;
								if (find(llo.emptyRules.begin(), llo.emptyRules.end(), sst) == llo.emptyRules.end() && p + 3 == j.size()) {
									for (auto& k : llo.follow) {
										if (k.sn == i.sn) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
								else {
									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
							}
							else if (isupper(j[p]) && (j[p + 1] == '\'' || isdigit(j[p + 1]))) {
								string sst = j.substr(p, 2);
								bool check = true;
								if (find(llo.emptyRules.begin(), llo.emptyRules.end(), sst) == llo.emptyRules.end() && p + 2 == j.size()) {
									for (auto& k : llo.follow) {
										if (k.sn == i.sn) {
											list <string> templist = rt.vars;
											rt.vars.splice(rt.vars.end(), templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(),templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
								else {
									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(),templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
							}
							else if (isupper(j[p]) && j[o]!=j[p]) {
								string sst = j.substr(p, 1);
								bool check = true;
								if (find(llo.emptyRules.begin(), llo.emptyRules.end(), sst) == llo.emptyRules.end() && p + 1 == j.size()) {
									for (auto& k : llo.follow) {
										if (k.sn == i.sn) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(),templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(),templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(),templist);
										}
									}
									llo.sort_Uni(llo);
								}
								else {
									for (auto& k : llo.first) {
										if (k.sn == sst) {
											list <string> templist = k.vars;
											rt.vars.splice(rt.vars.begin(),templist);
										}
										if (k.sn == rt.sn)check = false;
									}

									if (check)llo.follow.push_back(rt); else {
										for (auto& k : llo.follow) {
											list <string> templist = rt.vars;
											if (k.sn == rt.sn)k.vars.splice(k.vars.end(), templist);
										}
									}
									llo.sort_Uni(llo);
								}
							}
						}

					}

				}
			}

		}


	}

	for (auto& i : llo.follow) {
		i.vars.sort();
		i.vars.unique();
		i.vars.remove("empty");
	}
}

void LLOne::writeTable(LLOne& llo) {
	for (auto& i : llo.v) {
		LLOne::Nod n;
		if (find(llo.emptyRules.begin(), llo.emptyRules.end(), i.sn) == llo.emptyRules.end()) {

			for (auto& j : i.vars) {
				if (islower(j[0])) {//&& j != "empty") {
					string ss = j.substr(0, 1);

					n.f = j[0];
					n.rule = j;
					llo.table[i.sn].push_back(n);
				}
			}
		}
		else {
			for (auto& j : i.vars) {
				if (islower(j[0])) {//&& j != "empty") {
					if (j != "empty") {
						string ss = j.substr(0, 1);
						n.f = j[0];
						n.rule = j;
						llo.table[i.sn].push_back(n);
					}
				}
			}


			for (auto& j : llo.follow) {
				if (j.sn == i.sn) {
					for (auto& k : j.vars) {
						n.f = k;
						n.rule = "empty";
						llo.table[i.sn].push_back(n);
					}

				}
			}
		}

	}
}

void LLOne::outputTable(LLOne& llo) {
	for (auto const& pair : llo.table) {
		std::cout <<  pair.first ;
		for (auto& i : pair.second) {
			cout << "      " << i.f <<":"<<i.rule;
		}
		cout << endl;
	}
}



void LLOne::checkWord(LLOne& llo, string word, string now) {
	if (llo.wordb)return;
	cout << word << " " << now << endl;
	if (word[0] == now[0])return;
	if (word == "") {
		llo.wordb = true;
	}
	for (auto r : llo.table[now]) {
		if (r.f[0] == word[0]) {
			if (r.rule == "empty") {
				llo.word = word;
				return;
			}
			for (int i = 0; i < r.rule.size(); i++) {
				if (llo.word.size() < word.size())word = llo.word;
				if (isupper(r.rule[i]) && (r.rule[i+1]=='\'' || isdigit(r.rule[i+1])) && isdigit(r.rule[i + 2])) {
					string s = r.rule.substr(i, 3);
					checkWord(llo, word.substr(1, word.size() - 1), s);
				}
				else if (isupper(r.rule[i]) && (r.rule[i + 1] == '\'' || isdigit(r.rule[i + 1]))) {
					string s = r.rule.substr(i, 2);
					checkWord(llo, word.substr(1, word.size() - 1), s);
				}
				else if (isupper(r.rule[i])) {
					string s = r.rule.substr(i, 1);
					checkWord(llo, word.substr(1, word.size() - 1), s);
				}
				else if (islower(r.rule[i])) {
					string s = r.rule.substr(i, 1);
					checkWord(llo, word.substr(1, word.size() - 1), s);
				}
			}
		}
	}

}


int main(){

	string line;
	ifstream MyReadFile("Source.txt");

	CNF g;
	GNF gg;
	LLOne llo;
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

	gg.v = g.v ;

	cout << "Chomsky Form :" << endl;

	g.cnfForm(g);

	g.output(g);

	cout << endl << endl;



	//gg.v = g.v ;
	gg.lengthterm = g.lengthterm;

	cout << "Greibah Form :" << endl;

	//gg.createInitGNF(gg);

	gg.output(gg);
	cout << endl << endl;


	cout << "Greibah Form after left recursion:" << endl;

	gg.leftRecursion(gg);

	gg.output(gg);
	cout << endl << endl;

	gg.leftProduction(gg);

	gg.output(gg);
	cout << endl << endl;

	MyReadFile.close();

	int indsv1 = gg.v.size();

	cout << "Delete first neterminal symbols and factorization:" << endl;
	gg.gnfForm(gg);

	gg.output(gg);
	cout << endl << endl;

	llo.v = gg.v;
	llo.lengthterm = gg.lengthterm;
	
	gg.gnfForm(gg);

	// transformam gramatica astfel incat sa eliminam toate recursiile stangi si factorizarea pentru a face mai usor first and follow
	// Gramatica se transforma in GREIBACH FORM 


	for (int j = 1; j <= 1; j++) {
		llo.v = gg.v;
		llo.lengthterm = gg.lengthterm;

		int indsv = llo.v.size();

		for (int i = 1; i <= indsv; i++)
			llo.left_fact(llo);

		gg.v = llo.v;
		gg.gnfForm(gg);

		llo.v = gg.v;


		llo.output(llo);
		cout << endl;

		llo.removedub(llo.v);

	}

	llo.output(llo);
	cout << endl;


	cout << "Facem first" << endl;
	llo.writeFirst(llo);
	llo.outputV(llo.first);
	cout << endl;

	cout << "Follow : " << endl;
	llo.writeFollow(llo);
	llo.writeFollow(llo);
	llo.writeFollow(llo);
	llo.writeFollow(llo);

	llo.outputV(llo.follow);
	cout << endl;

	//creem tabelul :
	llo.writeTable(llo);

	//llo.outputTable(llo);
	//cout << endl;

	string word;
	cout << endl << endl << "Dati cuvantul = ";
	cin >> word;

	llo.wordb = false;
	llo.word = word;
	llo.checkWord(llo, word, "S");
	if (llo.wordb)cout << "primit" << endl; else cout << "fals" << endl;

	return 0;
}

