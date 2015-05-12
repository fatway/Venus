#pragma once

#include <map>
#include <vector>
#include <string>
#include <io.h>
#include <direct.h>
#include <ShlObj.h>

using namespace std;

typedef struct tagShortCut
{
	string fileName;
	string filePath;
}ShortCut;

// 本地资源表
class LocalPath
{
public:
	LocalPath(void);
	~LocalPath(void);

	// 初始化本地索引表
	void InitIndex();
	// 左匹配键值
	bool FindIndex(CString key, vector<CString> &matchs);
	// 精确获取值
	CString GetLocalPath(CString key);

private:
	map<CString, CString> index;
};

