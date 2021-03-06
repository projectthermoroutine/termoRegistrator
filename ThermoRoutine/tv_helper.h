#pragma once

#include <Windows.h>
#include <string>

namespace tv_helper
{
#define TV_MAXPOINT  160226 // 120000
#define TV_MAXX      400
#define TV_MAXY      300

#define TV_WIDTH   320
#define TV_HEIGHT  240

	enum class UNIT_TYPE { // ����� �������������
		UNIT_OS,  // ��������� ������
		UNIT_T,   // �����������
		UNIT_NUM
	};

	struct TVpalette {  // �������

		TVpalette() { LoadDefault(); }
		void LoadDefault();  // �����-����� ������� 256 ������
		bool Load(const std::wstring &fname);

		COLORREF special[7];     // ����������� �����
		int numS;                // ����� ����������� ������
		COLORREF image[256];     // ����� ��� ������ ��������
		int numI;                // ����� ������ ��� ������ ��������
		std::wstring m_fname;     // ���� � ����� ������� 
	};
}