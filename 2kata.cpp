#include<iostream>
#include<stack>
#include<string>
#include<cmath>

bool is_digit(char c)
{
  if (c >= '0' && c <= '9') return true;
  return false;
}
int priority(char c)
{
  switch (c)
  {
  case '+': return 1; break;
  case '-': return 1; break;
  case '*': return 2; break;
  case '/': return 2; break;
  case '(': return 3; break;
  default: return 0;
  }
  return 0;
}
double get_num(std::string str, int& i)
{
  double sum = 0;
  while (str[i] == '.' || is_digit(str[i]))
  {
    if (str[i] != '.')
      sum = (sum * 10) + (double)(str[i++] - '0');
    else
    {
      i++;
      int cnt = -1;
      while (is_digit(str[i]))
      {
        sum += ((double)(str[i++] - '0') * pow(10, cnt));
        cnt--;
      }
      break;
    }
  }
  return sum;
}
void calculate(std::stack<double> &Num, std::stack<char> &Op)
{
  if (Op.empty() || Num.empty()) return;
  if (Op.top() == '(') return;

  double tmp = Num.top();
  Num.pop();

  if (!Num.empty())
    switch (Op.top())
    {
    case '+': Num.top() += tmp; break;
    case '-': Num.top() -= tmp; break;
    case '*': Num.top() *= tmp; break;
    case '/':
      if (tmp == 0) Num.top() = 0;
      else Num.top() /= tmp;
      break;
    }
  Op.pop();
}
bool check(std::stack<char>& Op)
{
  if (Op.empty()) return false;
  std::stack<char> tmp(Op);
  tmp.pop();
  if (tmp.empty()) return false;
  if (Op.top() == '(' && tmp.top() == '(') return false;
  if (priority(Op.top()) >= priority(tmp.top())) return true;
  return false;
}
void sum(std::stack<double>& Num)
{
  if (Num.empty()) return;
  double tmp = Num.top();
  Num.pop();
  Num.top() += tmp;
}

double calc2(std::string str, int& i) //рекурсивная функция вычисления выражения в скобках
{
  std::stack<double> Num;
  std::stack<char> Op;
  Op.emplace(str[i]);
  i++;
  for (; str[i]; i++)
  {
    if (str[i] == ' ') continue;
    if (
      (str[i] == '-' && is_digit(str[i + 1]) && i == 0) ||
      (i > 0 && str[i] == '-' && is_digit(str[i + 1]) && (str[i - 1] == ' ' || str[i - 1] == '('))
      )//отрицательное число
    {
      i++;
      Num.emplace(-get_num(str, i));
      if (!Op.empty() && priority(Op.top()) == 2) calculate(Num, Op);
      i--;
    }
    else
    {
      if (str[i] == '-' && str[i + 1] == '(')
      {
        i++;
        Num.emplace(-1 * calc2(str, i));
        continue;
      }
      else
        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') // +-*/
        {
          if (check(Op) || (!Op.empty() && (priority(Op.top()) >= priority(str[i])))) calculate(Num, Op);
          if (check(Op) || (!Op.empty() && (priority(Op.top()) >= priority(str[i])))) calculate(Num, Op);
          Op.emplace(str[i]); // +-*/ стек пустой
          continue;
        }

      if (str[i] == '(')
      {
        Op.emplace(str[i]);
        continue;
      }

      if (str[i] == ')')
      {
        while (Op.top() != '(')
          calculate(Num, Op);
        Op.pop();
        if (Op.empty())return Num.top();
        continue;
      }

      if (is_digit(str[i]))
      {
        Num.emplace(get_num(str, i));
        i--;
        continue;
      }
    }
  }
  while (!Op.empty()) calculate(Num, Op);
  while (Num.size() > 1) sum(Num);

  return Num.top();
}

double calc(std::string str) 
{
  std::stack<double> Num;
  std::stack<char> Op;

  for (int i = 0; str[i]; i++)
  {

    if (str[i] == ' ') continue;
    if (
      (str[i] == '-' && is_digit(str[i + 1]) && i == 0) ||
      (i > 0 && str[i] == '-' && is_digit(str[i + 1]) && (str[i - 1] == ' ' || str[i - 1] == '('))
       )//отрицательное число
    {
      i++;
      Num.emplace(-get_num(str, i));
      if (!Op.empty() && priority(Op.top()) == 2) calculate(Num, Op);
      i--;
    }
    else
    {
      if (str[i] == '-' && str[i + 1] == '(')
      {
        i++;
        Num.emplace(-1*calc2(str, i));
        continue;
      }
      else
      if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') // +-*/
      {
        if (check(Op) || (!Op.empty() && (priority(Op.top()) >= priority(str[i])))) calculate(Num, Op);
        if (check(Op) || (!Op.empty() && (priority(Op.top()) >= priority(str[i])))) calculate(Num, Op);
        Op.emplace(str[i]); // +-*/ стек пустой
        continue;
      }

      if (str[i] == '(')
      {
        Op.emplace(str[i]);
        continue;
      }

      if (str[i] == ')')
      {
        while (Op.top() != '(')
          calculate(Num, Op);
        Op.pop();
        continue;
      }

      if (is_digit(str[i]))
      {
        Num.emplace(get_num(str, i));
        i--;
        continue;
      }
    }
  }
  while (!Op.empty()) calculate(Num, Op);
  while (Num.size() > 1) sum(Num);

  return Num.top();
}