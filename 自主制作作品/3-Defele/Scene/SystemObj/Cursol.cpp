#include "Cursol.h"
#include "../../Input/KeyBoard.h"
#include "../../Input/JoyPad.h"
#include "../../common/ImageMng.h"
#include "../SceneMng.h"
#include "../SystemObj/InputString.h"
#include "../../common/Easing.h"

Cursol::Cursol()
{
}

Cursol::~Cursol()
{
}

void Cursol::Init(void)
{
	//入力情報の取得
	if (GetJoypadNum())
	{
		input_ = std::make_unique<JoyPad>();
	}
	else
	{
		input_ = std::make_unique<KeyBoard>();
	}

	input_->Init();

	//初期化
	speed_ = 8;

	//マウスカーソルの削除
#ifndef _DEBUG
	SetMouseDispFlag(false);
#endif

	//カーソルのサイズの取得
	GetGraphSize(lpImageMng.GetID("Resource/image/UI/cursol.png", "Cursol")[0], &size_.x_, &size_.y_);

	//トレイルの初期化
	TrailClear();

	//画面サイズの取得
	int tmpcolor = 0;
	GetScreenState(&Screensize_.x_, &Screensize_.y_, &tmpcolor);
}

void Cursol::Update(void)
{
	input_->Update();
	//キー情報の取得
	auto inputdata = input_->GetInputData(InputType::NOW);
	auto oldinputdata = input_->GetInputData(InputType::OLD);

	//キーボード操作チェック
	for (auto& data : inputdata)
	{
		if (data.second != 0)
		{
			keyChecker = true;
			break;
		}
	}

	//マウス情報の取得
	auto m_data = GetMouseInput(InputType::NOW);
	auto m_dataold = GetMouseInput(InputType::OLD);

	Vector2 mousepos = { m_data[MouseInputID::POSX] ,m_data[MouseInputID::POSY] };
	Vector2 oldmousepos = { m_dataold[MouseInputID::POSX] ,m_dataold[MouseInputID::POSY] };

	//マウスの座標が前回と違えばマウス入力にする
	if (mousepos != oldmousepos)
	{
		keyChecker = false;
		pos_ = mousepos;
	}

	int add =  speed_ + Tolerance;

	//座標の更新
	if (0 <= pos_.y_ - speed_)
	{
		if (inputdata[InputID::UP])
		{
			pos_.y_ -= speed_;
		}
	}
	if (pos_.y_ + add <= Screensize_.y_)
	{
		if (inputdata[InputID::DOWN])
		{
			pos_.y_ += speed_;
		}
	}
	if (pos_.x_ + add <= Screensize_.x_)
	{
		if (inputdata[InputID::RIGHT])
		{
			pos_.x_ += speed_;
		}
	}
	if (0 <= pos_.x_ - speed_)
	{
		if (inputdata[InputID::LEFT])
		{
			pos_.x_ -= speed_;
		}
	}

	//キーボード処理がない場合、マウス座標の更新
	if (!keyChecker)
	{
		input_->SetMousePos(pos_);
	}

	//左クリックされたときの水面
	if (m_data[MouseInputID::LEFT] && !m_dataold[MouseInputID::LEFT])
	{
		waterTimeVec_.push_back(WaterSurfaceTime);
		waterPosVec_.push_back(pos_);
	}

	for (auto& waterTime : waterTimeVec_)
	{
		if (0 < waterTime)
		{
			//秒数カウント
			waterTime -= lpSceneMng.GetDeltaTime();
		}
	}

	for (auto waterTimeItr = waterTimeVec_.begin(); waterTimeItr != waterTimeVec_.end();)
	{
		if ((*waterTimeItr) <= 0.0)
		{
			waterTimeItr = waterTimeVec_.erase(waterTimeItr);

			//座標の削除
			waterPosVec_.erase(waterPosVec_.begin());
			continue;
		}
		waterTimeItr++;
	}

	//トレイルリストへ位置を挿入
	Vector2Flt FltPos;
	FltPos.x_ = static_cast<float>(pos_.x_);
	FltPos.y_ = static_cast<float>(pos_.y_);

	trailList_.push_front(FltPos);

	//トレイル数が限界を超えていれば古いほうから破棄
	if (trailList_.size() > trailLimit)
	{
		trailList_.pop_back();
	}
}

void Cursol::Draw(bool isTutorial)
{
	//DrawCircle(pos_.x_, pos_.y_, 5, 0xff5555);

	//トレイルの描画
	Vector2Flt lastpos;
	//ひとつ前の座標
	lastpos.x_ = static_cast<float>(pos_.x_);
	lastpos.y_ = static_cast<float>(pos_.y_);

	//Vertカウント用
	int vertCnt = 0;

	//アルファ値変更用
	byte alpha = 255;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	for (auto& pos : trailList_)
	{
		//△
		Vert[vertCnt].pos = { pos.x_ - TrailSizeH ,pos.y_,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 0.0f;
		Vert[vertCnt].v = 0.0f;

		vertCnt++;

		Vert[vertCnt].pos = { lastpos.x_ + TrailSizeH,lastpos.y_ ,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 1.0f;
		Vert[vertCnt].v = 1.0f;

		vertCnt++;

		Vert[vertCnt].pos = { lastpos.x_ - TrailSizeH,lastpos.y_ ,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 0.0f;
		Vert[vertCnt].v = 1.0f;

		vertCnt++;

		//▽
		Vert[vertCnt].pos = { pos.x_ - TrailSizeH,pos.y_,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 0.0f;
		Vert[vertCnt].v = 0.0f;

		vertCnt++;

		Vert[vertCnt].pos = { pos.x_ + TrailSizeH,pos.y_,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 1.0f;
		Vert[vertCnt].v = 0.0f;

		vertCnt++;

		Vert[vertCnt].pos = { lastpos.x_ + TrailSizeH,lastpos.y_,0 };
		Vert[vertCnt].rhw = 1.0f;
		Vert[vertCnt].dif = GetColorU8(255, 255, 255, alpha);
		Vert[vertCnt].u = 1.0f;
		Vert[vertCnt].v = 1.0f;

		vertCnt++;

		//DrawLineAA(lastpos.x_, lastpos.y_, pos_.x_, pos_.y_, 0xffffff, 10);
		//thickness *=0.95f;
		lastpos = pos;
		alpha -= 18;
	}

	//DrawPolygon2D(Vert, trailLimit, DX_NONE_GRAPH, true);

	DrawGraph(pos_.x_, pos_.y_, lpImageMng.GetID("Cursol")[0], true);

	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	for (int i = 0; i < static_cast<int>(waterTimeVec_.size()); i++)
	{
		if (0 < waterTimeVec_[i])
		{
			//倍率
			int doublepoint = 18;

			auto amountofMove = Easing::OutCirc(waterTimeVec_[i], WaterSurfaceTime);

			//α値減少	
			int alpha = 0;

			if (waterTimeVec_[i] < WaterSurfaceTime / 4)
			{
				//残り時間が1/4のときフェードアウト
				alpha = static_cast<int>((WaterSurfaceTime - waterTimeVec_[i]) * 400);
			}
			else
			{
				alpha = static_cast<int>((WaterSurfaceTime - waterTimeVec_[i]) * 120);
			}

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);

			DrawCircle(waterPosVec_[i].x_, waterPosVec_[i].y_, static_cast<int>(amountofMove * doublepoint), 0xffffff, false);

			//半分まで描画
			if (WaterSurfaceTime / 2 < waterTimeVec_[i])
			{
				DrawCircle(waterPosVec_[i].x_, waterPosVec_[i].y_, static_cast<int>(amountofMove * doublepoint) + 1, 0xffffff, false);
			}
		}
	}

	//α値
	alpha = 255;
	if (isTutorial)
	{
		//チュートリアル時のα値を変える
		alpha = lpSceneMng.alpha_;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}

void Cursol::TrailClear(void)
{
	trailList_.clear();
}

MouseInputData Cursol::GetMouseInput(InputType inputtype)
{
	auto mouse = input_->GetMouseData(inputtype);

	//パッドの場合はパッドの入力も使用する
	if (input_->GetInputClass() == InputClass::PAD)
	{
		auto pad = input_->GetInputData(inputtype);
		mouse[MouseInputID::LEFT] |= pad[InputID::A];
		mouse[MouseInputID::RIGHT] |= pad[InputID::B];
		mouse[MouseInputID::MIDDLE] += pad[InputID::RB];
		mouse[MouseInputID::MIDDLE] -= pad[InputID::LB];

		//ボタン入力
		mouse[MouseInputID::BUTTON_X] |= pad[InputID::X];
		mouse[MouseInputID::BUTTON_Y] |= pad[InputID::Y];
	}

	return mouse;
}
