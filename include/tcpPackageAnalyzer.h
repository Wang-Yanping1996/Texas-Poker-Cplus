#pragma once
#include "tcpCommand.h"
//为了让server和client接收数据部分更清楚，写的tcp拆包类
//拆包逻辑似乎只能写成while循环+break，暂时没想到更清楚易懂的方式
//以后可以考虑写成多线程，vector有进入就派发？
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


