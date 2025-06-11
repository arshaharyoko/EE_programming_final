#ifndef INFIX_TOOLS_H
#define INFIX_TOOLS_H
#include <string>
#include <array>
#include <stack>
#include <map>
#include <stdexcept>
#include <cmath>
#include <iostream>

class InfixTools {
    private:
        double apply(double a, double b, char op);
        int precedence(char c);

    public:
        std::string infix_to_postfix(std::string infix_expr);
        double evalute_postfix_func(std::string postfix_expr, std::map<char, double> variable_dict);
};

#endif