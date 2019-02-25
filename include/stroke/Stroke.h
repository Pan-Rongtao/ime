/**********************************************
*	编写者：	Pan.瞳
*
*	日期：		 2014-9-17
*	
*	文件名称：	Stroke.h
*
*	描述：		提供笔画输入法查询接口。
*
*	说明：		五种笔画 1			  2			  3			    4 			   5
*						 一（横）	|（竖）	   丿（撇）		丶（点、捺）	乙（折）
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

namespace ime{ namespace stroke {

class Stroke_Internal;
class IME_API Stroke
{
public:
	Stroke();
	~Stroke();
	Stroke(const Stroke &other) = delete;
	void operator=(const Stroke &other) = delete;

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
	bool search(const std::string &stroke);

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
	bool promote(const std::string &stroke, const std::string &word);

private:
	Stroke_Internal	*m_internal;
};

}}