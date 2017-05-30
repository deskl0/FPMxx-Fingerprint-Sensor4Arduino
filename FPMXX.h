/*********************************
* FPMXX Fingerprint Sensor for Arduino
* Version: 1.0
*
* Author: Chenhe ����
* Created: 2017/5/28
* Blog: http://chenhe.cc
*
*********************************/

#ifndef _FPMXX_h
#define _FPMXX_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <SoftwareSerial.h>

#define FPMXX_DEFAULT_REPLY_TIME_OUT 3000

/*��������ֵ���漰ͨѶ�ľ����ܷ���*/
#define FPMXX_REPLY_TIME_OUT 0xFF
#define FPMXX_REPLY_BAD_PACKAGE 0xFE
/*Ӧ����еĹ���ȷ����*/
#define FPMXX_CODE_OK 0x00
#define FPMXX_CODE_DATA_ERROR 0x01 //ģ���հ��д�
/*����ȷ����*/
#define FPMXX_VERIFY_PASSWORD_INCORRECT 0x13 //�������
#define FPMXX_GET_IMAGE_NO_FINGER 0x02 //��������û����ָ
#define FPMXX_GET_IMAGE_FAILED 0x03 //¼�벻�ɹ�
#define FPMXX_IMAGE2TZ_CHAOS 0x06 //ָ��ͼ��̫�Ҷ�����������
#define FPMXX_IMAGE2TZ_LESS 0x07 //������̫�ٶ�����������
#define FPMXX_IMAGE2TZ_NO_IMAGE 0x15 //ͼ�񻺳�����û����Чԭʼͼ
#define FPMXX_BUILD_TEMPLATE_FAILED 0x0A //�ϲ�ʧ�ܣ���öָ�Ʋ�����ͬһ��ָ��
#define FPMXX_STROE_FINGER_OVERFLOW 0x0B //PageId������Χ
#define FPMXX_STROE_FINGER_FLASH_ERR 0x18 //д FLASH ����
#define FPMXX_LOAD_FINGER_FAILED 0x0C //�����д��ģ����Ч
#define FPMXX_LOAD_FINGER_OVERFLOW 0x0B // PageID ����ָ�ƿⷶΧ
#define FPMXX_DELETE_FINGER_FAILED 0x10 //ɾ��ָ��ʧ��
#define FPMXX_EMPTY_FINGER_FAILED 0x11 //���ָ��ʧ��
#define FPMXX_CONTRAST_FINGER_MISMATCHING 0x08 //ָ�Ʋ�ƥ��
#define FPMXX_SEARCH_FINGER_MISMATCHING 0x09 //û��������ƥ��ָ��


class FPMXX
{
public:
	FPMXX(SoftwareSerial *softSerial);
	FPMXX(SoftwareSerial *softSerial, uint8_t adder[]);
	FPMXX(SoftwareSerial *softSerial, uint16_t baudRate);
	FPMXX(SoftwareSerial *softSerial, uint8_t adder[], uint16_t baudRate);
	/*����ģ���ַ������ͨѶ��������Ĭ��Ϊ0xFFFFFFFF*/
	void setCurrentAddress(uint8_t address[]);
	/*��ȡָ�ƿ�����*/
	uint16_t getFingerStoreSize(uint16_t *num);
	/*��ȡ����ָ�Ƹ���*/
	uint16_t getFingerNum(uint16_t *num);
	/*����ģ�鰲ȫ�ȼ�*/
	uint8_t setSecurityLevel(uint8_t level);
	/*����ģ������*/
	uint8_t setPassword(uint8_t password[]);
	/*��֤ģ�����루�ǳ�ʼ�����������֤��*/
	uint8_t verifyPassword();
	uint8_t verifyPassword(uint8_t password[]);
	/*¼��ָ��ͼ��*/
	uint8_t getFingerImage();
	/*ͼ������ָ������*/
	uint8_t image2tz(uint8_t bufferId);
	/*2�������ϳ�ָ��ģ��*/
	uint8_t buildTemplate();
	/*����ָ��*/
	uint8_t storeFinger(uint8_t bufferId, uint16_t pageId);
	/*��ָ�ƿ��ȡָ������*/
	uint8_t loadFinger(uint8_t bufferId, uint16_t pageId);
	/*ɾ��ָ��*/
	uint8_t deleteFinger(uint16_t pageId, uint16_t count);
	/*���ָ�ƿ�*/
	uint8_t emptyFinger();
	/*��ȷ�ȶ�2��ָ��*/
	uint8_t contrastFinger(uint16_t * score);
	/*����ָ�ƿ�*/
	uint8_t searchFinger(uint8_t bufferId, uint16_t startPage, uint16_t endPage, uint16_t *pageId, uint16_t *score);
	uint8_t searchFinger(uint8_t bufferId, uint16_t startPage, uint16_t endPage, uint16_t *pageId);

	/*���·���Ϊ�����Զ������ݼ������Ӧ���ݡ����Ƽ�ʹ�ã������������װ�õķ�����*/
	void sendCmd(uint16_t length, uint8_t *content);
	uint8_t getReply(uint8_t pkg[], uint16_t timeout);
	uint8_t getReply(uint8_t pkg[]);

private:
	SoftwareSerial *mySerial;

	uint8_t Pack_Head[2] = { 0xEF, 0x01 };
	uint8_t Adder[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
};

#endif
