#pragma once

#include <string>
#include <functional>
#include <mutex>
#include <vector>

namespace signals
{
	template<class Sig>
	class event;
	template<class R, class... Args>
	class event<R(Args...)>
	{
		typedef std::function<R(Args...)> Slot;
	public:
		event(){}
		~event(){}

	public:

		event & operator+=(const Slot& delegate) { std::lock_guard<decltype(_lock)> lock(_lock); _delegates.push_back(delegate); return *this; }
		event & operator-=(const Slot& delegate)
		{
			std::lock_guard<decltype(_lock)> lock(_lock);
			auto iter = std::find_if(_delegates.cbegin(), _delegates.cend(), [&delegate](const Slot& block){return block.target<R(Args...)>() == delegate.target<R(Args...)>(); });
			if (iter != _delegates.cend())
				_delegates.erase(iter);

			return *this;
		}

		void operator()(Args... args) const
		{
			for (auto && delegate : _delegates){
				delegate(args...);
			}
		}

	private:
		std::vector<Slot> _delegates;
		std::mutex	_lock;
	};

}