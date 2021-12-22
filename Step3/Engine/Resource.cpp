
// filesystem : text, png�� ������ ��θ� ã�� �о���� �۾� 
#include <filesystem>
#include "Resource.h"
using namespace std;

void Resource::Import(string const directory, void (* const function)(string const& file))
{
	// directory�� ����� �̸��� ������ �����Ѵٸ� ����
	if (filesystem::exists(directory))
	{
		// directory_iterator : �˻��� �����ȿ� �ִ� �ڷ���� ��� ����
		// recursive : �˻��� ������ ���� ��� ��ο� �ִ� �ڷ���� ��� ����
		// path : �����ȿ� �ִ� �� �ڷ� �ϳ��� ���

		for (filesystem::path const& path : filesystem::recursive_directory_iterator(directory))
		{
			// path�� ������ �ƴ� �⺻ �����̶�� ����
			if (filesystem::is_regular_file(path)) 
			{
				string file = path.string();
				replace(file.begin(), file.end(), '\\', '/');
				function(file);
			}
		}
	}
}
