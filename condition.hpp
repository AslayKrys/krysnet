#include "mutex.hpp"

namespace krys
<%

class condition
{
public:
	explicit condition (mutex& m) :m_ (m)
	{
		pthread_cond_init (&pcondition_, nullptr);
	}

	condition (const condition&) = delete;
	condition (condition&&) = delete;
	void operator = (const condition&) = delete;


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
