#pragma once
#include <mutex>

template <class Type>
class ISingleton
{
private:
	static Type* Instance;
public:
	static Type& GetInstance();
	static Type& Get();
public:
	ISingleton(const ISingleton&) = delete;
	ISingleton(ISingleton&&)	  = delete;
	ISingleton()				  = default;
	virtual ~ISingleton()		  = default;
};

template<class Type>
Type*			ISingleton<Type>::Instance = nullptr;

template<class Type>
Type& ISingleton<Type>::GetInstance()
{
	static std::once_flag flag;
	// 한번만 실행
	std::call_once(flag,
		[]() {
		if (Instance == nullptr)
			Instance = new Type();
	});

	return *Instance;
}

template<class Type>
Type& ISingleton<Type>::Get()
{
	static std::once_flag flag;
	// 한번만 실행
	std::call_once(flag,
		[]() {
		if(Instance == nullptr)
			Instance = new Type();
	});

	return *Instance;
}