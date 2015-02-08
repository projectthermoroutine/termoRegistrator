/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 29.09.2014
// Краткое описание: сериализация в строку и обратно enum-ов из scanning_operations
/////////////////////////////////////

#pragma once
#include <sstream>
#include <scanning_operations/scanning_operations.h>

std::wistream & operator >> (std::wistream & stream, scanning_operations::clean_mode & val);
std::wostream & operator << (std::wostream & stream, const scanning_operations::clean_mode & val);
