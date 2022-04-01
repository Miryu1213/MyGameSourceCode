#pragma once
#include <map>
#include <string>
#include "InputID.h"
#include "Mouse.h"

enum class InputClass
{
	KETBOARD,
	PAD
};

//�L�[���̓f�[�^���
using InputData = std::map<InputID, int>;

//�}�E�X���̓f�[�^�ƍ��W
using MouseInputData = std::map<MouseInputID, int>;

class Input
{
public:
	Input();
	virtual ~Input();

	virtual bool Init(void) = 0;
	virtual bool Update(void) = 0;

	//���͏��̎擾
	const InputData& GetInputData(InputType type);

	//�}�E�X���̎擾
	const MouseInputData GetMouseData(InputType inputtype);
	//�}�E�X�֍��W���Z�b�g
	void SetMousePos(Vector2 pos);

	virtual InputClass GetInputClass(void) = 0;

protected:
	InputData Inputdata_;			//���̓L�[
	InputData oldInputdata_;		//�ЂƂO�̓��̓L�[

	Mouse mouse_;
};


