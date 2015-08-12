#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include "irb_frame_helper.h"
#include <common\handle_holder.h>
#include <iostream>
#include "irb_frame_spec_info.h"

namespace irb_file_helper
{

	class irb_file_exception : public std::runtime_error
	{
	public:
		irb_file_exception(HRESULT error_code, const std::string & message);
		const char * what() const override;
		HRESULT get_error_code() const;
	private:
		std::string _message;
		HRESULT _error_code;
	};

	enum class irb_file_version
	{
		original = 100,
		patched	 = 200
	};

	using stream_ptr_t = std::shared_ptr<std::fstream>;

	const auto default_frames_per_file = 1200UL;

	struct stream_spec_info_t {
		void *p_data;
		uint16_t size; // bytes
	};

	stream_ptr_t 
	create_irb_file(
		const std::string& name, 
		irb_file_version file_version = irb_file_version::original,
		unsigned int max_frames_per_file = default_frames_per_file
	);

	stream_ptr_t
		create_irb_file(
		const std::string& name,
		const stream_spec_info_t & info,
		irb_file_version file_version = irb_file_version::original,
		unsigned int max_frames_per_file = default_frames_per_file
		);



	struct irb_block_info_t
	{
		WORD Type;	// тип 
		WORD subType;	// подтип
		DWORD version;	// версия
		DWORD indexID;	// номер блока данных
	};


	using irb_frames_key_span_t = std::pair<irb_frame_helper::irb_frame_key, irb_frame_helper::irb_frame_key>;
	using irb_frames_key_list_t = std::vector<irb_frame_helper::irb_frame_key>;


	using irb_frames_coordinate_span_t = std::pair<uint64_t, uint64_t>;
	using irb_frames_coordinate_list_t = std::vector<uint64_t>;

	using irb_frames_time_span_t = std::pair<double, double>;
	using irb_frames_time_list_t = std::vector<double>;

	using irb_frames_span_t = std::pair<unsigned int, unsigned int>;
	using irb_frames_id_list_t = std::vector<unsigned int>;

	using namespace irb_frame_helper;
	using irb_frame_shared_ptr_t = std::shared_ptr<IRBFrame>;
	class IRBFile final
	{
	protected:
		IRBFile();
	public:
		IRBFile(const std::string & file_name);
		IRBFile(stream_ptr_t & stream, const std::string &stream_name = "");
		~IRBFile();

	private:
		struct Impl;
		std::unique_ptr<Impl> _p_impl;

	public:

		bool get_stream_spec_info(stream_spec_info_t & info) const;
		void write_stream_spec_info(const stream_spec_info_t & info) const;


		irb_frame_ptr_t read_frame_by_id(frame_id_t id);
		irb_frame_ptr_t read_frame_by_index(uint32_t index);
		irb_frame_ptr_t read_frame_by_coordinate(coordinate_t coordinate);
		irb_frame_ptr_t read_frame_by_time(double time);
		void write_frame_by_id(frame_id_t id, const IRBFrame & frame);
		void write_frame_by_index(uint32_t index, const IRBFrame & frame);
		void append_frames(const std::vector<irb_frame_shared_ptr_t> & frames);
		unsigned int count_frames();

		const char * file_name();

		void write_block_data(const irb_block_info_t& block_info, const irb_frame_helper::IRBFrame& block_data);
		void write_block_data(const irb_block_info_t& block_info, const irb_frame_spec_info::irb_frame_spec_info& block_data);

	public:

		irb_frames_key_span_t get_frames_key_span();
		irb_frames_key_list_t get_frames_key_list();


		irb_frames_span_t get_frames_id_span();
		irb_frames_id_list_t get_frames_id_list();

		irb_frames_coordinate_span_t get_frames_coordinate_span();
		irb_frames_coordinate_list_t get_frames_coordinate_list();

		irb_frames_time_span_t get_frames_time_span();
		irb_frames_time_list_t get_frames_time_list();


	};

	typedef int32_t  camera_offset_t;

	stream_ptr_t
		create_irb_file(
		const std::string& name,
		camera_offset_t camera_offset,
		irb_file_version file_version = irb_file_version::original,
		unsigned int max_frames_per_file = default_frames_per_file
		);


	camera_offset_t read_camera_offset_from_file(const IRBFile& file);
	void write_camera_offset_to_file(const IRBFile& file, camera_offset_t offset);


}