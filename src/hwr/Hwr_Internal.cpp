#include "Hwr_Internal.h"

using namespace ime::hwr;

#define CAP_KEY		"hwr.local.letter"

Hwr_Internal::Hwr_Internal()
	: m_init(false)
	, m_sessionId(-1)
	, m_candidates{nullptr, 0}
{
}

Hwr_Internal::~Hwr_Internal()
{
}

bool Hwr_Internal::init(const std::string &authDir, const std::string &appKey, const std::string &developerKey, const std::string &cloudUrl, const std::string &dataDir, const std::string &logPath)
{
	deinit();
	HCI_ERR_CODE err_code = HCI_ERR_NONE;
	//����ֶ�֮����','�������ֶ������ִ�Сд��
	std::string init_config = "";
	init_config += "appKey=" + appKey;						//����Ӧ�����
	init_config += ",developerKey=" + developerKey;			//���ƿ�������Կ
	init_config += ",cloudUrl=" + cloudUrl;					//�����Ʒ���Ľӿڵ�ַ
	init_config += ",authpath=" + authDir;					//��Ȩ�ļ�����·������֤��д
	init_config += ",logfilepath=" + logPath;				//��־��·��
	init_config += ",loglevel=5";							//��־�ĵȼ�
	init_config += ",logfilesize=512";						//��־�ļ��Ĵ�С
															//��������ʹ��Ĭ��ֵ��������ӣ���������ÿ��Բο������ֲ�
	err_code = hci_init(init_config.c_str());
	if (err_code != HCI_ERR_NONE)
	{
		printf("hci sys init fail return (%d:%s)\n", err_code, hci_get_error_info(err_code));
		return false;
	}

	init_config += std::string("initcapkeys=") + CAP_KEY;	//������initcapkeys�������Ҫʹ�õ�������������ߵ�һ��ʶ���Ч��
	init_config += ",datapath=" + dataDir;					//����Ǳ����������˴�ָ����������������Դ
	err_code = hci_hwr_init(init_config.c_str());
	if (err_code != HCI_ERR_NONE)
	{
		printf("hci hwr init fail return (%d:%s)\n", err_code, hci_get_error_info(err_code));
		return false;
	}

	//session start
	std::string session_config = std::string("capkey=") + CAP_KEY;
	err_code = hci_hwr_session_start(session_config.c_str(), &m_sessionId);
	if (err_code != HCI_ERR_NONE)
	{
		printf("hci_hwr_session_start return (%d:%s)\n", err_code, hci_get_error_info(err_code));
		return false;
	}

	m_init = err_code == HCI_ERR_NONE;
	return m_init;
}

bool Hwr_Internal::hasInit() const
{
	return m_init;
}

bool Hwr_Internal::deinit()
{
	m_init = false;
	return hci_hwr_session_stop(m_sessionId) == HCI_ERR_NONE && hci_hwr_release() == HCI_ERR_NONE && hci_release() == HCI_ERR_NONE;
}

void Hwr_Internal::setCandidatePageSize(unsigned int size)
{
	if (size == 0)	throw std::out_of_range("setCandidatePageSize size by 0.");
	m_candidatePageSize = size;
}

unsigned int Hwr_Internal::getCandidatePageSize() const
{
	return m_candidatePageSize;
}

bool Hwr_Internal::recognize(const std::vector<short>& points)
{
	checkInit();
	hci_hwr_free_recog_result(&m_candidates);
	bool b = hci_hwr_recog(m_sessionId, (short *)points.data(), points.size() * sizeof(short), "", &m_candidates) == HCI_ERR_NONE;
	return b;
}

unsigned int Hwr_Internal::getCandidateCount() const
{
	return m_candidates.uiResultItemCount;
}

unsigned int Hwr_Internal::getCandidatePageCount() const
{
	checkInit();
	auto count = getCandidateCount();
	return (count % getCandidatePageSize() == 0) ? count / getCandidatePageSize() : count / getCandidatePageSize() + 1;
}

void Hwr_Internal::getCandidate(unsigned int index, unsigned int count, std::vector<std::string>& candidates) const
{
	checkInit();
	if (count == 0)
		return;

	if (index >= getCandidateCount())
		throw std::out_of_range(std::string(__FUNCTION__) + "->index(" + std::to_string(index) + ") is out of range [0, " + std::to_string(getCandidateCount()) + ")");

	int nGet = 0;
	for (auto i = index; nGet != count && i < getCandidateCount(); ++i)
	{
		HWR_RECOG_RESULT_ITEM &item = m_candidates.psResultItemList[i];
		candidates.push_back(item.pszResult);
		++nGet;
	}
}

void Hwr_Internal::getCandidateByPage(unsigned int page, std::vector<std::string>& candidates) const
{
	getCandidate(page * getCandidatePageSize(), getCandidatePageSize(), candidates);
}

void Hwr_Internal::checkInit() const
{
	if (!m_init)
	{
		printf("any api must called after init success, exit.\r\n");
		std::exit(-1);
	}
}
