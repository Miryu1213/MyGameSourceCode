言語　　　　　　： C#
使用エンジン　　： Unity
プラットフォーム： PC
制作時期　　　　： 2021年4～2022年8月（2年前期）
制作人数　　　　： 四人
ジャンル　　　　： 戦略シミュレーション

[ゲーム概要]
攻防を繰り返し自分の領地を広め、世界の統一を行う戦略シミュレーションゲームです。
当作品では初のチーム制作作品でUnityを使用して制作しました。
担当課所スクリプトはOptionフォルダのGame以外です。

[フォルダ構造]
SchlachtGuerre
│  readme.txt
│
├─Assets
│  │  Plugins.meta
│  │  Resources.meta
│  │  Scenes.meta
│  │  Script.meta
│  │  TextMesh Pro.meta
│  │
│  ├─Scenes
│  │      ClearScene.unity
│  │      GameOverScene.unity
│  │      GameScene.unity
│  │      MapScene.unity
│  │      MyTurnScene.unity
│  │      SampleScene.unity
│  │      SelectScene.unity
│  │      TitleScene.unity
│  │
│  └Script
│     │  FadeManager.cs
│     │  Option.meta
│     │  SingletonMonoBehaviour.cs
│     │
│     ├─Battle
│     │      BattleSystem.cs
│     │      button.cs
│     │      ChangeColor.cs
│     │      chara.cs
│     │      drawtext.cs
│     │      game.cs
│     │      HpBarCtrl.cs
│     │      RayCast.cs
│     │      Serect.cs
│     │      Shaking.cs
│     │      Vibration.cs
│     │
│     └─Option
│         │  Currentselected.cs
│         │  SelectedMng.cs
│         │  Selecter.cs
│         │
│         ├─Clear
│         │      ClearOption.cs
│         │
│         ├─Game
│         │      atkDfImg.cs
│         │      atkDfImg1.cs
│         │      CharImage.cs
│         │      GameMng.cs
│         │      GameOption.cs
│         │      SliderImg.cs
│         │      TextEdit.cs
│         │      TextMng.cs
│         │      TextWriter.cs
│         │      UIText.cs
│         │
│         ├─GameOver
│         │      GameOverOption.cs
│         │
│         ├─Map
│         │      MapOption.cs
│         │
│         ├─MyTurn
│         │      EnemyMove.cs
│         │      MyTurnOption.cs
│         │
│         ├─Select
│         │      SelectOption.cs
│         │
│         ├─Summary
│         │      AdaptImgMng.cs
│         │      ArmorMng.cs
│         │      GroupMng.cs
│         │      MapCtl.cs
│         │
│         └─Title
│                 TitleOption.cs
│
└─exe
　  │  SchlachtGuerre.exe
  　│  UnityPlayer.dll
 　 │
  　├─MonoBleedingEdge
  　└─SchlachtGuerre_Data