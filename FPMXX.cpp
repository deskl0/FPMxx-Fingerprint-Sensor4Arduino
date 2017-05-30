/*********************************
* FPMXX Fingerprint Sensor for Arduino
* Version: 1.0
*
* Author: Chenhe ����
* Created: 2017/5/28
* Blog: http://chenhe.cc
*
*********************************/

#if ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "FPMXX.h"

/*
ʹ��Ĭ�ϵ�ַ��0xFFFFFFFF��Ĭ�ϲ����ʣ�576000.
*/
FPMXX::FPMXX(SoftwareSerial * softSerial)
{
	mySerial = softSerial;
	mySerial->begin(57600);
}

FPMXX::FPMXX(SoftwareSerial * softSerial, uint8_t adder[])
{
	mySerial = softSerial;
	mySerial->begin(57600);
	for (int i = 0; i < 4; i++)
	{
		Adder[i] = adder[i];
	}
}

FPMXX::FPMXX(SoftwareSerial * softSerial, uint16_t baudRate)
{
	mySerial = softSerial;
	mySerial->begin(baudRate);
}

FPMXX::FPMXX(SoftwareSerial * softSerial, uint8_t adder[], uint16_t baudRate)
{
	mySerial = softSerial;
	mySerial->begin(baudRate);
	for (int i = 0; i < 4; i++)
	{
		Adder[i] = adder[i];
	}
}

/*
����ģ���ַ������ͨѶ��Ĭ��Ϊ0xFFFFFFFF��
�����������ÿ��ڲ��ı����Ա�ͨѶ���������޸�ģ��ĵ�ַ��
Params:
address: ģ���ַ������Ϊ4��
*/
void FPMXX::setCurrentAddress(uint8_t address[])
{
	for (int i = 0; i < 4; i++)
		Adder[i] = address[i];
}

/*
��ȡָ��������
Params:
num: ����ָ��������
*/
uint8_t FPMXX::getFingerStoreSize(uint16_t *num)
{
	uint8_t content[] = { 0x01,0x00,0x03,0x0F };
	sendCmd(3, content);
	uint8_t reply[17];
	uint8_t r = getReply(reply);
	if (r == 17)
	{
		/*Serial.println("--------");
		for (int i = 0;i < 17;i++)
		Serial.println(reply[i],HEX);
		Serial.println("--------");*/
		if (reply[0] != FPMXX_CODE_OK) return FPMXX_CODE_DATA_ERROR;
		*num = reply[5];
		*num <<= 8;
		*num |= reply[6];
		return reply[0];
	}
	return r;
}

/*
��ȡ�Ѵ����ָ�Ƹ�����
Params:
num: ���������
*/
uint8_t FPMXX::getFingerNum(uint16_t *num)
{
	uint8_t content[] = { 0x01,0x00,0x03,0x1D };
	sendCmd(3, content);
	uint8_t r = getReply(content);
	if (r == 3)
	{
		if (content[0] != FPMXX_CODE_OK) return FPMXX_CODE_DATA_ERROR;
		*num = content[1];
		*num <<= 8;
		*num |= content[2];
		return content[0];
	}
	return r;
}

/*
����ģ�鰲ȫ�ȼ����ȼ�Խ�߾�����Խ�ߡ�
��������Ч��
Params:
level: 0~5
*/
uint8_t FPMXX::setSecurityLevel(uint8_t level)
{
	uint8_t content[] = { 0x01,0x00,0x05,0x0E,0x05,0x00 };
	content[5] = level;
	sendCmd(5, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
����ģ����
Params:
password: �¿������Ϊ4��
*/
uint8_t FPMXX::setPassword(uint8_t password[])
{
	uint8_t content[] = { 0x01,0x00,0x07,0x12,0x00,0x00,0x00,0x00 };
	for (int i = 0; i < 4; i++)
		content[4 + i] = password[i];
	sendCmd(7, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
��֤ģ����ʹ��Ĭ��:0x00000000
*/
uint8_t FPMXX::verifyPassword()
{
	uint8_t content[] = { 0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00 };
	sendCmd(7, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
��֤ģ����
Params:
password: �������Ϊ4��
*/
uint8_t FPMXX::verifyPassword(uint8_t password[])
{
	uint8_t content[] = { 0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00 };
	for (int i = 0; i < 4; i++)
		content[4 + i] = password[i];
	sendCmd(7, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
¼��ָ��ͼ��
̽�⵽��¼��ָ��ͼ�����ģ��ImageBuffer
*/
uint8_t FPMXX::getFingerImage()
{
	uint8_t content[] = { 0x01,0x00,0x03,0x01 };
	sendCmd(3, content);
	uint8_t r = getReply(content, 1000);
	if (r == 1)
		return content[0];
	return r;
}

/*
��ImageBufferͼ������ָ��������
���� CharBuffer1 �� CharBuffer2
Params:
bufferId: 1~2������ֵ��2����
*/
uint8_t FPMXX::image2tz(uint8_t bufferId)
{
	uint8_t content[] = { 0x01,0x00,0x04,0x02,0x00 };
	content[4] = bufferId;
	sendCmd(4, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
�� CharBuffer1 �� CharBuffer2 �е������ļ��ϲ�����ģ�塣
������� CharBuffer1 �� CharBuffer2(����������ͬ)��
*/
uint8_t FPMXX::buildTemplate()
{
	uint8_t content[] = { 0x01,0x00,0x03,0x05 };
	sendCmd(3, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
����ָ�ơ�
Params:
bufferId: ����bufferId��1~2��
pageId: ָ�ƿ�λ�ã���0��ʼ��
*/
uint8_t FPMXX::storeFinger(uint8_t bufferId, uint16_t pageId)
{
	uint8_t content[] = { 0x01,0x00,0x06,0x06,0x00,0x00,0x00 };
	content[4] = bufferId;
	content[5] = (uint8_t)(pageId >> 8);
	content[6] = (uint8_t)pageId;
	sendCmd(6, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
��ȡָ�ơ�
�� flash ���ݿ���ָ�� ID �ŵ�ָ��ģ����뵽ģ�建������
Params:
bufferId: ����bufferId��1~2��
pageId: ָ�ƿ�λ�ã���0��ʼ��
*/
uint8_t FPMXX::loadFinger(uint8_t bufferId, uint16_t pageId)
{
	uint8_t content[] = { 0x01,0x00,0x06,0x07,0x00,0x00,0x00 };
	content[4] = bufferId;
	content[5] = (uint8_t)(pageId >> 8);
	content[6] = (uint8_t)pageId;
	sendCmd(6, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
ɾ��ָ�ơ�
ɾ��ָ�� ID �ſ�ʼ�� N ��ָ��ģ�塣
Params:
pageId: ָ�ƿ�λ�ã���0��ʼ��
count: Ҫɾ���ĸ�����
*/
uint8_t FPMXX::deleteFinger(uint16_t pageId, uint16_t count)
{
	uint8_t content[] = { 0x01,0x00,0x07,0x0C,0x00,0x00,0x00,0x00 };
	content[4] = (uint8_t)(pageId >> 8);
	content[5] = (uint8_t)pageId;
	content[6] = (uint8_t)(count >> 8);
	content[7] = (uint8_t)count;
	sendCmd(7, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
���ָ�ƿ⡣
*/
uint8_t FPMXX::emptyFinger()
{
	uint8_t content[] = { 0x01,0x00,0x03,0x0D };
	sendCmd(3, content);
	uint8_t r = getReply(content);
	if (r == 1)
		return content[0];
	return r;
}

/*
��ȷ�ȶ� CharBuffer1 �� CharBuffer2 �е������ļ���
�������������е����ݲ��䡣
Params:
score: ��¼�ȶԵ÷֡�
*/
uint8_t FPMXX::contrastFinger(uint16_t * score)
{
	uint8_t content[] = { 0x01,0x00,0x03,0x03 };
	sendCmd(3, content);
	uint8_t r = getReply(content);
	if (r == 3) {
		if (content[0] != FPMXX_CODE_OK) *score = 0;
		else {
			*score = content[1];
			*score <<= 8;
			*score |= content[2];
		}
		return content[0];
	}
	return r;
}

/*
��buffer�е���������ָ�ƿ⡣
Params:
bufferId: 1~2��
startPage: ��ʼҳ�档
endPage: ����ҳ�롣
pageId: ��¼��������ҳ�롣
score: ��¼�ȶԵ÷֡�
*/
uint8_t FPMXX::searchFinger(uint8_t bufferId, uint16_t startPage, uint16_t endPage, uint16_t * pageId, uint16_t * score)
{
	uint8_t content[] = { 0x01,0x00,0x08,0x04,0x00,0xFF,0xFF,0x00,0x00 };
	content[4] = bufferId;
	content[5] = (uint8_t)(startPage >> 8);
	content[6] = (uint8_t)startPage;
	content[7] = (uint8_t)(endPage >> 8);
	content[8] = (uint8_t)endPage;
	sendCmd(8, content);
	uint8_t r = getReply(content);
	if (r == 5) {
		if (content[0] != FPMXX_CODE_OK) *score = 0;
		else {
			*pageId = content[1];
			*pageId <<= 8;
			*pageId |= content[2];
			*score = content[3];
			*score <<= 8;
			*score |= content[4];
		}
		return content[0];
	}
	return r;
}

uint8_t FPMXX::searchFinger(uint8_t bufferId, uint16_t startPage, uint16_t endPage, uint16_t * pageId)
{
	uint16_t s;
	return searchFinger(bufferId, startPage, endPage, pageId, &s);
}

/*
�򴫸����������ݣ����
Params:
len: ͨ��Э���еİ�����ֵ
content: Ҫ���͵����ݣ��Ӱ���ʶ��ʼ��
*/
void FPMXX::sendCmd(uint16_t len, uint8_t *content)
{
	mySerial->listen();
	uint8_t i;

	//����У���
	uint16_t sum = 0;
	for (i = 0; i < len + 1; i++)
		sum += *(content + i);
	//�ȴ�������
	mySerial->flush();

	//���Ͱ�ͷ
	for (i = 0; i < 2; i++)
		mySerial->write(Pack_Head[i]);

	//����ģ���ַ
	for (i = 0; i < 4; i++)
		mySerial->write(Adder[i]);

	//����ָ������
	for (i = 0; i < len + 1; i++)
	{
		mySerial->write(*content);
		content++;
	}

	//����У���
	mySerial->write((uint8_t)(sum >> 8));
	mySerial->write((uint8_t)sum);
}

/*
���մ������ķ������ݲ��������������
�����ش�ȷ������У���֮ǰ�������볤��
param:
pkg: ���ڴ洢�������ݵı���
timeout: ��ʱ���أ����룩
return: �������ݳ��ȣ���FPMXX_REPLY_��ͷ�����룻
*/
uint8_t FPMXX::getReply(uint8_t pkg[], uint16_t timeout)
{
	uint16_t timer = 0;
	uint16_t len = 0; //ͨ��Э���а����ȵ�ֵ
	uint8_t reply[40], i = 0;
	uint8_t pkgType; //ͨ��Э���а���ʶ��ֵ
	while (1)
	{
		while (!mySerial->available())
		{
			delay(1);
			timer++;
			if (timer >= timeout) return FPMXX_REPLY_TIME_OUT;
		}

		reply[i] = mySerial->read();
		//Serial.println(reply[i],HEX);

		//�������
		if (i == 1)
		{
			if (reply[0] != Pack_Head[0] ||
				reply[1] != Pack_Head[1])
				//��ͷ������
				return FPMXX_REPLY_BAD_PACKAGE;
		}
		else if (i == 5)
		{
			if (reply[2] != Adder[0] ||
				reply[3] != Adder[1] ||
				reply[4] != Adder[2] ||
				reply[5] != Adder[3])
				//��ַ������
				return FPMXX_REPLY_BAD_PACKAGE;
		}
		else if (i == 6) pkgType = reply[6]; //ȡ����ʶ
		else if (i == 8) //ȡ������
		{
			len = reply[7];
			len <<= 8;
			len |= reply[8];
		}
		if (i == 8 + len) break; //��Ϊi��0��ʼ���Բ���9����8
		i++;
	}

	//���ݽ������
	if (i == 8 + len)
	{
		//У��
		uint16_t sum = 0;
		for (i = 6; i < 6 + len + 1; i++)
			sum += reply[i];

		if (((uint8_t)(sum >> 8)) != reply[7 + len] ||
			((uint8_t)sum) != reply[8 + len])
			//У�鲻ͨ��
			return FPMXX_REPLY_BAD_PACKAGE;

		len -= 2; //ȥ������У���
		for (i = 0; i < len; i++) //��ȷ���뿪ʼ����
			pkg[i] = reply[9 + i];
		return len;
	}
}

uint8_t FPMXX::getReply(uint8_t pkg[])
{
	return getReply(pkg, FPMXX_DEFAULT_REPLY_TIME_OUT);
}