#include "tv_helper.h"
#include <common\on_exit.h>
#include <common\fs_helpers.h>

#include <io.h>
#include <memory>
#include <fstream>


namespace tv_helper
{
	// -------------------------------------------------------------
	// калибровка
	// -------------------------------------------------------------
	float TVcalibr::GetT(float code)
	{
		if (num < 2) return code / 1200;
		float toReturn;
		for (int i = 1; i < num; i++) { //  ЫЫЫЫ йа вставил эту херню
			if (code <= os[i]) break;
			if (i == num) i = num - 1;
			if (os[i] == os[i - 1]) return t[i];
			double x1 = code - os[i - 1];
			double x2 = os[i] - os[i - 1];
			//		return float(t[i-1]+(t[i]-t[i-1])*x1/x2);
			//	}
			toReturn = float(t[i - 1] + (t[i] - t[i - 1])*x1 / x2);
		}
		return toReturn;
	}
	float TVcalibr::GetOS(float T)
	{
		float toReturn = 0.0;
		if (num < 2) return T * 1200;
		for (int i = 1; i < num; i++) { // ЫЫЫ и эту херню тоже ((=
			if (T <= t[i]) break;
			if (i == num) i = num - 1;
			if (t[i] == t[i - 1]) return os[i];
			double x1 = T - t[i - 1];
			double x2 = t[i] - t[i - 1];
			//		return float(os[i-1]+(os[i]-os[i-1])*x1/x2);
			//	}
			toReturn = float(os[i - 1] + (os[i] - os[i - 1])*x1 / x2);
		}
		return toReturn;
	}

	BOOL TVcalibr::Remove(int index)
	{
		if (index < 0 || index >= num) return FALSE;
		for (int i = index; i < num; i++) { t[i] = t[i + 1]; os[i] = os[i + 1]; }
		num--;
		return TRUE;
	}
	int TVcalibr::Add(float OS, float T)
	{
		if (num >= 32767) return -1;
		int toReturn;
		for (int i = 0; i<num; i++) { // здесь тоже я постарался ((=
			if (os[i]>OS) break;
			memmove(&os[i + 1], &os[i], 4 * (num - i));
			memmove(&t[i + 1], &t[i], 4 * (num - i));
			os[i] = OS;
			t[i] = T;
			num++;
			//	return i;
			// }
			toReturn = i;
		}
		return toReturn;
	}

#define CALIBR_SIGN  "Калибровка тепловизора"
	BOOL TVcalibr::Save(const char *fname)
	{
		FILE *fp = fopen(fname, "w");
		if (!fp) return FALSE;
		fprintf(fp, "%s", CALIBR_SIGN);
		fprintf(fp, "%c", 0x0d);
		for (int i = 0; i < num; i++)  fprintf(fp, "%.0f %.1f%c", os[i], t[i], 0x0d);
		fclose(fp);
		return TRUE;
	}
	BOOL TVcalibr::Load(const char *fname)
	{
		if (!strlen(fname)) return FALSE;
		FILE *fp = fopen(fname, "r");
		if (!fp) return FALSE;
		char str[1000]; memset(str, 0, 1000);
		fread(str, strlen(CALIBR_SIGN), 1, fp);
		if (strcmp(str, CALIBR_SIGN)) { fclose(fp); return FALSE; }
		num = 0;
		while (2 == fscanf(fp, "%f%f", &os[num], &t[num]))
			num++;
		fclose(fp);
		return TRUE;
	}

	// -------------------------------------------------------------
	// палитра
	// -------------------------------------------------------------
	void TVpalette::LoadDefault()
	{
		numS = 0;
		numI = 256;
		for (int i = 0; i < numI; i++)
			image[i] = RGB(i, i, i);
	}

	static const auto max_pallete_file_size = 100000ULL;

	bool TVpalette::Load(const char *fname)
	{
		auto file = std::make_unique<std::ifstream>(fname);
		auto file_size = fs_helpers::get_stream_data_size(*file.get());

		if (file_size == 0 || file_size > max_pallete_file_size)
			return false;

		std::string file_data(
			(std::istreambuf_iterator<char>(*file.get())),
			std::istreambuf_iterator<char>()
			);

		for (auto curr_iter = file_data.begin(); curr_iter != file_data.end(); curr_iter++)
		{
			if (*curr_iter <= 0x0d)
				*curr_iter = 0;
			else
			{
				if (*curr_iter == '#')
				{
					for (auto iter = curr_iter; iter != file_data.end(); iter++, curr_iter++)
					{
						if (*curr_iter <= 0x0d)
						{
							curr_iter--;
							break;
						}
						*curr_iter = ' ';
					}
				}
				else
				if (!isdigit(*curr_iter))
					*curr_iter = ' ';
			}
		}

		auto data_begin = file_data.c_str();
		auto pbuf = data_begin;
		int _numS, _numI, offs = 0, flag = 0, counter = 0;
		// чтение числа цветов   
		while (1) {
			flag = sscanf(pbuf, "%d%d", &_numS, &_numI);
			offs += (strlen(pbuf) + 1);
			if (offs >= file_size) break;
			pbuf = data_begin + offs;
			if (flag == 2) break;
		}
		if (flag != 2 || _numS<0 || _numI<0 || _numS>7 || _numI>256) {
			return false;
		}

		COLORREF color[1000];
		int r, g, b;
		// чтение цветов   
		while (counter < _numS + _numI) {
			flag = sscanf(pbuf, "%d%d%d", &b, &g, &r);
			offs += (strlen(pbuf) + 1);
			if (offs >= file_size) break;
			pbuf = data_begin + offs;
			if (flag == 3) color[counter++] = RGB(r, g, b);
		}
		if (counter != (_numS + _numI)) { return false; }

		numS = _numS;
		numI = _numI;
		memcpy(special, color, sizeof(COLORREF)*numS);
		memcpy(image, &color[numS], sizeof(COLORREF)*numI);

		m_fname = fname;
		return true;
	}


}