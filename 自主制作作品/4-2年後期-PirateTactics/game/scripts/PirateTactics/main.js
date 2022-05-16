
/**
 * enchant.js を使う前にフレームワークを有効するための必要な処理。
 */
enchant();

window.onload = function(){

    var game = new Core(960, 640);
    game.fps =30;

    /**
     * 必要なファイルを相対パスで引数に指定する。 ファイルはすべて、ゲームが始まる前にロードされる。
     */
    //ゲームの枠
    var mapFrame  = "../../resources/mapframe.png";
    game.preload(mapFrame);

    //背景
    var mapBackground00  = "../../resources/map00.png";
    game.preload(mapBackground00);

    //マップタイル
    var mapTiles  = "../../resources/maptiles.png";
    game.preload(mapTiles);

    //船
    var shipsSpriteSheet  = "../../resources/ships.png";
    game.preload(shipsSpriteSheet);

    //海賊立ち絵
    var pirateSprites = [
        "../../resources/pirate00.png",
        "../../resources/pirate01.png",
        "../../resources/pirate02.png",
        "../../resources/pirate03.png",
    ];
    for (var i=0; i < pirateSprites.length; ++i) {
        game.preload(pirateSprites[i]);
    }

    //海賊ミニキャラ
    var pirateChibiSprites = [
        "../../resources/pirateChibi00.png",
        "../../resources/pirateChibi01.png",
        "../../resources/pirateChibi02.png",
        "../../resources/pirateChibi03.png",
        "../../resources/pirateChibi04.png",
        "../../resources/pirateChibi05.png",
        "../../resources/pirateChibi06.png",
        "../../resources/pirateChibi07.png",
    ];
    for (var i=0; i < pirateChibiSprites.length; ++i) {
        game.preload(pirateChibiSprites[i]);
    }

    //マップUI
    var mapUI = "../../resources/mapui.png";
    game.preload(mapUI);

    //爆発エフェクト
    var explosionSpriteSheet  = "../../resources/explosion.png";
    game.preload(explosionSpriteSheet);

    //1*1黒いブロック
    var ui1x1Black    = "../../resources/1x1black.png";
    game.preload(ui1x1Black);

    //ボタンクリック時のUI背景
    var uiWindowSprite    = "../../resources/window.png";
    game.preload(uiWindowSprite);

    //アラート画面UI
    var uiAlertScreen   = "../../resources/alertScreen.png";
    game.preload(uiAlertScreen);

    //初めの画面
    var uiStartScreen   = "../../resources/startScreen.png";
    game.preload(uiStartScreen);

    //ストーリーでの画面
    var uiStoryScreen   = "../../resources/storyScreen.png";
    game.preload(uiStoryScreen);

    //VSでの画面
    var uiVSScreen   = "../../resources/vsScreen.png";
    game.preload(uiVSScreen);

    //ストーリーでのボタン
    var uiStoryBtnSprite = "../../resources/btnStory.png";
    game.preload(uiStoryBtnSprite);

    //VSでのボタン
    var uiVersusBtnSprite = "../../resources/btnVS.png";
    game.preload(uiVersusBtnSprite);

    //VSでの人間選択ボタン
    var uiHumanBtnSprite = "../../resources/btnHuman.png";
    game.preload(uiHumanBtnSprite);

    //VSでのCPU選択ボタン
    var uiCpuBtnSprite = "../../resources/btnCPU.png";
    game.preload(uiCpuBtnSprite);

    //コンテニューボタン
    var uiContinueBtnSprite = "../../resources/btnContinue.png";
    game.preload(uiContinueBtnSprite);

    //新規ボタン
    var uiNewBtnSprite = "../../resources/btnNew.png";
    game.preload(uiNewBtnSprite);

    //ツイッターボタン
    var uiTwitterBtnSprite = "../../resources/twitter.png";
    game.preload(uiTwitterBtnSprite);

    //設定ボタン
    var uiSettingsSprite    = "../../resources/settings.png";
    game.preload(uiSettingsSprite);

    //キャンセルボタン
    var uiCancelBtnSprite = "../../resources/btnCancel.png";
    game.preload(uiCancelBtnSprite);

    //スキルボタン
    var uiSkillBtnSprite = "../../resources/btnSkill.png";
    game.preload(uiSkillBtnSprite);

    //矢印ボタン
    var uiArrowSprite = "../../resources/arrow.png";
    game.preload(uiArrowSprite);

    //HPバーブラック
    var uiHealthBack      = "../../resources/healthBack.png";
    game.preload(uiHealthBack);

    //HPバーレッド
    var uiHealthRed       = "../../resources/healthRed.png";
    game.preload(uiHealthRed);

    //HPバーグリー
    var uiHealthGreen     = "../../resources/healthGreen.png";
    game.preload(uiHealthGreen);

    //プレイヤーバナー1
    var uiPlayerBanner1   = "../../resources/playerBanner1.png";
    game.preload(uiPlayerBanner1);

    //プレイヤーバナー2
    var uiPlayerBanner2   = "../../resources/playerBanner2.png";
    game.preload(uiPlayerBanner2);

    //勝利UI
    var uiWin             = "../../resources/win.png";
    game.preload(uiWin);

    //敗北UI
    var uiLose            = "../../resources/lose.png";
    game.preload(uiLose);

    /**
     * Audio
     */
    //BGM
    var sndBGM            = "../../resources/music/highseas.mp3";
    game.preload(sndBGM);

    //ボタンクリック時
    var sndClick          = "../../resources/sound/se2.wav";
    game.preload(sndClick);

    //爆発時
    var sndExplosion      = "../../resources/sound/bomb1.wav";
    game.preload(sndExplosion);

    //沈没時
    var sndSinkShip       = "../../resources/sound/bomb2.wav";
    game.preload(sndSinkShip);

    //洗濯船変更時
    var sndChangeShips    = "../../resources/sound/se4.wav";
    game.preload(sndChangeShips);


    var fontStyle = "32px 'ＭＳ ゴシック', arial, sans-serif";

     /**
     * Utils
     */
    var utils = {
        //二点間の距離を求める
        getEuclideanDistance: function(startI, startJ, endI, endJ) {
            var distanceSq = Math.pow(startI -endI, 2) +Math.pow(startJ -endJ, 2);
            var distance   = Math.sqrt(distanceSq);
            return distance;
        },

        getManhattanDistance: function(startI, startJ, endI, endJ) {
            var distance = Math.abs(startI -endI) +Math.abs(startJ -endJ);
            return distance;
        },

        getChebyshevDistance: function(startI, startJ, endI, endJ) {
            var distance = Math.max(Math.abs(startI -endI), Math.abs(startJ -endJ));
            return distance;
        },

        beginUIShield: function() {
            if (!utils.shieldSprite) {
                var shieldSprite = new Sprite(960, 640);
                shieldSprite.image = game.assets[ui1x1Black];
                shieldSprite.opacity = 0.0;
                game.currentScene.addChild(shieldSprite);
                utils.shieldSprite = shieldSprite;
            }
        },

        endUIShield: function() {
            if (utils.shieldSprite) {
                utils.shieldSprite.parentNode.removeChild(utils.shieldSprite);
                utils.shieldSprite = null;
            }
        },
    };

    /**
     * Map のマスの定義
     */
    var tileTypes = {
        sea:  {id:0, name:"sea"},
        rough: {id:1, name:"rough"},
        shallow: {id:2, name:"shallow"},
        land: {id:3, name:"land"},
        rock:  {id:4, name:"rock"},
    };

    /**
     * Map クラス
     */
    var GameMap = Class.create({
        initialize: function(scene, mapData) {
            // 枠
            var frame = new Sprite(960, 640);
            frame.image = game.assets[mapFrame];
            scene.addChild(frame);
            this.frame = frame;

            // 背景
            var background = new Sprite(64*13, 64*9);
            background.image = game.assets[mapBackground00];
            background.x = 64;
            background.y = 10;
            scene.addChild(background);
            this.background = background;

            // マス
            var tiles = new Map(64, 64);
            tiles.image = game.assets[mapTiles];
            tiles.x = 64;
            tiles.y = 10;
            tiles.loadData(mapData);
            tiles.opacity = 0;
            scene.addChild(tiles);
            this.tiles = tiles;

            // マップを大きさを保存
            this.mapHeight = mapData.length;
            this.mapWidth  = mapData[0].length;

            //　元のマップデータから陸や岩のcollisionデータを生成します
            var mapCollisionData = [];
            for(var j=0; j < this.mapHeight; j++) 
            {
                mapCollisionData[j] = []
                for(var i=0; i < this.mapWidth; i++) 
                {
                    if (mapData[j][i] == tileTypes.land.id || mapData[j][i] == tileTypes.rock.id) 
                    {
                        mapCollisionData[j].push(1);
                    } 
                    else
                    {
                        mapCollisionData[j].push(0);
                    }
                }
            }
            this.tiles.collisionData = mapCollisionData

             // 検索用のデータ
            var mapSearchData = [];         //通常コスト
            var mapSearchDataLight  = [];   //軽いユニットのコスト
            var mapSearchDataHeavy  = [];   //重いユニットのコスト

            for(var j=0; j < this.mapHeight; j++) {
                mapSearchData[j] = [];
                mapSearchDataLight[j] = [];
                mapSearchDataHeavy[j] = [];
                for(var i=0; i < this.mapWidth; i++) {
                    if (mapData[j][i] == tileTypes.land.id || mapData[j][i] == tileTypes.rock.id) {
                        //岩か陸だった場合移動不可
                        mapSearchData[j].push(0);
                        mapSearchDataLight[j].push(0);
                        mapSearchDataHeavy[j].push(0);
                    } 
                    else {
                        if (mapData[j][i] == tileTypes.rough.id) {
                            //荒い海だった場合のコスト
                            mapSearchData[j].push(2);
                            mapSearchDataLight[j].push(3);
                            mapSearchDataHeavy[j].push(1);
                        } 
                        else if (mapData[j][i] == tileTypes.shallow.id) {
                            //浅い海
                            mapSearchData[j].push(2);
                            mapSearchDataLight[j].push(1);
                            mapSearchDataHeavy[j].push(3);
                        } 
                        else {
                            //通常
                            mapSearchData[j].push(1);
                            mapSearchDataLight[j].push(1);
                            mapSearchDataHeavy[j].push(1);
                        }
                    }
                }
            }

            this.searchGraph = new Graph(mapSearchData);
            this.searchGraphLight = new Graph(mapSearchDataLight);
            this.searchGraphHeavy = new Graph(mapSearchDataHeavy);

            //プレイヤーのレイヤー
            var playLayer = new Group()
            scene.addChild(playLayer);
            this.playLayer = playLayer;

            //追加レイヤー
            var overLayer = new Group()
            scene.addChild(overLayer);
            this.overLayer = overLayer;

            //下位のレイヤー
            var underLayer = new Group()
            scene.addChild(underLayer);
            this.underLayer = underLayer;

            var self = this;
            tiles.touchEnabled = true;

            //タッチ終了時
            tiles.addEventListener(enchant.Event.TOUCH_END, function(params){
                self.ontouchend(params);
            });

            //タッチが始まったさい
            tiles.addEventListener(enchant.Event.TOUCH_START, function(params){
                self.ontouchupdate(params);
            });

            //タッチが移動した際
            tiles.addEventListener(enchant.Event.TOUCH_MOVE, function(params){
                self.ontouchupdate(params);
            });

            //フレーム開始時にZsortを行う
            tiles.addEventListener(enchant.Event.ENTER_FRAME, function(params){
                self.zsort();
            });
        },

        toLocalSpace:function(worldX, worldY) {
            //ワールド座標からローカルへ
            var localX = worldX -this.tiles.x;
            var localY = worldY -this.tiles.y;
            return {x:localX, y:localY};
        },

        toWorldSpace:function(localX, localY) {
            //ローカル座標からワールドへ
            var worldX = localX +this.tiles.x;
            var worldY = localY +this.tiles.y;
            return {x:worldX, y:worldY};
        },

        getMapTileAtPosition: function(localX, localY) {
            //ローカル座標からマス目へ
            return {i: Math.floor(localX/64),j: Math.floor(localY/64)};
        },

        getMapPositionAtTile: function(i,j) {
            //マス目からローカル座標へ
            return {localX: i *64,localY: j *64};
        },

        outofBorders:function(i,j){
            //範囲外チェック
            if(i < 0 || this.mapWidth <= i ||
             j < 0 || this.mapHeight <= j){
                //範囲外
                return true;
            }
            return false;
        },

        getTileInfo:function(id) {
            //一致したIDに応じたオブジェクトを取得
            for(t in tileTypes) {
                if (tileTypes[t].id == id) {
                    return tileTypes[t];
                }
            }
        },

        addChild: function(object) {
            //子要素の追加
            this.playLayer.addChild(object);
        },

        positonObject: function(object, i, j) {
            //オブジェクトの座標をセット
            var postion = this.getMapPositionAtTile(i, j);
            var worldPosition = this.toWorldSpace(postion.localX, postion.localY);

            object.x = worldPosition.x;
            object.y = worldPosition.y;

            //タイル座標
            object.i = i;
            object.j = j;
        },

        positionFune: function(fune, i, j) {
            //船の座標セット
            this.positonObject(fune, i, j);

        },

        moveFune: function(fune, path, onEnd) {
            //船を段階的に移動
            //パスの要素数が存在する間
            if (path.length > 0) {
                //先頭要素の削除
                var nextMasu = path.shift();
                var i = nextMasu.y;
                var j = nextMasu.x;
                var cost = nextMasu.getCost()

                var postion       = this.getMapPositionAtTile(i, j);
                var worldPosition = this.toWorldSpace(postion.localX, postion.localY);

                //console.log("path length:", path.length, "move: ", i, j, worldPosition.x, worldPosition.y);

                fune.i = i;
                fune.j = j;

                var self = this;
                fune.tl.moveTo(worldPosition.x, worldPosition.y, 10 *cost, enchant.Easing.QUAD_EASEINOUT).then(function(){
                    self.moveFune(fune, path, onEnd);
                });
            } 
            else {
                onEnd();
            }
        },

        setActiveFune: function(fune) {
            //船を動かせる状態へ
            fune.map = this;
            this.activeFune = fune;
            this.drawMovementRange();
        },

        getEuclideanDistance: function(startI, startJ, endI, endJ) {
            return utils.getEuclideanDistance(startI, startJ, endI, endJ);
        },

        getManhattanDistance: function(startI, startJ, endI, endJ) {
            return utils.getManhattanDistance(startI, startJ, endI, endJ);
        },

        getChebyshevDistance: function(startI, startJ, endI, endJ) {
            return utils.getChebyshevDistance(startI, startJ, endI, endJ);
        },

        outOfBorders: function(i, j) {
            //範囲外チェック
            if (i < 0) return true;
            if (i >= this.mapWidth) return true;
            if (j < 0) return true;
            if (j >= this.mapHeight) return true;

            return false;
        },

        mapGetDistance: function(startX,startY,endX,endY){
            //二点間の距離の取得
            return(Math.abs(endX - startX) + Math.abs(endY - startY));
        },

        getPath: function(fune, i,j, targetI, targetJ) {
            //A*でのルート探索
            var path;
            if (fune.moveType == "normal") {
                //通常
                var start = this.searchGraph.grid[j][i];
                var end   = this.searchGraph.grid[targetJ][targetI];
                path = astar.search(this.searchGraph, start, end);
            }
            if (fune.moveType == "light") {
                //軽い
                var start = this.searchGraphLight.grid[j][i];
                var end   = this.searchGraphLight.grid[targetJ][targetI];
                path = astar.search(this.searchGraphLight, start, end);
            }
            if (fune.moveType == "heavy") {
                //重い
                var start = this.searchGraphHeavy.grid[j][i];
                var end   = this.searchGraphHeavy.grid[targetJ][targetI];
                path = astar.search(this.searchGraphHeavy, start, end);
            }

            path.cost = 0;
            for(var i=0; i<path.length;i++){
                path.cost += path[i].getCost();
            }
            return path;
        },

        setController: function(controller) {
            //呼び出し元クラスの取得
            this.controller = controller;
        },

        ontouchend:function(params) {
             if (this.mapMarker) {
                //移動時マップマーカーの削除
                this.overLayer.removeChild(this.mapMarker)
                delete this.mapMarker;
            }
            //タイルクリック時の処理
            var localPosition = this.toLocalSpace(params.x, params.y);

            var tileData = this.tiles.checkTile(localPosition.x, localPosition.y);
            var tileInfo = this.getTileInfo(tileData);

            if (this.tiles.hitTest(localPosition.x, localPosition.y) == true) {
                //alert("通れない、"+tileInfo.name);
                console.log("通れない", tileInfo.name, "world X", params.x, "localX", localPosition.x, "worldY", params.y, "localY", localPosition.y)
            } else {
                //alert("通れる、"+tileInfo.name);
                console.log("通れる", tileInfo.name, "world X", params.x, "localX", localPosition.x, "worldY", params.y, "localY", localPosition.y)

                var tilePos = this.getMapTileAtPosition(localPosition.x,localPosition.y);
                //距離が移動力以下ならば
                if(this.mapGetDistance(this.activeFune.i,this.activeFune.j,tilePos.i,tilePos.j) <= this.activeFune.getMovement()){
                    //船の位置をタッチしたマスへ
                    var path = this.getPath(this.activeFune, this.activeFune.i, this.activeFune.j, tilePos.i, tilePos.j);
                    if (path.cost <= this.activeFune.getMovement()) {
                        var self = this;
                        utils.beginUIShield();
                        self.moveFune(self.activeFune, path, function() {
                            self.controller.endTurn();
                        });
                    }
                }
            }
        },

        ontouchupdate:function(params){
            //タイルをタッチした際の処理(移動可否)
            var localPos = this.toLocalSpace(params.x,params.y);
            var tilePos = this.getMapTileAtPosition(localPos.x,localPos.y);
            if(this.outofBorders(tilePos.i,tilePos.j)){
                //範囲外
                return;
            }

            if(this.mapMarker == undefined){
                //×のオブジェクトが生成されていない場合生成
                this.mapMarker = new Sprite(64,64);
                this.mapMarker.image = game.assets[mapUI];
                this.positonObject(this.mapMarker,tilePos.i,tilePos.j);
                this.overLayer.addChild(this.mapMarker);
            }
            else{
                //座標の切り替え
                this.positonObject(this.mapMarker,tilePos.i,tilePos.j);
            }

            if(this.tiles.hitTest(localPos.x,localPos.y) == true){
                //通行不可の場合灰色へ
                this.mapMarker.frame = 1;
            }
            else{
                if(this.mapGetDistance(this.activeFune.i,this.activeFune.j,tilePos.i,tilePos.j) <= this.activeFune.getMovement()){
                    var path = this.getPath(this.activeFune, this.activeFune.i, this.activeFune.j, tilePos.i, tilePos.j);
                    if (path.cost <= this.activeFune.getMovement()) {
                        //距離範囲内は赤く
                        this.mapMarker.frame = 0;
                    } 
                    else {
                        //距離範囲外は灰色
                        this.mapMarker.frame = 1;
                    }
                }
                else{
                    //距離範囲外は灰色
                    this.mapMarker.frame = 1;
                }
            }
        },

        drawMovementRange:function(){
            //移動範囲の描画
            if(this.areaRangeLayer){
                //移動範囲を示すレイヤーが存在した場合削除
                this.underLayer.removeChild(this.areaRangeLayer);
                delete this.areaRangeLayer;
            }

            this.areaRangeLayer = new Group();
            this.underLayer.addChild(this.areaRangeLayer);

            //現在地を中心に四角形の範囲を左下から右上へ調べていく
            for(var rangeI = -this.activeFune.getMovement(); rangeI <= this.activeFune.getMovement(); rangeI++){
                var targetI = this.activeFune.i + rangeI;
                for(rangeJ = -this.activeFune.getMovement(); rangeJ <= this.activeFune.getMovement(); rangeJ++){
                    var targetJ = this.activeFune.j + rangeJ;
                    if(!this.outofBorders(targetI,targetJ)){
                        //範囲内時
                        if(this.mapGetDistance(this.activeFune.i,this.activeFune.j,targetI,targetJ) <= this.activeFune.getMovement()){
                            //A*で最短経路を求める
                            var path = this.getPath(this.activeFune, this.activeFune.i, this.activeFune.j, targetI, targetJ);
                            //行動範囲内示画像描画
                            if (path.cost <= this.activeFune.getMovement()) {
                                var areaSprite = new Sprite(64, 64);
                                areaSprite.touchEnabled = false;
                                areaSprite.image = game.assets[mapUI];
                                var position = this.getMapPositionAtTile(targetI, targetJ);

                                if (this.tiles.hitTest(position.localX, position.localY) == true) {
                                    areaSprite.frame = 3;
                                } else {
                                    areaSprite.frame = 2;
                                }
                                this.positonObject(areaSprite, targetI, targetJ);
                                this.areaRangeLayer.addChild(areaSprite);
                            }
                        }
                    }
                }
            }
        },

        getMovementRange: function(fune, openOnly) {
            //移動範囲内を取得
            var funeList = this.controller.getFuneList();
            var moveList = [];

            for (var rangeI = -fune.getMovement(); rangeI <= fune.getMovement(); rangeI++) {
                var targetI = fune.i +rangeI;
                for (var rangeJ = -fune.getMovement(); rangeJ <= fune.getMovement(); rangeJ++) {
                    var targetJ = fune.j +rangeJ;

                    if (!this.outOfBorders(targetI, targetJ)) {
                        //範囲内
                        if (this.getManhattanDistance(fune.i, fune.j, targetI, targetJ) <= fune.getMovement()) {
                            //移動範囲内
                            var path = this.getPath(fune, fune.i, fune.j, targetI, targetJ);
                            if (path.cost <= fune.getMovement()) {
                                //移動コスト以下だった場合
                                var position = this.getMapPositionAtTile(targetI, targetJ);
                                var isOpen = true;
                                if (this.tiles.hitTest(position.localX, position.localY) == true) {
                                    isOpen = false;
                                } 
                                else {
                                    for (var i=0; i < funeList.length; i++) {
                                        var otherFune = funeList[i];
                                        if (targetI == otherFune.i && targetJ == otherFune.j) {
                                            //他の船と場所が被る場合、移動不可
                                            isOpen = false;
                                        }
                                    }
                                }
                                if (isOpen == false) {
                                    if (openOnly == false) {
                                        //移動不可
                                        moveList.push({i:targetI, j:targetJ, open:false});
                                    }
                                } 
                                else {
                                    //移動可能
                                    moveList.push({i:targetI, j:targetJ, open:true});
                                }
                            }
                        }
                    }
                }
            }

            return moveList;
        },

        zsort: function() {
            //Zソート
            var zorder = [];
            for (var c=0; c < this.playLayer.childNodes.length; ++c) {
                zorder.push(this.playLayer.childNodes[c]);
            }
            //Y軸が低いほものを上に描画
            zorder.sort(function(a, b) {
                if (a.y > b.y) {
                    return 1;
                } 
                else if (a.y == b.y) {
                    if (a.x > b.x) {
                        return 1;
                    } 
                    else if (a.x == b.x) {
                        return 0;
                    } 
                    else {
                        return -1;
                    }
                } 
                else {
                    return -1;
                }
            });
            for (var i=0; i < zorder.length; ++i) {
                this.playLayer.addChild(zorder[i]);
            }
        }
    });


    /**
     * 基本船のクラス
     */
    var BaseFune = Class.create(Group, {
        initialize: function(id, stats) {
            Group.call(this);

            var fune = new Sprite(64, 64);
            this.fune = fune;
            fune.image = game.assets[shipsSpriteSheet];
            //通常アニメーション
            fune.frame = [0,0,0,0,1,1,1,2,2,1,1,0,0,0,0,3,3,3];

            //死亡時のアニメーション
            fune.sinkFrame = [3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, null];
            this.addChild(fune);

            //HPバー
            var healthBackSprite   = new Sprite(64, 12);
            this.healthBackSprite  = healthBackSprite;
            healthBackSprite.image = game.assets[uiHealthBack];
            healthBackSprite.y     = 64 -6;
            this.addChild(healthBackSprite);

            var healthRedSprite   = new Sprite(64, 12);
            this.healthRedSprite  = healthRedSprite;
            healthRedSprite.originX = 0
            healthRedSprite.image = game.assets[uiHealthRed];
            healthRedSprite.y     = 64 -6;
            this.addChild(healthRedSprite);

            var healthGreenSprite   = new Sprite(64, 12);
            this.healthGreenSprite  = healthGreenSprite;
            healthGreenSprite.originX = 0
            healthGreenSprite.image = game.assets[uiHealthGreen];
            healthGreenSprite.y     = 64 -6;
            this.addChild(healthGreenSprite);

            //ステータス
            this.stats = { 
                id:        id,
                movement:  stats.movement,
                range:     stats.range,
                attack:    stats.attack,
                defense:   stats.defense,
                hpMax:     stats.hpMax,
            };
            this.stats.hp = this.stats.hpMax;

            this.moveType   = "normal";
            this.usedSkill  = false;
        },

        activateSkill: function(onEnd) {
            //スキル
            utils.beginUIShield();
            this.usedSkill = true;
            var pirateChibi = new Sprite(512, 512);
            pirateChibi.image = this.getChibiImage();
            pirateChibi.opacity = 0;
            if (this.scaleX > 0) {
                pirateChibi.x = -50;
            } 
            else {
                pirateChibi.x = 960 -512 +50;
            }
            var alertWindow = new AlertWindow(this.getSkillName(), this.player.controller);
            alertWindow.addChild(pirateChibi, alertWindow.firstChild);
            pirateChibi.tl.fadeIn(10);
            var self = this;
            alertWindow.onTouch = function() {
                self.processSkill(onEnd);
            }
        },

        processSkill: function(onEnd) {
            onEnd();
        },

        refreshSkill: function() {
            //スキルの回復
            this.usedSkill = false;
        },

        canUseSkill: function() {
            //スキルが使用可能か
            return this.usedSkill == false;
        },

        sinkShip: function() {
            //船消滅時
            this.player.removeFune(this);
            this.fune.frame = this.fune.sinkFrame;
            this.counter = 1;
            this.onenterframe = function(){ // enterframe event listener
                this.counter++;
                if (this.counter == 12 ) {
                    this.parentNode.removeChild(this);
                }
            };
        },

        getId: function() {
            return this.stats.id;
        },

        getMovement(){
            //移動力の取得
            return this.stats.movement;
        },

        getRange(){
            //攻撃範囲の取得
            return this.stats.range;
        },

        getAttack(){
            //攻撃力の取得
            return this.stats.attack;
        },

        getDefense(){
            //防御力の取得
            return this.stats.defense;
        },

        getHP: function() {
            //現在のHPの取得
            return this.stats.hp;
        },

        getHPMax(){
            //最大HPの取得
            return this.stats.hpMax;
        },

        getCaptainName: function() {
            return "無名";
        },

        getImage: function() {
            //画像の取得
            return game.assets[pirateSprites[this.getId() -1]]
        },

        getChibiImage: function() {
            //ちび画像の取得
            return game.assets[pirateChibiSprites[this.getId() -1]]
        },

        withinRange: function(i, j) {
            //指定座標が攻撃範囲内かチェック
            var distance = utils.getManhattanDistance(this.i, this.j, i, j);
            console.log("withinRange", "distance", distance, "range", this.stats.range, distance <= this.stats.range);
            if (distance <= this.stats.range) {
                return true;
            } else {
                return false;
            }
        },

        updateHPBar: function() {
            //HPバーの更新
            var ratio = Math.max(this.stats.hp / this.stats.hpMax, 0);
            if (ratio > 0.5) {
                this.healthGreenSprite.scaleX = ratio;
            } else {
                this.healthGreenSprite.scaleX = 0;
            }
            this.healthRedSprite.scaleX = ratio;
        },

        takeDamage: function(damage) {
            //ダメージを与える処理
           var actualDamage = Math.max(damage -this.stats.defense, 1);
            this.stats.hp -= actualDamage;
            this.updateHPBar();
            return this.stats.hp;
        },

        healDamage: function(recover) {
            //HP回復処理
            this.stats.hp = Math.min(this.stats.hp + recover, this.stats.hpMax);
            this.updateHPBar();
        },

        attackFune: function(otherFune) {
            //入力の受け付けない
            utils.beginUIShield();
            var damage;
            var baseDamage = this.stats.attack;
            //-0.5～0.5の乱数発生
            var variance   = Math.random() -0.5;
            //基本ダメージの1/10から乱数を適応し補正値を出す
            var variableDamage = (baseDamage /10) * variance;
            //0～1の乱数発生
            var attackRoll = Math.random();
            var isCritical = false;

            // クリティカルヒット 10%
            // ミス 10%
            if (attackRoll > 0.9) {
                // クリティカル　ダメージx2
                damage = (baseDamage +variableDamage) *2;
                var isCritical = true;
            } 
            else if (attackRoll < 0.1) {
                // ミス ダメージ 0
                damage = 0;
            } 
            else {
                //通常
                damage = baseDamage +variableDamage;
            }

            //小数以下切り捨て
            damage = Math.ceil(damage);

            if (damage > 0) 
            {
                //ダメージ処理
                var beforeHp = otherFune.getHP();
                var afterHp  = otherFune.takeDamage(damage);

                //爆発エフェクト
                var explosion = new Explosion();
                explosion.x = otherFune.x +32;
                explosion.y = otherFune.y +32;

                this.player.controller.sndManager.playFX(sndExplosion);
                game.currentScene.addChild(explosion);

                if (isCritical) 
                {
                    //クリティカル時処理
                    var alertWindow = new AlertWindow("クリティカル！", this.player.controller);
                    var self = this;
                    alertWindow.onTouch = function() 
                    {
                        if (afterHp <= 0)
                         {
                            //沈没
                            var alertWindow = new AlertWindow("沈没...", self.player.controller);
                            alertWindow.onTouch = function() 
                            {
                                otherFune.sinkShip();
                                self.player.controller.endTurn();
                            }
                        } 
                        else
                        {
                            //通常
                            self.player.controller.endTurn();
                        }
                    }
                } 
                else 
                {
                    if (afterHp <= 0) 
                    {
                        //沈没
                        var alertWindow = new AlertWindow("沈没...", this.player.controller);
                        var self = this;
                        alertWindow.onTouch = function() 
                        {
                            otherFune.sinkShip();
                            self.player.controller.endTurn();
                        }
                    } 
                    else 
                    {
                        //通常
                        this.player.controller.endTurn();
                    }
                }
            } 
            else 
            {
                //ミス
                var alertWindow = new AlertWindow("ミス！", this.player.controller);
                var self = this;
                alertWindow.onTouch = function()
                {
                    self.player.controller.endTurn();
                }
            }
        },

        ontouchend:function(params){
            //自分のターンか
             if (this.player.isActive()) {
                //自分が選ばれているか
                if (this.player.getActiveFune() == this) {
                    //ポップアップ表示
                    var popup = new FunePopup(this);
                    popup.onCancel = function() {

                    }
                    var self = this;
                    popup.onSkill = function() {
                        if (self.usedSkill == false) {
                            self.usedSkill = true;
                            self.activateSkill(function() {
                                self.player.controller.endTurn();
                            })
                        }
                    }
                }
                else {
                    //自分自身を選択状態へ
                    this.player.setActiveFune(this);
                }
            } 
            else {
                //相手の船選択時
                var activePlayer = this.player.controller.getActivePlayer();
                var activeFune   = activePlayer.getActiveFune();
                if (activeFune.withinRange(this.i, this.j)) {
                    //攻撃
                    activeFune.attackFune(this);
                } else {
                    new AlertWindow("攻撃は届きません", this.player.controller);
                }
            }
        },
    });

    //各船の設定
    var CaptainFune = Class.create(BaseFune,{
        initialize:function(id){
            //継承元を呼び出す
            BaseFune.call(this,id, {
                movement: 3,
                range: 3,
                attack: 100,
                defense: 50,
                hpMax: 100,
            })
            //アニメーションフレーム
            this.fune.frame = [0, 0, 0, 0, 1, 1, 1, 2, 2, 1, 1, 0, 0, 0, 0, 3, 3, 3];
            this.fune.sinkFrame = [3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, null];
        },

        getCaptainName: function() {
            return "キャプテン";
        },

        getSkillName: function() {
            return "オウエン";
        },

        processSkill: function(onEnd) {
            //全味方回復
            var count = this.player.getFuneCount();
            for(var i=0; i < count; i++) {
                var fune = this.player.getFune(i);
                var toHeal = Math.ceil(fune.getHPMax() /2);
                fune.healDamage(toHeal);
            }
            onEnd();
        },
    })

        var HayaiFune = Class.create(BaseFune, {
        initialize: function(id) {
            BaseFune.call(this, id, {
                movement:  5,
                range:     3,
                attack:   80,
                defense:  60,
                hpMax:    80,
            });

            this.moveType   = "light";
            this.fune.frame = [8, 8, 8, 8, 9, 9, 9, 10, 10, 9, 9, 8, 8, 8, 8, 11, 11, 11];
            this.fune.sinkFrame = [11, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, null];
        },

        getCaptainName: function() {
            return "はやいちゃん";
        },

        getSkillName: function() {
            return "ハリーアップ";
        },

        processSkill: function(onEnd) {
            //相手のターンスキップ
            this.player.controller.getFreeTurns(this.player, 2);
            onEnd();
        },
    });

    var KataiFune = Class.create(BaseFune, {
        initialize: function(id) {
            BaseFune.call(this, id, {
                movement:  3,
                range:     3,
                attack:   80,
                defense:  60,
                hpMax:   240,
            });

            this.moveType   = "heavy";
            this.fune.frame = [16, 16, 16, 16, 17, 17, 17, 18, 18, 17, 17, 16, 16, 16, 16, 19, 19, 19];
            this.fune.sinkFrame = [19, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, null];
            this.indestructible = false;
        },

        getCaptainName: function() {
            return "かたいちゃん";
        },

        getSkillName: function() {
            return "アイロンシールド";
        },

        processSkill: function(onEnd) {
            //1度だけダメージ無効
            this.indestructible = true;
            onEnd();
        },

        attackFune: function(otherFune) {
            this.indestructible = false;
            BaseFune.prototype.attackFune.call(this, otherFune);
        },

        takeDamage: function(damage) {
            if (this.indestructible) {
                this.indestructible = false;
                return this.getHP()
            } 
            else {
                return BaseFune.prototype.takeDamage.call(this, damage);
            }
        },
    });

    var KougekiFune = Class.create(BaseFune, {
        initialize: function(id) {
            BaseFune.call(this, id, {
                movement:  3,
                range:     3,
                attack:  120,
                defense:  40,
                hpMax:   150,
            });

            this.fune.frame = [24, 24, 24, 24, 25, 25, 25, 26, 26, 25, 25, 24, 24, 24, 24, 27, 27, 27];
            this.fune.sinkFrame = [27, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, null];
        },

        getCaptainName: function() {
            return "こうげきちゃん";
        },

         getSkillName: function() {
            return "バレットストーム";
        },

        processSkill: function(onEnd) {
            //範囲内の敵全員に攻撃
            var damage = this.stats.attack;
            var count = this.player.controller.getNonActivePlayer().getFuneCount();
            for(var i=0; i < count; i++) {
                var fune = this.player.controller.getNonActivePlayer().getFune(i);
                if (this.withinRange(fune.i, fune.j)) {
                    var afterHp = fune.takeDamage(damage);
                    var explosion = new Explosion();
                    explosion.x = fune.x +32;
                    explosion.y = fune.y +32;
                    this.player.controller.sndManager.playFX(sndExplosion);
                    game.currentScene.addChild(explosion);

                    if (afterHp <= 0) {
                        fune.sinkShip();
                        i--;
                        count--;
                    }
                }
            }
            onEnd();
        },
    });

    var BossTeki = Class.create(CaptainFune, {
        //ボス敵
        initialize: function(id) {
            BaseFune.call(this, id, {
                movement:  3,
                range:     3,
                attack:  100,
                defense:  60,
                hpMax:   240,
            });

            this.moveType   = "normal";
            this.fune.frame = [40, 40, 40, 40, 41, 41, 41, 42, 42, 41, 41, 40, 40, 40, 40, 43, 43, 43];
            this.fune.sinkFrame = [43, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, null];
        },

        getCaptainName: function() {
            return "ボス";
        },

        getSkillName: function() {
            return "カミワザ";
        },

        processSkill: function(onEnd) {
            //全体回復
            var count = this.player.getFuneCount();
            for(var i=0; i < count; i++) {
                var fune = this.player.getFune(i);
                var toHeal = Math.ceil(fune.getHPMax() /4);
                fune.healDamage(toHeal);
            }

            //敵への全体攻撃
            var damage = this.stats.attack;
            var count = this.player.controller.getNonActivePlayer().getFuneCount();
            for(var i=0; i < count; i++) {
                var fune = this.player.controller.getNonActivePlayer().getFune(i);
                if (this.withinRange(fune.i, fune.j)) {
                    var afterHp = fune.takeDamage(damage);
                    var explosion = new Explosion();
                    explosion.x = fune.x +32;
                    explosion.y = fune.y +32;
                    this.player.controller.sndManager.playFX(sndExplosion);
                    game.currentScene.addChild(explosion);

                    if (afterHp <= 0) {
                        fune.sinkShip();
                    }
                }
            }
            onEnd();
        },
    });

    var HayaiTeki = Class.create(HayaiFune, {
        initialize: function(id) {
            HayaiFune.call(this, id);

            this.stats.hpMax = Math.floor(0.5 * this.stats.hpMax);
            this.stats.hp = this.stats.hpMax;

            this.fune.frame = [40, 40, 40, 40, 41, 41, 41, 42, 42, 41, 41, 40, 40, 40, 40, 43, 43, 43];
            this.fune.sinkFrame = [43, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, null];
        },

        getCaptainName: function() {
            return "ハヤイくん";
        },
    });

    var KataiTeki = Class.create(KataiFune, {
        initialize: function(id) {
            KataiFune.call(this, id);

            this.stats.hpMax = Math.floor(0.5 * this.stats.hpMax);
            this.stats.hp = this.stats.hpMax;

            this.fune.frame = [48, 48, 48, 48, 49, 49, 49, 50, 50, 49, 49, 48, 48, 48, 48, 51, 51, 51];
            this.fune.sinkFrame = [51, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55, null];
        },

        getCaptainName: function() {
            return "カタイくん";
        },
    });

    var KougekiTeki = Class.create(KougekiFune, {
        initialize: function(id) {
            KougekiFune.call(this, id);

            this.stats.hpMax = Math.floor(0.5 * this.stats.hpMax);
            this.stats.hp = this.stats.hpMax;

            this.fune.frame = [56, 56, 56, 56, 57, 57, 57, 58, 58, 57, 57, 56, 56, 56, 56, 59, 59, 59];
            this.fune.sinkFrame = [59, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, null];
        },

        getCaptainName: function() {
            return "コウゲキくん";
        },
    });

    //爆発エフェクト
    var Explosion = Class.create(Sprite, {
        initialize: function(id, stats) {
            Sprite.call(this, 32, 32);

            this.image = game.assets[explosionSpriteSheet];
            this.frame = [0,1,2,3,1,2,3,4,null];

            this.counter = 0;
        },
        onenterframe:function(){ // enterframe event listener
            this.counter++;
            if (this.counter == 9 ) {
                this.parentNode.removeChild(this);
            }
        },
    });

    //プレイヤー
    var GamePlayer = Class.create({
        initialize: function(id, data) {
            this.funeList = [];
            this.id   = id;
            this.data = data;

            this.funeCountInitial = 0;
        },

        isActive: function() {
            //アクティブ状態か否かを返す
            return this.myTurn;
        },

        setActive: function(flag) {
            //状態を変更
            this.myTurn = flag;
        },

        getData: function(key) {
            return this.data[key];
        },

        setData: function(key, value) {
            //データのセット
            this.data[key] = value;
        },

        setController: function(controller) {
            //マネージャクラスなどを渡す
            this.controller = controller;
        },

        addFune: function(fune) {
            //リストへ船を登録
            fune.player = this;
            this.funeList.push(fune)
            this.funeCountInitial++;
        },

        getFune: function(index) {
            //指定した船を取得
            return this.funeList[index];
        },

        getFuneCount: function() {
            //船の個数を返す
            return this.funeList.length;
        },

        getFuneCountInitial: function() {
            return this.funeCountInitial;
        },

        getActiveFune: function() {
            //アクティブ状態の船を返す
            if (this.activeFune) {
                return this.activeFune;
            } 
            else {
                return this.funeList[0];
            }
        },

        setActiveFune: function(fune) {
            //アクティブ状態の船の登録
            this.activeFune = fune;
            //ターン情報の更新
            this.controller.updateTurn();
        },

        removeFune: function(fune) {
            //船の削除
            delete fune.player;

            //船のリストの再生成
            var newList = [];
            for (var i=0; i < this.getFuneCount(); ++i) {
                if (this.getFune(i) != fune) {
                    newList.push(this.getFune(i));
                }
            }
            this.funeList = newList;

            //アクティブ状態が自分の場合は削除
            if (this.activeFune == fune) {
                this.activeFune = null;
            }
        },
    });

    //AI処理
    var AIPlayer = Class.create(GamePlayer, {
        initialize: function(id, data) {
            GamePlayer.call(this, id, data);
            this.state = new OpeningState(this);
        },

        resetState: function() {
            this.state = new OpeningState(this);
            this.funeCountInitial = 0;
        },

        simulatePlay: function() {
            //ステートマシン
            this.state = this.state.updateState();
            var action = this.state.chooseAction();

            this.setActiveFune(action.fune);

            var self = this;
            setTimeout(function() {
                switch(action.type) {
                    case "attack":
                        //攻撃状態
                        action.fune.attackFune(action.target);
                        break;
                    case "skill":
                        //スキル
                        action.fune.activateSkill(function() {
                            self.controller.endTurn();
                        })
                        break;
                    case "move":
                        //移動
                        if (action.path && action.path.length > 0) {
                            self.controller.map.moveFune(action.fune, action.path, function() {
                                self.controller.endTurn();
                            })
                        } 
                        else {
                            self.controller.endTurn();
                        }
                        break;
                }
            }, 1000);   //1000ミリ停止
        },
    });

    var BaseState = Class.create({
        //状態に応じて動きを変える
        initialize: function(player) {
            this.player = player;
        },

        updateState: function() {
            //次に使うステートを返します
            var nextState = this;　//今回はまだ一つしかありませんので自分を
            return nextState;
        },

        chooseAction: function() {
            //行動選択
            var maxFuneIndex = this.player.getFuneCount();
            //行動する船をランダムに選択
            var funeIndex    = Math.floor(Math.random() * maxFuneIndex);
            var fune         = this.player.getFune(funeIndex);

            //スキルを使うか否か
            if (fune.canUseSkill() && Math.random() < 0.2) {
                var action = {
                    type: "skill",
                    fune: fune,
                }
                return action
            }

            //敵の探索
            var count = this.player.controller.getNonActivePlayer().getFuneCount();
            for(var i=0; i < count; i++) {
                var targetFune = this.player.controller.getNonActivePlayer().getFune(i);
                if (fune.withinRange(targetFune.i, targetFune.j)) {
                    //攻撃範囲内
                    var action = {
                        type:   "attack",
                        fune:   fune,
                        target: targetFune,
                    }
                    return action;
                }
            }

            //アクションが実行されない場合はランダムに移動
            var openOnly = true;
            var moveList = this.player.controller.map.getMovementRange(fune, openOnly);
            //移動可能なマスをランダムに取得
            var randomIndex = Math.floor(Math.random() * moveList.length);
            var targetPosition = moveList[randomIndex];

            //ルートを取得
            var path     = this.player.controller.map.getPath(fune, fune.i, fune.j, targetPosition.i, targetPosition.j);
            var action = {
                type:"move",
                fune: fune,
                path: path,
            };
            return action;
        },

        getRandomFune: function() {
            //ランダムな船の取得
            var maxFuneIndex = this.player.getFuneCount();
            var funeIndex    = Math.floor(Math.random() * maxFuneIndex);
            var fune         = this.player.getFune(funeIndex);
            return fune;
        },

        getTargetsWithinRange: function(fune) {
            //範囲内の敵取得
            var targets    = [];
            var otherCount = this.player.controller.getNonActivePlayer().getFuneCount();
            for(var j=0; j < otherCount; j++) {
                var targetFune = this.player.controller.getNonActivePlayer().getFune(j);
                if (fune.withinRange(targetFune.i, targetFune.j)) {
                    targets.push(targetFune);
                }
            }
            return targets;
        },

        isTargeted: function(fune) {
            //ターゲットが攻撃範囲内か
            var otherCount = this.player.controller.getNonActivePlayer().getFuneCount();
            for(var j=0; j < otherCount; j++) {
                var targetFune = this.player.controller.getNonActivePlayer().getFune(j);
                if (targetFune.withinRange(fune.i, fune.j)) {
                    return true;
                }
            }
            return false;
        },

        getMovePosition: function(fune, avoidEnemyRate) {
            //移動先が敵の攻撃範囲外の場合の場所のみ返す
            var openOnly = true;
            var moveList = this.player.controller.map.getMovementRange(fune, openOnly);
            var moveTargetList = [];
            for (var i=0; i < moveList.length; i++) {
                var targetPosition = moveList[i];
                var otherCount = this.player.controller.getNonActivePlayer().getFuneCount();
                for(var j=0; j < otherCount; j++) {
                    var otherFune = this.player.controller.getNonActivePlayer().getFune(j);
                    if (otherFune.withinRange(targetPosition.i, targetPosition.j)) {
                        if (Math.random() < avoidEnemyRate) {
                            continue;
                        }
                    }
                    moveTargetList.push(targetPosition)
                }
            }
            return moveTargetList;
        },

        getAttackPath:function(fune){
            //開始時の敵の挙動
            //敵へ向かって突撃
            var openOnly = true;
            var moveList = this.player.controller.map.getMovementRange(fune, openOnly);

            var otherCount = this.player.controller.getNonActivePlayer().getFuneCount();
            var pathList = [];
            //移動可能なマスのパスを取得
            for(var j = 0; j < moveList.length; j++){
                //移動可能な場所のみ
                var path = this.player.controller.map.getPath(fune, fune.i, fune.j, moveList[j].i, moveList[j].j);
                if (path.length > 0) {
                    pathList.push(path);
                }
            }

            //移動後のマス目からすべての敵の距離を取得
            var movedPath = [];
            for(var i=0; i < otherCount; i++) {
                var otherFune = this.player.controller.getNonActivePlayer().getFune(i);
                for(var j = 0;j < moveList.length;j++){
                    var path = this.player.controller.map.getPath(fune, moveList[j].i, moveList[j].j, otherFune.i, otherFune.j);
                    if (path.length > 0) {
                        movedPath.push({Path:path,MovePoint:moveList[j]});
                    }
                }
            }

            //長い順へ
            var sortedPath = this.sortPathByLength2(movedPath);
            //一番近いものを取り出す
            //var path = sortedPath.pop();

            console.log(fune.getCaptainName());
            console.log(sortedPath.MovePos[sortedPath.MovePos.length - 1]);

            var Posision = sortedPath.MovePos.pop();

            var pathd = this.player.controller.map.getPath(fune, fune.i, fune.j, Posision.i, Posision.j);

            return pathd;
        },

        getRandomPath: function(fune, avoidEnemyRate) {
            //移動リストからランダムな一つのパスを返す
            var moveList = this.getMovePosition(fune, avoidEnemyRate);
            var pathList = [];
            for (var i=0; i < moveList.length; i++) {
                var targetPosition = moveList[i];
                var path = this.player.controller.map.getPath(fune, fune.i, fune.j, targetPosition.i, targetPosition.j);
                if (path.length > 0) {
                    pathList.push(path);
                }
            }

            var path = pathList[Math.floor(Math.random() * pathList.length)];
            return path;
        },

        sortPathByLength: function(pathList) {
            //パスリストを距離が長い順から降順で返す
            pathList.sort(function(a, b) {
              if (a.length < b.length)
                 return 1;
              if (a.length > b.length)
                return -1;
              return 0;
            });
            return pathList;
        },

        sortPathByLength2: function(pathList) {
            //パスリストを距離が長い順から降順で返す
            //パスリストを分割
            var movePos = [];
            var pathListOnly = [];
            for(var i=0;i<pathList.length;i++)
            {
                pathListOnly.push(pathList[i].Path);
                movePos.push(pathList[i].MovePoint);
            }

            for(var i = 0;i < pathList.length - 1; i++)
            {
                for(var j = i + 1;j < pathList.length; j++)
                {
                    if(pathListOnly[i].length < pathListOnly[j].length)
                    {
                        var tmp = pathListOnly[i];
                        pathListOnly[i] = pathListOnly[j];
                        pathListOnly[j] = tmp;

                        var pos = movePos[i];
                        movePos[i] = movePos[j];
                        movePos[j] = pos;
                    }
                }
            }

            return {Path:pathListOnly,MovePos:movePos};
        },

        testSkillUseInCombat: function(fune) {
            //スキル使用
            if (fune.canUseSkill()) {
                if (fune instanceof CaptainFune) {
                    //キャプテンだった場合
                    var count = this.player.getFuneCount();
                    var woundedCount = 0;
                    for(var j=0; j < count; j++) {
                        var woundedFune = this.player.getFune(j);
                        if (woundedFune.getHP() < (woundedFune.getHPMax() *0.7)) {
                            //HPが7割を切っていたさい確率上昇
                            woundedCount++;
                        }
                    }
                    if (Math.random() <= (woundedCount/this.player.getFuneCount()) ) {
                        //スキル発動
                        return {
                            type: "skill",
                            fune: fune,
                        }
                    }
                } 
                else if (fune instanceof KougekiFune) {
                    //攻撃だった場合
                    var targetCount = 0;
                    //敵が攻撃範囲内にいればいるほど確率上昇
                    var otherCount = this.player.controller.getNonActivePlayer().getFuneCount();
                    for(var j=0; j < otherCount; j++) {
                        var targetFune = this.player.controller.getNonActivePlayer().getFune(j);
                        if (fune.withinRange(targetFune.i, targetFune.j)) {
                            targetCount++;
                        }
                    }
                    if (Math.random() <= (targetCount *0.3) ) {
                        //スキル発動
                        return {
                            type: "skill",
                            fune: fune,
                        }
                    }
                } 
                else {
                    //他の船
                    if (Math.random() < 0.2) {
                        //2割で発動
                        return {
                            type: "skill",
                            fune: fune,
                        }
                    }
                }
            }
            return;
        }
    });

    var OpeningState = Class.create(BaseState, {
        //開始時の状態
        updateState: function() {
            var count = this.player.getFuneCount();
            for(var i=0; i < count; i++) {
                var fune    = this.player.getFune(i);
                var targets = this.getTargetsWithinRange(fune);
                if (targets.length > 0) {
                    //自分の船のいずれかがターゲットの攻撃範囲内に入った場合状態変更
                    console.log("AI switch from OpeningState to MidGameState");
                    return new MidGameState(this.player);
                }
            }
            console.log("AI in OpeningState");
            return this;
        },

        chooseAction: function() {
            //行動選択
            var fune = this.getRandomFune();

            // スキル
            if (fune instanceof KataiFune) {
                //防御船のみ序盤発動可
                if (fune.canUseSkill() && Math.random() < 0.3) {
                    console.log("AI use skill", fune.getCaptainName(), fune.getSkillName());
                    return {
                        type: "skill",
                        fune: fune,
                    }
                }
            } else {
                //他の船のスキルは使わない
            }

            //他のアクションがない場合ランダムに動く
            var moveList = this.getMovePosition(fune, 0.25);    //第二引数は逃亡確率
            var pathList = [];
            for (var i=0; i < moveList.length; i++) {
                var targetPosition = moveList[i];
                if (targetPosition.i <= (fune.i +2) && targetPosition.j >= (fune.j -2)) {
                    pathList.push(this.player.controller.map.getPath(fune, fune.i, fune.j, targetPosition.i, targetPosition.j));
                }
            }
            //移動距離が長いものを手前に
            pathList = this.sortPathByLength(pathList);
            //30％のバラツキを
            var randomLongPathIndex = Math.floor(Math.random() *(pathList.length *0.3));
            var path = pathList[randomLongPathIndex];
            if (path == null) {
                //安全なパスがない場合完全ランダム
                console.log("AI no safe path");
                path = this.getRandomPath(fune, 0.0);
            }

            //50の確率で突撃
            if(Math.random() < 0.5){
                path = this.getAttackPath(fune);
                console.log("AI attack Move", fune.getCaptainName());
            }
            else
            {
                console.log("AI random long Move", fune.getCaptainName());
            }
            
            return {
                type: "move",
                fune: fune,
                path: path,
            };
        },
    });

    var MidGameState = Class.create(BaseState, {
        //戦闘開始状態
        updateState: function() {
            //負けている場合
            if (this.player.getFuneCount() <= Math.ceil(this.player.getFuneCountInitial() /2)) {
                //自分の船の個数が初期の半分以下の場合
                console.log("AI switch from MidGameState to EndGameBadState");
                return new EndGameBadState(this.player);
            }
            //勝っている場合
            if (this.player.controller.getNonActivePlayer().getFuneCount() <= Math.ceil(this.player.controller.getNonActivePlayer().getFuneCountInitial() /2)) {
                //相手の船の個数が初期の半分以下の場合
                console.log("AI switch from MidGameState to EndGameGoodState");
                return new EndGameGoodState(this.player);
            }
            console.log("AI in MidGameState");
            return this;
        },

        chooseAction: function() {
            //行動選択
            var count = this.player.getFuneCount();
            for(var i=0; i < count; i++) {
                var fune = this.player.getFune(i);

                // skill を使う
                var skillUse = this.testSkillUseInCombat(fune);
                if (skillUse) {
                    console.log("AI use skill", fune.getCaptainName(), fune.getSkillName());
                    return skillUse;
                }

                //攻撃範囲内に敵が存在する場合
                var targets = this.getTargetsWithinRange(fune);
                if (targets.length > 0) {
                    if (Math.random() < 0.8) {
                        //8割で攻撃
                        var target = targets[Math.floor(Math.random() *targets.length)];
                        console.log("AI attack from", fune.getCaptainName(), "on", target.getCaptainName());
                        return {
                            type:   "attack",
                            fune:   fune,
                            target: target,
                        }
                    }
                }

                //HPが半分未満の場合
                if (fune.getHP() < (fune.getHPMax() * 0.5) ) {
                    if (Math.random() < 0.3) {
                        //3割の確率で逃亡
                        if (this.isTargeted(fune)) {
                            var path = this.getRandomPath(fune, 0.9);
                            if (path) {
                                console.log("AI escaping", fune.getCaptainName());
                                return {
                                    type:"move",
                                    fune: fune,
                                    path: path,
                                }
                            }
                        }
                    }
                }
            }
            //アクションがない場合ランダムに移動
            var fune = this.getRandomFune();
            var path = this.getRandomPath(fune, 0.5);
            if (path == null) {
                console.log("AI no safe path");
                path = this.getRandomPath(fune, 0.0);
            }
            console.log("AI random move", fune.getCaptainName());
            return {
                type:"move",
                fune: fune,
                path: path,
            }
        },
    });

    var EndGameGoodState = Class.create(BaseState, {
        //終盤のいい状態での動き
        updateState: function() {
            if (this.player.getFuneCount() < this.player.controller.getNonActivePlayer().getFuneCount()) {
                //敵が上回っている場合
                console.log("AI switch from EndGameGoodState to EndGameBadState");
                return new EndGameBadState(this.player);
            }
            console.log("AI in EndGameGoodState");
            return this;
        },

        chooseAction: function() {
            var count = this.player.getFuneCount();
            for(var i=0; i < count; i++) {
                var fune = this.player.getFune(i);

                // skill を使う
                var skillUse = this.testSkillUseInCombat(fune);
                if (skillUse) {
                    console.log("AI use skill", fune.getCaptainName(), fune.getSkillName());
                    return skillUse;
                }

                //攻撃範囲内に敵が存在する場合
                var targets = this.getTargetsWithinRange(fune);
                if (targets.length > 0) {
                    //9割で攻撃
                    if (Math.random() < 0.9) {
                        var target = targets[Math.floor(Math.random() *targets.length)];
                        console.log("AI attack from", fune.getCaptainName(), "on", target.getCaptainName());
                        return {
                            type:   "attack",
                            fune:   fune,
                            target: target,
                        }
                    }
                }
            }
            //移動
            var fune = this.getRandomFune();
            var path = this.getRandomPath(fune, 0.5);
            if (path == null) {
                console.log("AI no safe path");
                path = this.getRandomPath(fune, 0.0);
            }
            console.log("AI random move", fune.getCaptainName());
            return {
                type:"move",
                fune: fune,
                path: path,
            }
        },
    });

    var EndGameBadState = Class.create(BaseState, {
        //終盤の悪い状態
        updateState: function() {
            if (this.player.getFuneCount() > this.player.controller.getNonActivePlayer().getFuneCount()) {
                //自分の船が上回った場合
                console.log("AI switch from EndGameBadState to EndGameGoodState");
                return new EndGameGoodState(this.player);
            }
            console.log("AI in EndGameBadState");
            return this;
        },

        chooseAction: function() {
            var count = this.player.getFuneCount();
            for(var i=0; i < count; i++) {
                var fune = this.player.getFune(i);

                //敵の船の逃亡
                if (fune.getHP() < (fune.getHPMax() * 0.5) ) {
                    if (Math.random() < 0.5) {
                        if (this.isTargeted(fune)) {
                            var path = this.getRandomPath(fune, 0.9);
                            if (path) {
                                console.log("AI escaping", fune.getCaptainName());
                                return {
                                    type:"move",
                                    fune: fune,
                                    path: path,
                                }
                            }
                        }
                    }
                }

                // skill を使う
                var skillUse = this.testSkillUseInCombat(fune);
                if (skillUse) {
                    console.log("AI use skill", fune.getCaptainName(), fune.getSkillName());
                    return skillUse;
                }

                //攻撃
                var targets = this.getTargetsWithinRange(fune);
                if (targets.length > 0) {
                    if (Math.random() < 0.9) {
                        var target = targets[Math.floor(Math.random() *targets.length)];
                        console.log("AI attack from", fune.getCaptainName(), "on", target.getCaptainName());
                        return {
                            type:   "attack",
                            fune:   fune,
                            target: target,
                        }
                    }
                }
            }
            //ランダムな移動
            var fune = this.getRandomFune();
            var path = this.getRandomPath(fune, 0.8);
            if (path == null) {
                console.log("AI no safe path");
                path = this.getRandomPath(fune, 0.0);
            }
            console.log("AI random move", fune.getCaptainName());
            return {
                type:"move",
                fune: fune,
                path: path,
            }
        },
    });

    //ゲーム管理クラス
    var GameManager = Class.create({
        initialize:function(){
            //プレイヤーオブジェクト管理
            this.playerList = [];
            this.turnCounter = 0;
            this.mode = "";

            //サウンドマネージャ
            this.sndManager = new SoundManager();
        },

        addPlayer: function(player) {
            //プレイヤーの追加
            player.setController(this);
            this.playerList.push(player)
        },

        setMap: function(map) {
            //マップの追加
            map.setController(this);
            this.map = map;
        },

        setFrameUI: function(ui) {
            this.frameUI = ui;
            ui.manager = this;
        },

        setStartPositions: function(startPositions) {
            //初期一の設定
            this.startPositions = startPositions;
        },

        getActivePlayer: function() {
            //現在誰のターンか返すメソッド
            return this.playerList[this.turnCounter % this.playerList.length];
        },

        getPlayer: function(number) {
            //指定したプレイヤーを取得
            return this.playerList[number -1];
        },

        getNonActivePlayer: function() {
            //アクティブでないプレイヤーを返す
           return this.playerList[(this.turnCounter +1) % this.playerList.length];
        },

        getFuneList: function() {
            //船全体の取得
            var funeList = [];
            var player1 = this.getPlayer(1);
            for(var i=0; i < player1.getFuneCount(); i++) {
                funeList.push(player1.getFune(i));
            }
            var player2 = this.getPlayer(2);
            for(var i=0; i < player2.getFuneCount(); i++) {
                funeList.push(player2.getFune(i));
            }
            return funeList;
        },

        beginVersusGame: function(opponent) {
            this.mode = "versus";
            // 船の初期の位置
            var startPositions = {
                player1: [
                    {i: 0, j: 8}, {i: 0, j: 6}, {i: 1, j: 7}, {i: 2, j: 8}
                ],
                player2: [
                    {i: 12, j: 0}, {i: 10, j: 0}, {i: 11, j: 1}, {i: 12, j: 2}
                ],
            }
            this.setStartPositions(startPositions);

            // プレイヤー１
            var player1 = new GamePlayer(1, {name:"プレイヤー１"});
            this.addPlayer(player1);
            // プレイヤー1に船を４つ
            player1.addFune(new CaptainFune(1));
            player1.addFune(new HayaiFune(2));
            player1.addFune(new KataiFune(3));
            player1.addFune(new KougekiFune(4));
           
            //プレイヤーの配置
            this.placePlayerShips(player1);

            // プレイヤー２
            var player2;
            if (opponent == "human") {
                player2 = new GamePlayer(2, {name:"プレイヤー２"});
            } else if (opponent == "ai") {
                player2 = new AIPlayer(2, {name:"プレイヤー２"});
            }
            this.addPlayer(player2);
            // プレイヤー2に船を４つ
            player2.addFune(new CaptainFune(1));
            player2.addFune(new HayaiFune(2));
            player2.addFune(new KataiFune(3));
            player2.addFune(new KougekiFune(4));

            //プレイヤーの配置
            this.placePlayerShips(player2);

            this.sndManager.playBGM();
            this.startTurn();
        },

        beginCampaignGame: function(stageId) {
            this.mode = "campaign";

            //セーブデータが存在する場合
            var funeList;
            var saveData = $.jStorage.get("save data");
            if (saveData) {
                stageId  = saveData.stageId;
                funeList = saveData.funeList;
            }

            //プレイヤー１
            var player1 = new GamePlayer(1, {name:"プレイヤー１"});
            this.addPlayer(player1);

            if (funeList) {
                //船リストが存在した場合
                for(var funeIndex = 0; funeIndex < funeList.length; funeIndex++) {
                    var fune = this.funeFactory(funeList[funeIndex]);
                    player1.addFune(fune);
                }
            } 
            else {
                // プレイヤー1に船を４つ
                player1.addFune(new CaptainFune(1));
                player1.addFune(new HayaiFune(2));
                player1.addFune(new KataiFune(3));
                player1.addFune(new KougekiFune(4));
            }

            //船の初期の位置
            var startPositions = {
                player1: [
                    {i: 0, j: 8}, {i: 0, j: 6}, {i: 1, j: 7}, {i: 2, j: 8}
                ],
            }

            //船の配
            this.setStartPositions(startPositions);

            this.placePlayerShips(player1);

            if (this.getPlayer(2) == undefined) {
                //プレイヤー2が定義されていない場合定義
                var player2 = new AIPlayer(2, {name:"敵"});
                this.addPlayer(player2);
            }

            if (stageId) {
                //ステージIDが存在する場合そのIDから
                this.setupStage(stageId);
            } else {
                this.setupStage(1);
            }

            this.sndManager.playBGM();
            this.startTurn();
        },

        placePlayerShips: function(player) {
            //場所の再配置
            for(var funeIndex = 0; funeIndex < player.getFuneCount(); funeIndex++) {
                var fune = player.getFune(funeIndex);
                this.map.addChild(fune);
                var startPosition;
                if (player.id == 1) {
                    startPosition = this.startPositions.player1[funeIndex];
                } else {
                    startPosition = this.startPositions.player2[funeIndex];
                }
                this.map.positionFune(fune, startPosition.i, startPosition.j);
            }
        },

        refreshPlayer: function(player) {
            //船の全回復
            for(var funeIndex = 0; funeIndex < player.getFuneCount(); funeIndex++) {
                var fune = player.getFune(funeIndex);
                var recoverHp = fune.getHPMax();
                fune.healDamage(recoverHp);
                fune.refreshSkill();
            }
            this.placePlayerShips(player);
            this.turnCounter = 0;
            this.skipTurns = 0;
            if (player instanceof AIPlayer) {
                player.resetState();
            }
        },

        setupStage: function(stageId) {
            this.stageId = stageId;

            var saveData = {
                //保存データ
                stageId: stageId,
                funeList: [],
            };

            //プレイヤーデータの保存
            var player = this.getPlayer(1);
            for(var funeIndex = 0; funeIndex < player.getFuneCount(); funeIndex++) {
                var fune = player.getFune(funeIndex);
                saveData.funeList.push(fune.getId());
            }
            $.jStorage.set("save data", saveData);

            //敵の生成
            var player2 = this.getPlayer(2);
            var stageIndex = (stageId-1) % StageData.length;
            var stageData = StageData[stageIndex];
            for(var i=0; i< stageData.startPositions.length; i++) {
                var entry = stageData.startPositions[i];

                var fune = this.funeFactory(entry.type);
                fune.originX = 32;
                fune.scaleX = -1;
                player2.addFune(fune);
                this.map.addChild(fune);
                this.map.positionFune(fune, entry.i, entry.j);
            }
        },

        startTurn: function() {
            //ターンの初期処理
            utils.endUIShield();
            //アクティブになっているプレイヤーをセット
            var player = this.getActivePlayer();
            if (this.skipTurns) {
                if (player != this.skipper) {
                    //相手のターンをスキップする
                    this.skipTurns--;
                    utils.beginUIShield();
                    return this.endTurn();
                }
            }
            player.setActive(true);
            this.updateTurn();

            if (player instanceof AIPlayer) {
                //AIのインスタンスがされている場合
                utils.beginUIShield();
                player.simulatePlay();
            }
        },

        updateTurn: function() {
            //ターンの更新処理
            //アクティブになっている船のセット、移動範囲の描画
            this.map.setActiveFune(this.getActivePlayer().getActiveFune());
            this.map.drawMovementRange();
            this.frameUI.updateTurn(this.turnCounter);
            this.frameUI.updatePlayer(this.getActivePlayer().getData("name"));
            this.sndManager.playFX(sndChangeShips);
        },

        endTurn: function() {
            //ターン終了処理
            //現在のアクティブのものをfalseに変え、ターンカウンタを進める
            var player = this.getActivePlayer();
            player.setActive(false);

            var winner = this.getWinner();
            if (winner) {
                var self = this;
                setTimeout(function(){
                    if (self.mode == "versus") {
                        self.versusOver(winner);
                    } else if (self.mode == "campaign") {
                        self.campaignOver(winner);
                    }
                }, 1000);
            } 
            else {
                //勝者が決まっていない場合
                this.turnCounter++;

                //ターン経過のバナーを表示
                var playerBanner = new Sprite(512, 256);
                if (player.id == 1) {
                    playerBanner.image = game.assets[uiPlayerBanner2];
                } 
                else if (player.id == 2) {
                    playerBanner.image = game.assets[uiPlayerBanner1];
                }

                playerBanner.opacity = 0;
                playerBanner.x = 480 -256;
                playerBanner.y = 320 -128;
                game.currentScene.addChild(playerBanner);

                var self = this;
                playerBanner.tl.fadeIn(20).delay(30).fadeOut(10).then(function() {
                    self.startTurn();
                    game.currentScene.removeChild(playerBanner);
                })
            }
        },

        versusOver: function(winner) {
            //VS画面でのゲームオーバー
            var touchable = new ShieldWindow(this);
            utils.beginUIShield();

            var playerBanner = new Sprite(512, 256);
            //勝者のバナー表示
            if (winner.id == 1) {
                playerBanner.image = game.assets[uiPlayerBanner1];
            } else if (winner.id == 2) {
                playerBanner.image = game.assets[uiPlayerBanner2];
            }

            playerBanner.opacity = 0;
            playerBanner.x = 480 -256;
            playerBanner.y = 320 -128;
            game.currentScene.addChild(playerBanner);

            var self = this;
            playerBanner.tl.fadeIn(20).delay(30).fadeOut(10).then(function() {
                game.currentScene.removeChild(playerBanner);

                var resultBanner = new Sprite(512, 256);
                resultBanner.image = game.assets[uiWin];
                resultBanner.opacity = 0;
                resultBanner.touchEnabled = false;
                resultBanner.x = 480 -256;
                resultBanner.y = 320 -128;
                game.currentScene.addChild(resultBanner);

                resultBanner.tl.fadeIn(20).then(function(){
                    touchable.onTouch = function() {
                        location.reload();
                    };
                    utils.endUIShield();
                });
            });
        },

        campaignOver: function(winner) {
            //ストーリーでのゲームオーバー処理
            var touchable = new ShieldWindow(this);
            //utils.beginUIShield();

            var playerBanner = new Sprite(512, 256);
            playerBanner.image = game.assets[uiPlayerBanner1];

            playerBanner.opacity = 0;
            playerBanner.x = 480 -256;
            playerBanner.y = 320 -128;
            game.currentScene.addChild(playerBanner);

            var self = this;
            playerBanner.tl.fadeIn(20).delay(30).fadeOut(10).then(function() {
                game.currentScene.removeChild(playerBanner);

                var resultBanner = new Sprite(512, 256);
                if (winner.id == 1) {
                    //プレイヤー1の勝利
                    resultBanner.image = game.assets[uiWin];
                    touchable.onTouch = function() {
                        //全回復で次のステージへ
                        self.refreshPlayer(winner);
                        self.setupStage(self.stageId +1);
                        self.startTurn();
                    };
                } 
                else if (winner.id == 2) {
                    //プレイヤー2の勝利
                    resultBanner.image = game.assets[uiLose];

                    //ツイート
                    var tweet = new TwitterButton({
                        stageId: self.stageId,
                        url: "https://www.dropbox.com/s/q0wvzsgdxmqersg/index.html?dl=0"
                    });
                    touchable.addChild(tweet);
                    tweet.x = 480 -32;
                    tweet.y = 450;

                    touchable.onTouch = function() {
                        //セーブデータの削除
                        $.jStorage.deleteKey("save data");
                        //ブラウザリロード
                        location.reload();
                    };
                }
                resultBanner.opacity = 0;
                resultBanner.touchEnabled = false;
                resultBanner.x = 480 -256;
                resultBanner.y = 320 -128;
                game.currentScene.addChild(resultBanner);

                //バナー表示
                resultBanner.tl.fadeIn(20).then(function(){
                    utils.endUIShield();
                });
            });
        },

        getWinner: function() {
            //アクティブ状態の船の数チェック
            if (this.getActivePlayer().getFuneCount() == 0) {
                //ノンアクティブ状態の船の数のチェック
                if (this.getNonActivePlayer().getFuneCount() == 0) {
                    //両方の船が0の場合は現在のアクティブ状態が勝利
                    return this.getActivePlayer();
                } 
                else {
                    //アクティブ状態の船が0の場合は相手の勝利
                    return this.getNonActivePlayer();
                }
            } 
            else if (this.getNonActivePlayer().getFuneCount() == 0) {
                //相手が0の場合は自分の勝利
                return this.getActivePlayer();
            }
            //勝利無し
            return null
        },

        openSettings: function() {
            //画面設定の取得
            new SettingsWindow(this);
        },

        getFreeTurns: function(player, turns) {
            this.skipper = player;
            this.skipTurns = turns;
        },

        funeFactory: function(name) {
            //名前に応じたものを生成
            switch(name) {
                case 1:
                case "captain":
                    return new CaptainFune(1);
                case 2:
                case "hayai":
                    return new HayaiFune(2);
                case 3:
                case "katai":
                    return new KataiFune(3);
                case 4:
                case "kougeki":
                    return new KougekiFune(4);
                case 5:
                case "teki_boss":
                    return new BossTeki(5);
                case 6:
                case "teki_hayai":
                    return new HayaiTeki(6);
                case 7:
                case "teki_katai":
                    return new KataiTeki(7);
                case 8:
                case "teki_kougeki":
                    return new KougekiTeki(8);
            }
        },
    })

     //ターン関係の情報を表示するクラス
    var FrameUI = Class.create({
        initialize: function(scene) {
            var fontColor = "rgba(255, 255, 105, 1.0)";

            this.turnLabel = new Label();
            scene.addChild(this.turnLabel);
            this.turnLabel.x = 64*5;
            this.turnLabel.y = 640 -40;
            this.turnLabel.font = fontStyle;
            this.turnLabel.color = fontColor;

            this.playerLabel = new Label();
            scene.addChild(this.playerLabel);
            this.playerLabel.x = 64;
            this.playerLabel.y = 640 -40;
            this.playerLabel.font = fontStyle;
            this.playerLabel.color = fontColor;

            //設定
            this.settingsButton = new Sprite(64, 64);
            scene.addChild(this.settingsButton);
            this.settingsButton.image = game.assets[uiSettingsSprite];
            this.settingsButton.x = 64*14;
            this.settingsButton.y = 640 -64;

            var self = this;
            this.settingsButton.addEventListener(enchant.Event.TOUCH_START, function(params) {
                self.settingsButton.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
                new SettingsWindow(self.manager);
            });
        },

        updateTurn: function(turn) {
            this.turnLabel.text = "ターン:"+turn;
        },

         updatePlayer: function(name) {
            this.playerLabel.text = name;
        },
    })

    /**
     * オーディオ管理
     */
    var SoundManager = Class.create(Sprite, {
        initialize: function() {
            //ボリュームの設定
            Sprite.call(this, 1,1);
            //ストレージから保存データを呼び出す
            this.volume = $.jStorage.get("sound volume", 0.5);
            this.bgmPlaying = false;
        },

        playBGM: function() {
            this.bgmPlaying = true;

            //サウンド再生
            game.assets[sndBGM].play();
            if(game.assets[sndBGM].src){
                //ループ再生
                game.assets[sndBGM].src.loop = true;
            } 
            else {
                game.currentScene.addChild(this);
            }
            game.assets[sndBGM].volume = this.volume *0.3;
        },

        playFX: function(name) {
            //サウンドの複製（連続して音を鳴らすため）
            var fx = game.assets[name].clone();
            fx.play();
            fx.volume = this.volume;
        },

        pauseBGM: function() {
            //ポーズ
            this.bgmPlaying = false;
            game.assets[sndBGM].pause();
        },

        stopBGM: function() {
            //ストップ
            this.bgmPlaying = false;
            game.assets[sndBGM].stop();
        },

        volumeUp: function() {
            //音量アップ
            this.volume += 0.05;
            if (this.volume > 1) {
                this.volume = 1;
            }
            console.log("volume", this.volume);
            //音量をストレージへ保存
            $.jStorage.set("sound volume", this.volume);
            game.assets[sndBGM].volume = this.volume *0.3;
            this.playFX(sndClick);
        },

        volumeDown: function() {
            //音量ダウン
            this.volume -= 0.05;
            if (this.volume < 0) {
                this.volume = 0;
            }
            console.log("volume", this.volume);
            $.jStorage.set("sound volume", this.volume);
            game.assets[sndBGM].volume = this.volume *0.3;
            this.playFX(sndClick);
        },

        getVolume: function() {
            //音量の取得
            return this.volume;
        },

        onenterframe: function(){
            if (this.bgmPlaying) {
                game.assets[sndBGM].play();
            }
        },
    })

    //キャラのポップアップウィンドウ
    var FunePopup = Class.create(Scene, {
        initialize: function(fune) {
            Scene.call(this);
            game.pushScene(this);

            fune.player.controller.sndManager.playFX(sndClick);

            //各画像の読み込み,セット
            var shieldSprite = new Sprite(960, 640);
            shieldSprite.image = game.assets[ui1x1Black];
            shieldSprite.opacity = 0.5
            this.addChild(shieldSprite);

            var windowGroup = new Group();
            windowGroup.x = (960 -512)/2;
            windowGroup.y = (640 -512)/2;
            this.addChild(windowGroup);

            var windowSprite = new Sprite(512, 512);
            windowSprite.image = game.assets[uiWindowSprite];
            windowGroup.addChild(windowSprite);

            var statsGroup = new Group();
            statsGroup.x = 64;
            statsGroup.y = 32;
            windowGroup.addChild(statsGroup);

            var fontColor = "rgba(255, 255, 105, 1.0)";

            //各ステータスの読み込み
            captainLabel = new Label("船長："+fune.getCaptainName());
            statsGroup.addChild(captainLabel);
            captainLabel.x = 0;
            captainLabel.y = 0;
            captainLabel.font = fontStyle;
            captainLabel.color = fontColor;

            attackLabel = new Label("攻撃力："+fune.getAttack());
            statsGroup.addChild(attackLabel);
            attackLabel.x = 0;
            attackLabel.y = 64 *1;
            attackLabel.font = fontStyle;
            attackLabel.color = fontColor;

            defenseLabel = new Label("防御力："+fune.getDefense());
            statsGroup.addChild(defenseLabel);
            defenseLabel.x = 0;
            defenseLabel.y = 64 *2;
            defenseLabel.font = fontStyle;
            defenseLabel.color = fontColor;

            movementLabel = new Label("移動力："+fune.getMovement());
            statsGroup.addChild(movementLabel);
            movementLabel.x = 0;
            movementLabel.y = 64 *3;
            movementLabel.font = fontStyle;
            movementLabel.color = fontColor;

            rangeLabel = new Label("攻撃の距離："+fune.getRange());
            statsGroup.addChild(rangeLabel);
            rangeLabel.x = 0;
            rangeLabel.y = 64 *4;
            rangeLabel.font = fontStyle;
            rangeLabel.color = fontColor;

            hpLabel = new Label("HP："+fune.getHP()+"/"+fune.getHPMax());
            statsGroup.addChild(hpLabel);
            hpLabel.x = 0;
            hpLabel.y = 64 *5;
            hpLabel.font = fontStyle;
            hpLabel.color = fontColor;
        
            var pirate = new Sprite(400, 640);
            pirate.x = 350;
            pirate.y = -50;
            pirate.opacity = 0;
            pirate.image = fune.getImage();
            windowGroup.addChild(pirate);

            var self = this;
            var cancelBtnSprite = new Sprite(128, 64);
            cancelBtnSprite.image = game.assets[uiCancelBtnSprite];
            cancelBtnSprite.x = 64;
            cancelBtnSprite.y = 512 -64 -32;
            windowGroup.addChild(cancelBtnSprite);

            //スキル
            var skillBtnSprite = new Sprite(128, 64);
            skillBtnSprite.image = game.assets[uiSkillBtnSprite];
            skillBtnSprite.x = 64 *4;
            skillBtnSprite.y = 512 -64 -32;
            windowGroup.addChild(skillBtnSprite);

            windowGroup.originX = 256;
            windowGroup.originY = 256;
            windowGroup.scaleX = 0.7;
            windowGroup.scaleY = 0.7;
            windowGroup.tl.scaleTo(1, 10, enchant.Easing.ELASTIC_EASEOUT).then(function() {
                pirate.y = -50;
                pirate.tl.moveBy(-50, -25, 5).and().fadeIn(10);

                //マウスボタンを押した際ボタンが少し大きくなる
                cancelBtnSprite.addEventListener(enchant.Event.TOUCH_START, function(params)
                {
                   cancelBtnSprite.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
                });

                //段階的に出現
                //離すとウィンドウが消える
                cancelBtnSprite.addEventListener(enchant.Event.TOUCH_END, function(params)
                {
                    shieldSprite.tl.fadeTo(0, 5);
                    cancelBtnSprite.tl.scaleTo(0.9, 3).and().fadeTo(0, 5);
                    pirate.tl.fadeTo(0, 5);
                    windowSprite.tl.fadeTo(0, 5).then(function()
                    {
                        game.popScene();
                        if (self.onCancel)
                        {
                            self.onCancel();
                        }
                    });
                });

                //スキルのウィンドウ表示
                skillBtnSprite.addEventListener(enchant.Event.TOUCH_START, function(params) {
                    skillBtnSprite.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
                });

                skillBtnSprite.addEventListener(enchant.Event.TOUCH_END, function(params) {
                    shieldSprite.tl.fadeTo(0, 5);
                    skillBtnSprite.tl.scaleTo(0.9, 3).and().fadeTo(0, 5);
                    pirate.tl.fadeTo(0, 5);
                    windowSprite.tl.fadeTo(0, 5).then(function() {
                        game.popScene();
                        fune.player.controller.sndManager.playFX(sndClick);
                        if (self.onSkill) {
                            self.onSkill()
                        }
                    });
                });
            });
        }
    })

        var TwitterButton = Class.create(Sprite, {
            //ツイート
            initialize: function(options) {
                Sprite.call(this, 64, 64);
                this.image = game.assets[uiTwitterBtnSprite];
                this.stageId = options.stageId;
                this.url     = options.url;
            },

            ontouchend: function(params) {
                window.open("https://twitter.com/intent/tweet?url="+encodeURIComponent(this.url)+"&text="+encodeURIComponent("ステージ"+this.stageId+"まで行けました！みんなはどこまで行ける？")+"&hashtags=piratesTactics,海賊", "twitter", "top=50, left=50, width=500, height=400");
            }
    })

    /**
     * シールドタッチのポップアップウィンドー
     */
    var ShieldWindow = Class.create(Scene, {
        initialize: function(gameManager) {
            Scene.call(this);
            game.pushScene(this);

            gameManager.sndManager.playFX(sndClick);

            var shieldSprite = new Sprite(960, 640);
            shieldSprite.image = game.assets[ui1x1Black];
            shieldSprite.opacity = 0.5
            this.addChild(shieldSprite);

            var self = this;
            shieldSprite.addEventListener(enchant.Event.TOUCH_END, function(params) {
                if (self.onTouch) {
                    gameManager.sndManager.playFX(sndClick);
                    game.popScene();
                    self.onTouch();
                }
            });
        }
    })

    //画面アラート
    var AlertWindow = Class.create(Scene, {
        initialize: function(message, gameManager) {
            Scene.call(this);
            game.pushScene(this);

            var shieldSprite = new Sprite(960, 640);
            shieldSprite.image = game.assets[ui1x1Black];
            shieldSprite.opacity = 0.2
            this.addChild(shieldSprite);

            var windowSprite = new Sprite(320, 160);
            windowSprite.image = game.assets[uiAlertScreen];
            windowSprite.x = (960 -320)/2;
            windowSprite.y = (640 -160)/2;
            this.addChild(windowSprite);

            var fontColor = "rgba(255, 255, 105, 1.0)";

            messageLabel = new Label(message);
            this.addChild(messageLabel);
            messageLabel.x = windowSprite.x +40;
            messageLabel.y = windowSprite.y +64;
            messageLabel.font = fontStyle;
            messageLabel.color = fontColor;

            var once = false;
            var self = this;
            this.addEventListener(enchant.Event.TOUCH_END, function(params) {
                if (once == false) {
                    once = true;
                    windowSprite.tl.fadeTo(0, 5).then(function() {
                        gameManager.sndManager.playFX(sndClick);
                        game.popScene();
                        if (self.onTouch) {
                            self.onTouch();
                        }
                    });
                }
            });
        },
    })

    /**
     * 音量調節
     */
    var SettingsWindow = Class.create(Scene, {
        initialize: function(gameManager) {
            Scene.call(this);
            game.pushScene(this);

            var shieldSprite = new Sprite(960, 640);
            shieldSprite.image = game.assets[ui1x1Black];
            shieldSprite.opacity = 0.5
            this.addChild(shieldSprite);

            var windowGroup = new Group();
            windowGroup.x = (960 -512)/2;
            windowGroup.y = (640 -512)/2;
            this.addChild(windowGroup);

            var windowSprite = new Sprite(512, 512);
            windowSprite.image = game.assets[uiWindowSprite];
            windowGroup.addChild(windowSprite);

            var settingsGroup = new Group();
            settingsGroup.x = 64;
            settingsGroup.y = 32;
            windowGroup.addChild(settingsGroup);

            var fontColor = "rgba(255, 255, 105, 1.0)";

            soundLabel = new Label("音量");
            settingsGroup.addChild(soundLabel);
            soundLabel.x = 0;
            soundLabel.y = 16;
            soundLabel.font = fontStyle;
            soundLabel.color = fontColor;

            var sndUpButton = new Sprite(64, 64);
            settingsGroup.addChild(sndUpButton);
            sndUpButton.x = 64 *4;
            sndUpButton.y = 0;
            sndUpButton.image = game.assets[uiArrowSprite];

            var isKeyPressed = false;
            //サウンド調節ボタン
            sndUpButton.addEventListener(enchant.Event.TOUCH_START, function(params) {
                if (gameManager.sndManager.getVolume() < 1) {
                    if (isKeyPressed == false) {
                        isKeyPressed = true;
                        sndUpButton.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT);
                    }
                }
            });

            //サウンド調節ボタンの段階的表示
            sndUpButton.addEventListener(enchant.Event.TOUCH_END, function(params) {
                if (gameManager.sndManager.getVolume() < 1) {
                    if (isKeyPressed == true) {
                        gameManager.sndManager.volumeUp();
                        sndUpButton.tl.scaleTo(1.0, 3).then(function() {
                            isKeyPressed = false;
                        });
                    }
                }
            });

            var sndDownButton = new Sprite(64, 64);
            settingsGroup.addChild(sndDownButton);
            sndDownButton.x = 64*5 +5;
            sndDownButton.y = 0;
            sndDownButton.rotation = 180;
            sndDownButton.image = game.assets[uiArrowSprite];

            sndDownButton.addEventListener(enchant.Event.TOUCH_START, function(params) {
                if (gameManager.sndManager.getVolume() > 0) {
                    if (isKeyPressed == false) {
                        isKeyPressed = true;
                        sndDownButton.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT);
                    }
                }
            });

            sndDownButton.addEventListener(enchant.Event.TOUCH_END, function(params) {
                if (gameManager.sndManager.getVolume() > 0) {
                    if (isKeyPressed == true) {
                        gameManager.sndManager.volumeDown();
                        sndDownButton.tl.scaleTo(1.0, 3).then(function() {
                            isKeyPressed = false;
                        });
                    }
                }
            });

            var self = this;
            var cancelBtnSprite = new Sprite(128, 64);
            cancelBtnSprite.image = game.assets[uiCancelBtnSprite];
            cancelBtnSprite.x = 64;
            cancelBtnSprite.y = 512 -64 -32;

            windowGroup.addChild(cancelBtnSprite);

            //ウィンドウ事態の表示
            windowGroup.originX = 256;
            windowGroup.originY = 256;
            windowGroup.scaleX = 0.7;
            windowGroup.scaleY = 0.7;
            windowGroup.tl.scaleTo(1, 10, enchant.Easing.ELASTIC_EASEOUT).then(function() {
                cancelBtnSprite.addEventListener(enchant.Event.TOUCH_START, function(params) {
                    cancelBtnSprite.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
                });

                cancelBtnSprite.addEventListener(enchant.Event.TOUCH_END, function(params) {
                    shieldSprite.tl.fadeTo(0, 5);
                    cancelBtnSprite.tl.scaleTo(0.9, 3).and().fadeTo(0, 5);
                    windowSprite.tl.fadeTo(0, 5).then(function() {
                        game.popScene();
                    });
                });
            });
        },
    })

    /**
     * 初期画面
     */
    var StartScreen = Class.create(Scene, {
        initialize: function(gameManager) {
            Scene.call(this);
            game.pushScene(this);

            var shieldSprite = new Sprite(960, 640);
            shieldSprite.image = game.assets[ui1x1Black];
            shieldSprite.opacity = 0.5
            this.addChild(shieldSprite);

            var windowGroup = new Group();
            windowGroup.x = (960 -512)/2;
            windowGroup.y = (640 -512)/2;
            this.addChild(windowGroup);

            var windowSprite = new Sprite(512, 512);
            windowSprite.image = game.assets[uiStartScreen];
            windowGroup.addChild(windowSprite);

            var self = this;
            var versusBtnSprite = new Sprite(128, 64);
            versusBtnSprite.image = game.assets[uiVersusBtnSprite];
            versusBtnSprite.x = 64 *1.5;
            versusBtnSprite.y = 512 -64 -32;
            windowGroup.addChild(versusBtnSprite);

            var campaignBtnSprite = new Sprite(128, 64);
            campaignBtnSprite.image = game.assets[uiStoryBtnSprite];
            campaignBtnSprite.x = 64 *4.5;
            campaignBtnSprite.y = 512 -64 -32;
            windowGroup.addChild(campaignBtnSprite);

            windowGroup.originX = 256;
            windowGroup.originY = 256;

            //ボタン入力時大きくなる
            versusBtnSprite.addEventListener(enchant.Event.TOUCH_START, function(params) {
                versusBtnSprite.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
            });

            //段階的に表示
            versusBtnSprite.addEventListener(enchant.Event.TOUCH_END, function(params) {
                shieldSprite.tl.fadeTo(0, 5);
                versusBtnSprite.tl.scaleTo(0.9, 3).and().fadeTo(0, 5);
                windowSprite.tl.fadeTo(0, 5).then(function() {
                    gameManager.sndManager.playFX(sndClick);
                    game.popScene();
                    new VersusScreen(gameManager);
                });
            });

            campaignBtnSprite.addEventListener(enchant.Event.TOUCH_START, function(params) {
                campaignBtnSprite.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
            });

            campaignBtnSprite.addEventListener(enchant.Event.TOUCH_END, function(params) {
                shieldSprite.tl.fadeTo(0, 5);
                campaignBtnSprite.tl.scaleTo(0.9, 3).and().fadeTo(0, 5);
                windowSprite.tl.fadeTo(0, 5).then(function() {
                    gameManager.sndManager.playFX(sndClick);
                    game.popScene();
                    //ストーリーモード
                    new CampaignScreen(gameManager);
                });
            });
        },
    })

    /**
     * 初期選択画面
     */
    var CampaignScreen = Class.create(Scene, {
        initialize: function(gameManager) {
            Scene.call(this);
            game.pushScene(this);

            var shieldSprite = new Sprite(960, 640);
            shieldSprite.image = game.assets[ui1x1Black];
            shieldSprite.opacity = 0.5
            this.addChild(shieldSprite);

            var windowGroup = new Group();
            windowGroup.x = (960 -512)/2;
            windowGroup.y = (640 -512)/2;
            this.addChild(windowGroup);

            var windowSprite = new Sprite(512, 512);
            windowSprite.image = game.assets[uiStoryScreen];
            windowGroup.addChild(windowSprite);

            var self = this;

            var saveData = $.jStorage.get("save data");
            if (saveData) {
                //セーブデータが見つかった
                console.log("Found Save Data", saveData.stageId)
                var continueBtnSprite = new Sprite(128, 64);
                continueBtnSprite.image = game.assets[uiContinueBtnSprite];
                continueBtnSprite.x = 64 *1.5;
                continueBtnSprite.y = 512 -64 -32;
                windowGroup.addChild(continueBtnSprite);

                continueBtnSprite.addEventListener(enchant.Event.TOUCH_START, function(params) {
                    continueBtnSprite.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
                });

                continueBtnSprite.addEventListener(enchant.Event.TOUCH_END, function(params) {
                    shieldSprite.tl.fadeTo(0, 5);
                    continueBtnSprite.tl.scaleTo(0.9, 3).and().fadeTo(0, 5);
                    windowSprite.tl.fadeTo(0, 5).then(function() {
                        gameManager.beginCampaignGame();
                        gameManager.sndManager.playFX(sndClick);
                        game.popScene();
                    });
                });
            }

            //新規に始める処理
            var newBtnSprite = new Sprite(128, 64);
            newBtnSprite.image = game.assets[uiNewBtnSprite];
            newBtnSprite.x = 64 *4.5;
            newBtnSprite.y = 512 -64 -32;
            windowGroup.addChild(newBtnSprite);

            windowGroup.originX = 256;
            windowGroup.originY = 256;

            newBtnSprite.addEventListener(enchant.Event.TOUCH_START, function(params) {
                newBtnSprite.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
            });

            newBtnSprite.addEventListener(enchant.Event.TOUCH_END, function(params) {
                //セーブデータの削除
                $.jStorage.deleteKey("save data")
                shieldSprite.tl.fadeTo(0, 5);
                newBtnSprite.tl.scaleTo(0.9, 3).and().fadeTo(0, 5);
                windowSprite.tl.fadeTo(0, 5).then(function() {
                    gameManager.beginCampaignGame();
                    gameManager.sndManager.playFX(sndClick);
                    game.popScene();
                });
            });
        },
    })

    //VS画面
    var VersusScreen = Class.create(Scene, {
        initialize: function(gameManager) {
            Scene.call(this);
            game.pushScene(this);

            var shieldSprite = new Sprite(960, 640);
            shieldSprite.image = game.assets[ui1x1Black];
            shieldSprite.opacity = 0.5
            this.addChild(shieldSprite);

            var windowGroup = new Group();
            windowGroup.x = (960 -512)/2;
            windowGroup.y = (640 -512)/2;
            this.addChild(windowGroup);

            var windowSprite = new Sprite(512, 512);
            windowSprite.image = game.assets[uiVSScreen];
            windowGroup.addChild(windowSprite);

            var self = this;

            var humanBtnSprite = new Sprite(128, 64);
            humanBtnSprite.image = game.assets[uiHumanBtnSprite];
            humanBtnSprite.x = 64 *1.5;
            humanBtnSprite.y = 512 -64 -32;
            windowGroup.addChild(humanBtnSprite);

            windowGroup.originX = 256;
            windowGroup.originY = 256;

            humanBtnSprite.addEventListener(enchant.Event.TOUCH_START, function(params) {
                humanBtnSprite.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
            });

            humanBtnSprite.addEventListener(enchant.Event.TOUCH_END, function(params) {
                shieldSprite.tl.fadeTo(0, 5);
                humanBtnSprite.tl.scaleTo(0.9, 3).and().fadeTo(0, 5);
                windowSprite.tl.fadeTo(0, 5).then(function() {
                    gameManager.beginVersusGame("human");
                    gameManager.sndManager.playFX(sndClick);
                    game.popScene();
                });
            });

            var cpuBtnSprite = new Sprite(128, 64);
            cpuBtnSprite.image = game.assets[uiCpuBtnSprite];
            cpuBtnSprite.x = 64 *4.5;
            cpuBtnSprite.y = 512 -64 -32;
            windowGroup.addChild(cpuBtnSprite);

            windowGroup.originX = 256;
            windowGroup.originY = 256;

            cpuBtnSprite.addEventListener(enchant.Event.TOUCH_START, function(params) {
                cpuBtnSprite.tl.scaleTo(1.1, 10, enchant.Easing.ELASTIC_EASEOUT)
            });

            cpuBtnSprite.addEventListener(enchant.Event.TOUCH_END, function(params) {
                shieldSprite.tl.fadeTo(0, 5);
                cpuBtnSprite.tl.scaleTo(0.9, 3).and().fadeTo(0, 5);
                windowSprite.tl.fadeTo(0, 5).then(function() {
                    gameManager.beginVersusGame("ai");
                    gameManager.sndManager.playFX(sndClick);
                    game.popScene();
                });
            });
        },
    })

    /**
     * ロードが完了した直後に実行される関数を指定している。
     */
    game.onload = function(){
        var sceneGameMain = new Scene();

        // ゲームロジックの管理
        var manager = new GameManager();

        // マスのデータ
        var mapDisplayData = [
            [3, 3, 2, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0],
            [3, 2, 0, 0, 2, 3, 3, 2, 0, 1, 0, 0, 0],
            [3, 0, 4, 0, 2, 3, 3, 2, 0, 0, 0, 0, 0],
            [3, 0, 0, 0, 0, 2, 2, 0, 1, 1, 0, 0, 0],
            [0, 0, 0, 0, 4, 0, 0, 0, 1, 1, 0, 4, 0],
            [1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2],
            [0, 0, 0, 3, 3, 2, 0, 0, 0, 0, 4, 2, 3],
            [0, 0, 0, 3, 3, 3, 2, 0, 0, 2, 2, 3, 3],
        ];

        var map = new GameMap(sceneGameMain, mapDisplayData);
        manager.setMap(map);

        //ターンUI管理
        var frameUI = new FrameUI(sceneGameMain);
        manager.setFrameUI(frameUI);

        // ゲームにシーンを追加
        game.pushScene(sceneGameMain);

        new StartScreen(manager);
    };

    game.start();
};
