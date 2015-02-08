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

	enum class UNIT_TYPE { // формы представления
		UNIT_OS,  // объектный сигнал
		UNIT_T,   // температура
		UNIT_NUM
	};

	struct TVextremum {  // описание экстремума
		float OS;      // значение, OS
		float T;       // значение, OS
		DWORD N;       // номер кадра
		int row, col;   // положение
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

	struct TVdiap // диапазон
	{
		float from, to;
	};

	struct TVcalibr {  // калибровка
		float os[32767], t[32767]; // сигнал и температура
		int num;                   // число элементов в массивах
		TVcalibr()
		{
			os[0] = 31612; t[0] = 26.0f;
			os[1] = 31680; t[1] = 27.0f;
			num = 2;
		}
		float GetT(float code);  // вычислить температуру по сигналу
		float GetOS(float t);    // вычислить сигнал по температуре
		BOOL Remove(int index);  // удалить точку
		int Add(float OS, float T); // добавить точку
		BOOL Save(const char *fname);  // сохранить калибровку
		BOOL Load(const char *fname);  // загрузить калибровку
	};


	struct TVpalette {  // палитра

		TVpalette() { LoadDefault(); }
		void LoadDefault();  // черно-белая палитра 256 цветов
		bool Load(const char *fname);

		COLORREF special[7];     // специальный цвета
		int numS;                // число специальных цветов
		COLORREF image[256];     // цвета для вывода картинок
		int numI;                // число цветов для вывода картинок
		std::string m_fname;     // путь к файлу палитры 
	};
}