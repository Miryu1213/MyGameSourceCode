#pragma once
#include <DxLib.h>
#include <memory>
#include <list>
#include <vector>
#include "../../Input/Mouse.h"
#include "../../common/Vector2.h"

class Input;

class Cursol
{
public:
	Cursol();
	~Cursol();

	void Init(void);
	void Update(void);
	void Draw(bool isTutorial);

	Vector2 GetPos(void) { return pos_; };

	MouseInputData GetMouseInput(InputType inputtype);

private:
	Vector2 pos_;
	int speed_;

	Vector2 size_;

	//���͏��
	std::unique_ptr<Input> input_;

	//�L�[�{�[�h���������邩�ۂ�
	bool keyChecker;

	//��ʃT�C�Y
	Vector2 Screensize_;

	//����
	std::vector<Vector2> waterPosVec_;
	std::vector<double> waterTimeVec_;
};

