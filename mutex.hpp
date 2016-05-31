#ifndef  __MUTEX__
#define  __MUTEX__
#include <pthread.h>

namespace krys
<%

class mutex_guard;
class condition;

class mutex
{
public:
	mutex ()
	{
		pthread_mutex_init (&mutex_, nullptr);
	}
	void* operator new (size_t size) = delete;

	mutex (const mutex&) = delete;
	mutex (mutex&&) = delete;
	void operator = (const mutex&) = delete;

	compl mutex ()
	{
		pthread_mutex_destroy (&mutex_);
	}

private:
	void lock ()
	{
		pthread_mutex_lock (&mutex_);
	}

	void unlock ()
	{
		pthread_mutex_unlock (&mutex_);
	}

	auto& get ()
	{
		return mutex_;
	}


	friend class mutex_guard;
	friend class condition;
private:
	pthread_mutex_t mutex_;
};


%>

#endif
