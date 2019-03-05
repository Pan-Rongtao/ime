#pragma once
#include <vector>
#include <string>
#include "hci_sys.h"
#include "hci_hwr.h"

namespace ime{ namespace hwr{

class Hwr_Internal
{
public:
	Hwr_Internal();
	~Hwr_Internal();
	Hwr_Internal(const Hwr_Internal &other) = delete;
	void operator =(const Hwr_Internal &other) = delete;

	bool init(const std::string &authDir, const std::string &appKey, const std::string &developerKey, const std::string &cloudUrl, const std::string &dataDir, const std::string &logPath);
	bool hasInit() const;
	bool deinit();
	void setCandidatePageSize(unsigned int size);
	unsigned int getCandidatePageSize() const;
	bool recognize(const std::vector<short> &points);
	unsigned int getCandidateCount() const;
	unsigned int getCandidatePageCount() const;
	void getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const;
	void getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const;

private:
	void checkInit() const;

	bool				m_init;
	int					m_candidatePageSize;
	int					m_sessionId;
	HWR_RECOG_RESULT	m_candidates;
};

}}