/////////////////////////////////////
// (C) 2014 ��� "��� ������������"
// ������: SNES-AV eset
// �����: ������ �����
// ������: 29.09.2014
// ������� ��������: ������������ � ������ � ������� enum-�� �� scanning_operations
/////////////////////////////////////

#pragma once
#include <sstream>
#include <scanning_operations/scanning_operations.h>

std::wistream & operator >> (std::wistream & stream, scanning_operations::clean_mode & val);
std::wostream & operator << (std::wostream & stream, const scanning_operations::clean_mode & val);
