#include "./Infix_Tools.hh"

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
            while(infix_stack.top()!='(') {
                postfix += c;
                infix_stack.pop();
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
        postfix += infix_stack.top();
        infix_stack.pop();
    }
    
    return postfix;
}

double InfixTools::evalute_postfix_func(std::string postfix_expr) {
    // for(int i=0;i<postfix_expr.size();i++) {
        
    // }
    return 0;
}