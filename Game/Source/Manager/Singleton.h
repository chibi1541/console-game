#pragma once

template<typename T>
class Singleton
{
public:

	static T& Get()
	{
		static T instance;
		return instance;
	}

	// 복사 생성자, 이동 생성자 삭제
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;

	// 대입, 복사 연산 삭제
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	virtual void Initialize() = 0;
	virtual void Shutdown() = 0;

protected:
	Singleton() = default;
	virtual ~Singleton() = default;

};