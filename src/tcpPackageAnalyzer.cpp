#include "tcpPackageAnalyzer.h"

vector<QByteArray> tcpPackageAnalyzer::getTcpPackages(QByteArray const & newData)
{
	vector<QByteArray> packages(0);
	this->m_buffer = this->m_buffer.append(newData);
	
	//	不够4（生成头） 或者 不够按头的长度生成包，如果按下面这么写，this->m_headLen 在进入while以前必然是 0 
	//while(this->m_buffer.length() >= 4) && (this->m_buffer.length() - 4 >= bytes4ToInt(this->m_buffer.left(4)))

	while (1) {												//可能一次读了多个包，加了个循环，不知道对不对
		if (this->m_headLen <= 0) {							//包体长度无意义，需要读取包体长度
			if (this->m_buffer.length() < 4)				//前4字节是包体长度,不够就等下次
				break;
			QByteArray head = this->m_buffer.left(4);
			this->m_buffer.remove(0, 4);
			this->m_headLen = bytes4ToInt(head);
		}
		const int len = this->m_buffer.length();
		if (len < this->m_headLen) {	//数据长度不够，等下次
			break;
		}
		QByteArray commandAndDataArray = this->m_buffer.left(this->m_headLen);
		this->m_buffer.remove(0, this->m_headLen);
		this->m_headLen = 0;						//读完数据，重置包体长度

		packages.push_back(commandAndDataArray);
	}
	return packages;
}

void tcpPackageAnalyzer::clear()
{
	this->m_buffer.clear();
	this->m_headLen = 0;
}
