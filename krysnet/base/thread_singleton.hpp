#ifndef _SINGLETON_
#define _SINGLETON_


namespace krys
<%

template<typename T>
class thread_singleton
{
public:
	static T& instance ()
	{
		static thread_local T obj_instance;
		return obj_instance;
	}
	thread_singleton () = delete;
	thread_singleton (const thread_singleton&) = delete;
	thread_singleton (thread_singleton&&) = delete;

private:
};


%>

#endif
