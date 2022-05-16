言語　　　　　　： C++
使用エンジン　　： DXライブラリ
プラットフォーム： PC
制作時期　　　　： 2021年9～2022年1月（2年後期）
制作人数　　　　： 一人
ジャンル　　　　： 戦術シミュレーション

[ゲーム概要]
※（）は当該箇所のファイル名です。
迫りくる大量の敵から自分のコアを守りながら、相手のコアを破壊する戦術シミュレーションゲームです。
当作品では相手のコアまでの最短移動ルートの算出として、ブレゼンハムアルゴリズムを用いたA*アルゴリズムを活用し実装を行いました。(AStarMng)
A*アルゴリズムによる計算や画像読み込みなどによって画面が静止するのを防ぐため、スレッドや非同期読み込みを活用しています。(TipsScene)
また、大量のユニットや施設といった大量のオブジェクトの生成と破棄を必要とするのでデザインパターンであるオブジェクトプールを活用し、
処理速度の向上を行っています。(GameScene)


[フォルダ構造]
Defele
│  Defele.exe(作品の実行はここです)
│  Icon.ico
│  IconID.rc
│  main.cpp
│  Project1.sln
│  readme.txt
│
├─common
│  │  AudioMng.cpp
│  │  AudioMng.h
│  │  Collision.h
│  │  csv.h
│  │  Easing.h
│  │  FontMng.cpp
│  │  FontMng.h
│  │  ImageMng.cpp
│  │  ImageMng.h
│  │  Vector2.h
│  │  Vector3.cpp
│  │  Vector3.h
│  │
│  ├─Deteals
│  │      Vector2.h
│  │
│  └─Directing
│          MoveObject.h
│
├─Input
│      Input.cpp
│      Input.h
│      InputID.h
│      JoyPad.cpp
│      JoyPad.h
│      KeyBoard.cpp
│      KeyBoard.h
│      Mouse.cpp
│      Mouse.h
│      MouseInputID.h
│
├─Resource
│  ├─Audio
│  │  ├─BGM
│  │  └─SE
│  │      ├─Button
│  │      └─Unit
│  │
│  ├─image
│  │  ├─Background
│  │  ├─core
│  │  ├─effect
│  │  │  ├─BackLight
│  │  │  ├─BlinkLight
│  │  │  ├─Convergence
│  │  │  ├─Explosion
│  │  │  ├─Fire
│  │  │  ├─OneBackLight
│  │  │  └─wind
│  │  │
│  │  ├─Plant
│  │  │  └─crack
│  │  │
│  │  ├─UI
│  │  │  ├─CountDown
│  │  │  ├─Result
│  │  │  ├─Select
│  │  │  ├─Signal
│  │  │  ├─Status
│  │  │  └─Title
│  │  └─Unit
│  │
│  └─TextData
│          MapTips.csv
│
├─Scene
│  │  BaseScene.cpp
│  │  BaseScene.h
│  │  CountDownScene.cpp
│  │  CountDownScene.h
│  │  GameScene.cpp
│  │  GameScene.h
│  │  ResultScene.cpp
│  │  ResultScene.h
│  │  SceneMng.cpp
│  │  SceneMng.h
│  │  SelectScene.cpp
│  │  SelectScene.h
│  │  TipsScene.cpp
│  │  TipsScene.h
│  │  TitleScene.cpp
│  │  TitleScene.h
│  │  TutorialScene.cpp
│  │  TutorialScene.h
│  │
│  ├─Deploy
│  │  │  DeployMng.cpp
│  │  │  DeployMng.h
│  │  │
│  │  └─Gimmick
│  │          BaseGimmick.cpp
│  │          BaseGimmick.h
│  │          GimmickType.h
│  │          Hurricane.cpp
│  │          Hurricane.h
│  │
│  ├─Obj
│  │  │  AnimTbl.h
│  │  │  Core.cpp
│  │  │  Core.h
│  │  │  Obj.cpp
│  │  │  Obj.h
│  │  │  RouteCtl.cpp
│  │  │  RouteCtl.h
│  │  │  SearchMapCtl.cpp
│  │  │  SearchMapCtl.h
│  │  │
│  │  ├─Plant
│  │  │      AllPlantHeader.h
│  │  │      ArcherPlant.cpp
│  │  │      ArcherPlant.h
│  │  │      KnightPlant.cpp
│  │  │      KnightPlant.h
│  │  │      Plant.cpp
│  │  │      Plant.h
│  │  │      PlantFlag.cpp
│  │  │      PlantFlag.h
│  │  │      TrailLight.cpp
│  │  │      TrailLight.h
│  │  │      WarriorPlant.cpp
│  │  │      WarriorPlant.h
│  │  │
│  │  ├─RouteAlgorithm
│  │  │      ANode.cpp
│  │  │      ANode.h
│  │  │      AStarMng.cpp
│  │  │      AStarMng.h
│  │  │      Bresenham.cpp
│  │  │      Bresenham.h
│  │  │
│  │  ├─Type
│  │  │      ArmyType.h
│  │  │      GenerateType.h
│  │  │      ObjType.h
│  │  │      State.h
│  │  │
│  │  └─Unit
│  │          AllUnitHeader.h
│  │          Archer.cpp
│  │          Archer.h
│  │          ArcherArrow.cpp
│  │          ArcherArrow.h
│  │          Knight.cpp
│  │          Knight.h
│  │          Smoke.cpp
│  │          Smoke.h
│  │          Unit.cpp
│  │          Unit.h
│  │          UnitDefState.h
│  │          Warrior.cpp
│  │          Warrior.h
│  │
│  ├─SystemObj
│  │      Camera.cpp
│  │      Camera.h
│  │      Cursol.cpp
│  │      Cursol.h
│  │      InputString.cpp
│  │      InputString.h
│  │      Signal.cpp
│  │      Signal.h
│  │      Tips.cpp
│  │      Tips.h
│  │      WarSituationGraph.cpp
│  │      WarSituationGraph.h
│  │
│  ├─tmx
│  │  │  map0.tmx
│  │  │  map1.tmx
│  │  │  map2.tmx
│  │  │  map4.tmx
│  │  │  MapObject.cpp
│  │  │  MapObject.h
│  │  │  rapidxml.hpp
│  │  │  rapidxml_iterators.hpp
│  │  │  rapidxml_print.hpp
│  │  │  rapidxml_utils.hpp
│  │  │  TmxObj.cpp
│  │  │  TmxObj.h
│  │  │
│  │  └─image
│  │
│  ├─Transition
│  │      FadeInOut.cpp
│  │      FadeInOut.h
│  │      FadeOut.cpp
│  │      FadeOut.h
│  │      Transition.cpp
│  │      Transition.h
│  │
│  └─UI
│          Box.cpp
│          Box.h
│          Circle.cpp
│          Circle.h
│          Place.h
│          Square.cpp
│          Square.h
│          UI.cpp
│          UI.h
│
└─_debug
        _DebugConOut.cpp
        _DebugConOut.h
        _DebugDispOut.cpp
        _DebugDispOut.h