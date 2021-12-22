
// filesystem : text, png등 파일의 경로를 찾아 읽어오는 작업 
#include <filesystem>
#include "Resource.h"
using namespace std;

void Resource::Import(string const directory, void (* const function)(string const& file))
{
	// directory에 저장된 이름의 파일이 존재한다면 실행
	if (filesystem::exists(directory))
	{
		// directory_iterator : 검색한 폴더안에 있는 자료들을 모두 저장
		// recursive : 검색한 폴더로 가는 모든 경로에 있는 자료들을 모두 저장
		// path : 폴더안에 있는 각 자료 하나의 경료

		for (filesystem::path const& path : filesystem::recursive_directory_iterator(directory))
		{
			// path가 폴더가 아닌 기본 파일이라면 실행
			if (filesystem::is_regular_file(path)) 
			{
				string file = path.string();
				replace(file.begin(), file.end(), '\\', '/');
				function(file);
			}
		}
	}
}
