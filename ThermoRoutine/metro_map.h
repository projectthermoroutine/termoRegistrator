#pragma once
#include <Windows.h>
#include <vector>
#include <memory>

namespace metro_map
{
	// Типы объектов
	enum class MAPMS_OBJECT_TYPE {
		MAPMS_PICK,       // 1811, 1812 пикеты   !!!! =First in this enum
		MAPMS_STATION,    // 1620       станции
		MAPMS_RZ,         // 1511       рельсовые цепи
		MAPMS_PKO,        // 1814       Пикетоотметчик
		MAPMS_GSTAT,      // 1621       Граница станции
		MAPMS_STOP1,      // 2510       Остановка 1-го вагона
		MAPMS_STRELKA,    // 1820       Стрелка
		MAPMS_SPATH,      // 1622       Станционный путь
		MAPMS_PPATH,      // 1623       Перегонный путь
		MAPMS_SEMAFOR,     //1512      Светофор
		MAPMS_PICKBORDER, // 1813       Граница пикетажа
		MAPMS_STARTPOINT, //            НАЧАЛО ПРОЕЗДА
		MAPMS_MEASPOINT,   //Начало измерений
		MAPMS_MEASFINISH,   //Конец измерений
		MAPMS_LR,          // 1410 ЛР
		MAPMS_KP,          // 1420 КП
		MAPMS_PJA,         // 1440 ПЯ
		MAPMS_DT,          // 1501 ДТ
		MAPMS_NUM
	};

	struct map_object;
	using map_object_ptr_t = std::shared_ptr<map_object>;
	using map_objects_list = std::vector<map_object_ptr_t>;


	struct map_object {
		MAPMS_OBJECT_TYPE type;        // Тип объекта
		int line, path;  // Линия и путь
		int pick, offs;   // Пикет и смещение (см)
		int coord;       // Смещение от начала маршрута, см
		int code;      // Код объекта по БД
		int len;         // Длина, см
		std::string name;  // Название объекта
		std::string short_name;
		int span;    // Положение (код перегона)
		int ObjData;   // Данные объекта
		int PtData;
		int dir;                     //12 или 21
		int sec;     // относительное время прохождения этого объекта; -1=не пройден
		time_t time;  // абсолютное время прохождения этого объекта
		BOOL is_forward;    //вперед головой или хвостом
		int cmp(const map_object &o)
		{
			if (coord < o.coord) return -1;
			if (coord > o.coord) return  1;
			return 0;
		}

		int cmpPBord(const map_object &o)
		{
			return cmp(o);
		}
	};

	class metro_map_basic;
	using metro_map_ptr_t = std::shared_ptr<metro_map_basic>;

	class metro_map_basic
	{
	public:
		metro_map_basic() {}
		virtual ~metro_map_basic() {}
	public:
		// Открыть файл карты name;
		// offset=смещение области описания карты от начала файла
		virtual int Open(char *name, DWORD offset = 0) = 0;
		virtual int Open(HANDLE fh, DWORD offset) = 0;
		virtual void Reset() = 0;
		virtual void Close() = 0;
		virtual int count_objects() = 0;

		virtual int LastObjNumForDistance(int coord) = 0; //номер последнего объекта пути для заданного расстояния coord (см)

		virtual map_object_ptr_t Get(int n = 0) = 0;

		virtual map_object_ptr_t FindObject(int code) = 0;  // Найти объект по его коду

		// Работа с маршрутом
		virtual int RpointLine(int i) = 0;
		virtual int RpointPath(int i) = 0;
		virtual int RpointPick(int i) = 0;
		virtual int RpointOffs(int i) = 0;
		virtual WORD RpointCh(int i) = 0;
	};

	class fake_metro_map : public metro_map_basic
	{
	public:
		fake_metro_map() {}
		virtual ~fake_metro_map() {}
	public:
		virtual int Open(char *name, DWORD offset = 0){ return 0; }
		virtual int Open(HANDLE fh, DWORD offset) { return 0; }
		virtual void Reset() {}
		virtual void Close() {}
		virtual int count_objects() { return 0; }

		virtual int LastObjNumForDistance(int coord) { return 0; }

		virtual map_object_ptr_t Get(int n = 0) { return map_object_ptr_t(); }

		virtual map_object_ptr_t FindObject(int code) { return map_object_ptr_t(); }

		virtual int RpointLine(int i) { return 0; }
		virtual int RpointPath(int i) { return 0; }
		virtual int RpointPick(int i) { return 0; }
		virtual int RpointOffs(int i) { return 0; }
		virtual WORD RpointCh(int i) { return 0; }
	};

}
