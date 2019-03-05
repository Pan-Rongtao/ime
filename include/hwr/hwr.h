/**********************************************
*	��д�ߣ�	Pan.ͫ
*
*	���ڣ�		 2019-3-4
*
*	�ļ����ƣ�	Hwr.h
*
*	������		�ṩ��дʶ��ģ�飬���Ĳ������ƿ���ƽ̨ https://www.aicloud.com/Dev/Tpl/index/sdk/list
*
*	˵����
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

	//��ʼ��
	//authDir����Ȩ�ļ�HCI_AUTH��HCI_USER_INFO����Ŀ¼
	//appKey��Ӧ�ÿ������
	//developerKey����������Կ
	//cloudUrl���Ʒ����ַ
	//dataPath����������·��letter.conf��letter.dic
	//logPath����־·��
	bool init(const std::string &authDir, const std::string &appKey, const std::string &developerKey, const std::string &cloudUrl, const std::string &dataDir, const std::string &logPath);

	//�Ƿ��ѳ�ʼ��
	bool hasInit() const;

	//����ʼ��
	bool deinit();

	//����ÿҳ��ѡ�ʸ���(Ĭ��5)
	void setCandidatePageSize(unsigned int size);

	//��ȡÿҳ��ѡ�ʸ���
	unsigned int getCandidatePageSize() const;

	//ʶ��
	//points���㣬��ʽ��x, y, x, y, x, y, ..., 0, -1, x, y, x, y, ..., -1, -1
	//(0, -1)�Ǳʻ�������(-1, -1)�Ǳʼ�����
	bool recognize(const std::vector<short> &points);

	//��ȡ��ѡ�ʸ���
	unsigned int getCandidateCount() const;

	//��ȡ���ҳ��
	unsigned int getCandidatePageCount() const;

	//��ȡ��ѡ��
	void getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const;

	//��ȡ�����ѡ���
	void getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const;

private:
	Hwr_Internal	*m_internal;
};

}}