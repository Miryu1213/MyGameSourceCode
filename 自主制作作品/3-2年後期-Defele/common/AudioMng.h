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
	//�摜�Ăяo���p�B�����̓t�@�C�����Ăяo���L�[ �Ԃ�l��Vector��int
	const int& GetID(std::string key);
	//f_name��key��ʂɂ��邽��
	const int& GetID(std::string f_name, std::string key);
	
	bool IsKey(std::string key);
	void DeleteID(const std::string& key);

	void SetVolume(const std::string& key, int volume);
	void RingSound(const std::string& key, int dxPlayType);

	//�S�Ẳ��O���t�B�b�N�n���h���̎擾
	const std::map<std::string, int>& GetAudioMap(void)
	{
		return audioMap_;
	}

	//�S�Ẳ��̍Đ��I��
	void StopMusicAll(void);

private:
	AudioMng();
	~AudioMng();
	//��������\�������邽�߁AVecter<int>��p����
	std::map<std::string, int> audioMap_;
};

