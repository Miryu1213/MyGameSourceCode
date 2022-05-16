#include<string>
#include "EnemyInput.h"
#include"EnemyMoveCtl.h"

EnemyInput::EnemyInput()
{
    count_ = 0;
}

EnemyInput::~EnemyInput()
{
}

bool EnemyInput::Update(obj& Obj, ObjList& objlist)
{
    //敵だけ
    if (Obj.GetObjType() != ObjType::Enemy)
    {
        return false;
    }
    for (auto& data : data_)
    {
        olddata_[data.first] = data.second;
        //ゼロクリア
        data.second = 0;
    }
    //スライム
    if (Obj.GetKey() == "Enemy1")
    {
        //落下
        if (CheckFallEne()(Obj))
        {
          //何もしない
        } 
        else
        {
            if (Obj.GetdirLR() == DIR_LR::L)
            {
                //左向き
                data_["左"] = 1;
            }
            if (Obj.GetdirLR() == DIR_LR::R)
            {
                //右向き
                data_["右"] = 1;
            }
        }
    }
    //フラワー
    if (Obj.GetKey() == "Enemy2")
    {
        if ((count_ % 100) == 20)
        {
            data_["A"] = 1;
            if (Obj.GetShotFlag())
            {
                data_["A"] = 0;
            }
        }
    }
    //ワーウルフ
    if (Obj.GetKey() == "Enemy3")
    {
        //落下
        if (CheckFallEne()(Obj))
        {
            //何もしない
        }
        else
        {
            for (auto pllist : objlist)
            {
                if (pllist->GetObjType() != ObjType::Player)
                {
                    continue;
                }
                auto plpos = pllist->GetPos();
                auto enepos = Obj.GetPos();
                Vector2 tmpsize = Vector2{ 138,96 };
                Vector2 tmppos = pllist->GetPos();
                if (pllist->GetdirLR() == DIR_LR::L)
                {
                    tmpsize.x -= 90;
                    tmppos.x += 15;
                }
                else
                {
                    tmpsize.x -= 90;
                    tmppos.x += 100;
                }
                if (plpos.y < enepos.y)
                {
                    //プレイヤーの下
                    if (enepos.x <= tmppos.x + tmpsize.x-20 &&
                        enepos.x + 80 >= tmppos.x)
                    {
                        if (Obj.GetState() != State::WALK)
                        {
                            data_["上"] = 1;
                        }
                    }
                }
                Vector2 warpos = Obj.GetPos();
                Vector2 warsize = Vector2{ 131,102 };
                //ワーウルフ剣の座標
                if (pllist->GetdirLR() == DIR_LR::L)
                {
                    warpos.x += 40;
                    warsize.x -= 0;
                }
                else
                {
                    warpos.x += 0;
                    warsize.x -= 40;
                }
                if ((plpos.y <= enepos.y + 102) &&				//プレイヤーの上が敵の下より小さい時
                    (plpos.y + 96) >= enepos.y)					//プレイヤーの下が敵の上より大きい時
                {
                    //Y軸があっているとき
                    if ((tmppos.x <= enepos.x + warsize.x) &&			//プレイヤーの左が敵の右より小さい時
                        (tmppos.x + tmpsize.x) >= warpos.x)				//プレイヤーの右が敵の左より大きい時)
                    {
                        data_["A"] = 1;
                    }
                }
                if (data_["A"] == 0)
                {
                    if (Obj.GetPos().x > tmppos.x)
                    {
                        //if (Obj.GetdirLR() == DIR_LR::L)
                        {
                            //左向き
                            data_["左"] = 1;
                        }
                    }
                    else
                    {
                        //if (Obj.GetdirLR() == DIR_LR::R)
                        {
                            //右向き
                            data_["右"] = 1;
                        }
                    }
                }
            }
        }
    }
    count_++;
    return false;
}
