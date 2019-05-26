#ifndef EXPRESSION
#define EXPRESSION
#import<string>
#import<vector>
#import<map>
#import<stack>
#import "El.h"

class Expression{
	public:
		std::string expression;
		std::string name;
		double answer;
		bool solved;
		std::vector<El> postfix;
		std::vector<El> arr;
		bool valid;

	public:
		Expression();
		Expression(const std::string&, const std::string&, bool );
		void checkValid();
		void infixToPostfix();
		double evaluate(std::map<std::string, double>& expressionMap);
		bool tokenize();
};
#endif

