#ifndef __MUTEX_GUARD__
#define __MUTEX_GUARD__


#include "krysnet/base/mutex.hpp"
#include "krysnet/base/usings.hpp"

namespace krys
<%

class mutex_guard
{ 
public:
	mutex_guard (mutex& m) noexcept:m_ (m)
	{
		m_.lock ();
	}

	void* operator new (size_t size) = delete;
	mutex_guard (const mutex_guard&) = delete;
	void operator = (const mutex_guard&) = delete;

	compl mutex_guard () noexcept
	{
		m_.unlock ();
	}
private:
	mutex& m_;

protected:
};

%>

#define GUARD_THIS(lock) \
	krys::mutex_guard CONCAT(guard,__LINE__) {lock}

#define mutex_guard(...) \
	static_assert (false, "cannot use mutex_guard this way")

#endif
