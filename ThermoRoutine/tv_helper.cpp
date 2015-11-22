#include "tv_helper.h"
#include <common\on_exit.h>
#include <common\fs_helpers.h>

#include <io.h>
#include <memory>
#include <fstream>

namespace tv_helper
{
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

	bool TVpalette::Load(const std::wstring &fname)
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
		for (;;)
		{
			flag = sscanf_s(pbuf, "%d%d", &_numS, &_numI);
			offs += (strlen(pbuf) + 1);
			if (offs >= file_size) break;
			pbuf = data_begin + offs;
			if (flag == 2) 
				break;
		}

		if (flag != 2 || _numS<0 || _numI<0 || _numS>7 || _numI>256) {
			return false;
		}

		COLORREF color[1000];
		int r, g, b;
		// чтение цветов   
		while (counter < _numS + _numI) {
			flag = sscanf_s(pbuf, "%d%d%d", &b, &g, &r);
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