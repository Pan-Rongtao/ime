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

namespace ime { namespace edic {

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

	//学习
	static bool learn(const std::string &word, int priority);

	//遗忘
	static bool forget(const std::string &word);

	//更新
	static bool update(const std::string &word, int proiority);

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

	bool search(const std::string &word, Condition wordCondition);
	unsigned int recordCount() const;
	std::tuple<char *, char *> getRecord(unsigned int index) const;
	void release();

private:
	char		**m_data{ nullptr };
	int			m_row{ 0 };
	int			m_col{ 0 };
};

}}