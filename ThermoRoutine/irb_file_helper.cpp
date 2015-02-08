#include "irb_file_helper.h"
#include <algorithm>
#include <common\fs_helpers.h>
#include <fstream>

//uint32_t get_frame_coordinate_type_offset();
//uint32_t get_frame_time_offset();


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
		DWORD Type;	// тип индекса
		DWORD version;	// версия (всегда 100)
		DWORD indexID;	// номер индекса
		DWORD dataPtr;	// абсолютное смещение данных, описываемых индексом
		DWORD dataSize;	// размер данных в байтах
		DWORD parent;	// 
		DWORD objectNr;	// не используем
		DWORD chksum;	// контрольная  сумма
		ULONG64 dataKey1; // ключ данных. например - координата фрейма
		ULONG64 dataKey2; // ключ данных. например - время фрейма
	};

#pragma pack(pop)

	enum class index_block_type
	{
		irb_frame = 1,
		irb_spec = 4
	};


	stream_ptr_t create_irb_file(
		const std::string& name,
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
		stream->write(reinterpret_cast<const char*>(&hdr), sizeof(IRBHeader));

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

	template<typename TKey>
	using get_key_func_t = TKey(*)(const IRBIndexBlockEx&);

	struct IRBFile::Impl
	{
		//using stream_ptr_t = std::unique_ptr<std::iostream>;

	public:
		Impl() :stream_size(0), frame_pos(0)
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


	public:
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

		}

		void write_header()
		{
			stream->seekg(0, std::ios::beg);
			stream->write(reinterpret_cast<const char*>(&header), sizeof(IRBHeader));
		}

		void read_index_blocks()
		{
			uint32_t number_blocks = 0;
			switch (static_cast<irb_file_version>(header.ffVersion)){
			case irb_file_version::original:
				number_blocks = header.nrAvIndexes;
				break;
			case irb_file_version::patched:
				number_blocks = header.nrAllIndexes;
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

			uint32_t number_frame_blocks = number_blocks;
			for (uint32_t i = 0; i < number_blocks; i++)
			{
				stream->read(reinterpret_cast<char*>(&index_blocks[i]), sizeof(IRBIndexBlock));
				if (stream->rdstate() == std::ios::failbit)
				{
					index_blocks.clear();
					throw irb_file_exception((HRESULT)(stream_size), "Invalid size of the irb frames stream.");
				}
				if (index_blocks[i].Type != static_cast<DWORD>(index_block_type::irb_frame))
				{
					i--; number_frame_blocks--;
				}
			}
			if (number_blocks != number_frame_blocks)
				index_blocks.resize(number_frame_blocks);

			retrieve_frames_keys();

		}

		void retrieve_frames_keys()
		{
			auto cur_pos = stream->tellg();

			auto frame_coord_offset = get_frame_coordinate_type_offset();
			auto frame_time_offset = get_frame_time_offset();
			int pos_distance = frame_coord_offset - frame_time_offset;
			for (auto &index_block : index_blocks)
			{
				index_block.dataKey1 = 0;
				index_block.dataKey2 = 0; 
				if (index_block.dataPtr == 0){
					continue;
				}
		
				stream->seekg(index_block.dataPtr - cur_pos + frame_time_offset, std::ios::cur);
				float frame_time = 0;
				stream->read(reinterpret_cast<char*>(&frame_time), sizeof(frame_time));
				if (stream->rdstate() == std::ios::failbit)
				{
					index_blocks.clear();
					throw irb_file_exception(index_block.dataPtr + frame_time_offset, "Invalid format of the irb frames stream.");
				}
				index_block.dataKey2 = (uint64_t)frame_time;
				stream->seekg(pos_distance, std::ios::cur);
				FrameCoord frame_coord;
				*stream >> frame_coord;
				if (stream->rdstate() == std::ios::failbit)
				{
					index_blocks.clear();
					throw irb_file_exception(index_block.dataPtr + frame_coord_offset, "Invalid format of the irb frames stream.");
				}
				index_block.dataKey1 = frame_coord.coordinate;
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

		IRBFrame * read_frame(frame_id_t id)
		{
			if (index_blocks.size() == 0 || id == 0){
				return nullptr;
			}

			auto begin_index = header.nextIndexID;
			if (id < begin_index || index_blocks.size() <= id - begin_index){
				return nullptr;
			}

			bool is_set_stream_pos = true;
			if (frame_pos == id - 1 && frame_pos != 0)
			{
				auto prev_index_index_block = frame_pos - begin_index;
				auto frame_index_block = index_blocks[prev_index_index_block];
				auto cur_pos = stream->tellg();
			}
	
			if (is_set_stream_pos){
				auto index_index_block = id - begin_index;
				auto frame_index_block = index_blocks[index_index_block];
				stream->seekg(frame_index_block.dataPtr, std::ios::beg);
			}

			frame_pos = id;

			IRBFrame *frame = new IRBFrame();
			*stream >> *frame;
			frame->id = id;
			return frame;
		}

		IRBFrame * read_frame_by_key1(DWORD64 key)
		{
			if (index_blocks.empty()){
				return nullptr;
			}

			for (auto & index_block : index_blocks)
			{
				if (index_block.dataKey1 == key)
				{
					stream->seekg(index_block.dataPtr, std::ios::beg);
					IRBFrame *frame = new IRBFrame();
					*stream >> *frame;
					frame->id = index_block.indexID;
					return frame;
				}
			}

			return nullptr;
		}

		IRBFrame * read_frame_by_key2(DWORD64 key)
		{
			if (index_blocks.empty()){
				return nullptr;
			}

			for (auto & index_block : index_blocks)
			{
				if (index_block.dataKey2 == key)
				{
					stream->seekg(index_block.dataPtr, std::ios::beg);
					IRBFrame *frame = new IRBFrame();
					*stream >> *frame;
					frame->id = index_block.indexID;
					return frame;
				}
			}

			return nullptr;
		}

		void write_frame(frame_id_t id, const IRBFrame & frame)
		{
			if (index_blocks.size() == 0 || id == 0){
				return;
			}

			auto begin_index = header.nextIndexID;
			auto count_index_blocks = index_blocks.size();
			if (count_index_blocks >= id - begin_index)
			{
				if (header.nrAllIndexes <= id - begin_index){
					return;
				}
				index_blocks.resize(id - begin_index + 1);
			}

			auto index_index_block = id - begin_index;
			auto frame_index_block = index_blocks[index_index_block];
			frame_index_block.indexID = id;
			frame_index_block.Type = static_cast<DWORD>(index_block_type::irb_frame);
			frame_index_block.version = 100;
			frame_index_block.dataKey1 = frame.coords.coordinate;
			frame_index_block.dataKey2 = (uint64_t)frame.header.presentation.imgMilliSecTime;

			auto seek_dir = std::ios::end;

			if (frame_index_block.dataPtr != 0)
			{
				auto data_size = serialized_irb_frame_size(frame);
				if (frame_index_block.dataSize < data_size)
				{
					frame_index_block.dataSize = 0;
					frame_index_block.dataPtr = 0;
				}
				else
				{
					seek_dir = std::ios::beg;
				}
			}

			stream->seekg(frame_index_block.dataPtr, seek_dir);

			auto data_begin = stream->tellg();

			frame_index_block.dataPtr = (DWORD)data_begin;
			*stream << frame;

			auto data_end = stream->tellg();

			frame_index_block.dataSize = (DWORD)(data_end - data_begin);

			write_index_block(index_index_block);

			bool is_need_write_header = false;
			if (index_blocks.size() > header.nrAvIndexes)
			{
				header.nrAvIndexes = (DWORD)index_blocks.size();
				is_need_write_header = true;
			}
			if (index_index_block == 0)
			{
				header.nextIndexID = id;
				is_need_write_header = true;
			}


			if (is_need_write_header){
				write_header();
			}
		}

		void append_frames(const std::vector<irb_frame_shared_ptr_t> & frames)
		{
			if (index_blocks.size() == 0 ){
				return;
			}
			if (frames.size() == 0){
				return;
			}

			if (frames.size() > index_blocks.size()){
				return;
			}


			auto result_size_frames = frames.size();

			auto begin_index = 0;
			auto count_index_blocks = index_blocks.size();
			if (header.nrAvIndexes < result_size_frames){
				return;
			}

			//index_blocks.resize(result_size_frames);

			stream->seekg(0, std::ios::end);

			for (unsigned int i = begin_index, j = 0; i < result_size_frames + begin_index; i++, j++)
			{
				auto &cur_frame = frames.at(j);
				auto &frame_index_block = index_blocks.at(i);
				frame_index_block.indexID = cur_frame->getFrameNum();
				frame_index_block.Type = static_cast<DWORD>(index_block_type::irb_frame);
				frame_index_block.version = 100;
				
				frame_index_block.dataKey1 = cur_frame->coords.coordinate;
				frame_index_block.dataKey2 = (uint64_t)cur_frame->header.presentation.imgMilliSecTime;

				auto data_begin = stream->tellg();
				frame_index_block.dataPtr = (DWORD)data_begin;
				*stream << *cur_frame;
				auto data_end = stream->tellg();
				frame_index_block.dataSize = (DWORD)(data_end - data_begin - 1);

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

	irb_frame_ptr_t IRBFile::read_frame(frame_id_t id)
	{
		return irb_frame_ptr_t(_p_impl->read_frame(id));
	}
	irb_frame_ptr_t IRBFile::read_frame_by_coordinate(coordinate_t coordinate)
	{
		return irb_frame_ptr_t(_p_impl->read_frame_by_key1(coordinate));
	}
	irb_frame_ptr_t IRBFile::read_frame_by_time(time_t time)
	{
		return irb_frame_ptr_t(_p_impl->read_frame_by_key2(time));
	}

	void IRBFile::write_frame(frame_id_t id, const IRBFrame & frame)
	{
		_p_impl->write_frame(id, frame);
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
		return _p_impl->get_frames_data_span<irb_frames_time_span_t, uint64_t>(FIELD_OFFSET(IRBIndexBlockEx, dataKey2));
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
		return _p_impl->get_frames_key_data_list<irb_frames_time_list_t, uint64_t>(FIELD_OFFSET(IRBIndexBlockEx, dataKey2));
	}


}
