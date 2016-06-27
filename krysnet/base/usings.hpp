#include <string>
#include <thread>
#include <exception>
#include <array>
#include <memory>
#include <deque>
#include <functional>
#include <experimental/string_view>

namespace krys 
<%
using std::experimental::string_view;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::istringstream;
using std::stringstream;
using std::deque;
using std::thread;
using std::ostringstream;
using std::function;
%>

#define CONCAT_HELPER(a,b) a##b
#define CONCAT(a,b) CONCAT_HELPER(a,b)
