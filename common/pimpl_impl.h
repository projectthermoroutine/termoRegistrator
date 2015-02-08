/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 07.10.2014
// Краткое описание: внутренняя реализация PIMPL
// RTFM: http://herbsutter.com/gotw/_101/
/////////////////////////////////////

#pragma once

#include <utility>

template<typename T>
template<typename ...Args>
pimpl<T>::pimpl(Args&& ...args)
	: m( new T( std::forward<Args>(args)... ) ) { }

template<typename T>
pimpl<T>::~pimpl() { }

template<typename T>
T* pimpl<T>::operator->() { return m.get(); }

template<typename T>
const T* pimpl<T>::operator->() const { return m.get(); }

template<typename T>
T& pimpl<T>::operator*() { return *m.get(); }

template<typename T>
const T& pimpl<T>::operator*() const { return *m.get(); }