#pragma once
#include <map>
#include "../common/Vector2.h"
#include "MouseInputID.h"

//�s���������e�͈�
constexpr int Tole = 18;

//�}�E�X���̓f�[�^�ƍ��W
using MouseInputData = std::map<MouseInputID, int>;

class Mouse
{
public:
	Mouse();
	~Mouse();

	bool Update(void);
	void Init(void);
	
	void SetMouseInputPos(Vector2 pos);
	MouseInputData GetMouseData(InputType inputtype);

private:
	std::map<MouseInputID, int> MouseTbl_;	//�}�E�X�e�[�u��

	MouseInputData MouseInputdata_;			//�}�E�X���
	MouseInputData oldMouseInputdata_;		//�O�t���[���̃}�E�X���

	Vector2 Screensize_;					//��ʃT�C�Y
};

