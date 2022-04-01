#pragma once
#include"KeyBoard.h"

class EnemyInput :
    public KeyBoard
{
public:
    EnemyInput();
    ~EnemyInput();
private:
    bool Update(obj& Obj, ObjList& objlist) override;

    int count_;
};

