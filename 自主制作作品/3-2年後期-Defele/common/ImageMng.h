#pragma once
#include <vector>
#include <string>
#include <map>
#include "../Scene/Obj/Type/ObjType.h"
#include "../Scene/Obj/Type/ArmyType.h"
#include "Vector2.h"

#define lpImageMng ImageMng::GetInstance()

using VecInt = std::vector<int>;

enum class TOCOLOR
{
	RED,
	BLUE,
	MAX
};

enum class CHECKCOLOR
{
	RED,
	GREEN,
	BLUE,
	MAX
};

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_Instance;
		return s_Instance;
	}
	//�摜�Ăяo���p�B�����̓t�@�C�����Ăяo���L�[ �Ԃ�l��Vector��int
	const VecInt& GetID(const std::string& key);
	//��L��int��
	const VecInt& GetID(int key);
	//f_name��key��ʂɂ��邽��
	const VecInt& GetID(std::string f_name, std::string key);
	//�����p
	const VecInt& GetID(std::string f_name, std::string key, Vector2 divsize, Vector2 divcnt);
	//�L�[��int
	const VecInt& GetID(std::string f_name, int key, Vector2 divsize, Vector2 divcnt);
	//�A������ID�̃Z�b�g
	void SetID(std::string f_name, std::string key);
	//���̃L�[���̒��g�����݂��邩
	bool IsKey(std::string key);

	void GraphBlightFilter(const std::string& key, int CmpType, int CmpParam, int ClipFillFlag, unsigned int ClipFillColor, int ClipFillAlpha);

	//�h�b�g�P�ʂŉ摜�ɃA�N�Z�X
	const VecInt& GetDotID(std::string f_name, std::string key);
	const VecInt& GetDotID(std::string key);
	const VecInt& GetDotID(std::string f_name, std::string key, Vector2 divsize, Vector2 divcnt);

	//�ǂݍ��񂾉摜�̐F�̕ύX
	void SetColorAndID(const int fromColor, const TOCOLOR toColor, const std::string& f_name, const std::string& key, Vector2 divsize, Vector2 divcnt);
	//�ǂݍ��񂾉摜�̐F�̕ύX(�S��)
	void SetAllColorAndID(const TOCOLOR toColor, const std::string& f_name, const std::string& key, Vector2 divsize, Vector2 divcnt);

	bool IsDotKey(std::string key);

	//�w�肵���L�[�̃��[�h���폜
	void DeleteID(const std::string& key);
	void DeleteID(int key);

	//�I�u�W�F�N�g�^�C�v���L�[�l�[���֕ϊ�
	std::string ConvertObjTypeToKey(ObjType objtype, ArmyType armytype);

private:
	ImageMng();
	~ImageMng();
	//��������\�������邽�߁AVecter<int>��p����
	std::map<std::string, VecInt> imageMap_;

	//�h�b�g�P�ʂł̉摜�n���h��
	std::map<std::string, VecInt> dotImageMap_;
};

