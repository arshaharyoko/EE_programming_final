#include "./Infix_Tools.hh"

double InfixTools::apply(double a, double b, char op) {
    switch(op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/': 
            if(b==0) throw std::domain_error("Unavailable division by zero.");
            return a / b;
        case '^': 
            return std::pow(a, b);
        default:
            throw std::invalid_argument(std::string("Unknown operator.")+op);
    }
}

int InfixTools::precedence(char c) {
    if(c=='^') {
        return 3;
    } else if(c=='/' || c=='*') {
        return 2;
    } else if(c=='+' || c=='-') {
        return 1;
    } else {
        return -1;
    }   
}

std::string InfixTools::infix_to_postfix(std::string infix_expr) { // TODO: Filter out spaces
    std::stack<char> infix_stack;
    std::string postfix;
    
    for(int i=0;i<infix_expr.length();i++) {
        char c = infix_expr[i];

        if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9')) {
            postfix += c;
        } else if(c=='(') {
            infix_stack.push(c);
        } else if(c==')') {
            while(!infix_stack.empty() && infix_stack.top()!='(') {
                postfix += infix_stack.top();
                infix_stack.pop();
            }
            if(infix_stack.empty()) {
                throw std::runtime_error("Mismatched parentheses: missing '('.");
                return "";
            }
            infix_stack.pop();
        } else {
            while(!infix_stack.empty() && precedence(c)<=precedence(infix_stack.top())) {
                postfix += infix_stack.top();
                infix_stack.pop();
            }
            infix_stack.push(c);
        }
    }
    
    while(!infix_stack.empty()) {
        char t = infix_stack.top();
        if(t=='(' || t==')') {
            throw std::runtime_error("Mismatched parentheses, ) or ( left.");
            return "";
        }
        postfix += infix_stack.top();
        infix_stack.pop();
    }
    
    return postfix;
}


//
// Contoh variable_dict dgn. fungsi "(1-k/A)^2":
// std::map<char, double> variable_dict = {{'k', 2.0}, {'A', 0.5}};
//
double InfixTools::evalute_postfix_func(std::string postfix_expr, std::map<char, double> variable_dict) {
    std::stack<double> postfix_stack;
    double result = 0.0;

    for(int i=0;i<postfix_expr.size();i++) {
        char c = postfix_expr[i];
        double a = 0.0, b = 0.0;

        if(std::isalnum(static_cast<unsigned char>(c))) { // If alphanumeric
            if(std::isdigit(static_cast<unsigned char>(c))) { // Digit
                postfix_stack.push(c-'0');
            } else { // Lookup variable
                auto it = variable_dict.find(c);
                if(it==variable_dict.end()) {
                    throw std::invalid_argument(std::string("Variable in function undefined in map: ") + c);
                    return 0.0;
                }
                postfix_stack.push(it->second);
            }
        } else {
            if(postfix_stack.size()<2) {
                throw std::runtime_error("Insufficient operands.");
                return 0.0;
            }

            b = postfix_stack.top();
            postfix_stack.pop();
            a = postfix_stack.top();
            postfix_stack.pop();
            postfix_stack.push(apply(a, b, c));
        }
    }

    return postfix_stack.top();
}