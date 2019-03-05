/**********************************************
*	编写者：	Pan.瞳
*
*	日期：		 2019-3-4
*
*	文件名称：	Hwr.h
*
*	描述：		提供手写识别模块，核心采用灵云开放平台 https://www.aicloud.com/Dev/Tpl/index/sdk/list
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

namespace ime { namespace hwr {

class Hwr_Internal;
class IME_API Hwr
{
public:
	Hwr();
	~Hwr();
	Hwr(const Hwr &other) = delete;
	void operator =(const Hwr &other) = delete;

	//初始化
	//authDir：授权文件HCI_AUTH、HCI_USER_INFO所在目录
	//appKey：应用开发序号
	//developerKey：开发者密钥
	//cloudUrl：云服务地址
	//dataPath：数据所在路径letter.conf、letter.dic
	//logPath：日志路径
	bool init(const std::string &authDir, const std::string &appKey, const std::string &developerKey, const std::string &cloudUrl, const std::string &dataDir, const std::string &logPath);

	//是否已初始化
	bool hasInit() const;

	//反初始化
	bool deinit();

	//设置每页候选词个数(默认5)
	void setCandidatePageSize(unsigned int size);

	//获取每页候选词个数
	unsigned int getCandidatePageSize() const;

	//识别
	//points：点，格式如x, y, x, y, x, y, ..., 0, -1, x, y, x, y, ..., -1, -1
	//(0, -1)是笔画结束，(-1, -1)是笔迹结束
	bool recognize(const std::vector<short> &points);

	//获取候选词个数
	unsigned int getCandidateCount() const;

	//获取结果页数
	unsigned int getCandidatePageCount() const;

	//获取候选词
	void getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const;

	//获取区域候选结果
	void getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const;

private:
	Hwr_Internal	*m_internal;
};

}}