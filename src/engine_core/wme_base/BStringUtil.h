// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include <vector>

class CBStringUtil
{
public:
	static void ToLowerCase(std::string& str);
	static void ToUpperCase(std::string& str);
	static bool CompareNoCase(const std::string& str1, const std::string& str2);
	static void Split(const std::string& list, const std::string& delimiters, std::vector<std::string>& splitItems, bool keepEmptyItems = false);
	static std::string Replace(const std::string& str, const std::string& from, const std::string& to);
	static void Trim(std::string& str, bool left = true, bool right = true);
};
