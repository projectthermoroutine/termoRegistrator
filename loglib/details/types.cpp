

#include <unordered_map>

#include <loglib/types.h>


namespace logger
{

    const level level_unknown(static_cast<level>(-1));


    namespace
    {
        auto const max_backup_index_up_limit = 100;
        auto const max_backup_index_down_limit = 1;
        auto const max_file_size_up_limit = 1024 * 1024 * 100;
        auto const max_file_size_down_limit = 1024 * 512;
        auto const max_buffer_size_up_limit = 1024 * 1024 * 2;
        auto const max_buffer_size_down_limit = 1024 * 512;

        const std::unordered_map<level, std::wstring> log_level_to_string_map
        {
            {level_unknown, L"UNKNOWN"},

            {level::trace, L"TRACE"},
            {level::debug, L"DEBUG"},
            {level::info,  L"INFO"},
            {level::warn,  L"WARN"},
            {level::error, L"ERROR"},
            {level::fatal, L"FATAL"}
        };

        const std::unordered_map<std::wstring, level> log_level_from_string_map([]()
        {
            std::unordered_map<std::wstring, level> result;
            for (const auto& pair : log_level_to_string_map)
            {
                result[pair.second] = pair.first;
            }
            return result;
        }());

    } // namespace anonymous


    settings::zip_t settings::zip() const
    {
        return std::make_tuple(
            this->level,
            this->use_developer_log,
            this->max_buffer_size,
            this->max_file_size,
            this->max_backup_index);
    }

    settings settings::unzip(const zip_t& zip)
    {
        logger::settings settings;

        std::tie(
            settings.level,
            settings.use_developer_log,
            settings.max_buffer_size,
            settings.max_file_size,
            settings.max_backup_index
            ) = zip;

        return settings;
    }

    void settings::validate(const std::function<void(const std::wstring & param_name, const std::wstring & original_value, const std::wstring & modified_value)>& invalid_notify)
    {
        /*Check max backup index*/
        if (this->max_backup_index < max_backup_index_down_limit)
        {
            invalid_notify(L"max_backup_index", std::to_wstring(this->max_backup_index), std::to_wstring(max_backup_index_down_limit));
            this->max_backup_index = max_backup_index_down_limit;
        }
        else if (this->max_backup_index > max_backup_index_up_limit)
        {
            invalid_notify(L"max_backup_index", std::to_wstring(this->max_backup_index), std::to_wstring(max_backup_index_up_limit));
            this->max_backup_index = max_backup_index_up_limit;
        }

        /*Check max buffer size*/
        if (this->max_buffer_size < max_buffer_size_down_limit)
        {
            invalid_notify(L"max_buffer_size", std::to_wstring(this->max_buffer_size), std::to_wstring(max_buffer_size_down_limit));
            this->max_buffer_size = max_buffer_size_down_limit;
        }
        else if (this->max_buffer_size > max_buffer_size_up_limit)
        {
            invalid_notify(L"max_buffer_size", std::to_wstring(this->max_buffer_size), std::to_wstring(max_buffer_size_up_limit));
            this->max_buffer_size = max_buffer_size_up_limit;
        }

        /*Check max file size*/
        if (this->max_file_size < max_file_size_down_limit)
        {
            invalid_notify(L"max_file_size", std::to_wstring(this->max_file_size), std::to_wstring(max_file_size_down_limit));
            this->max_file_size = max_file_size_down_limit;
        }
        else if (this->max_file_size > max_file_size_up_limit)
        {
            invalid_notify(L"max_file_size", std::to_wstring(this->max_file_size), std::to_wstring(max_file_size_up_limit));
            this->max_file_size = max_file_size_up_limit;
        }
    }

    bool settings::operator==(const settings& other) const
    {
        return std::tie(this->max_file_size, this->max_backup_index, this->use_developer_log, this->level, this->max_buffer_size)
            == std::tie(other.max_file_size, other.max_backup_index, other.use_developer_log, other.level, other.max_buffer_size);
    }

    bool settings::operator!=(const settings& other) const
    {
        return !this->operator==(other);
    }

    std::wstring log_level_to_string(level lvl)
    {
        const auto it(log_level_to_string_map.find(lvl));
        return it == std::end(log_level_to_string_map)
            ? log_level_to_string_map.at(level_unknown)
            : it->second;
    }

    level log_level_from_string(const std::wstring& str_sev)
    {
        const auto it(log_level_from_string_map.find(str_sev));
        return it == std::end(log_level_from_string_map)
            ? level_unknown
            : it->second;
    }


} // namespace logger
