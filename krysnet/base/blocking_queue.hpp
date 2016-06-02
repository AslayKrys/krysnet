#ifndef __BLOCKING_QUEUE__
#define __BLOCKING_QUEUE__


#include <deque>
#include "krysnet/base/mutex.hpp"
#include "krysnet/base/mutex_guard.hpp"
#include "krysnet/base/condition.hpp"

namespace krys
<%

template<typename T>
class blocking_queue
{
public:
	blocking_queue () noexcept
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

	T pop () noexcept
	{
		mutex_guard lock (m_);
		while (queue_.empty ())
		{
			not_empty_.wait ();
		}

		T front_item = std::move (queue_.front ());
		queue_.pop_front ();
		return front_item;
	}

	size_t size () const noexcept
	{
		mutex_guard lock (m_);
		return queue_.size ();
	}


private:
	mutable mutex m_ {};
	condition not_empty_ {m_};
	std::deque<T> queue_ {};
};


%>

#endif
