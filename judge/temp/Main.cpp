#include <string>
using std::string;
  
class Account {
public:
    // user:储户姓名 balance:账户余额
    Account(const string& user, double balance = 0);
    // 用密码验证用户身份 key:提供的密码 return:是否认证成功
    bool Certify(const string& key);
    // 重设密码 oldkey:账户当前的密码 newkey:要设置的新密码 return:密码是否设置成功
    bool SetKey(const string& oldkey, const string& newkey);
    // 获取该账户的储户姓名，无需通过验证 return:储户姓名
    const string& UserName();
    // 获取该账户的余额，需通过验证 key:提供的密码 return:账户余额
    double Balance(const string& key);
    // 存款，需通过验证 cash:金额 key:提供的密码 return:取款是否成功
    bool Deposit(double cash, const string& key);
    // 取款，需通过验证 cash:金额 key:提供的密码 return:存款是否成功
    bool WithDeaw(double cash, const string& key);
    // 转账，需通过验证 acct:目标账户 cash:金额 key:提供的密码 return:转账是否成功
    bool Transfer(Account& acct, double cash, const string& key);
    // 设定利率 rt:万元收益，约在1.1-1.8之间
    static void SetRate(double rt);
    // 结算日息将利息计入余额
    void InterestSettle();
private:
    Account(const Account&) {}
    static bool JudgeKey(const string& key);
    string userName;
    string keyword;
    double money;
    static double rate;
};
double Account::rate = 1.6337;
  
class WrongKey{};
bool Account::JudgeKey(const string& key)
{
    if (key.length() != 6) return false;
    for (int i = 0; i < 6; ++i)
        if (key[i] > '9' || key[i] < '0')
            return false;
    return true;
}
  
Account::Account(const string& user, double balance)
: userName(user), money(balance), keyword("888888") {}
  
bool Account::Certify(const string& key)
{
    return key == keyword;
}
  
bool Account::SetKey(const string& oldkey, const string& newkey)
{
    if (!Certify(oldkey)) return false;
    keyword = newkey;
    return true;
}
  
const string& Account::UserName()
{
    return userName;
}
  
double Account::Balance(const string& key)
{
    return money;
}
  
bool Account::Deposit(double cash, const string& key)
{
    if (!Certify(key)) return false;
    money += cash;
    return true;
}
  
bool Account::WithDeaw(double cash, const string& key)
{
    if (!Certify(key)) return false;
    if (cash > money) return false;
    money -= cash;
    return true;
}
  
bool Account::Transfer(Account& acct, double cash, const string& key)
{
    if (!Certify(key)) return false;
    if (cash > money) return false;
    acct.money += cash;
    money -= cash;
    return true;
}
  
void Account::SetRate(double rt)
{
    rate = rt;
}
  
void Account::InterestSettle()
{
    money += money / 10000 * rate;
}
//#include "solver.h"

#include <cstdio>

class WrongAnswer{};

int main()
{
	try {
		Account first("Sine", 5e5);
		bool getCash = first.WithDeaw(1e5, "123456");
		if (getCash) throw WrongAnswer();
		bool haskey = first.Certify("888888");
		if (!haskey) throw WrongAnswer();
		bool changekey = first.SetKey("888888", "920728");
		if (!changekey) throw WrongAnswer();
		changekey = first.Certify("920728");
		if (!changekey) throw WrongAnswer();
		bool oldkey = first.Certify("888888");
		if (oldkey) throw WrongAnswer();
		if (first.UserName() != "Sine") throw WrongAnswer();
		Account another("other");
		first.Transfer(another, 2e5, "920728");
		printf("%.2lf\n", another.Balance("888888"));
		getCash = first.WithDeaw(4e5, "920728");
		if (getCash) throw WrongAnswer();
		Account::SetRate(1.6337);
		first.InterestSettle();
		another.InterestSettle();
		printf("%.2lf %.2lf\n", first.Balance("920728"), another.Balance("888888"));
		first.Deposit(438556.23, "920728");
		printf("%.2lf\n", first.Balance("920728"));
		Account::SetRate(1.3421);
		first.InterestSettle();
		another.InterestSettle();
		printf("%.2lf %.2lf\n", first.Balance("920728"), another.Balance("888888"));
		puts("AC");
	}
	catch (WrongAnswer) {
		puts("WA");
	}
	return 0;
}