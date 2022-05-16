言語　　　　　　： C++
使用エンジン　　： DXライブラリ
プラットフォーム： PC
制作時期　　　　： 2021年1月（1年後期）
制作人数　　　　： 一人
ジャンル　　　　： 2Dアクション

[ゲーム概要]
※（）内は当該箇所のファイル名です。
制限時間まで生き延びることが目的の2Dアクションゲームです。
当作品ではマップエディターと搭載し、自由にマップのエディットができるように実装を行っています。
また、マップデータはバイナリーデータで保存しロードのさいにエラーチェックを行っています。（MapCtl）


[フォルダ構造]
ForestGuardian
│  ForestGuardian.exe
│  ForestGuardian.sln
│  main.cpp
│  mapdata.txt
│
├─common
│  │  Easing.h
│  │  ImageMng.cpp
│  │  ImageMng.h
│  │  vector2.h
│  │
│  └─deteals
│          vector2.h
├─image
├─music
├─Scene
│  │  BaseScene.cpp
│  │  BaseScene.h
│  │  Camera.cpp
│  │  Camera.h
│  │  DrawChip.h
│  │  EditScene.cpp
│  │  EditScene.h
│  │  fadeinout.cpp
│  │  fadeinout.h
│  │  GameScene.cpp
│  │  GameScene.h
│  │  MapCtl.cpp
│  │  MapCtl.h
│  │  SceneID.h
│  │  SceneMng.cpp
│  │  SceneMng.h
│  │  TitleScene.cpp
│  │  TitleScene.h
│  │  Trandition.cpp
│  │  Trandition.h
│  │
│  ├─Input
│  │      EnemyInput.cpp
│  │      EnemyInput.h
│  │      EnemyMoveCtl.h
│  │      Input.cpp
│  │      Input.h
│  │      inputSelecter.h
│  │      JoyPad.cpp
│  │      JoyPad.h
│  │      KeyBoard.cpp
│  │      KeyBoard.h
│  │      Mouse.cpp
│  │      Mouse.h
│  │
│  └─Obj
│          ChipGP.cpp
│          ChipGP.h
│          ChipID.cpp
│          ChipID.h
│          EditCar.cpp
│          EditCar.h
│          Enemy.cpp
│          Enemy.h
│          Flour.cpp
│          Flour.h
│          MoveCtl.h
│          Obj.cpp
│          Obj.h
│          Player.cpp
│          Player.h
│          Slime.cpp
│          Slime.h
│          Unit.cpp
│          Unit.h
│          Warwolf.cpp
│          Warwolf.h
│
└─_debug
        _DebugConOut.cpp
        _DebugConOut.h
        _DebugDispOut.cpp
        _DebugDispOut.h