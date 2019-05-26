#include "Expression.h"
#define INF 1.79769e+308;


Expression::Expression(){
	solved = false;
	valid = false;
}


Expression:: Expression(const std::string& n, const std::string& e, bool v){
	expression = e;
	name = n;
	answer = 0;
	solved = false;
	valid = v;
}

void Expression::checkValid(){
	bool flag = true;



	if (tokenize()){
		// check if the expression is properly parenthesized.

		int leftPar = 0;
		int rightPar = 0;
		for (int i = 0; i < expression.length(); i++){
			if (expression.at(i) == '('){
				leftPar++;
			}
			else if (expression.at(i) == ')'){
				rightPar++;
			}
		}
		if (leftPar != rightPar){
			flag = false;
		}

		for (int i = 0; i < arr.size(); i++){
			//{ NUM = 0, VAR = 1, BIN = 2, UNA = 3, PAR = 4};
			//check if binary operator is valid. Binary operator is preceded by NUM/VAR/closing parenthesis. Binary operator is followed by NUM/VAR/opening parenthesis/unary operator.
			if (arr[i].type == 2){
          
              	if(i>0){
                  if(arr[i-1].type == 2 || arr[i-1].type == 3)
                    flag = false;
                  
                  if(arr[i-1].type == 4 && arr[i-1].symbol == "(")
                    flag = false;
                  
                }else{
                  flag = false;
                }
              
              
              	if(i < arr.size()-1){
                  if (arr[i+1].type == 2)
                    flag = false;
                  if (arr[i+1].type == 4 && arr[i+1].symbol == ")")
                    flag = false;
                  
                }else{
                  flag = false;
                }
              
			}
			//check if unary operator is valid. Unary operator is preceded by opening parenthesis/binary operator. Unary operator is followed by a VAR/NUM/opening parenthesis. 
			if (arr[i].type == 3){
              
              
              	if(i < arr.size()-1){
                  
                  if(arr[i+1].type == 2 || arr[i+1].type == 3)
                    flag = false;
                  
                  if(arr[i+1].type == 4 && arr[i+1].symbol == ")")
                      flag = false;
                  
            	}else{
                  flag = false;
                }
                if (i > 0){
                	if (arr[i-1].type == 1 || arr[i-1].type == 0 || arr[i-1].type == 3)
                		flag = false;
                	if (arr[i-1].type == 4 && arr[i-1].symbol == ")")
                		flag = false;
                }
              
			}
		}

	}
	else flag = false;
	valid = flag;
}

void Expression::infixToPostfix(){
	//uses the expression std::string "(al*2)+23"
	std::stack<El> operators;
	//stack operators: keep track of the operators in the infix expression to convert to postfix expression.
	for (int i = 0; i < arr.size() ; i++){

		//parenthesis. If opening parenthesis, push into stack operators. Else pop from stack operators and store in postfix until reach the opening parenthesis.
		if (arr[i].type == 4){
			if (arr[i].symbol == "("){
				operators.push(arr[i]);
			}
			else {
				while (operators.top().symbol != "("){
					postfix.push_back(operators.top());
					operators.pop();
				}
				operators.pop();
			}
		}

		//binary operators. Pop from the stack operators and store in postfix if operators.top() is the operator that has higher or equivalent precedence. Push the current operator into the stack operators.
		else if (arr[i].type == 2){
			if (arr[i].symbol == "+" || arr[i].symbol == "-"){
				while (!operators.empty() && (operators.top().type == 2 || operators.top().type == 3) ){
					postfix.push_back(operators.top());
					operators.pop();
				}
				operators.push(arr[i]);
			}
			else {
				while (!operators.empty() && (operators.top().type == 3 || operators.top().symbol == "*" || operators.top().symbol == "/" || operators.top().symbol == "%" || operators.top().symbol == "mod") ){
					postfix.push_back(operators.top());
					operators.pop();
				}
				operators.push(arr[i]);
			}
		}

		//unary operators. Pop from the stack operators and store in postfix if operators.top() is the operator that has higher or equivalent precedence. Push the current operator into the stack operators.
		else if (arr[i].type == 3){
			if (arr[i].symbol == "**"){
				while (!operators.empty() && (operators.top().symbol == "**") ){
					postfix.push_back(operators.top());
					operators.pop();
				}
				operators.push(arr[i]);
			}
			else if (arr[i].symbol == "--" || arr[i].symbol == "++"){
				while (!operators.empty() && (operators.top().symbol == "**" || operators.top().symbol == "--" || operators.top().symbol == "++") ){
					postfix.push_back(operators.top());
					operators.pop();
				}
				operators.push(arr[i]);
			}
			else if (arr[i].symbol == "-"){
				while (!operators.empty() && (operators.top().symbol == "**" || operators.top().symbol == "-")){
					postfix.push_back(operators.top());
					operators.pop();
				}
				operators.push(arr[i]);
			}
		}

		//variables or digits: push into postfix.
		else if (arr[i].type == 1 || arr[i].type == 0){
			postfix.push_back(arr[i]);
		}
	}

	while (!operators.empty()){
		postfix.push_back(operators.top());
		operators.pop();
	}
}

double Expression::evaluate(std::map<std::string, double>& expressionMap){
	//uses the array of Els and properly apply calculations using stack, and boolean postpone to check if any variable in the expression is not solved yet.

	std::stack<El> S;
	bool postpone = false;
	double a,b;

	//{ NUM = 0, VAR = 1, BIN = 2, UNA = 3};

	for (int i = 0; i < postfix.size(); ++i)
	{
		El tmp;
		switch(postfix[i].type)
		{	
		    case 0: 
		    	S.push(postfix[i]);
		    	break;

		    case 1:
		    	if(expressionMap.find(postfix[i].symbol) != expressionMap.end() ){
		    		tmp.type = 1;
		    		tmp.value = expressionMap.at(postfix[i].symbol);
		    		S.push(tmp);
		    	}else{
		    		postpone = true;
		    	}
		    	break;

		    case 2:
		    	b = S.top().value;
		    	S.pop();
		    	a = S.top().value;
		    	S.pop();
		    	tmp.type = 2;
		    	if(postfix[i].symbol == "+"){
		    		tmp.value = a + b;
		    		S.push(tmp);
		    	}else if(postfix[i].symbol == "-"){
		    		tmp.value = a - b;
		    		S.push(tmp);
		    	}else if(postfix[i].symbol == "*"){
		    		tmp.value = a*b;
		    		S.push(tmp);
		    	}else if(postfix[i].symbol == "/"){
		    		tmp.value = a/b;
		    		S.push(tmp);
		    	}else if(postfix[i].symbol == "mod"){
		    		tmp.value = (int(a))%(int(b));
		    		S.push(tmp);
		    	}else if(postfix[i].symbol == "%"){
		    		int t = a/b;
		    		tmp.value = t;
		    		S.push(tmp);
		    	} 

		    	break;
		    
		    case 3:
		    	a = S.top().value;
		    	S.pop();
		    	tmp.type = 3;
		    	if(postfix[i].symbol == "**"){
		    		tmp.value = a*a;
		    		S.push(tmp);
		    	}else if(postfix[i].symbol == "--"){
		    		tmp.value = a-1;
		    		S.push(tmp);
		    	}else if(postfix[i].symbol == "++"){
		    		tmp.value = a+1;
		    		S.push(tmp);
		    	}else if(postfix[i].symbol == "-"){
		    		tmp.value = (-1)*a;
		    		S.push(tmp);
		    	}

		    	break;
		}

		if(postpone) break;
	}

	if(!postpone){
      expressionMap.insert(std::pair<std::string, double>(name, S.top().value));
      solved = true;
      return S.top().value;
    }

	return INF;
}


bool Expression::tokenize(){
	std::vector<std::string> split;
	//In this function, we return true/false (checking special characters, invalid numbers) to imply if the expression is valid or not.

	for (int i = 0; i < expression.length(); i++){

		//case 1: binary operators (except +, - , *) and parenthesis.
		if (expression.at(i) == ')' || expression.at(i) == '(' || expression.at(i) == '/' || expression.at(i) == '%'){
			std::string str = "";
			str.push_back(expression.at(i));
			split.push_back(str);
		}
		else if( i<expression.length()-2 && expression.at(i) == 'm' && expression.at(i+1) == 'o' && expression.at(i+2) == 'd'){
			
			split.push_back("mod");
			i+=2;
	
		}

		//case 2: variable name.
		else if ( (expression.at(i) >= 65 && expression.at(i) <= 90) || (expression.at(i) >= 97 && expression.at(i) <= 122) || expression.at(i) == '_'){

			std::string tmp = "";
			tmp += expression.at(i);
  			if (i < expression.length()-1){
				i = i+1;
				while ((expression.at(i) >= 65 && expression.at(i) <= 90) || (expression.at(i) >= 97 && expression.at(i) <= 122) || expression.at(i) == '_' || (expression.at(i) >= 48 && expression.at(i) <= 57) ){
					tmp += expression.at(i);
                  	if (i < expression.length()-1){
                      i = i+1;
                    }
                  	else break;
                }
				i = i-1;
			}
			
			split.push_back(tmp);
		}

		//case 3: empty space
		else if (expression.at(i) == ' '){
			continue;
		}

		//case 4: either binary operators (+, -, *) or unary operators (++, --, **), or the combination of the operators such as +++ or --- or ***.
		else if ((i < expression.length()-1) && (expression.at(i) == '+' || expression.at(i) == '-' || expression.at(i) == '*')){
			std::string tmp = "";
			tmp += expression.at(i);
			int countOperator = 1;
			while(i < expression.length()-1 && expression.at(i+1) == expression.at(i)){
				i = i+1;
				countOperator++;
			}
			if (countOperator % 2 == 0){
				tmp += tmp;
				for (int k = 0; k < countOperator/2; k++){
					split.push_back(tmp);
				}
			}
			else {
				split.push_back(tmp);
				tmp += tmp;
				for (int k = 0; k < (countOperator-1)/2; k++){
					split.push_back(tmp);
				}
			}
		}

		//case 5: digits
		else if (expression.at(i) >= 48 && expression.at(i) <= 57){

			std::string tmp = "";
			tmp += expression.at(i);
			int cnt = 0;
			while ((i < expression.length()-1) && ((  (expression.at(i+1) >= 48 && expression.at(i+1) <= 57) ) || expression.at(i+1) == '.')){


				tmp += expression.at(i+1);
				if (expression.at(i+1) == '.'){
					cnt++;
				}
				i++;
			}
			if (cnt > 1){
				return false;
			}

			split.push_back(tmp);

		}
		else return false;


	}
	//After we properly split the expression, we store each element with proper type into vector arr.
	for (int i = 0; i < split.size(); i++){
		El tmp;
		tmp.symbol = split[i];
		if (split[i] == "(" || split[i] == ")"){
			tmp.type = 4;
		}
		else if (split[i] == "/" || split[i] == "*"||split[i] == "%" ||split[i] == "mod" || split[i] == "+"){
			tmp.type = 2;
		}
		else if (split[i] == "-"){
			if (i == 0 || split[i-1] == "("){
				tmp.type = 3;
			}
			else {
				tmp.type = 2;
			}
		}
		else if (split[i] == "**" || split[i] == "--" || split[i] == "++"){
			tmp.type = 3;
		}
		else if ( (split[i].at(0) >= 65 && split[i].at(0) <= 90) || (split[i].at(0) >= 97 && split[i].at(0) <= 122) ){
			tmp.type = 1;
		}
		else {
			tmp.type = 0;
			tmp.value = stod(split[i]);
		}
		arr.push_back(tmp);
	}
	return true;
}