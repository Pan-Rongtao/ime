/**********************************************
*	编写者：	潘荣涛 Never_More
*
*	日期：		 2014-9-17
*	
*	文件名称：	IPRTWubiApi.h
*
*	描述：		提供五笔输入法查询接口。
*
*	说明：		字根按照键盘布局好的字根
*	
************************************************/
#pragma once
#include <string>
#include <vector>

#ifdef WIN32
#define IME_API	__declspec(dllexport)
#else
#define IME_API
#endif

namespace ime{ namespace wubi {

class Wubi_Internal;
class IME_API Wubi
{
public:
	Wubi();
	~Wubi();
	Wubi(const Wubi &other) = delete;
	void operator=(const Wubi &other) = delete;

	//设置字典路径
	bool init(const std::string &sFilePath);

	//是否已初始化
	bool hasInit() const;

	//反初始化
	bool deinit();

	//设置每页候选词个数(默认5)
	void setCandidatePageSize(unsigned int size);

	//获取每页候选词个数
	unsigned int getCandidatePageSize() const;

	//执行搜索
	bool search(const std::string &zigen);

	//获取候选词个数
	unsigned int getCandidateCount() const;

	//获取结果页数
	unsigned int getCandidatePageCount() const;

	//获取候选词
	void getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const;

	//获取区域候选结果
	void getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const;

	//提升汉字频率
	//prefix: 12345（笔画）
	//word: 汉字
	bool promote(const std::string &zigen, const std::string &word);

private:
	Wubi_Internal	*m_internal;
};

}}