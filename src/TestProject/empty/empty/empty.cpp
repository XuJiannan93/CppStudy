#include <iostream>
//普通员工有销售奖金，累计奖金，部门经理除此之外还有团队奖金；后面可能会添加环比增长奖金，同时可能产生不同的奖金组合；
//销售奖金 = 当月销售额 * 4%
//累计奖金 = 总的回款额 * 0.2%
//部门奖金 = 团队销售额 * 1%
//环比奖金 = (当月销售额-上月销售额) * 1%
//销售后面的参数可能会调整
using namespace std;
class Context {
public:
	bool isMgr;
	// User user;
	// double groupsale;
};


class CalcBonus {
public:
	CalcBonus(CalcBonus* c = nullptr) : cc(c) {}
	virtual double Calc(Context& ctx) {
		cout << "calc base bonus" << endl;
		return 0.0; // 基本工资
	}
	virtual ~CalcBonus() {}

protected:
	CalcBonus* cc;
};

class CalcMonthBonus : public CalcBonus {
public:
	CalcMonthBonus(CalcBonus* c) : CalcBonus(c) {}
	virtual double Calc(Context& ctx) {
		double mbonus = 0; /*= 计算流程忽略*/;
		cout << "calc month bonus" << endl;
		return mbonus + cc->Calc(ctx);
	}
};

class CalcSumBonus : public CalcBonus {
public:
	CalcSumBonus(CalcBonus* c) : CalcBonus(c) {}
	virtual double Calc(Context& ctx) {
		double sbonus = 0; /*= 计算流程忽略*/;
		cout << "calc sum bonus" << endl;
		return sbonus + cc->Calc(ctx);
	}
};

class CalcGroupBonus : public CalcBonus {
public:
	CalcGroupBonus(CalcBonus* c) : CalcBonus(c) {}
	virtual double Calc(Context& ctx) {
		double gbnonus = 0;/*= 计算流程忽略*/;
		cout << "calc group bonus" << endl;
		return gbnonus + cc->Calc(ctx);
	}
};

class CalcCycleBonus : public CalcBonus {
public:
	CalcCycleBonus(CalcBonus* c) : CalcBonus(c) {}
	virtual double Calc(Context& ctx) {
		double gbnonus = 0; /*= 计算流程忽略*/;
		cout << "calc cycle bonus" << endl;
		return gbnonus + cc->Calc(ctx);
	}
};

int main() {
	//1. 普通员工
	Context ctx1;
	CalcBonus* base = new CalcBonus();
	CalcBonus* cb1 = new CalcMonthBonus(base);
	CalcBonus* cb2 = new CalcSumBonus(cb1);
	cb2->Calc(ctx1);

	cout << "**********" << endl;

	// 2. 部门经理
	Context ctx2;
	CalcBonus* cb3 = new CalcGroupBonus(cb2);
	CalcBonus* cb4 = new CalcCycleBonus(cb3);
	cb4->Calc(ctx2);

	getchar();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
