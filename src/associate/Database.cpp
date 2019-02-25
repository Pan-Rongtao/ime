#include "Database.h"
#ifdef WIN32 
#include <io.h>
#else
#include <unistd.h>
#endif

using namespace ime::associate;
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

bool Database::learn(const std::string & word, int priority)
{
	char *error = nullptr;
	std::string sCmd = "insert into learn values ('" + word + "', '" + std::to_string(priority) + "')";
	return sqlite3_exec(m_sqlHandle, sCmd.data(), nullptr, nullptr, &error) == SQLITE_OK;
}

bool Database::forget(const std::string & word)
{
	char *error = nullptr;
	std::string sCmdSystemTab = "delete from system where cizu = '" + word + "'";
	std::string sCmdLearnTab = "delete from learn where cizu = '" + word + "'";
	return sqlite3_exec(m_sqlHandle, sCmdSystemTab.data(), nullptr, nullptr, &error) == SQLITE_OK
		&& sqlite3_exec(m_sqlHandle, sCmdLearnTab.data(), nullptr, nullptr, &error) == SQLITE_OK;
}

bool Database::update(const std::string & word, int proiority)
{
	std::string sCmdSystemTab = "update system set priority = " + std::to_string(proiority) + " where cizu = '" + word + "'";
	std::string sCmdLearnTab = "update learn set priority = " + std::to_string(proiority) + " where cizu = '" + word + "'";
	char *error = nullptr;
	return sqlite3_exec(m_sqlHandle, sCmdSystemTab.data(), nullptr, nullptr, &error) == SQLITE_OK
		&& sqlite3_exec(m_sqlHandle, sCmdLearnTab.data(), nullptr, nullptr, &error) == SQLITE_OK;
}

sqlite3 *Database::sqlHandle()
{
	return m_sqlHandle;
}

//////////////
bool Query::search(const std::string &word, Condition wordCondition)
{
	release();
	if (word.empty())
		return true;

	std::string sQueryWord = wordCondition == Condition::like ? std::string("like '") + word + "_%'" : std::string("= '") + word + "'";
	std::string sCmd = "select distinct cizu, priority from (select cizu, priority from system where cizu " + sQueryWord + 
		" union select cizu, priority from learn where cizu " + sQueryWord + ") order by priority desc";
	char *error = nullptr;
	return sqlite3_get_table(Database::sqlHandle(), sCmd.data(), &m_data, &m_row, &m_col, &error) == SQLITE_OK;
}

unsigned int Query::recordCount() const
{
	return m_row;
}

std::tuple<char*, char*> Query::getRecord(unsigned int index) const
{
	return std::make_tuple(m_data[m_col + index * m_col + 0], m_data[m_col + index * m_col + 1]);
}

void Query::release()
{
	sqlite3_free_table(m_data);
	m_data = nullptr;
	m_row = 0;
	m_col = 0;
}
