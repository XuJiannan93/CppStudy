#include "CELLConfig.h"
#include "CELLLog.h"

void CELLConfig::Init(int argc, char* args[])
{
	_exePath = args[0];

	for (int i = 1; i < argc; i++)
	{
		//CELLLog::Debug(args[i]);
		_MakeCMD(args[i]);
	}
}

const char* CELLConfig::GetExePath() const
{
	return _exePath.c_str();
}

const char* CELLConfig::GetString(const char* key, const char* def) const
{
	auto itr = _kv.find(key);
	if (itr == _kv.end())
	{
		CELLLog_Error("CELLConfig::GetString() Not Found <%s>", key);
	}
	else {
		def = itr->second.c_str();
		CELLLog_Info("CELLConfig::GetString() set <%s>|<%s>", key, def);
	}
	return def;
}

int CELLConfig::GetInt(const char* key, int def) const
{
	auto itr = _kv.find(key);
	if (itr == _kv.end())
	{
		CELLLog_Error("CELLConfig::GetInt() Not Found <%s>", key);
	}
	else {
		def = atoi(itr->second.c_str());
		CELLLog_Info("CELLConfig::GetInt() set <%s>|<%d>", key, def);
	}
	return def;
}

bool CELLConfig::HasKey(const char* key) const
{
	auto iter = _kv.find(key);
	return iter != _kv.end();
}

void CELLConfig::_MakeCMD(char* cmd)
{
	char* val = strchr(cmd, '=');
	if (val)
	{
		*val = '\0';
		val++;
		_kv[cmd] = val;
		CELLLog_Debug("make cmd: key<%s>|value<%s>", cmd, val);
	}
	else
	{
		_kv[cmd] = "";
		CELLLog_Debug("make cmd: key<%s>", cmd);
	}
}
