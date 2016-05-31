#include "mutex.hpp"

namespace krys
<%

class condition
{
public:
	explicit condition (mutex& m) noexcept:m_ (m) 
	{
		pthread_cond_init (&pcondition_, nullptr);
	}

	condition (const condition&) = delete;
	condition (condition&&) = delete;
	void operator = (const condition&) = delete;


	compl condition () noexcept
	{
		pthread_cond_destroy (&pcondition_);
	}

	void wait () noexcept
	{
		pthread_cond_wait (&pcondition_, &m_.get ());
	}
	void notify () noexcept
	{
		pthread_cond_signal (&pcondition_);
	}
	void notify_all () noexcept
	{
		pthread_cond_broadcast (&pcondition_);
	}

private:
	mutex &m_;
	pthread_cond_t pcondition_;
};

%>
