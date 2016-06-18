#include <stdio.h>
#include <memory>
#include <experimental/optional>

using std::experimental::optional;
using std::string;

struct fclose_deleter
{
	void operator ()(FILE* fp)
	{
		fclose (fp);
	}
};

using file_ptr = std::unique_ptr<FILE, fclose_deleter>;



inline file_ptr open_file (const char* path, const char* mode)
{
	return file_ptr{fopen (path, mode)};
}


inline optional<string> read_all (const string& filename)
{
	return {};
}
