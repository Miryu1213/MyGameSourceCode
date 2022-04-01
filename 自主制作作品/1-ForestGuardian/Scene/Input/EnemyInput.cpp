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
    //�G����
    if (Obj.GetObjType() != ObjType::Enemy)
    {
        return false;
    }
    for (auto& data : data_)
    {
        olddata_[data.first] = data.second;
        //�[���N���A
        data.second = 0;
    }
    //�X���C��
    if (Obj.GetKey() == "Enemy1")
    {
        //����
        if (CheckFallEne()(Obj))
        {
          //�������Ȃ�
        } 
        else
        {
            if (Obj.GetdirLR() == DIR_LR::L)
            {
                //������
                data_["��"] = 1;
            }
            if (Obj.GetdirLR() == DIR_LR::R)
            {
                //�E����
                data_["�E"] = 1;
            }
        }
    }
    //�t�����[
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
    //���[�E���t
    if (Obj.GetKey() == "Enemy3")
    {
        //����
        if (CheckFallEne()(Obj))
        {
            //�������Ȃ�
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
                    //�v���C���[�̉�
                    if (enepos.x <= tmppos.x + tmpsize.x-20 &&
                        enepos.x + 80 >= tmppos.x)
                    {
                        if (Obj.GetState() != State::WALK)
                        {
                            data_["��"] = 1;
                        }
                    }
                }
                Vector2 warpos = Obj.GetPos();
                Vector2 warsize = Vector2{ 131,102 };
                //���[�E���t���̍��W
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
                if ((plpos.y <= enepos.y + 102) &&				//�v���C���[�̏オ�G�̉���菬������
                    (plpos.y + 96) >= enepos.y)					//�v���C���[�̉����G�̏���傫����
                {
                    //Y���������Ă���Ƃ�
                    if ((tmppos.x <= enepos.x + warsize.x) &&			//�v���C���[�̍����G�̉E��菬������
                        (tmppos.x + tmpsize.x) >= warpos.x)				//�v���C���[�̉E���G�̍����傫����)
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
                            //������
                            data_["��"] = 1;
                        }
                    }
                    else
                    {
                        //if (Obj.GetdirLR() == DIR_LR::R)
                        {
                            //�E����
                            data_["�E"] = 1;
                        }
                    }
                }
            }
        }
    }
    count_++;
    return false;
}
