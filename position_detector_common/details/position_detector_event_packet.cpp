#include "position_detector_packet.h"
#include "position_detector_packet_details.h"
#include <pugixml.hpp>

#include <loglib\log.h>
#include <Windows.h>

#include <vector>
#include <map>
#include <locale>
#include <regex>
#include <common\string_utils.h>


namespace position_detector
{
	namespace events
	{
		#define EVENT_CODE_PAGE	1251
		#define CONVERT_TO_UTF16_FROM_ANSI(_ansi_str) string_utils::ConvertToUTF16((_ansi_str),EVENT_CODE_PAGE)
		#define MAX_NAME_LENGTH_CB  64
		#define CONVERT_TO_UTF8(_utf16_str)  string_utils::convert_wchar_to_utf8((_utf16_str))
		#define CONVERT_TO_UTF16(_utf8_str)  string_utils::convert_utf8_to_wchar((_utf8_str))

		#define EVENT_FRAME_ROOT_NAME "/event"
		template<typename TEvent>
		pugi::xml_node & operator >> (pugi::xml_node & node, TEvent &);
		template<> pugi::xml_node & operator >> (pugi::xml_node &, event_packet &);

		template<typename event_packet_t>
		event_packet_ptr_t create_event_packet_obj(pugi::xml_node &);

		using create_event_packet_obj_func_t = event_packet_ptr_t(*)(pugi::xml_node &);

		static std::map<std::string, create_event_packet_obj_func_t> g_map_event_type_to_create_obj_func =
		{
			{ "StartCommandEvent", &create_event_packet_obj<StartCommandEvent_packet> },
			{ "CoordinateCorrected", &create_event_packet_obj<CoordinateCorrected_packet> },
			{ "PassportChangedEvent", &create_event_packet_obj<PassportChangedEvent_packet> },
			{ "ReverseEvent", &create_event_packet_obj<ReverseEvent_packet> },
			{ "StopCommandEvent", &create_event_packet_obj<StopCommandEvent_packet> }
		};

		static std::map<std::string, event_type> g_map_event_type_name_to_event_type =
		{
			{ "StartCommandEvent", event_type::EvStartCommandEvent },
			{ "CoordinateCorrected", event_type::EvCoordinateCorrected },
			{ "PassportChangedEvent", event_type::EvPassportChangedEvent },
			{ "ReverseEvent", event_type::EvReverseEvent },
			{ "StopCommandEvent", event_type::EvStopCommandEvent }
		};

		pugi::xml_node get_first_event_node(const pugi::xml_document & packet);

	}
	using namespace events;
		template<>
		event_packet_ptr_t parce_packet_from_message(const BYTE * message, unsigned int message_size)
		{
			LOG_STACK();

			if (message == nullptr)
				throw std::invalid_argument("The passed argument message can't be equal to nullptr");

			pugi::xml_document doc;

			pugi::xml_parse_result result = doc.load_buffer(message, message_size, pugi::parse_default, pugi::encoding_utf8);

			if (!result){
				std::ostringstream ss;
				ss << "Could not parse message. Error: " << result.description();

				throw deserialization_error(ss.str().c_str());
			}

			auto event_type_node = get_first_event_node(doc);

			if (event_type_node.empty()){
				throw deserialization_error("Could not find event type node.");
			}

			auto event_type_attribute = event_type_node.attribute("type");

			std::string event_type_name(event_type_attribute.value());
			auto event_type = event_type_node.child(event_type_name.c_str());
			if (event_type.empty())
			{
				//kostil
				auto event_type_name_hack = event_type_name + "Event";
				event_type = event_type_node.child(event_type_name_hack.c_str());
				if (event_type.empty())
				{
					throw deserialization_error(std::string(("Could not find the event node with type: ") + std::string(event_type_name)).c_str());
				}
			}

			auto map_iter = g_map_event_type_to_create_obj_func.find(event_type_name);
			if (map_iter == g_map_event_type_to_create_obj_func.end()){
				std::ostringstream ss;
				ss << "Could not find create object function for event type: " << event_type_name;

				throw deserialization_error(ss.str().c_str());
			}

			auto event_message = map_iter->second(event_type);

			if (event_message)
			{
				event_raw_data_t raw_data(message_size);
				std::copy_n(message, message_size, raw_data.begin());
				event_message->event_raw_data = std::move(raw_data);
			}

			return event_message;
		}

		namespace events
		{
			pugi::xml_node get_first_event_node(const pugi::xml_document & packet)
			{
				pugi::xpath_node event_node = packet.select_single_node(EVENT_FRAME_ROOT_NAME);
				return event_node.node();
			}

			template<typename event_packet_t>
			event_packet_ptr_t create_event_packet_obj(pugi::xml_node & node)
			{
				event_packet_ptr_t event_packet_obj(std::make_unique<event_packet_t>());

				node >> *(static_cast<event_packet_t*>(event_packet_obj.get()));

				return event_packet_obj;
			}


			enum token_index {
				TOKEN_ALL = 0,
				TOKEN_YEAR = 1,
				TOKEN_MONTH = 2,
				TOKEN_DAY = 3,
				TOKEN_HOUR = 4,
				TOKEN_MINUTE = 5,
				TOKEN_SECOND = 6,
				TOKEN_MILLISEC = 7,
				TOKEN_TZ_HOUR = 8,
				TOKEN_TZ_MINUTE = 9
			};

			static const unsigned int g_date_time_expression_minimum_size = 21; /* YYYY-MM-DDThh:mm:ss.ms[+hh:mm]*/
			static const unsigned int g_date_time_tokens_minimum_count = 7 + 1; /* (YYYY)-(MM)-(DD)T(hh):(mm):(ss).(ms)[+(hh):(mm)]*/

			static const char g_date_time_reg_exp[] = "^([0-9]{4})-(0[1-9]|1[012])-(0[1-9]|1[0-9]|2[0-9]|3[01])T(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])\\.([0-9]+)\\+(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])$";

			void parse_date_time_token(const std::string& token, unsigned int & result)
			{
				try{
					result = std::stoul(token);
				}
				catch (const std::invalid_argument&)
				{
					std::ostringstream ss;
					ss << "Could not parse data time item: " << token;

					throw deserialization_error(ss.str().c_str());
				}
				catch (const std::out_of_range&)
				{
					std::ostringstream ss;
					ss << "Could not parse data time item: " << token;

					throw deserialization_error(ss.str().c_str());
				}

			}

#define _1mksec 1000000ULL
#define _1min 60ULL
#define _1hour 3600ULL

			void parse_date_time_expression(const std::string& str, datetime & data_time)
			{
				LOG_STACK();

				if (str.size() < g_date_time_expression_minimum_size)
				{
					std::ostringstream ss;
					ss << "Date-time expression is too short: " << str.size();
					throw deserialization_error(ss.str().c_str());
				}

				std::vector<std::string> tokens;

				std::regex tokens_regex(g_date_time_reg_exp);

				const auto begin = std::sregex_iterator(std::cbegin(str), std::cend(str), tokens_regex);
				const auto end = std::sregex_iterator();
				auto count_tokens = begin->size();

				if (count_tokens < g_date_time_tokens_minimum_count)
				{
					std::ostringstream ss;
					ss << "Date-time expression is mismatch: " << str;
					throw deserialization_error(ss.str().c_str());
				}

				for (auto i = begin->begin(); i != begin->end(); ++i)
				{
					auto token = i->str();
					tokens.push_back(std::move(token));
				}

				parse_date_time_token(tokens[TOKEN_YEAR], data_time.year);
				parse_date_time_token(tokens[TOKEN_MONTH], data_time.month);
				parse_date_time_token(tokens[TOKEN_DAY], data_time.day);
				parse_date_time_token(tokens[TOKEN_HOUR], data_time.hour);
				parse_date_time_token(tokens[TOKEN_MINUTE], data_time.minute);
				parse_date_time_token(tokens[TOKEN_SECOND], data_time.second);
				parse_date_time_token(tokens[TOKEN_MILLISEC], data_time.millisecond);
				data_time.tzd = 0;

				if (count_tokens == g_date_time_tokens_minimum_count + 2){
					unsigned int tz_hour, tz_minute;
					parse_date_time_token(tokens[TOKEN_TZ_HOUR], tz_hour);
					parse_date_time_token(tokens[TOKEN_TZ_MINUTE], tz_minute);
					data_time.tzd = (short)(tz_hour * 60 + tz_minute);
				}
				auto count_numbers = tokens[TOKEN_MILLISEC].size();
				double second_part = data_time.millisecond / (std::pow(10,count_numbers));
				ULONG64 usec = (ULONG64)(second_part * _1mksec);
				data_time.timestamp = usec + (data_time.hour*_1hour + data_time.minute*_1min + data_time.second) * _1mksec;
			}

			std::string get_attribute_value(pugi::xml_node & node, const char * attribute_name)
			{
				auto attribute = node.attribute(attribute_name);
				auto attribute_value = attribute.value();
				if (attribute.empty()){
					std::string error("Could not find attribute in the event type node. Attribute name: '" + std::string(attribute_name) + "'");
					throw deserialization_error(error.c_str());
				}

				return attribute_value;
			}

			std::wstring get_utf16_string(const std::string& source)
			{
				std::wstring res;
				bool is_ansi_str = false;
				try{
					res = CONVERT_TO_UTF16(source);
				}
				catch (const std::range_error&)
				{
					is_ansi_str = true;
				}
				if (is_ansi_str){
					res = CONVERT_TO_UTF16_FROM_ANSI(source);
				}

				return res;
			}

			template<> pugi::xml_node & operator >> (pugi::xml_node & node, event_packet &evt_packet)
			{
				try{
					evt_packet.counter = std::stoul(get_attribute_value(node, "counter"));
				}
				catch (const std::invalid_argument&)
				{
					std::ostringstream ss;
					ss << "Could not parse Counter item: " << node.text();

					throw deserialization_error(ss.str().c_str());
				}
				catch (const std::out_of_range&)
				{
					std::ostringstream ss;
					ss << "Could not parse Counter item: " << node.text();

					throw deserialization_error(ss.str().c_str());
				}

				evt_packet.dataTime = get_attribute_value(node, "creationDateTime");
				evt_packet.guid = get_attribute_value(node, "id");
				evt_packet.source = get_utf16_string(get_attribute_value(node, "source"));


				parse_date_time_expression(evt_packet.dataTime, evt_packet.data_time);
				return node;
			}


			template<>
			pugi::xml_node & operator >> (pugi::xml_node & node, event_item_t & packet_item)
			{
				try{
					auto child_node(node.child("Key"));
					packet_item.key = get_utf16_string(child_node.child_value());
					child_node = node.child("Name");
					packet_item.name = get_utf16_string(child_node.child_value());
					child_node = node.child("Code");
					packet_item.code = get_utf16_string(child_node.child_value());
				}
				catch (const std::invalid_argument&)
				{
					std::ostringstream ss;
					ss << "Could not parse Event item: " << node.text();

					throw deserialization_error(ss.str().c_str());
				}
				catch (const std::out_of_range&)
				{
					std::ostringstream ss;
					ss << "Could not parse Event item: " << node.text();

					throw deserialization_error(ss.str().c_str());
				}


				return node;
			}


			template<>
			pugi::xml_node & operator >> (pugi::xml_node & node, coordinate_item_t & packet_item)
			{
				try{
					packet_item.km = std::stol(get_attribute_value(node, "km"));
					packet_item.m = std::stol(get_attribute_value(node, "m"));
					packet_item.mm = std::stol(get_attribute_value(node, "mm"));
				}
				catch (const std::invalid_argument&)
				{
					std::ostringstream ss;
					ss << "Could not parse Coordinate item: " << node.text();

					throw deserialization_error(ss.str().c_str());
				}
				catch (const std::out_of_range&)
				{
					std::ostringstream ss;
					ss << "Could not parse Coordinate item: " << node.text();

					throw deserialization_error(ss.str().c_str());
				}

				return node;
			}

			template<>
			pugi::xml_node & operator >> (pugi::xml_node & node, way_direction_item_t & packet_item)
			{
				try{
					packet_item.id = get_utf16_string(get_attribute_value(node, "id"));
					packet_item.naturalId = get_utf16_string(get_attribute_value(node, "naturalId"));
					packet_item.name = get_utf16_string(get_attribute_value(node, "name"));
					packet_item.kind = get_utf16_string(get_attribute_value(node, "kind"));

					if (packet_item.kind == L"Station"){
						packet_item.direction_code = get_utf16_string(get_attribute_value(node, "stationCode"));
						packet_item.direction_name = get_utf16_string(get_attribute_value(node, "stationName"));
					}
					else{
						if (packet_item.kind == L"Main"){
							packet_item.direction_code = get_utf16_string(get_attribute_value(node, "directionCode"));
							packet_item.direction_name = get_utf16_string(get_attribute_value(node, "directionName"));
						}
					}

				}
				catch (const std::invalid_argument&)
				{
					std::ostringstream ss;
					ss << "Could not parse Way direction item: " << node.text();

					throw deserialization_error(ss.str().c_str());
				}
				catch (const std::out_of_range&)
				{
					std::ostringstream ss;
					ss << "Could not parse Way direction item: " << node.text();

					throw std::out_of_range("");
				}

				return node;
			}

			typedef struct _nonstandard_km {
				int km;
				int length;
			}nonstandard_km_t;


			void split(std::vector<std::string>& dest, const char* str, const char* delim)
			{
				auto pTempStr = _strdup(str);
				char *next_token = nullptr;
				auto pWord = strtok_s(pTempStr, delim, &next_token);
				while (pWord != nullptr)
				{
					dest.push_back(pWord);
					pWord = strtok_s(nullptr, delim, &next_token);
				}

				free(pTempStr);
			}


			bool parse_nonstandard_km(const std::string& str, nonstandard_km_t & nonstandard_km)
			{
				static unsigned int min_non_standart_km_item_size = 3;

				if (str.size() < min_non_standart_km_item_size){

					std::ostringstream ss;
					ss << "nonstandard km expression is to short: " << str;
					throw deserialization_error(ss.str().c_str());
				}

				bool positive = true;
				auto item_str = const_cast<char*>(str.c_str());
				if (item_str[0] == '-' || (item_str[0] == '0' && item_str[1] == '0')){
					positive = false;
					item_str += 1;
				}

				std::vector<std::string> result;
				split(result, item_str, "-");

				if (result.size() != 2)
				{
					std::ostringstream ss;
					ss << "nonstandard km expression is mismatch: " << str;
					throw deserialization_error(ss.str().c_str());
				}

				try{
					nonstandard_km.km = std::stol(result[0]);
					nonstandard_km.length = std::stoul(result[1]);
				}
				catch (const std::invalid_argument&)
				{
					std::ostringstream ss;
					ss << "nonstandard km expression is mismatch: " << str;
					throw deserialization_error(ss.str().c_str());
				}
				catch (const std::out_of_range&)
				{
					std::ostringstream ss;
					ss << "nonstandard km expression is mismatch: " << str;
					throw deserialization_error(ss.str().c_str());
				}

				return positive;

			}

			std::vector<std::string> get_nonstandard_km_items(const std::string& str)
			{
				std::vector<std::string> result;
				split(result, str.c_str(), ";");
				return result;
			}

			void parse_nonstandard_kms_item(const std::string& str, nonstandard_kms_item_t & nonstandard_kms)
			{
				LOG_STACK();

				auto nonstandart_km_items = get_nonstandard_km_items(str);
	
				for (const auto & item : nonstandart_km_items)
				{
					nonstandard_km_t nonstandard_km;
					if (parse_nonstandard_km(item, nonstandard_km)){
						nonstandard_kms.positive_kms.emplace(nonstandard_km.km, nonstandard_km.length);
					}
					else{
						nonstandard_kms.negative_kms.emplace(nonstandard_km.km, nonstandard_km.length);
					}
				}
			}

			template<>
			pugi::xml_node & operator >> (pugi::xml_node & node, nonstandard_kms_item_t & packet_item)
			{
				parse_nonstandard_kms_item(node.child_value(), packet_item);
				return node;
			}


			template<>
			pugi::xml_node & operator >> (pugi::xml_node & node, position_item_t & packet_item)
			{
				auto child_node(node.child("Railway"));
				child_node >> packet_item.railway_item;
				child_node = node.child("Direction");
				child_node >> packet_item.direction_item;
				child_node = node.child("Coordinate");
				child_node >> packet_item.coordinate_item;
				child_node = node.child("WayDto");
				child_node >> packet_item.way_direction_item;

				return node;
			}

			template<>
			pugi::xml_node & operator >> (pugi::xml_node & node, track_settings_item_t & packet_item)
			{
				packet_item.name = get_utf16_string(get_attribute_value(node, "name"));
				packet_item.orientation = get_attribute_value(node, "orientation");
				packet_item.movement_direction = get_attribute_value(node, "movementDirection");

				auto child_node(node.child("UserStart"));
				child_node >> packet_item.user_start_item;
				child_node = node.child("UserEnd");
				child_node >> packet_item.user_end_item;

				child_node = node.child("Kms");
				child_node >> packet_item.kms;

				return node;
			}


			template<>
			pugi::xml_node & operator >> (pugi::xml_node & node, change_passport_point_direction_item_t & packet_item)
			{
				auto child_node(node.child("Start"));
				child_node >> packet_item.start_item;

				return node;
			}


			template<>
			pugi::xml_node & operator >> (pugi::xml_node & node, correct_direction_item_t & packet_item)
			{
				try{
					packet_item.counter = std::stoul(get_attribute_value(node, "Counter"));
					packet_item.direction = get_attribute_value(node, "Direction");
					packet_item.type = get_attribute_value(node, "Type");
					packet_item.coordinate_item.km = std::stoul(get_attribute_value(node, "Km"));
					packet_item.coordinate_item.m = std::stoul(get_attribute_value(node, "M"));
					packet_item.coordinate_item.mm = 0;
				}
				catch (const std::invalid_argument&)
				{
					std::ostringstream ss;
					ss << "Could not parse correct direction item: " << node.text();

					throw deserialization_error(ss.str().c_str());
				}
				catch (const std::out_of_range&)
				{
					std::ostringstream ss;
					ss << "Could not parse correct direction item: " << node.text();

					throw deserialization_error(ss.str().c_str());
				}
				return node;
			}

			template<> pugi::xml_node & operator >> (pugi::xml_node & node, CoordinateCorrected_packet &evt_packet)
			{
				operator >> <event_packet>(node, evt_packet);
				auto child_node(node.child("CorrectionDto"));
				child_node >> evt_packet.correct_direction;

				return node;
			}

			template<> pugi::xml_node & operator >> (pugi::xml_node & node, StartCommandEvent_packet &evt_packet)
			{
				operator >> <event_packet>(node, evt_packet);
				auto child_node(node.child("TrackSettings"));
				child_node >> evt_packet.track_settings;

				return node;
			}

			template<> pugi::xml_node & operator >> (pugi::xml_node & node, PassportChangedEvent_packet &evt_packet)
			{
				operator >> <event_packet>(node, evt_packet);

				auto child_node(node.child("RouteMovement"));
				evt_packet.direction =child_node.child_value();

				child_node = node.child("ChangePassportPointDto");
				child_node >> evt_packet.change_passport_point_direction;

				child_node = node.child("Kms");
				child_node >> evt_packet.change_passport_point_direction.kms;

				return node;
			}

			template<> pugi::xml_node & operator >> (pugi::xml_node & node, ReverseEvent_packet &evt_packet)
			{
				operator >> <event_packet>(node, evt_packet);

				auto child_node(node.child("IsReverse"));

				evt_packet.is_reverse = false;
				if (std::string(child_node.child_value()) == "true")
					evt_packet.is_reverse = true;

				child_node = node.child("IsAuto");
				evt_packet.is_auto = false;
				if (std::string(child_node.child_value()) == "true")
					evt_packet.is_auto = true;

				child_node = node.child("Orientation");
				evt_packet.orientation = child_node.child_value();
				return node;
			}
			template<> pugi::xml_node & operator >> (pugi::xml_node & node, StopCommandEvent_packet &evt_packet)
			{
				operator >> <event_packet>(node, evt_packet);

				auto child_node(node.child("Timeout"));
				try{
					evt_packet.timeout = std::stoul(child_node.child_value());
				}
				catch (const std::invalid_argument&)
				{
					evt_packet.timeout = 0;
				}
				catch (const std::out_of_range&)
				{
					evt_packet.timeout = 0;
				}


				return node;
			}

			std::wostream & operator << (std::wostream & out, const event_packet & data)
			{
				out << data.event_name << std::wstring(L" packet data: ");

				out << std::wstring(L"Counter: ") << data.counter
					<< std::wstring(L" Guid: ") << data.guid.c_str()
					<< std::wstring(L" DataTime: ") << data.dataTime.c_str()
					<< std::wstring(L" Source: ") << data.source;
				return out;
			}

			std::wostream & operator << (std::wostream & out, const coordinate_item_t & data)
			{
				out << std::wstring(L"km: ") << data.km
					<< std::wstring(L" m: ") << data.m
					<< std::wstring(L" mm: ") << data.mm;
				return out;
			}

			std::wostream & operator << (std::wostream & out, const correct_direction_item_t & data)
			{
				out << std::wstring(L"Counter: ") << data.counter
					<< std::wstring(L" direction: ") << data.direction.c_str()
					<< std::wstring(L" coordinate: ") << data.coordinate_item;
				return out;
			}

			std::wostream & operator << (std::wostream & out, const event_item_t & data)
			{
				out << data.name;
				return out;
			}

			std::wostream & operator << (std::wostream & out, const way_direction_item_t & data)
			{
				out << data.name;
				return out;
			}


			std::wostream & operator << (std::wostream & out, const position_item_t & data)
			{
				out << std::wstring(L" Railway: ") << data.railway_item
					<< std::wstring(L" Line: ") << data.direction_item
					<< std::wstring(L" Path: ") << data.way_direction_item
					<< std::wstring(L" coordinate: ") << data.coordinate_item;
				return out;
			}

			
			std::wostream & operator << (std::wostream & out, const nonstandard_kms_item_t & data)
			{
				if (data.negative_kms.empty() && data.positive_kms.empty())
					return out;
				out << std::wstring(L" Non standart kms: ");
				for (const auto & map_item : data.negative_kms)
				{
					out << "-" << map_item.first << std::wstring(L":") << map_item.second << std::wstring(L";");
				}
				for (const auto & map_item : data.positive_kms)
				{
					out << map_item.first << std::wstring(L":") << map_item.second << std::wstring(L";");
				}

				return out;
			}

			std::wostream & operator << (std::wostream & out, const track_settings_item_t & data)
			{
				out << std::wstring(L" Name: ") << data.name
					<< std::wstring(L" direction: ") << data.movement_direction.c_str()
					<< std::wstring(L" orientation: ") << data.orientation.c_str();

				out << data.user_start_item;
				out << data.kms;
				return out;
			}


		}// namespace events

} // namespace position_detector

namespace std
{
	using namespace logger;
	template<typename TEvent>
	const TEvent& event_cast(const ::position_detector::events::event_packet * data)
	{
		return *reinterpret_cast<const TEvent*>(data);
	}

	std::wostream & operator << (std::wostream & out, const ::position_detector::events::event_packet * data)
	{
		if (data == nullptr){
			out << std::wstring(L"nullptr");
			return out;
		}

		switch (data->type){
		case ::position_detector::events::event_type::EvStartCommandEvent:
			return logger::operator <<(out, event_cast<::position_detector::events::StartCommandEvent_packet>(data));
		case ::position_detector::events::event_type::EvCoordinateCorrected:
			return logger::operator <<(out, event_cast<::position_detector::events::CoordinateCorrected_packet>(data));
		case ::position_detector::events::event_type::EvPassportChangedEvent:
			return logger::operator <<(out, event_cast<::position_detector::events::PassportChangedEvent_packet>(data));
		case ::position_detector::events::event_type::EvReverseEvent:
			return logger::operator <<(out, event_cast<::position_detector::events::ReverseEvent_packet>(data));
		case ::position_detector::events::event_type::EvStopCommandEvent:
			return logger::operator <<(out, event_cast<::position_detector::events::StopCommandEvent_packet>(data));
		}

		return out;
	}

}

namespace logger
{

	std::wostream & operator << (std::wostream & out, const ::position_detector::events::CoordinateCorrected_packet & data)
	{
		using namespace ::position_detector::events;

		out << *reinterpret_cast<const event_packet*>(&data);
		out << data.correct_direction;
		return out;
	}

	std::wostream & operator << (std::wostream & out, const ::position_detector::events::StartCommandEvent_packet & data)
	{
		using namespace ::position_detector::events;

		out << *reinterpret_cast<const event_packet*>(&data);

		out << data.track_settings;
		return out;
	}

	std::wostream & operator << (std::wostream & out, const ::position_detector::events::StopCommandEvent_packet & data)
	{
		using namespace ::position_detector::events;

		out << *reinterpret_cast<const event_packet*>(&data);
		return out;
	}

	std::wostream & operator << (std::wostream & out, const ::position_detector::events::ReverseEvent_packet & data)
	{
		using namespace ::position_detector::events;

		out << *reinterpret_cast<const event_packet*>(&data);
		out << std::wstring(L" Orientation: ") << data.orientation.c_str();
		out << std::wstring(L" is reverse: ") << std::boolalpha << data.is_reverse;
		out << std::wstring(L" is auto: ") << std::boolalpha << data.is_auto;
		return out;
	}

	std::wostream & operator << (std::wostream & out, const ::position_detector::events::PassportChangedEvent_packet & data)
	{
		using namespace ::position_detector::events;

		out << *reinterpret_cast<const event_packet*>(&data);

		out << data.change_passport_point_direction.start_item;
		position_detector::events::operator << (out, data.change_passport_point_direction.kms);
//		out << data.change_passport_point_direction.kms;

		return out;
	}

}