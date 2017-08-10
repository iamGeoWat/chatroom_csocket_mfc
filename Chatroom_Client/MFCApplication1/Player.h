#pragma once
class Player
{
private:
	int level;
	// ×ø±ê£¨ÔÝ¶¨£©
	int x;
	int y;
	CString nickname;
	CString ID;
	CString psword;
	CString filename;
public:
	Player();
	Player(CString ID, CString psword, CString nickname);
	~Player();
	void setLocation(int x, int y);
	int getX();
	int getY();
	int getLevel();
	void setLevel(int level);
	void setNickname(CString name);
	CString getNickname();
	void setID(CString ID);
	CString getID();
	void setPsword(CString psword);
	CString getPsword();
	CString getFilename();
	void Serialize(CArchive &ar);
	void WriteFile();
	void ReadFile();
	void CreateFileW();
};

