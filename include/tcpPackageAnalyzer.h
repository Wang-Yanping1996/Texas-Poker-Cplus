#pragma once
#include "tcpCommand.h"
//Ϊ����server��client�������ݲ��ָ������д��tcp�����
//����߼��ƺ�ֻ��д��whileѭ��+break����ʱû�뵽������׶��ķ�ʽ
//�Ժ���Կ���д�ɶ��̣߳�vector�н�����ɷ���
class tcpPackageAnalyzer {
private:
	QByteArray m_buffer;
	int m_headLen;
public:
	tcpPackageAnalyzer(QByteArray const& buffer = QByteArray(), const int headLen = 0) : m_buffer(buffer), m_headLen(headLen) {};
	tcpPackageAnalyzer(const tcpPackageAnalyzer& other) = delete;
	vector<QByteArray> getTcpPackages(QByteArray const& newData = QByteArray());
	void clear();
};


