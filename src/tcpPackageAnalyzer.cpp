#include "tcpPackageAnalyzer.h"

vector<QByteArray> tcpPackageAnalyzer::getTcpPackages(QByteArray const & newData)
{
	vector<QByteArray> packages(0);
	this->m_buffer = this->m_buffer.append(newData);
	
	//	����4������ͷ�� ���� ������ͷ�ĳ������ɰ��������������ôд��this->m_headLen �ڽ���while��ǰ��Ȼ�� 0 
	//while(this->m_buffer.length() >= 4) && (this->m_buffer.length() - 4 >= bytes4ToInt(this->m_buffer.left(4)))

	while (1) {												//����һ�ζ��˶���������˸�ѭ������֪���Բ���
		if (this->m_headLen <= 0) {							//���峤�������壬��Ҫ��ȡ���峤��
			if (this->m_buffer.length() < 4)				//ǰ4�ֽ��ǰ��峤��,�����͵��´�
				break;
			QByteArray head = this->m_buffer.left(4);
			this->m_buffer.remove(0, 4);
			this->m_headLen = bytes4ToInt(head);
		}
		const int len = this->m_buffer.length();
		if (len < this->m_headLen) {	//���ݳ��Ȳ��������´�
			break;
		}
		QByteArray commandAndDataArray = this->m_buffer.left(this->m_headLen);
		this->m_buffer.remove(0, this->m_headLen);
		this->m_headLen = 0;						//�������ݣ����ð��峤��

		packages.push_back(commandAndDataArray);
	}
	return packages;
}

void tcpPackageAnalyzer::clear()
{
	this->m_buffer.clear();
	this->m_headLen = 0;
}
