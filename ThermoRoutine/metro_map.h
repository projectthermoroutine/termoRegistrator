#pragma once
#include <Windows.h>
#include <vector>
#include <memory>

namespace metro_map
{
	// ���� ��������
	enum class MAPMS_OBJECT_TYPE {
		MAPMS_PICK,       // 1811, 1812 ������   !!!! =First in this enum
		MAPMS_STATION,    // 1620       �������
		MAPMS_RZ,         // 1511       ��������� ����
		MAPMS_PKO,        // 1814       ��������������
		MAPMS_GSTAT,      // 1621       ������� �������
		MAPMS_STOP1,      // 2510       ��������� 1-�� ������
		MAPMS_STRELKA,    // 1820       �������
		MAPMS_SPATH,      // 1622       ����������� ����
		MAPMS_PPATH,      // 1623       ���������� ����
		MAPMS_SEMAFOR,     //1512      ��������
		MAPMS_PICKBORDER, // 1813       ������� ��������
		MAPMS_STARTPOINT, //            ������ �������
		MAPMS_MEASPOINT,   //������ ���������
		MAPMS_MEASFINISH,   //����� ���������
		MAPMS_LR,          // 1410 ��
		MAPMS_KP,          // 1420 ��
		MAPMS_PJA,         // 1440 ��
		MAPMS_DT,          // 1501 ��
		MAPMS_NUM
	};

	struct map_object;
	using map_object_ptr_t = std::shared_ptr<map_object>;
	using map_objects_list = std::vector<map_object_ptr_t>;


	struct map_object {
		MAPMS_OBJECT_TYPE type;        // ��� �������
		int line, path;  // ����� � ����
		int pick, offs;   // ����� � �������� (��)
		int coord;       // �������� �� ������ ��������, ��
		int code;      // ��� ������� �� ��
		int len;         // �����, ��
		std::string name;  // �������� �������
		std::string short_name;
		int span;    // ��������� (��� ��������)
		int ObjData;   // ������ �������
		int PtData;
		int dir;                     //12 ��� 21
		int sec;     // ������������� ����� ����������� ����� �������; -1=�� �������
		time_t time;  // ���������� ����� ����������� ����� �������
		BOOL is_forward;    //������ ������� ��� �������
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
		// ������� ���� ����� name;
		// offset=�������� ������� �������� ����� �� ������ �����
		virtual int Open(char *name, DWORD offset = 0) = 0;
		virtual int Open(HANDLE fh, DWORD offset) = 0;
		virtual void Reset() = 0;
		virtual void Close() = 0;
		virtual int count_objects() = 0;

		virtual int LastObjNumForDistance(int coord) = 0; //����� ���������� ������� ���� ��� ��������� ���������� coord (��)

		virtual map_object_ptr_t Get(int n = 0) = 0;

		virtual map_object_ptr_t FindObject(int code) = 0;  // ����� ������ �� ��� ����

		// ������ � ���������
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
