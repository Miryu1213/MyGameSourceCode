#pragma once
#include <vector>
#include <string>
#include <map>
#include "Vector2.h"

#define lpAudioMng AudioMng::GetInstance()

class AudioMng
{
public:
	static AudioMng& GetInstance()
	{
		static AudioMng s_Instance;
		return s_Instance;
	}
	//画像呼び出し用。引数はファイルを呼び出すキー 返り値はVectorのint
	const int& GetID(std::string key);
	//f_nameとkeyを別にするため
	const int& GetID(std::string f_name, std::string key);
	
	bool IsKey(std::string key);
	void DeleteID(const std::string& key);

	void SetVolume(const std::string& key, int volume);
	void RingSound(const std::string& key, int dxPlayType);

	//全ての音グラフィックハンドルの取得
	const std::map<std::string, int>& GetAudioMap(void)
	{
		return audioMap_;
	}

	//全ての音の再生終了
	void StopMusicAll(void);

private:
	AudioMng();
	~AudioMng();
	//分割する可能性があるため、Vecter<int>を用いる
	std::map<std::string, int> audioMap_;
};

