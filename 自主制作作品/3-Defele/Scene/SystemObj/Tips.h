#pragma once
#include <map>

#define lpTips Tips::GetInstance()

//読み込んだチップス保存用
class Tips
{
public:
	static Tips& GetInstance(void)
	{
		static Tips s_Instance_;
		return s_Instance_;
	}

	std::string GetTip(int num);
private:
	Tips();
	~Tips() {};

	std::map<int,std::string> tips_;
};

