#include <string>
#include <iostream>

class Context {
public:
	std::string name;
	int day;
};

// �ȶ��� ����  �仯�� ��չ ����̬��
//  �ӵ�������ڵ���������ܴ����Ҵ����Ҳ��ܴ�������һ���˴���
//  �����ϵ��γ���

class IHandler {
public:
	virtual ~IHandler() : next(nullptr) {}

	void SetNextHandler(IHandler* next) { // �����ϵ
		next = next;
	}
	bool Handle(const Context& ctx) {
		if (CanHandle(ctx)) {
			return HandleRequest(ctx);
		}
		else if (GetNextHandler()) {
			return GetNextHandler()->Handle(ctx);
		}
		else {
			// err
		}
		return false;
	}
	// ͨ������������ ����ڵ�ĸ���  ����ڵ�˳��
	static bool handler_leavereq(Context& ctx) {
		IHandler* h1 = new HandleByMainProgram();
		IHandler* h2 = new HandleByProjMgr();
		IHandler* h3 = new HandleByBoss();
		h1->SetNextHandler(h2);
		h2->SetNextHandler(h3);
		return h1->Handle(ctx);
	}
protected:
	virtual bool HandleRequest(const Context& ctx) { return true; }
	virtual bool CanHandle(const Context& ctx) { return true; }
	IHandler* GetNextHandler() {
		return next;
	}
private:
	IHandler* next; // ��ϻ���ָ��
};

// �ܲ��ܴ����Լ���ô����
class HandleByMainProgram : public IHandler {
protected:
	virtual bool HandleRequest(const Context& ctx) {
		//
		return true;
	}
	virtual bool CanHandle(const Context& ctx) {
		//
		if (ctx.day <= 10)
			return true;
		return false;
	}
};

class HandleByProjMgr : public IHandler {
protected:
	virtual bool HandleRequest(const Context& ctx) {
		//
		return true;
	}
	virtual bool CanHandle(const Context& ctx) {
		//
		if (ctx.day <= 20)
			return true;
		return false;
	}
};
class HandleByBoss : public IHandler {
protected:
	virtual bool HandleRequest(const Context& ctx) {
		//
		return true;
	}
	virtual bool CanHandle(const Context& ctx) {
		//
		if (ctx.day < 30)
			return true;
		return false;
	}
};

class HandleByBeauty : public IHandler {
protected:
	virtual bool HandleRequest(const Context& ctx) {
		//
		return true;
	}
	virtual bool CanHandle(const Context& ctx) {
		//
		if (ctx.day <= 3)
			return true;
		return false;
	}
};

int main() {
	// IHandler * h1 = new HandleByMainProgram();
	// IHandler * h2 = new HandleByProjMgr();
	// IHandler * h3 = new HandleByBoss();
	// h1->SetNextHandler(h2);
	// h2->SetNextHandler(h3);
// ���󹤳�
// nginx http ���� 
	// ������һָ�� 
	Context ctx;
	if (IHandler::handler_leavereq(ctx)) {
		std::cout << "��ٳɹ�";
	}
	else {
		std::cout << "���ʧ��";
	}

	return 0;
}