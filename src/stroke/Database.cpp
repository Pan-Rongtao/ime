#include "Database.h"
#ifdef WIN32 
#include <io.h>
#else
#include <unistd.h>
#endif

using namespace ime::stroke;
sqlite3 *Database::m_sqlHandle = nullptr;

Database::Database()
{
}

Database::~Database()
{
	unload();
}

bool Database::load(const std::string & path)
{	
	//需要判断db数据库是否存在，不存在它会自动创建的
	if (access(path.c_str(), 0) != 0)
		return false;

	//打开数据库
	char *pErrorMsg = nullptr;
	return sqlite3_open(path.data(), &m_sqlHandle) == SQLITE_OK;
}

void Database::unload()
{
	sqlite3_close(m_sqlHandle);
}

bool Database::update(const std::string & word, int priority)
{
	char *error = nullptr;
	std::string sCmd = "update system set priority = " + std::to_string(priority) + " where word = '" + word + "'";
	return sqlite3_exec(m_sqlHandle, sCmd.data(), nullptr, nullptr, &error) == SQLITE_OK;
}

sqlite3 *Database::sqlHandle()
{
	return m_sqlHandle;
}

/////////////////
bool Query::search(const std::string & stroke, Condition strokeCondition)
{
	release();
	if (stroke.empty())
		return true;

	std::string sQueryStroke = strokeCondition == Condition::like ? std::string("like '") + stroke + "%'" : std::string("= '") + stroke + "'";
	std::string sCmd = "select distinct word from (select word, priority from system where stroke " + sQueryStroke + ") order by priority desc";	//word可能重复，需要distinct
	char *error = nullptr;
	return sqlite3_get_table(Database::sqlHandle(), sCmd.data(), &m_data, &m_row, &m_col, &error) == SQLITE_OK;
}

int Query::searchHighestPriority(const std::string & firstLetter)
{
	int ret = 0;
	std::string sCmd = "select priority from system where stroke like '" + firstLetter + "%' order by priority desc";
	char *error = nullptr;
	char **data = nullptr;
	int row = 0, col = 0;
	sqlite3_get_table(Database::sqlHandle(), sCmd.data(), &data, &row, &col, &error);
	if (row != 0)
		ret = std::stoi(data[col]);
	sqlite3_free_table(data);
	return ret;
}

unsigned int Query::recordCount() const
{
	return m_row;
}

std::tuple<char*> Query::getRecord(unsigned int index) const
{
	return std::make_tuple(m_data[m_col + index * m_col + 0]);
}

void Query::release()
{
	sqlite3_free_table(m_data);
	m_data = nullptr;
	m_row = 0;
	m_col = 0;
}
