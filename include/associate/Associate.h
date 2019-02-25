/**********************************************
*	编写者：	Pan.瞳
*
*	日期：		 2014-4-6
*	
*	文件名称：	Associate.h
*
*	描述：		提供汉字联想接口。
*
*	说明：		字符串编码要求UTF-8
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

namespace ime{ namespace associate{

class Associate_Internal;
class IME_API Associate
{
public:
	Associate();
	~Associate();
	Associate(const Associate &other) = delete;
	void operator =(const Associate &other) = delete;

	//初始化
	//associateDBPath：数据库路径
	bool init(const std::string &associateDBPath);

	//是否已初始化
	bool hasInit() const;

	//反初始化
	bool deinit();

	//设置每页候选词个数(默认5)
	void setCandidatePageSize(unsigned int size);

	//获取每页候选词个数
	unsigned int getCandidatePageSize() const;

	//执行搜索
	bool search(const std::string &prefix);

	//获取候选词个数
	unsigned int getCandidateCount() const;

	//获取结果页数
	unsigned int getCandidatePageCount() const;

	//获取候选词
	//异常：std::out_of_range
	void getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const;

	//获取区域候选结果
	//异常：std::out_of_range
	void getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const;

	//学习一个新词
	bool learn(const std::string &word);

	//忘记一个词
	bool forget(const std::string &word);

	//提升词频率
	//prefix: 我
	//word: 我的
	bool promote(const std::string &prefix, const std::string &word);

private:
	Associate_Internal		*m_internal;
};

}}