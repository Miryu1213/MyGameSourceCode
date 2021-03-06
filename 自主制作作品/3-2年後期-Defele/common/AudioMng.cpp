#include <DxLib.h>
#include "AudioMng.h"

#include"../_debug/_DebugConOut.h"

constexpr int LIMIT_SOUND = 18;

const int& AudioMng::GetID(std::string key)
{
	//keyの要素がimageMap_内に存在するかチェック
	if (audioMap_.find(key) == audioMap_.end())
	{
		//存在していないとき
		audioMap_[key] = LoadSoundMem(key.c_str());
		if ((audioMap_[key]) == -1)
		{
			//assert(!"読み込み失敗");
			TRACE("keyName:%s読み込み失敗\n", key.c_str());
		}
	}
	return audioMap_[key];
}

const int& AudioMng::GetID(std::string f_name, std::string key)
{
	//keyの要素がimageMap_内に存在するかチェック
	if (audioMap_.find(key) == audioMap_.end())
	{
		//存在していないとき
		audioMap_[key] = LoadSoundMem(f_name.c_str());
		if ((audioMap_[key]) == -1)
		{
			//assert(!"読み込み失敗");
			TRACE("%s読み込み失敗\n", f_name.c_str());
		}
	}
	return audioMap_[key];
}

bool AudioMng::IsKey(std::string key)
{
	return audioMap_.find(key) != audioMap_.end();
}

void AudioMng::DeleteID(const std::string& key)
{
	DeleteMusicMem(audioMap_[key]);
	audioMap_.erase(key);
}

void AudioMng::SetVolume(const std::string& key, int volume)
{
	if (audioMap_.find(key) != audioMap_.end())
	{
		volume = max(min(volume, 255), 0);
		ChangeVolumeSoundMem(volume, audioMap_[key]);
	}
}

void AudioMng::RingSound(const std::string& key, int dxPlayType)
{
	PlaySoundMem(audioMap_[key], dxPlayType);
}

void AudioMng::StopMusicAll(void)
{
	for (auto& audioGh : audioMap_)
	{
		//再生されていると止める
		if (CheckSoundMem(audioGh.second))
		{
			StopSoundMem(audioGh.second);
		}
	}
}

AudioMng::AudioMng()
{
}

AudioMng::~AudioMng()
{
}
