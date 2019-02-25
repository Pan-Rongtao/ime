/**********************************************
*	编写者：	Pan.瞳
*
*	日期：		 2014-9-17
*	
*	文件名称：	Edic.h
*
*	描述：		提供英文单词查询接口。
*
*	说明：		
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

namespace ime{ namespace edic{

class Edic_Internal;
class IME_API Edic
{
public:
	Edic();
	~Edic();
	Edic(const Edic &other) = delete;
	void operator=(const Edic &other) = delete;

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
	bool search(const std::string &prefix);

	//获取候选词个数
	unsigned int getCandidateCount() const;

	//获取结果页数
	unsigned int getCandidatePageCount() const;

	//获取候选词
	void getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const;

	//获取区域候选结果
	void getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const;

	//学习一个新词
	bool learn(const std::string &word);

	//忘记一个词
	bool forget(const std::string &word);

	//提升词频率
	//prefix: lov
	//word: love
	bool promote(const std::string &prefix, const std::string &word);

private:
	Edic_Internal	*m_internal;
};

}}