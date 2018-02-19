#pragma once

#include <Windows.h>
#include <cstdint>

namespace logger
{
    namespace details
    {
        using thread_id_t = decltype(GetCurrentThreadId());
        std::size_t get_thread_history_log_count_messages(thread_id_t id);

        std::wstring get_last_insert_message_in_history(thread_id_t id, logger::level* sev_ptr = nullptr);

    } // namespace details

} // namespace logger
