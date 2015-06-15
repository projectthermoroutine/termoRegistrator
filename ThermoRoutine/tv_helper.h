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