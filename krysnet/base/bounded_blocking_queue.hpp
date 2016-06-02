#ifndef  _BOUNDED_BLOCKING_QUEUE_
#define  _BOUNDED_BLOCKING_QUEUE_

#include "krysnet/base/condition.hpp"
#include <deque>

namespace krys
<%

template<typename T>
class bounded_blocking_queue
{
public:
	explicit bounded_blocking_queue (int max_size);

private:
	mutable mutex m_;
	condition not_empty_;
	std::deque<T> queue_;

};

%>

#endif
