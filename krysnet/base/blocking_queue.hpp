#ifndef __BLOCKING_QUEUE__
#define __BLOCKING_QUEUE__


#include <queue>
#include "krysnet/base/mutex.hpp"
#include "krysnet/base/mutex_guard.hpp"
#include "krysnet/base/condition.hpp"

namespace krys
<%

template<typename T>
class blocking_queue
{
public:
	blocking_queue () noexcept: m_ (),not_empty_ (m_),queue_ ()
	{

	}
	blocking_queue (const blocking_queue&) = delete;
	blocking_queue (blocking_queue&&) = delete;
	compl blocking_queue () = default;

	template<typename ... ARGS>
	void emplace(ARGS&& ... args) noexcept
	{
		mutex_guard guard (m_);
		queue_.emplace_back (std::forward<ARGS>(args)...);
		not_empty_.notify ();
	}



private:
	mutex m_;
	condition not_empty_;
	std::queue<T> queue_;
};


%>

#endif
