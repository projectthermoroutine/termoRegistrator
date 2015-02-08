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

	struct TVextremum {  // �������� ����������
		float OS;      // ��������, OS
		float T;       // ��������, OS
		DWORD N;       // ����� �����
		int row, col;   // ���������
		inline void Prepare(DWORD n, float os, float t) { N = n; OS = os; T = t; row = col = 0; }
		inline void Set(float os, float t, int r, int c) {
			OS = os; T = t; row = r; col = c;
		}
	};

	struct TVstatistics {
		float min, max, mid;
		inline void Prepare() {
			min = 64000.0f;
			max = -64000.0f;
		}
		inline void Set(float val) {
			if (min > val) min = val;
			if (max < val) max = val;
		}
	};

	struct TVdiap // ��������
	{
		float from, to;
	};

	struct TVcalibr {  // ����������
		float os[32767], t[32767]; // ������ � �����������
		int num;                   // ����� ��������� � ��������
		TVcalibr()
		{
			os[0] = 31612; t[0] = 26.0f;
			os[1] = 31680; t[1] = 27.0f;
			num = 2;
		}
		float GetT(float code);  // ��������� ����������� �� �������
		float GetOS(float t);    // ��������� ������ �� �����������
		BOOL Remove(int index);  // ������� �����
		int Add(float OS, float T); // �������� �����
		BOOL Save(const char *fname);  // ��������� ����������
		BOOL Load(const char *fname);  // ��������� ����������
	};


	struct TVpalette {  // �������

		TVpalette() { LoadDefault(); }
		void LoadDefault();  // �����-����� ������� 256 ������
		bool Load(const char *fname);

		COLORREF special[7];     // ����������� �����
		int numS;                // ����� ����������� ������
		COLORREF image[256];     // ����� ��� ������ ��������
		int numI;                // ����� ������ ��� ������ ��������
		std::string m_fname;     // ���� � ����� ������� 
	};
}