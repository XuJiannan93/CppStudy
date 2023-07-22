#ifndef _CU_SINGLETON_H_
#define _CU_SINGLETON_H_

template <typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}

protected:
	virtual ~Singleton() {}
	Singleton() {}

private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};

#endif // !_CU_SINGLETON_H_

/*
class DesignPattern : public Singleton<DesignPattern>
{
	friend class Singleton<DesignPattern>;

private:
	DesignPattern() {}
	~DesignPattern() {}
};
*/
