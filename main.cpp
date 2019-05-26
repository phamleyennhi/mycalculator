#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include "El.h"
#include "Expression.h"

using namespace std;


int main(int argc, char *args[]){
	//START ARG
	string inputfile = "";
	string outputfile = "";
	int number = -1;
	for (int i = 0; i < argc; i++){
		if (string(args[i]) == "-i" && i < argc-1){
			inputfile = args[i+1];
		}
		else if (string(args[i]) == "-o" && i < argc-1){
			outputfile = args[i+1];
		}
	}
	if (inputfile == ""){
		cout << "Usage ./mycalc -i <inputfile> -o <outfile>" << endl;
		return EXIT_SUCCESS;
	}


	//READ FILE
	map<string, double>* expressionMap;
	vector<Expression>* exList;
	expressionMap = new map<string, double>();
	exList = new vector<Expression>();
	string name, ex;
	bool valid = true;

	ifstream fin;
	fin.open(inputfile);
	if (!fin.fail()){
		while (!fin.eof()){
			char str[512];
			fin.getline(str, 512);
			name = "";
			ex = "";
			valid = false;

			bool eq = true;
			for (int i = 0; i < 512; ++i)
			{

				if(eq){

					if(str[i] == ' ' && str[i+1] == '='){
						eq = false;
						i+=2;
						continue;
					}

					if (str[i] >= 65 || str[i] <= 90 || str[i] >= 97 || str[i] <= 122){
						name+=str[i];
					}else{
						break;
					}
					
				}else{
					if(str[i] == ';'){
						valid = true;
						break;
					}
					ex+=str[i];
				}
			}

			exList->push_back(Expression(name, ex, valid));
		}
	}

	//POSTFIX STACKS
	vector<Expression>::iterator it = exList->begin();


	for (it=exList->begin(); it!=exList->end(); ++it){
		it->checkValid();
		it->infixToPostfix();
	}


	//SOLVE
	for (int i = 0; i < exList->size(); ++i)
	{
		for (it=exList->begin(); it!=exList->end(); ++it){
			
			if(it->valid && !it->solved){
				double res = it->evaluate(*expressionMap);
				if(it->solved){
					it->answer = res;
				}
			}
			
		}
	}

	//OUTPUT

	if (outputfile == ""){
		//Output to terminal

		for (it=exList->begin(); it!=exList->end(); ++it){

			if(it->solved){
				if (it->answer == -0){
					it->answer = 0;
				}
				cout << it->name << " = " << it->answer<<endl;
			}
			else{
				cout << it->name << " = INVALID"<<endl;
			}
		}

	}else{
		//Save to file

		ofstream fout;
		fout.open(outputfile);

		if (fout.is_open()){

			for (it=exList->begin(); it!=exList->end(); ++it){

				if(it->solved){
					if (it->answer == -0){
						it->answer = 0;
					}
					fout << it->name << " = " << it->answer<<'\n';
				}
				else{
					fout << it->name << " = INVALID"<<'\n';
				}
			}

			fout.close();
		}
	}




	

	return EXIT_SUCCESS;
}