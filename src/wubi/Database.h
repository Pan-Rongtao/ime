/**************************************
*
*	1、直接使用sqlite3的api建立与数据库字典
*	的连接
*
****************************************/
#pragma once
#include <string>
#include <tuple>
#include "sqlite3.h"

namespace ime { namespace wubi {

class Database
{
public:
	Database();
	~Database();
	Database(const Database &other) = delete;
	void operator =(const Database &other) = delete;

	//加载
	static bool load(const std::string &path);
	static void unload();

	//更新
	static bool update(const std::string &word, int priority);

	static sqlite3 *sqlHandle();

private:
	static sqlite3		*m_sqlHandle;
};

class Query
{
public:
	enum class Condition
	{
		none,
		eq,
		like,
	};

	bool search(const std::string &zigen, Condition zigenCondition);
	int searchHighestPriority(const std::string &firstLetter);
	unsigned int recordCount() const;
	std::tuple<char *> getRecord(unsigned int index) const;
	void release();

private:
	char		**m_data{ nullptr };
	int			m_row{ 0 };
	int			m_col{ 0 };
};

}}