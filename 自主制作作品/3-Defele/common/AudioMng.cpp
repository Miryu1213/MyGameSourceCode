#include <DxLib.h>
#include "AudioMng.h"

#include"../_debug/_DebugConOut.h"

constexpr int LIMIT_SOUND = 18;

const int& AudioMng::GetID(std::string key)
{
	//key‚Ì—v‘f‚ªimageMap_“à‚É‘¶İ‚·‚é‚©ƒ`ƒFƒbƒN
	if (audioMap_.find(key) == audioMap_.end())
	{
		//‘¶İ‚µ‚Ä‚¢‚È‚¢‚Æ‚«
		audioMap_[key] = LoadSoundMem(key.c_str());
		if ((audioMap_[key]) == -1)
		{
			//assert(!"“Ç‚İ‚İ¸”s");
			TRACE("keyName:%s“Ç‚İ‚İ¸”s\n", key.c_str());
		}
	}
	return audioMap_[key];
}

const int& AudioMng::GetID(std::string f_name, std::string key)
{
	//key‚Ì—v‘f‚ªimageMap_“à‚É‘¶İ‚·‚é‚©ƒ`ƒFƒbƒN
	if (audioMap_.find(key) == audioMap_.end())
	{
		//‘¶İ‚µ‚Ä‚¢‚È‚¢‚Æ‚«
		audioMap_[key] = LoadSoundMem(f_name.c_str());
		if ((audioMap_[key]) == -1)
		{
			//assert(!"“Ç‚İ‚İ¸”s");
			TRACE("%s“Ç‚İ‚İ¸”s\n", f_name.c_str());
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
		if (ChangeVolumeSoundMem(volume, audioMap_[key]) == -1)
		{
			TRACE("‰¹•ÏX¸”s\n");
		}
	}
}

void AudioMng::RingSound(const std::string& key, int dxPlayType)
{
	//if (!CheckSoundMem(audioMap_[key]))
	PlaySoundMem(audioMap_[key], dxPlayType);
}

void AudioMng::StopMusicAll(void)
{
	for (auto& audioGh : audioMap_)
	{
		//Ä¶‚³‚ê‚Ä‚¢‚é‚Æ~‚ß‚é
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
