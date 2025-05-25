#include <iostream>
#include <stack>
#include <sstream>
#include <string>
#include <cctype>
#include <map>
#include <stdexcept>
#include <vector>

using namespace std;

// Check if the character is an operator
bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Get precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Apply an operator to two operands
double apply_operator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw runtime_error("Division by zero");
            return a / b;
        default: throw runtime_error("Unknown operator");
    }
}

// Convert infix expression to postfix (RPN) using Shunting Yard algorithm
vector<string> infix_to_postfix(const string& expression) {
    vector<string> output;
    stack<char> ops;
    string num;

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (isspace(c)) continue;

        if (isdigit(c) || c == '.') {
            num += c;
        } else {
            if (!num.empty()) {
                output.push_back(num);
                num.clear();
            }

            if (c == '(') {
                ops.push(c);
            } else if (c == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    output.push_back(string(1, ops.top()));
                    ops.pop();
                }
                if (ops.empty()) throw runtime_error("Mismatched parentheses");
                ops.pop(); // remove '('
            } else if (is_operator(c)) {
                while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                    output.push_back(string(1, ops.top()));
                    ops.pop();
                }
                ops.push(c);
            } else {
                throw runtime_error("Invalid character in expression");
            }
        }
    }

    if (!num.empty()) output.push_back(num);

    while (!ops.empty()) {
        if (ops.top() == '(' || ops.top() == ')') throw runtime_error("Mismatched parentheses");
        output.push_back(string(1, ops.top()));
        ops.pop();
    }

    return output;
}

// Evaluate postfix expression
double evaluate_postfix(const vector<string>& tokens) {
    stack<double> values;
    for (const string& token : tokens) {
        if (isdigit(token[0]) || (token.length() > 1 && token[0] == '-')) {
            values.push(stod(token));
        } else if (token.length() == 1 && is_operator(token[0])) {
            if (values.size() < 2) throw runtime_error("Invalid expression");
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();
            values.push(apply_operator(a, b, token[0]));
        } else {
            throw runtime_error("Unknown token: " + token);
        }
    }
    if (values.size() != 1) throw runtime_error("Invalid postfix expression");
    return values.top();
}

// Main calculator function
double evaluate_expression(const string& expression) {
    vector<string> postfix = infix_to_postfix(expression);
    return evaluate_postfix(postfix);
}

int main() {
    cout << "Simple Calculator (no parser class)\n";
    cout << "Enter expressions to evaluate. Type 'exit' to quit.\n";

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "exit") break;
        if (input.empty()) continue;

        try {
            double result = evaluate_expression(input);
            cout << "Result: " << result << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
