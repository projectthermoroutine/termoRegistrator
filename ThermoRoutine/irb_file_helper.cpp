#include "irb_file_helper.h"
#include <algorithm>
#include <common\fs_helpers.h>
#include <fstream>
#include "irb_frame_spec_info.h"

namespace irb_file_helper
{

	irb_file_exception::irb_file_exception(HRESULT error_code, const std::string & message) :
		std::runtime_error(message), _error_code(error_code)
	{
			std::ostringstream ss;
			ss << message << " Error: " << std::hex << std::showbase << error_code;
			_message = ss.str();
		}

	const char * irb_file_exception::what() const
	{
		return _message.c_str();
	}

	HRESULT irb_file_exception::get_error_code() const
	{
		return _error_code;
	}


#pragma pack(push,1)

	struct IRBHeader // ��������� IRB-�����
	{
		char formatID[5];  // ������ IRB-�����
		char origin[16];   // ��������
		DWORD ffVersion;   // ������ �����
		DWORD indexOff;	   // ���������� �������� ��� ������� ��������
		DWORD nrAvIndexes; // ���������� ��������� ��������
		DWORD nextIndexID; // ��������� ������
		union {
			DWORD nrAllIndexes; // ���������� ���� ��������
			char filler[27];   // �� ������������
		};
	};

	struct IRBIndexBlock // ��������� ����
	{
		WORD Type;	// ��� �������
		WORD subType;	// �� ����������
		DWORD version;	// ������ (������ 100)
		DWORD indexID;	// ����� �������
		DWORD dataPtr;	// ���������� �������� ������, ����������� ��������
		DWORD dataSize;	// ����� ������
		DWORD parent;	// 
		DWORD objectNr;	// �� ����������
		DWORD chksum;	// �����������  �����
	};

	struct IRBFrameKey
	{
		ULONG64 dataKey1; // ���� ������. �������� - ���������� ������
		ULONG64 dataKey2; // ���� ������. �������� - ����� ������
	};

	struct IRBIndexBlockEx // ��������� ����
	{
		WORD Type;	// ��� �������
		WORD subType;	// �� ����������
		DWORD version;	// ������ (������ 100)
		DWORD indexID;	// ����� �������
		DWORD dataPtr;	// ���������� �������� ������, ����������� ��������
		DWORD dataSize;	// ������ ������ � ������
		DWORD parent;	// 
		DWORD objectNr;	// �� ����������
		DWORD chksum;	// �����������  �����
		ULONG64 dataKey1; // ���� ������. �������� - ���������� ������
		double dataKey2; // ���� ������. �������� - ����� ������
	};

#pragma pack(pop)

	enum class index_block_type
	{
		irb_frame = 1,
		irb_spec = 4
	};

	enum class index_block_sub_type
	{
		v1 = 0,
		v2 = 1,
		v3 = 2,
		v4 = 3,
	};



	void
		create_irb_file(
		const std::wstring& name,
		camera_offset_t camera_offset,
		irb_file_version file_version,
		unsigned int max_frames_per_file
		)
	{
		create_irb_file(name, { &camera_offset, sizeof(camera_offset_t) }, file_version, max_frames_per_file);
	}



	void create_irb_file(
		const std::wstring& name,
		irb_file_version file_version,
		unsigned int max_frames_per_file
		)
	{
		create_irb_file(name, { nullptr, 0 }, file_version, max_frames_per_file);
	}


	void
		create_irb_file(
		const std::wstring& name,
		const stream_spec_info_t & info,
		irb_file_version file_version,
		unsigned int max_frames_per_file
		)
	{

		std::fstream stream;
		stream.open(name.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
		if (stream.rdstate() == std::ios::failbit)
		{
			auto result = ::GetLastError();
			throw irb_file_exception(result, "Can't open irb frames stream.");
		}

		IRBHeader hdr;
		std::memset(&hdr, 0, sizeof(IRBHeader));
		hdr.formatID[0] = (char)255;
		hdr.formatID[1] = 'I';
		hdr.formatID[2] = 'R';
		hdr.formatID[3] = 'B';
		hdr.formatID[4] = 0;
		hdr.ffVersion = static_cast<DWORD>(file_version);
		hdr.nrAvIndexes = max_frames_per_file;
		std::memcpy(hdr.origin, "VarioCAM", sizeof("VarioCAM"));
		hdr.indexOff = sizeof(IRBHeader);

		bool insert_header_spec_block = false;
		if (info.p_data != nullptr && info.size > 0)
		{
			hdr.indexOff = sizeof(IRBHeader) + info.size;
			insert_header_spec_block = true;
		}

		stream.write(reinterpret_cast<const char*>(&hdr), sizeof(IRBHeader));
		if (insert_header_spec_block)
			stream.write(reinterpret_cast<const char*>(info.p_data), info.size);

		//����� ������ ��������� �����
		IRBIndexBlock index_block;
		std::memset(&index_block, 0, sizeof(IRBIndexBlock));
		index_block.Type = static_cast<WORD>(index_block_type::irb_frame);
		index_block.version = 100;

		for (unsigned int i = 0; i < max_frames_per_file; ++i)
		{
			index_block.indexID = i + 1;
			stream.write(reinterpret_cast<const char*>(&index_block), sizeof(IRBIndexBlock));
		}
		//stream.close();
		//stream.open(name.c_str(), std::ios::in | std::ios::out | std::ios::binary);

//		return stream;
	}

	using namespace irb_frame_helper;


	inline void read_frame_coord(std::fstream & stream, FrameCoord &frame_coord, index_block_sub_type type)
	{
		switch (type)
		{
		case index_block_sub_type::v1:
		{
			stream >> (*reinterpret_cast<FrameCoord_v1*>(&frame_coord));
			break;
		}
		case index_block_sub_type::v2:
		{
			stream >> (*reinterpret_cast<FrameCoord_v2*>(&frame_coord));
			break;
		}
		case index_block_sub_type::v3:
		{
			stream >> (*reinterpret_cast<FrameCoord_v3*>(&frame_coord));
			break;
		}
		case index_block_sub_type::v4:
		{
			stream >> frame_coord;
			break;
		}
		default:
		{
			throw irb_file_exception(static_cast<HRESULT>(type), "Invalid format of the irb index block.");
		}
		};

	}

	template<typename TKey>
	using get_key_func_t = TKey(*)(const IRBIndexBlockEx&);

	template<typename TKey>
	using get_frame_key_func_t = std::function<TKey(const IRBFrame&)>;

	irb_frame_helper::counter_t get_frame_counter(const IRBFrame& frame){ return frame.coords.counter > 0 ? frame.coords.counter : frame.id; }

	struct IRBFile::Impl
	{
		using stream_t = std::fstream;
		typedef std::vector<IRBIndexBlockEx>::const_iterator const_index_iterator;
		typedef std::vector<IRBIndexBlockEx>::iterator index_iterator;

	public:
		Impl():
			stream_size(0),
			frame_pos(0),
			header_spec_info_size(0),
			_frames_keys_retrieved(false),
			number_filled_frame_indexes(0),
			get_frame_key_func(get_frame_counter)
		{
			std::memset(&header, 0, sizeof(IRBHeader));
		}

	public:
		stream_t stream;
		fs_helpers::stream_data_size_t stream_size;
		std::vector<IRBIndexBlockEx> index_blocks;
		IRBHeader header;
		frame_id_t frame_pos;
		uint32_t number_filled_frame_indexes;
		get_frame_key_func_t<irb_frame_helper::counter_t> get_frame_key_func;


		std::wstring stream_name;

		
		std::unique_ptr<char[]> header_spec_info;
		uint16_t header_spec_info_size;

	private:
		bool _frames_keys_retrieved;

	public:
		void open()
		{
			LOG_STACK();
			if (stream.is_open())
				return;

			stream.open(stream_name.c_str(), std::ios::in | std::ios::out | std::ios::binary);
			if (stream.rdstate() == std::ios::failbit)
			{
				auto result = ::GetLastError();
				throw irb_file_exception(result, "Can't open irb frames stream.");
			}
		}

		void open(const std::wstring& name)
		{
			LOG_STACK();
			if (stream.is_open()) 
				stream.close();
			const auto current_stream_name = stream_name;
			stream_name = name;
			try{
				open();
			}
			catch (const irb_file_exception&)
			{
				stream_name = current_stream_name;
				throw;
			}
		}

		void close() { LOG_STACK(); if (stream.is_open()) stream.close(); }

		bool get_stream_spec_info(stream_spec_info_t & info)
		{
			if (header_spec_info_size > 0)
			{
				if (info.size == header_spec_info_size && info.p_data != nullptr)
				{
					std::memcpy(info.p_data, header_spec_info.get(), header_spec_info_size);
					return true;
				}
			}

			return false;
		}

		void read_header()
		{
			stream_size = fs_helpers::get_stream_data_size(stream);
			if (stream_size < sizeof(IRBHeader))
			{
				throw irb_file_exception((HRESULT)stream_size, "Invalid size of the irb frames stream.");
			}

			stream.seekg(0, std::ios::beg);
			stream.read(reinterpret_cast<char*>(&header), sizeof(IRBHeader));
			if (stream.rdstate() == std::ios::failbit)
			{
				auto result = ::GetLastError();
				throw irb_file_exception(result, "Can't read irb frames stream.");
			}

			if (header.indexOff > sizeof(IRBHeader))
			{
				header_spec_info_size = (uint16_t)(header.indexOff - sizeof(IRBHeader));
				header_spec_info = std::make_unique<char[]>(header_spec_info_size);
				stream.read(reinterpret_cast<char*>(header_spec_info.get()), header_spec_info_size);
				if (stream.rdstate() == std::ios::failbit)
				{
					auto result = ::GetLastError();
					throw irb_file_exception(result, "Can't read irb frames stream.");
				}
			}
		}

		void write_header()
		{
			stream.seekg(0, std::ios::beg);
			stream.write(reinterpret_cast<const char*>(&header), sizeof(IRBHeader));
		}

		void write_stream_spec_info(const stream_spec_info_t & info)
		{
			if (header_spec_info_size > 0)
			{
				if (info.size == header_spec_info_size && info.p_data != nullptr)
				{
					stream.seekg(sizeof(IRBHeader), std::ios::beg);
					stream.write(reinterpret_cast<const char*>(info.p_data), header_spec_info_size);
				}
			}
		}


		void read_index_blocks()
		{
			uint32_t number_blocks = 0;
			switch (static_cast<irb_file_version>(header.ffVersion)){
			case irb_file_version::original:
				number_blocks = header.nrAvIndexes;
				break;
			case irb_file_version::patched:
				number_blocks = header.nrAvIndexes;// header.nrAllIndexes;
				break;
			default:
				throw irb_file_exception((HRESULT)(header.ffVersion), "Unsupported irb frames stream format.");
			}
			if (number_blocks == 0){
				throw irb_file_exception(0, "Invalid number index blocks in the irb frames stream.");
			}
			if (number_blocks*sizeof(IRBIndexBlock)+sizeof(IRBHeader) > stream_size)
				throw irb_file_exception((HRESULT)(stream_size), "Invalid size of the irb frames stream.");

			index_blocks.resize(number_blocks);
			auto cur_pos = stream.tellg();
			if (header.indexOff != cur_pos){
				stream.seekg(header.indexOff, std::ios::beg);
			}

			uint32_t number_frame_blocks = 0;
			number_filled_frame_indexes = 0;
			uint32_t last_frame_id = 0;
			for (uint32_t i = 0; i < number_blocks; ++i, ++number_frame_blocks, ++number_filled_frame_indexes)
			{
				stream.read(reinterpret_cast<char*>(&index_blocks[number_frame_blocks]), sizeof(IRBIndexBlock));
				if (stream.rdstate() == std::ios::failbit)
				{
					index_blocks.clear();
					throw irb_file_exception((HRESULT)(stream_size), "Invalid size of the irb frames stream.");
				}
				const auto & index_block = index_blocks[number_frame_blocks];
				if (index_block.Type != static_cast<DWORD>(index_block_type::irb_frame))
				{
					number_frame_blocks--; number_filled_frame_indexes--;
				}
				else
				{
					if (index_block.dataSize == 0 || index_block.dataPtr == 0)
						number_filled_frame_indexes--;
				}
			}
			if (number_blocks != number_frame_blocks)
				index_blocks.resize(number_frame_blocks);

		}

		void retrieve_frames_keys()
		{
			_frames_keys_retrieved = true;

			if (index_blocks.size() == 0)
				return;

			stream.seekg(0, std::ios::beg);

			auto cur_pos = stream.tellg();
			bool is_patched_file = true;
			if (static_cast<irb_file_version>(header.ffVersion) != irb_file_version::patched)
				is_patched_file = false;

			auto frame_coord_offset = get_frame_coordinate_type_offset();
			auto frame_time_offset = get_frame_time_offset();
			auto seek_strategy = std::ios::beg;

			for (auto &index_block : index_blocks)
			{
				index_block.dataKey1 = 0;
				index_block.dataKey2 = 0; 
				if (index_block.dataPtr == 0){
					continue;
				}
		
				stream.seekg(index_block.dataPtr - cur_pos + frame_time_offset, std::ios::cur);
				double frame_time = 0;
				stream.read(reinterpret_cast<char*>(&frame_time), sizeof(frame_time));
				if (stream.rdstate() == std::ios::failbit)
				{
					index_blocks.clear();
					throw irb_file_exception(index_block.dataPtr + frame_time_offset, "Invalid format of the irb frames stream.");
				}
				index_block.dataKey2 = frame_time;

				index_block.dataKey1 = 0;
				if (is_patched_file)
				{
					stream.seekg(index_block.dataPtr + index_block.dataSize, seek_strategy);

					FrameCoord frame_coord;
					read_frame_coord(stream, frame_coord, static_cast<index_block_sub_type>(index_block.subType));

					if (stream.rdstate() == std::ios::failbit)
					{
						index_blocks.clear();
						throw irb_file_exception(index_block.dataPtr + index_block.dataSize, "Invalid format of the irb frames stream.");
					}
					index_block.dataKey1 = frame_coord.coordinate;
				}
				cur_pos = stream.tellg();
			}

		}

		void write_index_block(unsigned int index_index_block)
		{
			
			if (index_index_block >= index_blocks.size()){
				return;
			}

			stream.seekg(header.indexOff + sizeof(IRBIndexBlock)*index_index_block, std::ios::beg);
			stream.write(reinterpret_cast<const char*>(&index_blocks[index_index_block]), sizeof(IRBIndexBlock));
		}

		void write_index_blocks()
		{
			if (index_blocks.size() == 0){
				return;
			}
			stream.seekg(header.indexOff, std::ios::beg);
			for (auto &index_block : index_blocks)
			{
				stream.write(reinterpret_cast<const char*>(&index_block), sizeof(IRBIndexBlock));
			}
		}

		IRBFrame * read_frame_by_index(uint32_t index)
		{
			if (index >= index_blocks.size()){
				return nullptr;
			}

			const auto & frame_index_block = index_blocks[index];
			stream.seekg(frame_index_block.dataPtr, std::ios::beg);

			IRBFrame *frame = new IRBFrame();
			stream >> *frame;

			if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
			{
				read_frame_coord(stream, frame->coords, static_cast<index_block_sub_type>(frame_index_block.subType));
			}

			if (stream.rdstate() == std::ios::failbit)
			{
				throw irb_file_exception(header.ffVersion, "Invalid format of the irb frames stream.");
			}

			return frame;
		}


		IRBFrame * read_frame_by_id(uint32_t id)
		{
			const auto &iter = find_index_block_by_id(id);
			if (iter == index_blocks.cend())
				return nullptr;

			IRBFrame *frame = new IRBFrame();
			stream >> *frame;
			frame->id = id;

			if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
			{
				read_frame_coord(stream, frame->coords, static_cast<index_block_sub_type>(iter->subType));
			}

			if (stream.rdstate() == std::ios::failbit)
			{
				throw irb_file_exception(header.ffVersion, "Invalid format of the irb frames stream.");
			}

			return frame;
		}

		IRBFrame * read_frame_by_key1(DWORD64 key)
		{
			if (index_blocks.empty()){
				return nullptr;
			}

			if (!_frames_keys_retrieved)
				retrieve_frames_keys();

			for (auto & index_block : index_blocks)
			{
				if (index_block.dataKey1 == key)
				{
					stream.seekg(index_block.dataPtr, std::ios::beg);
					IRBFrame *frame = new IRBFrame();
					stream >> *frame;
					frame->id = index_block.indexID;
					if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
						read_frame_coord(stream, frame->coords, static_cast<index_block_sub_type>(index_block.subType));

					return frame;
				}
			}

			return nullptr;
		}

		IRBFrame * read_frame_by_key2(double key)
		{
			if (index_blocks.empty()){
				return nullptr;
			}

			if (!_frames_keys_retrieved)
				retrieve_frames_keys();

			for (auto & index_block : index_blocks)
			{
				if (index_block.dataKey2 == key)
				{
					stream.seekg(index_block.dataPtr, std::ios::beg);
					IRBFrame *frame = new IRBFrame();
					stream >> *frame;
					frame->id = index_block.indexID;
					if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
						read_frame_coord(stream, frame->coords, static_cast<index_block_sub_type>(index_block.subType));
					return frame;
				}
			}

			return nullptr;
		}


		index_iterator find_index_block_by_id(unsigned int index_block_id)
		{
			return std::find_if(index_blocks.begin(), index_blocks.end(), [index_block_id](IRBIndexBlockEx& block){return block.indexID == index_block_id; });
		}
		const_index_iterator find_index_block_by_id(unsigned int index_block_id) const
		{
			return std::find_if(index_blocks.cbegin(), index_blocks.cend(), [index_block_id](const IRBIndexBlockEx& block){return block.indexID == index_block_id; });
		}

		IRBIndexBlockEx & append_index_block()
		{
			auto count_index_blocks = index_blocks.size();
			index_blocks.resize(count_index_blocks + 1);
			std::memset(&index_blocks[count_index_blocks], 0, sizeof(IRBIndexBlockEx));
			return index_blocks.at(count_index_blocks++);
		}


		template<typename TIndexBlock>
		void set_index_block_data(const irb_block_info_t& block_info, TIndexBlock &index_block)
		{
			index_block.indexID = block_info.indexID;
			index_block.Type = block_info.Type;
			index_block.subType = block_info.subType;
			index_block.version = block_info.version;
		}

		template<typename TIndexBlock,class T>
		void write_block_data(uint32_t index_index_block, TIndexBlock& index_block, const T& block_data)
		{
			auto seek_dir = std::ios::end;

			if (index_block.dataPtr != 0)
				seek_dir = std::ios::beg;

			stream.seekg(index_block.dataPtr, seek_dir);

			auto data_begin = stream.tellg();

			index_block.dataPtr = (DWORD)data_begin;
			stream << block_data;

			auto data_end = stream.tellg();
			index_block.dataSize = (DWORD)(data_end - data_begin);

			write_index_block(index_index_block);

			if (index_index_block == 0)
			{
				header.nextIndexID = index_block.indexID;
				write_header();
			}
		}


		void write_frame_by_id(uint32_t id, const IRBFrame & frame)
		{
			auto iter = find_index_block_by_id(id);
			if (iter == index_blocks.end())
				return;

			WORD subType = 0;
			if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
				subType = static_cast<WORD>(index_block_sub_type::v4);

			irb_block_info_t block_info = { static_cast<WORD>(index_block_type::irb_frame), subType, 100, id };

			auto & index_block = *iter;
			set_index_block_data(block_info, index_block);
			write_frame(std::distance(index_blocks.begin(), iter), index_block, frame);
		}

		void write_frame_by_index(uint32_t index, const IRBFrame & frame)
		{
			if (index >= index_blocks.size())
				return;

			WORD subType = 0;
			if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
				subType = static_cast<WORD>(index_block_sub_type::v4);

			uint32_t indexID = frame.id;
			if (get_frame_key_func){
				indexID = get_frame_key_func(frame);
			}

			irb_block_info_t block_info = { static_cast<WORD>(index_block_type::irb_frame), subType, 100, indexID };
			
			auto & index_block = index_blocks[index];
			bool empty_index_block = false;
			if (index_block.dataSize == 0){
				empty_index_block = true;
			}
			set_index_block_data(block_info, index_block);
			write_frame(index,index_block, frame);
			if (empty_index_block){
				++number_filled_frame_indexes;
			}
		}

		template<typename TIndexBlock>
		void write_frame(uint32_t index_index_block, TIndexBlock &index_block, const IRBFrame & frame)
		{
			write_block_data(index_index_block,index_block, frame);

			if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
			{
				if (index_block.Type == static_cast<WORD>(index_block_type::irb_frame) &&
					index_block.dataPtr != 0 &&
					index_block.dataSize != 0
					)
				{
					stream.seekg(index_block.dataPtr + index_block.dataSize, std::ios::beg);
					stream << frame.coords;
				}
			}

		}
		
		void append_frames(const std::vector<irb_frame_shared_ptr_t> & frames)
		{
			if (index_blocks.size() == 0 ){
				return;
			}

			auto result_size_frames = frames.size();

			if (result_size_frames == 0){
				return;
			}

			if (result_size_frames > index_blocks.size()){
				return;
			}


			auto begin_index = number_filled_frame_indexes;
			auto count_index_blocks = index_blocks.size();
			if (header.nrAvIndexes < result_size_frames){
				return;
			}

			bool write_coords = true;
			WORD subType = static_cast<WORD>(index_block_sub_type::v4);
			if (static_cast<irb_file_version>(header.ffVersion) != irb_file_version::patched){
				write_coords = false;
				subType = 0;
			}

			stream.seekg(0, std::ios::end);

			for (unsigned int i = begin_index, j = 0; i < result_size_frames + begin_index; ++i, ++j)
			{
				auto &cur_frame = frames.at(j);
				auto &frame_index_block = index_blocks.at(i);
				frame_index_block.indexID = get_frame_key_func ? get_frame_key_func(*cur_frame) : cur_frame->id;

				frame_index_block.Type = static_cast<DWORD>(index_block_type::irb_frame);
				frame_index_block.version = 100;
				frame_index_block.subType = subType;

				frame_index_block.dataKey1 = cur_frame->coords.coordinate;
				frame_index_block.dataKey2 = cur_frame->header.presentation.imgTime;

				auto data_begin = stream.tellg();
				frame_index_block.dataPtr = (DWORD)data_begin;
				stream << *cur_frame;
				auto data_end = stream.tellg();
				frame_index_block.dataSize = (DWORD)(data_end - data_begin);

				if (write_coords)
					stream << cur_frame->coords;

				++number_filled_frame_indexes;

			}

			write_index_blocks();

			bool is_need_write_header = false;
			//if (index_blocks.size() > header.nrAvIndexes)
			//{
			//	header.nrAvIndexes = index_blocks.size();
			//	is_need_write_header = true;
			//}
			if (begin_index == 0)
			{
				header.nextIndexID = index_blocks[0].indexID;
				is_need_write_header = true;
			}

			if (is_need_write_header){
				write_header();
			}
		}

		unsigned int count_frames()
		{
			return (uint32_t)number_filled_frame_indexes;
		}
		unsigned int max_number_frames()
		{
			return (uint32_t)index_blocks.size();
		}

		template<typename TSpan = irb_frames_key_span_t, typename TKey = irb_frame_key>
		TSpan get_frames_key_span(get_key_func_t<TKey> get_key_func)
		{
			if (index_blocks.size() == 0)
				return TSpan(0, 0);

			return TSpan(get_key_func(index_blocks.front()), get_key_func(index_blocks.back()));
		}

		template<typename TList = irb_frames_key_list_t, typename TKey = irb_frame_key>
		TList get_frames_key_list(get_key_func_t<TKey> get_key_func)
		{
			if (index_blocks.size() == 0)
				return TList();

			TList list;
			//list.resize(index_blocks.size());
			for (auto &index_block : index_blocks)
			{
				list.emplace_back(get_key_func(index_block));
			}

			return list;
		}


		template<typename TSpan,typename TKey>
		TSpan get_frames_data_span(unsigned int offset)
		{
			if (index_blocks.size() == 0)
				return TSpan(0, 0);
			
			char* p_first_block = reinterpret_cast<char*>(&index_blocks.front());
			char* p_last_block = reinterpret_cast<char*>(&index_blocks.back());
			TKey min_value = *reinterpret_cast<TKey*>(p_first_block + offset);
			TKey max_value = *reinterpret_cast<TKey*>(p_last_block + offset);

			return TSpan(min_value, max_value);
		}

		template<typename TList, typename TKey>
		TList get_frames_key_data_list(unsigned int offset)
		{
			if (index_blocks.size() == 0)
				return TList();

			TList list;
			//list.resize(index_blocks.size());
			for (auto &index_block : index_blocks)
			{
				char* p_block = reinterpret_cast<char*>(&index_block);
				list.push_back(*reinterpret_cast<TKey*>(p_block + offset));
			}

			return list;
		}

	};


	irb_frame_key default_get_frame_key_from_index_block(const  IRBIndexBlockEx& data)
	{
		return irb_frame_key(data.indexID, data.dataKey1, data.dataKey2);
	}

	IRBFile::IRBFile() :_p_impl(new IRBFile::Impl())
	{
	}

	IRBFile::IRBFile(const std::wstring & file_name) : IRBFile()
	{
		_p_impl->open(file_name);
		_p_impl->read_header();
		_p_impl->read_index_blocks();
		_p_impl->close();
	}

	IRBFile::~IRBFile() = default;

	void IRBFile::open() 
	{
		_p_impl->open();
	}

	void IRBFile::close()
	{
		_p_impl->close();
	}

	irb_frame_ptr_t IRBFile::read_frame_by_id(uint32_t id)
	{
		return irb_frame_ptr_t(_p_impl->read_frame_by_id(id));
	}
	irb_frame_ptr_t IRBFile::read_frame_by_index(uint32_t index)
	{
		return irb_frame_ptr_t(_p_impl->read_frame_by_index(index));
	}

	irb_frame_ptr_t IRBFile::read_frame_by_coordinate(coordinate_t coordinate)
	{
		return irb_frame_ptr_t(_p_impl->read_frame_by_key1(coordinate));
	}
	irb_frame_ptr_t IRBFile::read_frame_by_time(double time)
	{
		return irb_frame_ptr_t(_p_impl->read_frame_by_key2(time));
	}

	void IRBFile::write_frame_by_id(uint32_t id, const IRBFrame & frame)
	{
		_p_impl->write_frame_by_id(id, frame);
	}

	void IRBFile::write_frame_by_index(uint32_t index, const IRBFrame & frame)
	{
		_p_impl->write_frame_by_index(index, frame);
	}
	
	void IRBFile::append_frames(const std::vector<irb_frame_shared_ptr_t> & frames)
	{
		_p_impl->append_frames(frames);
	}

	unsigned int IRBFile::count_frames()
	{
		return _p_impl->count_frames();
	}

	unsigned int IRBFile::max_number_frames()
	{
		return _p_impl->max_number_frames();
	}
	

	const wchar_t * IRBFile::file_name()
	{
		return  _p_impl->stream_name.c_str();
	}

	irb_frames_key_span_t IRBFile::get_frames_key_span()
	{
		return _p_impl->get_frames_key_span(default_get_frame_key_from_index_block);
	}
	irb_frames_key_list_t IRBFile::get_frames_key_list()
	{
		return _p_impl->get_frames_key_list(default_get_frame_key_from_index_block);
	}


	irb_frames_coordinate_span_t IRBFile::get_frames_coordinate_span()
	{
		return _p_impl->get_frames_data_span<irb_frames_coordinate_span_t, uint64_t>(FIELD_OFFSET(IRBIndexBlockEx, dataKey1));
	}
	irb_frames_time_span_t IRBFile::get_frames_time_span()
	{
		return _p_impl->get_frames_data_span<irb_frames_time_span_t, double>(FIELD_OFFSET(IRBIndexBlockEx, dataKey2));
	}

	irb_frames_span_t IRBFile::get_frames_id_span()
	{
		return _p_impl->get_frames_data_span<irb_frames_span_t, uint32_t>(FIELD_OFFSET(IRBIndexBlock, indexID));
	}

	irb_frames_id_list_t IRBFile::get_frames_id_list()
	{
		return _p_impl->get_frames_key_data_list<irb_frames_id_list_t, uint32_t>(FIELD_OFFSET(IRBIndexBlock, indexID));
	}

	irb_frames_coordinate_list_t IRBFile::get_frames_coordinate_list()
	{
		return _p_impl->get_frames_key_data_list<irb_frames_coordinate_list_t, uint64_t>(FIELD_OFFSET(IRBIndexBlockEx, dataKey1));
	}

	irb_frames_time_list_t IRBFile::get_frames_time_list()
	{
		return _p_impl->get_frames_key_data_list<irb_frames_time_list_t, double>(FIELD_OFFSET(IRBIndexBlockEx, dataKey2));
	}

	void IRBFile::write_block_data(const irb_block_info_t& block_info, const IRBFrame& block_data)
	{
		auto iter = _p_impl->find_index_block_by_id(block_info.indexID);
		if (iter == _p_impl->index_blocks.end())
			return;

		auto & index_block = *iter;
		_p_impl->set_index_block_data(block_info, index_block);
		_p_impl->write_block_data(std::distance(_p_impl->index_blocks.begin(), iter), index_block, block_data);
	}

	void IRBFile::write_block_data(const irb_block_info_t& block_info, const irb_frame_spec_info::irb_frame_spec_info& block_data)
	{
		auto iter = _p_impl->find_index_block_by_id(block_info.indexID);
		if (iter == _p_impl->index_blocks.end())
			return;

		auto & index_block = *iter;
		_p_impl->set_index_block_data(block_info, index_block);
		_p_impl->write_block_data(std::distance(_p_impl->index_blocks.begin(), iter), index_block, block_data);
	}

	bool IRBFile::get_stream_spec_info(stream_spec_info_t & info) const
	{
		return _p_impl->get_stream_spec_info(info);
	}

	void IRBFile::write_stream_spec_info(const stream_spec_info_t & info) const
	{
		_p_impl->write_stream_spec_info(info);
	}


	camera_offset_t read_camera_offset_from_file(const IRBFile& file)
	{
		camera_offset_t camera_offset = 0;
		stream_spec_info_t info{ &camera_offset, sizeof(camera_offset_t) };

		file.get_stream_spec_info(info);

		return camera_offset;
	}

	void write_camera_offset_to_file(const IRBFile& file, camera_offset_t offset)
	{
		stream_spec_info_t info{ &offset, sizeof(camera_offset_t) };
		file.write_stream_spec_info(info);
	}


}
