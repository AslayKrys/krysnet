#ifndef _SINGLETON_
#define _SINGLETON_

namespace krys
<%


template<typename T>
class singleton
{
public:
	static T& instance ()
	{
		static T obj_instance;
		return obj_instance;
	}
	singleton () = delete;
	singleton (const singleton&) = delete;
	singleton (singleton&&) = delete;

private:
};



%>

#endif
