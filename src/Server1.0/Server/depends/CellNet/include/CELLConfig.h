#ifndef _CELL_CONFIG_H
#define _CELL_CONFIG_H

#include <string>
#include <map>

#include "Singleton.h"

class __declspec(dllexport) CELLConfig : public Singleton<CELLConfig>
{
	friend class Singleton<CELLConfig>;

public:
	void Init(int argc, char* args[]);

	const char* GetExePath() const;
	const char* GetString(const char* key, const char* def) const;
	int GetInt(const char* key, int def) const;
	bool HasKey(const char* key) const;

private:
	CELLConfig() {}
	~CELLConfig() {}

	void _MakeCMD(char* cmd);

private:
	std::string _exePath;
	std::map<std::string, std::string> _kv;
};

#endif // !_CELL_CONFIG_H
