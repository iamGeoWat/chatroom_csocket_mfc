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
		//写变量的数据到文件
		ar << ID;
		ar << psword;
		ar << nickname;
		ar << x;
		ar << y;
		ar << level;
	}
	else
	{
		//读文件内容并保存到变量：
		ar >> ID;
		ar >> psword;
		ar >> nickname;
		ar >> x;
		ar >> y;
		ar >> level;

	}
}


//数据写文件
void Player::WriteFile()
{
	CFile file;
	if (file.Open(filename, CFile::modeWrite, NULL))//创建磁盘文件并打开
	{
		CArchive ar(&file, CArchive::store);//根据打开的文件，创建文件串行化对象
		Serialize(ar); //写文件内容

					   //结束后关闭对象
		ar.Close();
		file.Close();
	}
}


//从文件上读数据
void Player::ReadFile()
{
	CFile file;

	if (file.Open(filename, CFile::modeRead, NULL))//打开磁盘文件
	{
		CArchive ar(&file, CArchive::load);//根据打开的文件，创建文件串行化对象
		Serialize(ar); //读文件内容

					   //结束后关闭对象
		ar.Close();
		file.Close();
	}
}

void Player::CreateFileW()
{
	CFile file;
	if (file.Open(filename, CFile::modeCreate, NULL))//创建磁盘文件并打开
	{
		file.Close();
	}
}
