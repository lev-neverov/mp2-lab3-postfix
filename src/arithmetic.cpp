// ���������� ������� � ������� ��� ���������� �������������� ���������

#include "../include/stack.h"
#include "../include/arithmetic.h"

double stringToDouble(string s)
{
	int temp1 = 1, dots = 0, qps = 0, z;
	double res = 0.0, dec = 10.0, num = 0.0;

	for (int i = 0; i < s.length(); ++i) {
		if ((int(s.at(i)) < 48 || int(s.at(i)) > 57) && (s.at(i) != '-') && (s.at(i) != '+') && (s.at(i) != 'e') && (s.at(i) != '.')) {
			throw invalid_argument("invalid expression");
		}
		if ((temp1) && (s.at(i) != '.') && (s.at(i) != 'e')) {
			res *= dec;
			res += (int(s.at(i)) - 48);
		}
		if (s.at(i) == '.') {
			++dots;
			temp1 = 0;
			dec = 0.1;
			if (dots > 1) {
				throw invalid_argument("invalid expression");
			}
		}
		if (s.at(i) == 'e') {
			if (i + 1 == s.length()) {
				throw invalid_argument("invalid expression");
			}
			++i;
			dec = 10.0;
			while (i < s.length()) {
				if ((int(s.at(i)) < 48 && int(s.at(i)) > 57) && (s.at(i) != '-') && (s.at(i) != '+') && (s.at(i) != 'e')) {
					throw invalid_argument("invalid expression");
				}
				if ((s.at(i) == 'e') || (s.at(i) == '.')) {
					throw invalid_argument("invalid expression");
				}
				else if (s.at(i) == '-') {
					++qps;
					z = -1;
					if (qps > 1) {
						throw invalid_argument("invalid expression");
					}
				}
				else if (s.at(i) == '+') {
					++qps;
					z = 1;
					if (qps > 1) {
						throw invalid_argument("invalid expression");
					}
				}
				else {
					num *= dec;
					num += int(s.at(i)) - 48;
				}
				++i;
			}
			res *= pow(10, z * num);
			temp1 = 1;

		}
		if (temp1 == 0 && s.at(i) != '.') {
			res += (int(s.at(i)) - 48) * dec;
			dec /= 10.0;
		}
	}
	return res;
}



TPostfix::TPostfix(string str) {
	m_infix = str;
	m_size = NULL;
}

void TPostfix::checkInfix() {
	size_t tsize = m_infix.length();
	for (size_t i = 0; i < tsize; ++i)
	{
		if (m_infix.at(i) >= '0' && m_infix.at(i) <= '9' || m_infix.at(i) == 'e' || m_infix.at(i) == '.' || m_infix.at(i) == '-'){
			if (m_infix.at(i) == '-' && tsize == 1){
				throw "Invalid expression";
			}
			if (m_infix.at(i) == 'e') {
				if (i == 0 || m_infix.at(i - 1) < '0' && m_infix.at(i - 1) != '.' || m_infix.at(i - 1) > '9' || (i + 1) == tsize || m_infix.at(i + 1) < '0' && m_infix.at(i + 1) != '-' || m_infix.at(i + 1) > '9') {
					throw "Wrong exponational notation";
				}
			}
			if (m_infix.at(i) == '.') {
				if (i == 0 || m_infix.at(i - 1) < '0' || m_infix.at(i - 1) > '9' || (i + 1) == tsize || m_infix.at(i + 1) < '0' || m_infix.at(i + 1) > '9' && m_infix.at(i + 1) != 'e') {
					throw "Invalid expression";
				}
			}
		}
		else {
			throw "Wrong symbol";
		}
	}
}

int Operation::order()
{
	if (value == "(")
	{
		return -1;
	}
	else if (value == "+" || value == "-")
	{
		return 0;
	}
	else if (value == "*" || value == "/")
	{
		return 1;
	}
	else if (value == "~")
	{
		return 2;
	}
	else
	{
		throw runtime_error("Invalid value");
	}
}

void TPostfix::add_variable(char name, double value)
{
	m_variables[name] = value;
}

void TPostfix::toPostfix() {
	string value;
	char variable = '\0';
	TStack<Operation*> stack;
	for (const auto& c : m_infix)
	{
		if (c >= '0' && c <= '9')
		{
			value += c;
		}
		else if (c >= 'a' && c <= 'z')
		{
			variable = c;
		}
		else if (c == '-' && value.empty() && variable == '\0' && (stack.IsEmpty() || stack.showTop()->show() == "("))
		{
			stack.push(new Operation('~'));
		}
		else
		{
			if (!value.empty())
			{
				m_postfix.push_back(new Value(value));
				value = "";
			}

			if (variable != '\0')
			{
				m_postfix.push_back(new Variable(variable));
				variable = '\0';
			}

			if (c == '-' || c == '+' || c == '*' || c == '/')
			{
				auto operation = new Operation(c);
				while (!stack.IsEmpty() && operation->order() <= stack.showTop()->order())
				{
					m_postfix.push_back(stack.pop());
				}
				stack.push(operation);
			}

			if (c == '(')
			{
				stack.push(new Operation('('));
			}

			if (c == ')')
			{
				if (stack.showTop()->show() == "(")
				{
					throw runtime_error("Empty brackets");
				}

				while(stack.showTop()->show() != "(")
				{
					m_postfix.push_back(stack.pop());
				}
				stack.pop();
			}
			
		}
	}

	if (!value.empty())
	{
		m_postfix.push_back(new Value(value));
	}

	if (variable != '\0')
	{
		m_postfix.push_back(new Variable(variable));
		variable = '\0';
	}

	while (!stack.IsEmpty())
	{
		m_postfix.push_back(stack.pop());
	}
}

string TPostfix::showPostfix() {
	string postfixStr = "";
	if (m_infix != "NULL" && m_postfix.empty()) {
		setInfix(m_infix);
		toPostfix();
	}
	if (!m_postfix.empty()) {
		for (auto& lexem : m_postfix) {
			postfixStr += lexem->show() + " ";
		}
		return postfixStr;
	}
	else {
		throw invalid_argument("invalid expression");
	}
}

void TPostfix::setInfix(string infx) {
	m_infix = infx;
	m_postfix.clear();
	m_size = 0;
}

size_t TPostfix::getSize()
{
	return m_postfix.size();
}

double TPostfix::CALCULATE()
{
	if (m_infix != "NULL") {
		setInfix(m_infix);
		toPostfix();
	}
	if (m_postfix.empty()) {
		throw invalid_argument("invalid expression");
	}
	double varTemp;
	double res, left, right;
	string showStr;
	TStack<double> value;
	for (auto& lexem : m_postfix) {
		showStr = lexem->show();
		if (showStr == "+") {
			right = value.pop();
			left = value.pop();
			value.push(left + right);
		}
		else if (showStr == "-") {
			right = value.pop();
			left = value.pop();
			value.push(left - right);
		}
		else if (showStr == "/") {
			right = value.pop();
			left = value.pop();
			if (fabs(right) < 10.e-16)  {
				throw exception("I can't divide by zero ;(");
			}
			value.push(left / right);
		}
		else if (showStr == "*") {
			right = value.pop();
			left = value.pop();
			value.push(left * right);
		}
		else if (showStr == "~") {
			value.push(-value.pop());
		}
		else if (showStr.at(0) >= 'a' && showStr.at(0) <= 'z') {
			value.push(m_variables[showStr.at(0)]);
		}
		else {
			value.push(stringToDouble(showStr));
		}
	}
	res = value.pop();
	return res;
}
