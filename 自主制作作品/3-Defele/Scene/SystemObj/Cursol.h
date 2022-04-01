#pragma once
#include <DxLib.h>
#include <memory>
#include <list>
#include <vector>
#include "../../Input/Mouse.h"
#include "../../common/Vector2.h"

//�s���������e�͈�
constexpr int Tolerance = 20;

//�g���C�����E��
constexpr int trailLimit = 15;

//�g���C���֌W
constexpr int TraiSize = 12;
constexpr int TrailSizeH = TraiSize / 2;

//����
//���ʍő�\������
constexpr double WaterSurfaceTime = 0.6;

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

	//�g���C���̃t���N���A
	void TrailClear(void);

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

	//�g���C�����X�g
	std::list<Vector2Flt> trailList_;
	//�g���C���p�|���S���ۑ�
	VERTEX2D Vert[trailLimit * 6];

	//����
	std::vector<Vector2> waterPosVec_;
	std::vector<double> waterTimeVec_;
};

