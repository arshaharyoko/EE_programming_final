#ifndef INFIX_TOOLS_H
#define INFIX_TOOLS_H
#include <string>
#include <array>
#include <stack>

class InfixTools {
    private:
        int precedence(char c);

    public:
        std::string infix_to_postfix(std::string infix_expr);
        double evalute_postfix_func(std::string postfix_expr);
};

#endif