#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	level = 0;
	x = 0;
	y = 0;
}

Player::Player(CString ID, CString psword, CString nickname)
{
	level = 0;
	x = 0;
	y = 0;
	this->ID = ID;
	this->psword = psword;
	this->nickname = nickname;
	this->filename = _T("C://Users//wangj//Desktop//save//") + ID + _T(".dat");

}

Player::~Player()
{
}

CString Player::getID()
{
	return ID;
}

CString Player::getNickname()
{
	return nickname;
}

CString Player::getPsword()
{
	return psword;
}

int Player::getLevel()
{
	return level;
}

int Player::getX()
{
	return x;
}

int Player::getY()
{
	return y;
}

void Player::setID(CString ID)
{
	this->ID = ID;
}

void Player::setLevel(int level)
{
	this->level = level;
}

void Player::setLocation(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Player::setNickname(CString nickname)
{
	this->nickname = nickname;
}

void Player::setPsword(CString psword)
{
	this->psword = psword;
}

CString Player::getFilename()
{
	return filename;
}
void Player::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		//д���������ݵ��ļ�
		ar << ID;
		ar << psword;
		ar << nickname;
		ar << x;
		ar << y;
		ar << level;
	}
	else
	{
		//���ļ����ݲ����浽������
		ar >> ID;
		ar >> psword;
		ar >> nickname;
		ar >> x;
		ar >> y;
		ar >> level;

	}
}


//����д�ļ�
void Player::WriteFile()
{
	CFile file;
	if (file.Open(filename, CFile::modeWrite, NULL))//���������ļ�����
	{
		CArchive ar(&file, CArchive::store);//���ݴ򿪵��ļ��������ļ����л�����
		Serialize(ar); //д�ļ�����

					   //������رն���
		ar.Close();
		file.Close();
	}
}


//���ļ��϶�����
void Player::ReadFile()
{
	CFile file;

	if (file.Open(filename, CFile::modeRead, NULL))//�򿪴����ļ�
	{
		CArchive ar(&file, CArchive::load);//���ݴ򿪵��ļ��������ļ����л�����
		Serialize(ar); //���ļ�����

					   //������رն���
		ar.Close();
		file.Close();
	}
}

void Player::CreateFileW()
{
	CFile file;
	if (file.Open(filename, CFile::modeCreate, NULL))//���������ļ�����
	{
		file.Close();
	}
}
