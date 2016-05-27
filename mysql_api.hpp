#include <mysql/mysql.h>
#include <memory>
#include <boost/utility/string_ref.hpp>


using string_ref = boost::string_ref;

struct mysql_closer { void operator () (MYSQL* con) { mysql_close (con); } };
using mysql_conn = std::unique_ptr<MYSQL, mysql_closer>;

struct mysql_res_destroyer {void operator () (MYSQL_RES* res) {mysql_free_result (res);}};
using mysql_res = std::unique_ptr<MYSQL_RES, mysql_res_destroyer>;

namespace krys 
<%

inline mysql_conn mysql_init ()
{
	return mysql_conn {mysql_init ((MYSQL*)NULL)};
}

inline MYSQL* mysql_real_connect (const mysql_conn& con, string_ref host = "127.0.0.1", 
		string_ref user = "root", string_ref pswd = "", string_ref db_name = "", 
		uint16_t port = 3306, const char* unix_socket = nullptr, uint32_t flag = 0)
{
	return mysql_real_connect (con.get (), host.data (), user.data (), pswd.data (), db_name.data (), port, unix_socket, flag);
}

inline int mysql_query (const mysql_conn& con, string_ref cmd)
{
	return mysql_query (con.get (), cmd.data ());
}

inline string_ref mysql_error (const mysql_conn& conn)
{
	return mysql_error (conn.get ());
}

inline mysql_res mysql_store_result (const mysql_conn& conn)
{
	return mysql_res {mysql_store_result (conn.get ())};
}

inline unsigned int mysql_num_fields (const mysql_res& res)
{
	return mysql_num_fields (res.get ());
}

inline unsigned long long mysql_num_rows (const mysql_res& res)
{
	return mysql_num_rows (res.get ());
}

inline MYSQL_ROW mysql_fetch_row (const mysql_res& res)
{
	return mysql_fetch_row (res.get ());
}

inline MYSQL_ROW_OFFSET mysql_row_tell_ (const mysql_res& res)
{
	return mysql_row_tell (res.get ());
}


template<typename ... STR_REF_CTOR>
inline std::string mysql_real_escape_string (const mysql_conn& conn, STR_REF_CTOR&& ... ref_ctor)
{
	string_ref input {std::forward<STR_REF_CTOR> (ref_ctor)...};

	std::string result;
	result.resize (input.size () * 2);

	auto result_len = mysql_real_escape_string (conn.get (), const_cast<char*> (result.data ()), input.data (), input.length ());

	result.resize (result_len);
	return result;
}


%>
