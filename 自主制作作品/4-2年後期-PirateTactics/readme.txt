言語　　　　　　： JavaScript
使用エンジン　　： enchant.js
プラットフォーム： PC
制作時期　　　　： 2021年12月（2年後期）
制作人数　　　　： 一人
ジャンル　　　　： タクティクスRPG

[ゲーム概要]
ターンごとにユニットを動かし敵を全滅させたら勝利のタクティクスゲームです。
当作品では「戦略シミュレーションゲームの作り方」という書籍を元に制作した作品です。
ユニットの移動計算にA*アルゴリズム、モーダンシールドを用いて目的の入力処理以外の防止、ステートマシンを用いた敵AIなど、
戦略シミュレーションゲームの基礎となるプログラムや非同期処理ならではの処理を勉強し実装を行いました。
ほとんどは書籍に記載されていた内容そのままで実装を行っていますが、敵AIについては一部オリジナルの実装として一番近くの敵に接近する突撃AIを制作しました。(main.js 1599行目)
※（）は当該箇所のファイル名です。


[フォルダ構造]
PirateTactics
│  readme.txt
│
├─build
│  │  enchant.js
│  │
│  └─plugins
│
└─game
    │
    ├─scripts
    │  └─PirateTactics
    │          index.html(作品の実行はここです)
    │          main.js
    │          stageData.js
    │
    ├─resources
    │  │
    │  ├─music
    │  └─sound
    │
    └─shared
        ├─javascript-astar
        └─jStorage-master