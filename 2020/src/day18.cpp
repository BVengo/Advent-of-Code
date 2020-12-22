#include <iostream>
#include <fstream>
#include <string>

/*

*/

int calculate(std::string expression) {
    std::cout << expression << std::endl;
    if(expression.size() == 0) {
        return 0;
    }
    int value = std::stoi(expression.substr(0, expression.find(' ')));
    expression.erase(0, expression.find(' ') + 1);

    if(expression[0] == '+') {
        value += calculate(expression.erase(0, 2));
    } 
    else if(expression[0] == '*') {
        value *= calculate(expression.erase(0, 2));
    } 
    else {
        std::cout << "Bit of a problem here...: '" << expression[0] << "'" << std::endl;
    }

    return value;
}

int main()
{
    std::ifstream file("../input/day18.txt");
    std::string calculation;

    long sum;

    while(std::getline(file, calculation)) {
        std::string expression;
        std::cout << "calculation:  " << calculation << ":" << std::endl;

        while(calculation.find('(') != std::string::npos) {
            int firstParenth = calculation.find('(');
            int secondParenth = calculation.find(')');
            int result;

            expression = calculation.substr(firstParenth, secondParenth-firstParenth+1);

            while(expression.find_last_of('(') != 0) {
                expression.erase(0, expression.find_last_of('('));
            }

            std::cout << "expression: " << expression << std::endl;

            result = calculate(expression.substr(1, expression.size()-2));

            std::cout << "result: " << result << std::endl;

            calculation.replace(calculation.find(expression), expression.length(), std::to_string(result));

            std::cout << "new calculation: " << calculation << std::endl;
        // }
        }

        sum += calculate(calculation);
        std::cout << "final sum: " << sum << std::endl;
    }

    std::cout << sum << std::endl;

    return 0;
}