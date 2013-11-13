// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BStringUtil.h"
#include <algorithm>


//////////////////////////////////////////////////////////////////////////
void CBStringUtil::ToLowerCase(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(),	tolower);
}


//////////////////////////////////////////////////////////////////////////
void CBStringUtil::ToUpperCase(std::string& str)
{
	std::transform(str.begin(),	str.end(), str.begin(), toupper);
}


//////////////////////////////////////////////////////////////////////////
bool CBStringUtil::CompareNoCase(const std::string& str1, const std::string& str2)
{
	std::string str1lc = str1;
	std::string str2lc = str2;

	ToLowerCase(str1lc);
	ToLowerCase(str2lc);

	return (str1lc == str2lc);
}


//////////////////////////////////////////////////////////////////////////
void CBStringUtil::Split(const std::string& list, const std::string& delimiters, std::vector<std::string>& splitItems, bool keepEmptyItems)
{
	splitItems.clear();

	std::string word = "";
	for (size_t i = 0; i < list.length(); i++)
	{
		char ch = list[i];
		if (delimiters.find(ch) != delimiters.npos)
		{
			if (!word.empty() || keepEmptyItems) splitItems.push_back(word);
			word.clear();
		}
		else word += ch;
	}
	if (!word.empty() || keepEmptyItems) splitItems.push_back(word);
}


//////////////////////////////////////////////////////////////////////////
std::string CBStringUtil::Replace(const std::string& str, const std::string& from, const std::string& to)
{
	if (from.empty() || from == to) return str;

	std::string result = str;
	size_t pos = 0;

	while (true)
	{
		pos = result.find(from, pos);
		if (pos == result.npos) break;

		result.replace(pos, from.size(), to);
		pos += to.size();
	}

	return result;
}


//////////////////////////////////////////////////////////////////////////
void CBStringUtil::Trim(std::string& str, bool left, bool right)
{
	static const std::string delims = " \t\r";

	if (right)
		str.erase(str.find_last_not_of(delims) + 1);

	if (left)
		str.erase(0, str.find_first_not_of(delims));
}
