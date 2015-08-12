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

	struct IRBHeader // заголовок IRB-файла
	{
		char formatID[5];  // маркер IRB-файла
		char origin[16];   // источник
		DWORD ffVersion;   // версия файла
		DWORD indexOff;	   // абсолютное смещение для таблицы индексов
		DWORD nrAvIndexes; // количество доступных индексов
		DWORD nextIndexID; // следующий индекс
		union {
			DWORD nrAllIndexes; // количество всех индексов
			char filler[27];   // не используется
		};
	};

	struct IRBIndexBlock // индексный блок
	{
		WORD Type;	// тип индекса
		WORD subType;	// не используем
		DWORD version;	// версия (всегда 100)
		DWORD indexID;	// номер индекса
		DWORD dataPtr;	// абсолютное смещение данных, описываемых индексом
		DWORD dataSize;	// объем данных
		DWORD parent;	// 
		DWORD objectNr;	// не используем
		DWORD chksum;	// контрольная  сумма
	};

	struct IRBFrameKey
	{
		ULONG64 dataKey1; // ключ данных. например - координата фрейма
		ULONG64 dataKey2; // ключ данных. например - время фрейма
	};

	struct IRBIndexBlockEx // индексный блок
	{
		WORD Type;	// тип индекса
		WORD subType;	// не используем
		DWORD version;	// версия (всегда 100)
		DWORD indexID;	// номер индекса
		DWORD dataPtr;	// абсолютное смещение данных, описываемых индексом
		DWORD dataSize;	// размер данных в байтах
		DWORD parent;	// 
		DWORD objectNr;	// не используем
		DWORD chksum;	// контрольная  сумма
		ULONG64 dataKey1; // ключ данных. например - координата фрейма
		double dataKey2; // ключ данных. например - время фрейма
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
		v3 = 2
	};



	stream_ptr_t
		create_irb_file(
		const std::string& name,
		camera_offset_t camera_offset,
		irb_file_version file_version,
		unsigned int max_frames_per_file
		)
	{
		return create_irb_file(name, { &camera_offset, sizeof(camera_offset_t) }, file_version, max_frames_per_file);
	}



	stream_ptr_t create_irb_file(
		const std::string& name,
		irb_file_version file_version,
		unsigned int max_frames_per_file
		)
	{
		return create_irb_file(name, { nullptr, 0 }, file_version, max_frames_per_file);
	}


	stream_ptr_t
		create_irb_file(
		const std::string& name,
		const stream_spec_info_t & info,
		irb_file_version file_version,
		unsigned int max_frames_per_file
		)
	{

		stream_ptr_t stream = std::make_shared<std::fstream>();
		stream->open(name.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
		if (stream->rdstate() == std::ios::failbit)
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

		stream->write(reinterpret_cast<const char*>(&hdr), sizeof(IRBHeader));
		if (insert_header_spec_block)
			stream->write(reinterpret_cast<const char*>(info.p_data), info.size);

		//Пишем пустые индексные блоки
		IRBIndexBlock index_block;
		std::memset(&index_block, 0, sizeof(IRBIndexBlock));
		index_block.Type = static_cast<WORD>(index_block_type::irb_frame);
		index_block.version = 100;

		for (unsigned int i = 0; i < max_frames_per_file; i++)
		{
			index_block.indexID = i + 1;
			stream->write(reinterpret_cast<const char*>(&index_block), sizeof(IRBIndexBlock));
		}
		stream->close();
		stream->open(name.c_str(), std::ios::in | std::ios::out | std::ios::binary);

		return stream;
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

	struct IRBFile::Impl
	{
		//using stream_ptr_t = std::unique_ptr<std::iostream>;

	public:
		Impl():
			stream_size(0), 
			frame_pos(0), 
			header_spec_info_size(0),
			_frames_keys_retrieved(false)
		{
			std::memset(&header, 0, sizeof(IRBHeader));
		}

		void set_stream(stream_ptr_t & stream)
		{
			this->stream.swap(stream);
		}

	public:
		stream_ptr_t stream;
		fs_helpers::stream_data_size_t stream_size;
		std::vector<IRBIndexBlockEx> index_blocks;
		IRBHeader header;
		frame_id_t frame_pos;

		std::string stream_name;

		
		std::unique_ptr<char[]> header_spec_info;
		uint16_t header_spec_info_size;

	private:
		bool _frames_keys_retrieved;

	public:

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
			stream_size = fs_helpers::get_stream_data_size(*stream.get());
			if (stream_size < sizeof(IRBHeader))
			{
				throw irb_file_exception((HRESULT)stream_size, "Invalid size of the irb frames stream.");
			}

			stream->seekg(0, std::ios::beg);
			stream->read(reinterpret_cast<char*>(&header), sizeof(IRBHeader));
			if (stream->rdstate() == std::ios::failbit)
			{
				auto result = ::GetLastError();
				throw irb_file_exception(result, "Can't read irb frames stream.");
			}

			if (header.indexOff > sizeof(IRBHeader))
			{
				header_spec_info_size = (uint16_t)(header.indexOff - sizeof(IRBHeader));
				header_spec_info = std::make_unique<char[]>(header_spec_info_size);
				stream->read(reinterpret_cast<char*>(header_spec_info.get()), header_spec_info_size);
				if (stream->rdstate() == std::ios::failbit)
				{
					auto result = ::GetLastError();
					throw irb_file_exception(result, "Can't read irb frames stream.");
				}
			}
		}

		void write_header()
		{
			stream->seekg(0, std::ios::beg);
			stream->write(reinterpret_cast<const char*>(&header), sizeof(IRBHeader));
		}

		void write_stream_spec_info(const stream_spec_info_t & info)
		{
			if (header_spec_info_size > 0)
			{
				if (info.size == header_spec_info_size && info.p_data != nullptr)
				{
					stream->seekg(sizeof(IRBHeader), std::ios::beg);
					stream->write(reinterpret_cast<const char*>(info.p_data), header_spec_info_size);
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
			auto cur_pos = stream->tellg();
			if (header.indexOff != cur_pos){
				stream->seekg(header.indexOff, std::ios::beg);
			}

			uint32_t number_frame_blocks = 0;
			uint32_t last_frame_id = 0;
			for (uint32_t i = 0; i < number_blocks; i++, number_frame_blocks++)
			{
				stream->read(reinterpret_cast<char*>(&index_blocks[number_frame_blocks]), sizeof(IRBIndexBlock));
				if (stream->rdstate() == std::ios::failbit)
				{
					index_blocks.clear();
					throw irb_file_exception((HRESULT)(stream_size), "Invalid size of the irb frames stream.");
				}
				const auto & index_block = index_blocks[number_frame_blocks];
				if (index_block.Type != static_cast<DWORD>(index_block_type::irb_frame))
				{
					number_frame_blocks--;
				}
				//else
				//{
				//	if (last_frame_id >= index_block.indexID)
				//		number_frame_blocks--;
				//	else
				//		last_frame_id = index_block.indexID;
				//}
			}
			if (number_blocks != number_frame_blocks)
				index_blocks.resize(number_frame_blocks);

		}

		void retrieve_frames_keys()
		{
			_frames_keys_retrieved = true;

			if (index_blocks.size() == 0)
				return;

			stream->seekg(0, std::ios::beg);

			auto cur_pos = stream->tellg();
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
		
				stream->seekg(index_block.dataPtr - cur_pos + frame_time_offset, std::ios::cur);
				double frame_time = 0;
				stream->read(reinterpret_cast<char*>(&frame_time), sizeof(frame_time));
				if (stream->rdstate() == std::ios::failbit)
				{
					index_blocks.clear();
					throw irb_file_exception(index_block.dataPtr + frame_time_offset, "Invalid format of the irb frames stream.");
				}
				index_block.dataKey2 = frame_time;

				index_block.dataKey1 = 0;
				if (is_patched_file)
				{
					stream->seekg(index_block.dataPtr + index_block.dataSize, seek_strategy);

					FrameCoord frame_coord;
					read_frame_coord(*stream, frame_coord, static_cast<index_block_sub_type>(index_block.subType));

					if (stream->rdstate() == std::ios::failbit)
					{
						index_blocks.clear();
						throw irb_file_exception(index_block.dataPtr + index_block.dataSize, "Invalid format of the irb frames stream.");
					}
					index_block.dataKey1 = frame_coord.coordinate;
				}
				cur_pos = stream->tellg();
			}

		}

		void write_index_block(unsigned int index_index_block)
		{
			
			if (index_index_block >= index_blocks.size()){
				return;
			}

			stream->seekg(header.indexOff + sizeof(IRBIndexBlock)*index_index_block, std::ios::beg);
			stream->write(reinterpret_cast<const char*>(&index_blocks[index_index_block]), sizeof(IRBIndexBlock));
		}

		void write_index_blocks()
		{
			if (index_blocks.size() == 0){
				return;
			}
			stream->seekg(header.indexOff, std::ios::beg);
			for (auto &index_block : index_blocks)
			{
				stream->write(reinterpret_cast<const char*>(&index_block), sizeof(IRBIndexBlock));
			}
		}

		IRBFrame * read_frame_by_index(uint32_t index)
		{
			if (index >= index_blocks.size()){
				return nullptr;
			}

			const auto & frame_index_block = index_blocks[index];
			stream->seekg(frame_index_block.dataPtr, std::ios::beg);

			IRBFrame *frame = new IRBFrame();
			*stream >> *frame;

			if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
			{
				read_frame_coord(*stream, frame->coords, static_cast<index_block_sub_type>(frame_index_block.subType));
			}

			if (stream->rdstate() == std::ios::failbit)
			{
				throw irb_file_exception(header.ffVersion, "Invalid format of the irb frames stream.");
			}

			return frame;
		}


		IRBFrame * read_frame_by_id(frame_id_t id)
		{
			if (index_blocks.empty() || id == 0){
				return nullptr;
			}

			auto begin_index = header.nextIndexID;
			if (id < begin_index || index_blocks.size() <= id - begin_index){
				return nullptr;
			}

			const auto index_index_block = id - begin_index;
			const auto & frame_index_block = index_blocks[index_index_block];
			stream->seekg(frame_index_block.dataPtr, std::ios::beg);

			frame_pos = id;

			IRBFrame *frame = new IRBFrame();
			*stream >> *frame;
			frame->id = id;

			if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
			{
				read_frame_coord(*stream, frame->coords, static_cast<index_block_sub_type>(frame_index_block.subType));
			}

			if (stream->rdstate() == std::ios::failbit)
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
					stream->seekg(index_block.dataPtr, std::ios::beg);
					IRBFrame *frame = new IRBFrame();
					*stream >> *frame;
					frame->id = index_block.indexID;
					if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
						read_frame_coord(*stream, frame->coords, static_cast<index_block_sub_type>(index_block.subType));

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
					stream->seekg(index_block.dataPtr, std::ios::beg);
					IRBFrame *frame = new IRBFrame();
					*stream >> *frame;
					frame->id = index_block.indexID;
					if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
						read_frame_coord(*stream, frame->coords, static_cast<index_block_sub_type>(index_block.subType));
					return frame;
				}
			}

			return nullptr;
		}


		std::vector<IRBIndexBlockEx>::iterator find_index_block_by_id(unsigned int index_block_id)
		{
			return std::find_if(index_blocks.begin(), index_blocks.end(), [index_block_id](IRBIndexBlockEx& block){return block.indexID == index_block_id; });
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

			stream->seekg(index_block.dataPtr, seek_dir);

			auto data_begin = stream->tellg();

			index_block.dataPtr = (DWORD)data_begin;
			*stream << block_data;

			auto data_end = stream->tellg();
			index_block.dataSize = (DWORD)(data_end - data_begin);

			write_index_block(index_index_block);

			if (index_index_block == 0)
			{
				header.nextIndexID = index_block.indexID;
				write_header();
			}
		}


		void write_frame_by_id(frame_id_t id, const IRBFrame & frame)
		{
			WORD subType = 0;
			if (static_cast<irb_file_version>(header.ffVersion) == irb_file_version::patched)
				subType = static_cast<WORD>(index_block_sub_type::v3);

			irb_block_info_t block_info = { static_cast<WORD>(index_block_type::irb_frame), subType, 100, id };

			auto iter = find_index_block_by_id(id);
			if (iter == index_blocks.end())
				return;

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
				subType = static_cast<WORD>(index_block_sub_type::v3);

			irb_block_info_t block_info = { static_cast<WORD>(index_block_type::irb_frame), subType, 100, frame.id };
			
			auto & index_block = index_blocks[index];
			set_index_block_data(block_info, index_block);

			write_frame(index,index_block, frame);
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
					stream->seekg(index_block.dataPtr + index_block.dataSize, std::ios::beg);
					*stream << frame.coords;
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


			auto begin_index = 0;
			auto count_index_blocks = index_blocks.size();
			if (header.nrAvIndexes < result_size_frames){
				return;
			}

			bool write_coords = true;
			WORD subType = static_cast<WORD>(index_block_sub_type::v3);
			if (static_cast<irb_file_version>(header.ffVersion) != irb_file_version::patched){
				write_coords = false;
				subType = 0;
			}

			stream->seekg(0, std::ios::end);

			for (unsigned int i = begin_index, j = 0; i < result_size_frames + begin_index; i++, j++)
			{
				auto &cur_frame = frames.at(j);
				auto &frame_index_block = index_blocks.at(i);
				frame_index_block.indexID = cur_frame->getFrameNum();
				frame_index_block.Type = static_cast<DWORD>(index_block_type::irb_frame);
				frame_index_block.version = 100;
				frame_index_block.subType = subType;

				frame_index_block.dataKey1 = cur_frame->coords.coordinate;
				frame_index_block.dataKey2 = cur_frame->header.presentation.imgTime;

				auto data_begin = stream->tellg();
				frame_index_block.dataPtr = (DWORD)data_begin;
				*stream << *cur_frame;
				auto data_end = stream->tellg();
				frame_index_block.dataSize = (DWORD)(data_end - data_begin);

				if (write_coords)
					*stream << cur_frame->coords;
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

	IRBFile::IRBFile(const std::string & file_name) : IRBFile()
	{
		stream_ptr_t file = std::make_shared<std::fstream>();
		file->open(file_name.c_str(), std::ios::in | std::ios::out | std::ios::binary);
		if (file->rdstate() == std::ios::failbit)
		{
			auto result = ::GetLastError();
			throw irb_file_exception(result, "Can't open irb frames stream.");
		}
		_p_impl->set_stream(file);
		_p_impl->stream_name = file_name;
		_p_impl->read_header();
		_p_impl->read_index_blocks();

	}

	IRBFile::IRBFile(stream_ptr_t & stream, const std::string &stream_name) : IRBFile()
	{
		_p_impl->set_stream(stream);
		_p_impl->stream_name = stream_name;
		_p_impl->read_header();
		_p_impl->read_index_blocks();
	}

	IRBFile::~IRBFile() = default;

	irb_frame_ptr_t IRBFile::read_frame_by_id(frame_id_t id)
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

	void IRBFile::write_frame_by_id(frame_id_t id, const IRBFrame & frame)
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

	const char * IRBFile::file_name()
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

	//template<class T>
	//void IRBFile::write_block_data(const irb_block_info_t& block_info, const T& block_data)
	//{
	//	_p_impl->write_block_data<T>(block_info, block_data);
	//}

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
