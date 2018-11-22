#include "pixels_mask_helper.h"
#include <loglib\log.h>
#include <type_traits>

#include <pugixml.hpp>

namespace pixels_mask_helper
{
#define SETTINGS_ROOT_NODE_NAME "/bad_pixels_settings"
#define CAMERA_SETTINGS_NODE_NAME "camera_settings"

	std::tuple<camera_settings_t, std::vector<bad_pixels_mask_ptr> > create_bad_pixels_mask(const std::wstring& camera_bad_pixels_settings)
	{
		LOG_STACK();

		if (camera_bad_pixels_settings.empty())
			return std::make_tuple<camera_settings_t, std::vector<bad_pixels_mask_ptr>>({}, {});

		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_buffer(camera_bad_pixels_settings.data(), camera_bad_pixels_settings.size()*sizeof(std::decay<decltype(camera_bad_pixels_settings)>::type::traits_type::char_type),
			pugi::parse_default, pugi::encoding_utf16);

		if (!result){
			std::ostringstream ss;
			ss << "Could not parse settings. Error: " << result.description();

			throw std::runtime_error(ss.str().c_str());
		}

		auto get_first_settings_node = [](const pugi::xml_document & packet)->pugi::xml_node
		{
			pugi::xpath_node event_node = packet.select_single_node(SETTINGS_ROOT_NODE_NAME);
			return event_node.node();
		};

		auto settings_node = get_first_settings_node(doc);

		if (settings_node.empty()){
			throw std::runtime_error("Could not find settings node.");
		}


		auto get_attribute_value = [](pugi::xml_node & node, const char * attribute_name)->std::string
		{
			auto attribute = node.attribute(attribute_name);
			auto attribute_value = attribute.value();
			if (attribute.empty()){
				std::string error("Could not find attribute in the node. Attribute name: '" + std::string(attribute_name) + "'");
				throw std::runtime_error(error.c_str());
			}

			return attribute_value;
		};

		auto get_integral_attribute_value = [&](pugi::xml_node & node, const char * attribute_name)
		{
			int16_t result;

			try{
				result = static_cast<decltype(result)>(std::stol(get_attribute_value(node, attribute_name)));
			}
			catch (const std::out_of_range&)
			{
				std::ostringstream ss;
				ss << "Could not parse attribute: " << node.text();

				throw std::runtime_error(ss.str().c_str());
			}

			return result;
		};

		auto get_unsigned_integral_attribute_value = [&](pugi::xml_node & node, const char * attribute_name)
		{
			uint16_t result;

			try {
				result = static_cast<decltype(result)>(std::stoul(get_attribute_value(node, attribute_name)));
			}
			catch (const std::out_of_range&)
			{
				std::ostringstream ss;
				ss << "Could not parse attribute: " << node.text();

				throw std::runtime_error(ss.str().c_str());
			}

			return result;
		};


		std::vector<bad_pixels_mask_ptr> pixels_masks;
		std::string camera_sn;

		for (auto & child_node : settings_node.children(CAMERA_SETTINGS_NODE_NAME))
		{
			uint16_t width,height;
			std::string current_camera_sn;

			current_camera_sn = get_attribute_value(child_node,"SN");
			if (!camera_sn.empty() && current_camera_sn != camera_sn)
				return std::make_tuple(std::move(camera_sn), std::move(pixels_masks));

			camera_sn = current_camera_sn;

			width = get_unsigned_integral_attribute_value(child_node, "shot_width");
			height = get_unsigned_integral_attribute_value(child_node, "shot_height");

			pixels_masks.emplace_back(std::make_unique<irb_frame_helper::bad_pixels_mask>(width, height));

			auto & pixel_mask = *pixels_masks.rbegin();

			using offset_t = irb_frame_helper::bad_pixels_mask::value_type;
			offset_t good_pixel_offset;


			using coordinate_t = irb_frame_helper::bad_pixels_mask::coordinate_t;
			std::vector<coordinate_t> bad_pixels_coordinates;
			for (auto & pixels_node : child_node.children("pixels"))
			{
				good_pixel_offset = static_cast<decltype(good_pixel_offset)>(get_integral_attribute_value(pixels_node, "good_pixel_offset"));
				for (auto & coordinate_node : pixels_node)
				{
					bad_pixels_coordinates.push_back({ get_unsigned_integral_attribute_value(coordinate_node, "x"), get_unsigned_integral_attribute_value(coordinate_node, "y") });
				}
				pixel_mask->set_value(bad_pixels_coordinates, good_pixel_offset);
			}
		}

		return std::make_tuple(std::move(camera_sn), std::move(pixels_masks));
	}

}