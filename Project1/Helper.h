#pragma once
#include <boost/filesystem.hpp>
#include <list>

namespace fs = boost::filesystem;

namespace Helper
{
    static void find_file(const fs::path& dir_path, std::list<std::string>& list_files_found)
    {
	   if (!fs::exists(dir_path))
		  return;

	   fs::directory_iterator end_itr; //default construction yeilds past-the-end
	   for (fs::directory_iterator itr(dir_path); itr != end_itr; ++itr)
	   {
		  if (fs::is_directory(itr->status()))
		  {
			 find_file(itr->path(), list_files_found);
		  }
		  else
		  {
			 list_files_found.push_back(itr->path().string());
		  }
	   }

    }
}

