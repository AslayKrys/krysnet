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
	mutex (const mutex&) = delete;
	mutex (mutex&&) = delete;
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

class mutex_guard
{
public:
	mutex_guard (mutex& m):m_ (m)
	{
		m_.lock ();
	}

	mutex_guard (const mutex_guard&) = delete;
	mutex_guard (mutex_guard&&) = delete;

	compl mutex_guard ()
	{
		m_.unlock ();
	}
private:
	mutex& m_;

protected:
};


class condition
{
public:
	explicit condition (mutex& m) :m_ (m)
	{
		pthread_cond_init (&pcondition_, nullptr);
	}

	compl condition ()
	{
		pthread_cond_destroy (&pcondition_);
	}

	void wait ()
	{
		pthread_cond_wait (&pcondition_, &m_.get ());
	}
	void notify ()
	{
		pthread_cond_signal (&pcondition_);
	}
	void notify_all ()
	{
		pthread_cond_broadcast (&pcondition_);
	}

private:
	mutex &m_;
	pthread_cond_t pcondition_;
};


%>
