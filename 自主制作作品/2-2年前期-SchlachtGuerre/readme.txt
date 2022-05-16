言語　　　　　　： C#
使用エンジン　　： Unity
プラットフォーム： PC
制作時期　　　　： 2021年4～2022年8月（2年前期）
制作人数　　　　： 四人
ジャンル　　　　： 戦略シミュレーション

[ゲーム概要]
攻防を繰り返し自分の領地を広め、世界の統一を行う戦略シミュレーションゲームです。
当作品では初のチーム制作作品でUnityを使用して制作しました。

担当箇所はバトルシーン以外全般で、画面デザイン、イラスト制作、BGM、SEの実装、下コントローラーでの移動、
軍団関係スクリプト、領地関係スクリプト、「雇用」などの機能、軍団における敵の動き（どこを攻めるか、など）などと幅広く担当しました。

担当課所スクリプトはOptionフォルダ内の
「Clear」「Gameover」「Map」「MyTurn」「Select」「Summary」「Title」「Currentselected」「SelectedMng」「Selecter」です。


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
　  │  SchlachtGuerre.exe(作品の実行はここです)
  　│  UnityPlayer.dll
 　 │
  　├─MonoBleedingEdge
  　└─SchlachtGuerre_Data