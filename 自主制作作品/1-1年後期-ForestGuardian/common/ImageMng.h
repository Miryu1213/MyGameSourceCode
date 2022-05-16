#pragma once
#include<map>
#include<vector>
#include<string>
#include"vector2.h"

//�V���O���g���N���X���Ƒ�����A�N�Z�X����ɂ͕K��static����s��
#define lpImageMng ImageMng::GetInstance()

//vector<�ς̔z��̌^>
using Vecint = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_instance;
		return s_instance;
	}
	//�摜�擾
	const Vecint& GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt);
	//�ꖇ�����̉摜�擾 �Ԃ�l�͉ς̔z��ϐ��B
	const Vecint& GetID(std::string key);

	//����O���t�B�b�N�n���h��
	int atack1[3];
	int atack2[3];
private:
	ImageMng();
	~ImageMng();
	//std::map<�ǂݍ��ތ^,�ς������^>
	std::map<std::string, Vecint> imgMap_;
};



