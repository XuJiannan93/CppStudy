#ifndef _CU_SINGLETON_HPP_
#define _CU_SINGLETON_HPP_

template <typename T>
class Singleton
{
public:
	static T& GetInstance()
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

#endif // !_CU_SINGLETON_HPP_
