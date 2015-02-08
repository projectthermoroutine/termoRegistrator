/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 07.10.2014
// Краткое описание: PIMPL, член класса в который инкапсулируется реализация
// RTFM: http://herbsutter.com/gotw/_101/
/////////////////////////////////////
#pragma once

#include <memory>

template<typename T>
class pimpl {
private:
	std::unique_ptr<T> m;
public:
	template<typename ...Args> pimpl(Args&& ...);
	~pimpl();
	T* operator->();
	const T* operator->() const;
	T& operator*();
	const T& operator*() const;
};