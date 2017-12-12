#include "pd_settings.h"
#include <common\pugixml.hpp>
#include <common\fs_utils.h>

#include <fstream>

#include <error_lib\win32_error.h>

#include <loglib\log.h>

namespace position_detector
{
	namespace settings
	{
		auto const config_file_root_tag = "pd_settings";

		auto const pd_connection_tag = "pd_connection";
		auto const pd_events_connection_tag = "pd_events_connection";
		auto const ip_key = "ip";
		auto const i_ip_key = "i_ip";
		auto const port_key = "port";

		auto const pd_settings_tag = "pd";
		auto const counter_size_key = "counter_size";

		auto const max_config_file_size = 1024 * 1024;


		template <typename xml_item_type_t>
		bool check_xml_item_not_exist(const xml_item_type_t &item)
		{
			return item.empty() ? true : false;
		}

#define CHECK_XML_ITEM(item) { if(check_xml_item_not_exist(item)) return {}; }

		settings_t read_settings(const std::wstring &full_config_path)
		{
			settings_t tmp_settings{};
			pugi::xml_document doc;

			if (fs_utils::get_file_size(full_config_path) > max_config_file_size)
			{
				return{};
			}

			pugi::xml_parse_result parse_result = doc.load_file(full_config_path.c_str());

			if (!parse_result)
			{
				LOG_ERROR() << parse_result.description() << L",status: " << std::to_wstring(parse_result.status)
							<< L",offset: " << std::to_wstring(parse_result.offset);
				return{};
			}

			pugi::xml_node node_pd_settings = doc.child(config_file_root_tag);
			CHECK_XML_ITEM(node_pd_settings);

			pugi::xml_node node_pd_connection = node_pd_settings.child(pd_connection_tag);
			CHECK_XML_ITEM(node_pd_connection);

			pugi::xml_node node_pd_events_connection = node_pd_settings.child(pd_events_connection_tag);
			CHECK_XML_ITEM(node_pd_events_connection);

			pugi::xml_node node_pd = node_pd_settings.child(pd_settings_tag);
			CHECK_XML_ITEM(node_pd);

			CHECK_XML_ITEM(node_pd_connection.attribute(ip_key));
			CHECK_XML_ITEM(node_pd_connection.attribute(i_ip_key));
			CHECK_XML_ITEM(node_pd_connection.attribute(port_key));

			CHECK_XML_ITEM(node_pd_events_connection.attribute(ip_key));
			CHECK_XML_ITEM(node_pd_events_connection.attribute(i_ip_key));
			CHECK_XML_ITEM(node_pd_events_connection.attribute(port_key));

			CHECK_XML_ITEM(node_pd.attribute(counter_size_key));

			tmp_settings.pd_address.ip = node_pd_connection.attribute(ip_key).value();
			tmp_settings.pd_address.i_ip = node_pd_connection.attribute(i_ip_key).value();
			tmp_settings.pd_address.port = static_cast<unsigned short>(node_pd_connection.attribute(port_key).as_uint());

			tmp_settings.pd_events_address.ip = node_pd_events_connection.attribute(ip_key).value();
			tmp_settings.pd_events_address.i_ip = node_pd_events_connection.attribute(i_ip_key).value();
			tmp_settings.pd_events_address.port = static_cast<unsigned short>(node_pd_events_connection.attribute(port_key).as_uint());

			tmp_settings.counter_size = static_cast<decltype(tmp_settings.counter_size)>(node_pd.attribute(counter_size_key).as_uint());

			return tmp_settings;
		}

		//void reset_settings(logger::settings&& settings)
		//{
		//	settings.validate([](const std::wstring & param_name, const std::wstring & original_value, const std::wstring &)
		//	{
		//		LOG_AND_THROW(std::invalid_argument("Invalid logging settings were passed.")) << L"Parameter '" << param_name << L"' has invalid value = " << original_value;
		//	});

		//	write_logger_settings(g_instance.get()->get_full_config_path(), settings);

		//	if (!g_instance.get()->is_watch_settings_changes())
		//		g_instance.get()->reload_settings_from_config_file();
		//}

		void write_settings(const std::wstring &full_file_path, const settings_t &settings)
		{
			pugi::xml_document doc;
			pugi::xml_node node_pd_settings = doc.append_child(config_file_root_tag);
			pugi::xml_node node_pd_connection = node_pd_settings.append_child(pd_connection_tag);
			node_pd_connection.append_attribute(ip_key) = settings.pd_address.ip.c_str();
			node_pd_connection.append_attribute(i_ip_key) = settings.pd_address.i_ip.c_str();
			node_pd_connection.append_attribute(port_key) = settings.pd_address.port;

			pugi::xml_node node_pd_events_connection = node_pd_settings.append_child(pd_events_connection_tag);
			node_pd_events_connection.append_attribute(ip_key) = settings.pd_events_address.ip.c_str();
			node_pd_events_connection.append_attribute(i_ip_key) = settings.pd_events_address.i_ip.c_str();
			node_pd_events_connection.append_attribute(port_key) = settings.pd_events_address.port;

			pugi::xml_node node_pd = node_pd_settings.append_child(pd_settings_tag);
			node_pd.append_attribute(counter_size_key) = settings.counter_size;


			std::ofstream file_stream(full_file_path, std::ios::app);
			if (file_stream.rdstate() == std::ios::failbit)
			{
				LOG_ERROR() << L"Failed open the file for write setting. File name: " << full_file_path << L"Error: "<< win32::get_last_error_code().message();
				return;
			}

			doc.save(file_stream, "\t", pugi::format_no_declaration | pugi::format_indent);
		}
	}
}//namespace position_detector
