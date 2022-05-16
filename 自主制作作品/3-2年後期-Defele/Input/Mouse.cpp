#include <DxLib.h>
#include "Mouse.h"

Mouse::Mouse()
{
	Init();
}

Mouse::~Mouse()
{
}

void Mouse::SetMouseInputPos(Vector2 pos)
{
	MouseInputdata_[MouseInputID::POSX] = pos.x_;
	MouseInputdata_[MouseInputID::POSY] = pos.y_;
}

MouseInputData Mouse::GetMouseData(InputType inputtype)
{
	if (inputtype == InputType::NOW)
	{
		return MouseInputdata_;
	}
	else
	{
		return oldMouseInputdata_;
	}
}

bool Mouse::Update(void)
{
	//old�փR�s�[
	for (auto& data : MouseInputdata_)
	{
		oldMouseInputdata_[data.first] = data.second;
	}

	//�}�E�X���W�̎擾
	GetMousePoint(&MouseTbl_[MouseInputID::POSX], &MouseTbl_[MouseInputID::POSY]);

	//�}�E�X����ʓ����X�V
	if (0 <= MouseTbl_[MouseInputID::POSX] && MouseTbl_[MouseInputID::POSX] <= Screensize_.x_ - Tole)
	{
		if (0 <= MouseTbl_[MouseInputID::POSY] && MouseTbl_[MouseInputID::POSY] <= Screensize_.y_ - Tole)
		{
			MouseInputdata_[MouseInputID::POSX] = MouseTbl_[MouseInputID::POSX];
			MouseInputdata_[MouseInputID::POSY] = MouseTbl_[MouseInputID::POSY];
		}
	}
	
	//�}�E�X�{�^���̎擾�Ƒ��
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		MouseInputdata_[MouseInputID::LEFT] = 1;
	}
	else
	{
		MouseInputdata_[MouseInputID::LEFT] = 0;
	}

	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		MouseInputdata_[MouseInputID::RIGHT] = 1;
	}
	else
	{
		MouseInputdata_[MouseInputID::RIGHT] = 0;
	}

	//�z�C�[���̉�]�ʂ̎擾
	MouseInputdata_[MouseInputID::MIDDLE] = GetMouseWheelRotVol();
	
	return true;
}

void Mouse::Init(void)
{
	//���͏��o�^�Ə�����
	MouseTbl_[MouseInputID::POSX] = 0;
	MouseTbl_[MouseInputID::POSY] = 0;
	MouseTbl_[MouseInputID::LEFT] = MOUSE_INPUT_LEFT;
	MouseTbl_[MouseInputID::RIGHT] = MOUSE_INPUT_RIGHT;
	MouseTbl_[MouseInputID::MIDDLE] = 0;

	//�{�^���p�@�}�E�X���ł͎g�p���Ȃ�
	MouseTbl_[MouseInputID::BUTTON_X] = 0;
	MouseTbl_[MouseInputID::BUTTON_X] = 0;

	//�O���A�N�Z�X�p�̃L�[�f�[�^�̏�����
	for (auto& tableData : MouseTbl_)
	{
		MouseInputdata_.emplace(tableData.first, 0);
	}

	GetScreenState(&Screensize_.x_, &Screensize_.y_, nullptr);
}
