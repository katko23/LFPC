#include <iostream>
#include <list>
#include <vector>
#include <fstream>
using namespace std;

class Lexer {	
	vector<string> function{ "\{","\}","Return","Func",";","\(","\)"};
	vector<string> bool_lit{"True","False"};
	vector<string> types{"Int","Double","Bool","Char","String","Void"};
	vector<char> digit{ '0','1','2','3','4','5','6','7','8','9','.'};
	vector<char> alpha{ 'a','b','c','d','e','f','g','h','i','j','k',
		'l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','\''};
	vector<char> arith_op{'+','-','*','/','%'};
	vector<string> comp_op{ "<",">","<=",">=","==","!="};
	vector<string> cond_op{ "&&","||","And","Or" };
	vector<string> assign_op{ "=","+=","-="};

	vector<string> output;

	string convertToString(char a){
		string s(1,a);
		return s;
	}

	void search(string in) {
		try {
			if (isupper(in[0])) {
				for (auto i : function)
					if (in == i) {
						if (in == "{")output.push_back("{ token Starting_block");
						else if (in == "}")output.push_back("} token Ending_block");
						else if (in == ";")output.push_back("; token Param_delimitation");
						else {
							for (int i = 0; i < in.size(); i++) {
								string ss(1, in[i]);
							output.push_back(ss + " token User_function");
							}

						}
						return;
					}
				for (auto i : types)
					if (in == i) {
						for (int i = 0; i < in.size(); i++) {
							string ss(1, in[i]);
							output.push_back(ss + " token Type");
						}
						return;
					}
				for (auto i : bool_lit)
					if (in == i) {
						for (int i = 0; i < in.size(); i++) {
							string ss(1, in[i]);
							output.push_back(ss + " token Bool_list");
						}
						return;
					}
				for (auto i : cond_op)
					if (in == i) {
						for (int i = 0; i < in.size(); i++) {
							string ss(1, in[i]);
							output.push_back(in + " token Cond_op");
						}
						return;
					}
			}
					
			for (int j = 0; j < in.size(); j++) {
				bool brak = false;
				for (auto i : alpha) {
					if (in[j] == i) {
						string ss(1, in[j]);
						output.push_back(ss + " token Char");
						brak = true;
						break;
					}
				}
				if (brak == true)continue;
				for (auto i : digit) {
					if (in[j] == i) {
						string ss(1, in[j]);
						output.push_back(ss + " token Digit");
						brak = true;
						break;
					}
				}
				if (brak == true)continue;

				if (in[j+1]=='=' || in[j + 1] == '&' || in[j + 1] == '|') {
					string temp(1,in[j]);
					temp = temp + in[j + 1];
					for (auto i : comp_op) {
						if (temp == i) {
							output.push_back(temp + " token Comp_op");
							j++;
							brak = true;
							break;
						}
					}
					for (auto i : cond_op) {
						if (temp == i) {
							output.push_back(temp + " token Cond_op");
							j++;
							brak = true;
							break;
						}
					}
					for (auto i : assign_op) {
						if (temp == i) {
							output.push_back(temp + " token Assing_op");
							j++;
							brak = true;
							break;
						}
					}
				}

				for (auto i : arith_op) {
					if (in[j] == i) {
						output.push_back(convertToString(in[j]) + " token Arith_op");
						brak = true;
						break;
					}
				}
				for (auto i : assign_op) {
					if (in == i) {
						output.push_back(in + " token Assign_op");
						brak = true;
						break;
					}
				}

				string ss(1, in[j]);
				for (auto i : function)
					if (ss == i) {
						if (ss == "{") { output.push_back("{ token Starting_block"); brak = true; break; }
						else if (ss == "}") { output.push_back("} token Ending_block"); brak = true; break; }
						else if (ss == ";") { output.push_back("; token Param_delimitation"); brak = true; break; }
						else if (ss == "(") { output.push_back("( token Param_start"); brak = true; break; }
						else if (ss == ")") { output.push_back(") token Param_end"); brak = true; break; }
					}
				
				if (brak == false)throw 404;
			}
		}
		catch (int error) {
			if (error == 404) { cout << "Caracter nedefinit , lipseste in elementele terminale"; 
			output.push_back(in+" token has an undefined terminal parameter");
			}

		}

	}

public:

	void citire(string file) {
		ifstream inFile(file);
		string s;

		while (!inFile.eof()) {
			inFile >> s;
			cout << s << endl;
			search(s);
		}

		inFile.close();
	}

	void inscriere(string file) {
		ofstream outFile(file);
		output.push_back("\nSfarsit");
		for (auto i = output.begin(); i != output.end(); ++i)
			outFile << *i << endl;
		outFile.close();
	}
};
int main(){
	Lexer v1 ;

	v1.citire("Source.txt");
	v1.inscriere("Output.txt");

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

