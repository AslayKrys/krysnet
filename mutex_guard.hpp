#include "mutex.hpp"

namespace krys
<%
class mutex_guard
{ 
public:
	mutex_guard (mutex& m):m_ (m)
	{
		m_.lock ();
	}

	void* operator new (size_t size) = delete;
	mutex_guard (const mutex_guard&) = delete;
	mutex_guard (mutex_guard&&) = delete;
	void operator = (const mutex_guard&) = delete;

	compl mutex_guard ()
	{
		m_.unlock ();
	}
private:
	mutex& m_;

protected:
};

%>
