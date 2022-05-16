#pragma once
#include <functional>
#include "../Obj/Type/ObjType.h"
#include "../../Input/MouseInputID.h"
#include "../../common/FontMng.h"
#include "../../common/Vector2.h"


class UI
{
public:
	UI();
	~UI();

	virtual void Draw(bool frame = false, int fontsize = 20, std::string str = "") = 0;

	//�����蔻��
	virtual bool CheckColition(Vector2 otherpos) = 0;
	//�{�^�����ʃ`�F�b�N
	bool CheckButtonInput(int inputid, int oldinputid);

	Vector2 GetPos(void)
	{
		return pos_;
	}
	void SetPos(Vector2 pos);

	Vector2 GetSize(void)
	{
		return size_;
	}

	std::function<bool(void)> GetFunction(void)
	{
		return function_;
	}

	std::function<bool(int)> GetFunction2(void)
	{
		return function2_;
	}

	virtual char* GetMapName(void)
	{
		//���ł͉����Ԃ��Ȃ�
		return nullptr;
	}

	MouseInputID GetCheckId(void)
	{
		return mouseId_;
	}

	std::string GetString(void)
	{
		return str_;
	}

	void SetModelID(int id)
	{
		modelKey_ = id;
	}
	int GetModelID(void)
	{
		return modelKey_;
	}

	ObjType GetObjType(void)
	{
		return objType_;
	}

protected:

	Vector2 pos_;
	Vector2 size_;
	std::string str_;

	std::function<bool(void)> function_;
	std::function<bool(int)> function2_;

	int modelKey_;

	//������炵�����ǂ���
	bool IsMakeSE_;

	//�`�F�b�N����{�^��
	MouseInputID mouseId_;

	ObjType objType_;
};

