#ifndef  __MUTEX__
#define  __MUTEX__
#include <pthread.h>
#include <mutex>

namespace krys
<%

class mutex_guard;
class condition;

class mutex
{
public:
	mutex () noexcept
	{
		pthread_mutex_init (&mutex_, nullptr);
	}
	void* operator new (size_t size) = delete;

	mutex (const mutex&) = delete;
	void operator = (const mutex&) = delete;

	compl mutex () noexcept
	{
		pthread_mutex_destroy (&mutex_);
	}

private:
	void lock () noexcept
	{
		pthread_mutex_lock (&mutex_);
	}

	void unlock () noexcept
	{
		pthread_mutex_unlock (&mutex_);
	}

	auto& get () noexcept
	{
		return mutex_;
	}


	friend class mutex_guard;
	friend class condition;
	friend class std::lock_guard<mutex>;
private:
	pthread_mutex_t mutex_;
};

using std_guard = ::std::lock_guard<mutex>;

#define LOCK_THIS(lock) \
	 ::krys::std_guard CONCAT(guard,__LINE__) {lock}


%>

#endif
