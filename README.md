天佑Project -system-
======================
制作中の長編RPG「天佑」のプログラムソースコードです。

## 「天佑Project」について
2014年春から10名ほどのメンバーとともに、2年後の完成を目指して制作している長編RPGです。  
システムのプログラムは現在すべて私NUNUが担当し、一からコードを書いています。  

現在はフィールドとイベント関連システムがほぼ完成し、戦闘システムを制作中です。  
シナリオエディターやマップエディターの制作も私が担当しています。


## プログラムの特徴

最大の特徴は、**プログラムと作品の分離**及び**作品設計自由度の高さ**です。  
そのための手段として**独自コマンド方式**による処理管理を組み込んでいます。

このプログラムではRPGのための枠組みのみを提供し、作品制作者（天佑プロジェクトの場合はシナリオライター）がこの枠組み上で独自のRPGを作っていきます。

具体的には、マップ・各種グラフィック・イベント・会話・戦闘データ等をすべて外部読み込みにしています。  
ゲーム処理は、外部読み込みファイル「.rpgファイル」上で「@コマンド」で表され、  
作品制作者は@コマンドを組み合わせることで、自分の世界観を自由に表現することが出来ます。  
（詳しくはReleaseフォルダ内の.rpgファイルをご覧ください。.rpgファイルの実体は只のテキストです）

#### 外部読み込みファイルの記述例
例：NPC"sister"の作成と会話設定をしたいとき

system.rpg（一部・例）
````````
@Load_Pic(tenyu_data/pic/npc00.png, npc_sis, NPC)  \画像読み込み
@Set_EventObj(0, 0, NPC, npc_sis, sister)　　\NPCの作成と位置の指定

\@Load_Pic(読み込む画像, 画像タグ（自由指定）, 画像の種別)
\@Set_EventObj(X, Y, イベントオブジェクトの種別, 画像タグ名, 名前（自由指定）)
````````
scenario.rpg（一部・例）
````````
@NPC_BEGIN(sister)  \system.rpgで作成したNPCに話しかけると自動的に会話が始まりこのブロック内のイベントが実行される
  @Anten(500)  \500msかけて画面を暗転
  @Meiten(500)  \500msかけて画面を明転
  @TName_Now(left, 妹)  \発言者ラベルの作成
  @Jump(sister)  \NPCがジャンプ
  お兄ちゃん、おかえり！
  @NextPage  /ページ送り
  @TName_Now(right, 僕)
  ああ、ただいま
  @NextPage  /ページ送り
  @TName_Now(left, 妹)
  お母さんも待ってるよ、先に家に入ってるから早く来てね！
  @NextPage  /ページ送り
  @Anten(500)
  @Visible_Set(sister, false)  /NPCの退場=画面が暗転している間に姿を消す
  @Meiten(500)  
@NPC_END
````````

## ソースコードについて
各ヘッダファイルとその中で定義されているクラスについての簡単な説明を以下に書きます。  
ソースコード本体にもほぼ同様のコメントが記載してあります。  

ファイル数の多いコードですので、解読の一助となれば幸いです。

----------------------------------------------

##### Define.h
すべてのcppで読み込まれます。  
グローバルな定数・型・関数を置いています。  
defineマクロでビルドモード（製品版/プログラマデバッグ版など）を管理しています。

##### nunuLib.h
自作ライブラリです。  
char型を扱う練習のため最近までstd::stringを封印していたため、charやchar*に関する関数が多くあります。  
エラー/デバッグログ出力用関数は自分の使い勝手の為にこだわりました。

##### Mrt.h
タイトル画面のデザインとシステムを分離するための仕組みです。  
将来的に他メンバーとプログラミングを分担する可能性を考えて試験的に作りました。  
いまのところ特に使用していません。

##### mrtLib.h
将来的に他メンバーとプログラミグぐを分担する可能性を考えて、他メンバーが自作ライブラリを置くところとして試験的に作りました。  
いまのところ特に使用していません。

##### Menu.h
汎用メニュークラスCMenuを定義しています。  
階層構造を持つメニューを作るときに継承させて使います。  
現在は戦闘の行動選択メニューで使用中。

-----------------------------------

##### Main/Main.h
ゲーム開始時の基点として、タイトル画面からの遷移を管理します。

##### Main/CmdList.h
このプログラムの肝です。  
独自の@コマンドを文字列として格納します。  
@コマンドは外部読みこみで主に使用しますが、システム内でも処理をスムーズにするために時々使われます。

#####  Main/CmdManager.h
このプログラムの肝です。  
CmdListから@コマンドを読み取り順に処理します。  
使用される状況に合わせて、戦闘用CmdManager、初期設定用CmdManagerなどがあります。

##### Main/TextBox.h
テキストボックスを表示して文字列を描画します。  
イベントごとに文字列を受け取り、キー操作に合わせて順に表示していきます。

##### Main/TextWrap.h
テキストボックスの亜種です。  
画面全体を使って文字を表示します。

##### Main/TalkName.h
テキストボックスの上に表示される発言者のラベルです。
CTextBoxが包含して使っています。

##### Main/ScreenChanger.h
ゲーム内各所で使う、様々な画面の切り替え効果を使いやすくするためのクラスです。  
現在は戦闘開始時に使われています。

##### Main/WorldManager.h
後述のField.hのCFieldクラスとBattle.hのCBattleクラスに継承させて使われています。  
共通の処理を、多態性を使って扱うために作りました。

-----------------------------------

##### Field/Field.h
プレイヤーの移動・マップの表示・イベントの開始など、  
タイトルと戦闘以外のゲーム内のほぼすべてのプレイヤー活動を掌握します。  
様々なクラスの実体を所持（包含）しています。

##### Field/Map.h
マップデータやマップチップの読み込み・保持や、マップの描写を行います。

##### Field/EveObj.h
フィールド上で調べる対象となる物体、NPC、目に見えない踏むイベントスイッチなどはすべて、  
「イベントオブジェクト」としてCEveObjクラスのインスタンスとして扱われます。

##### Field/EveManger.h
イベントオブジェクトの配列を持ちあらゆる管理を行います。  
各イベントオブジェクトからメッセージを取り出したり、座標を代入したりする処理は、  
すべてイベントマネージャーを介して行われます。

##### Field/Load.h
.rpgファイルの読込みなどを管理します。  
読み込んだ内容はCMapやCEveManagerに渡されます。

-----------------------------------

##### Battle/Battle.h
戦闘全般を掌握します。  
様々なクラスの実体を所持（包含）しています。

##### Battle/Actor.h
戦闘に参加するキャラクターを敵味方共通管理するためのクラス。  
戦闘で使われる各種ステータス値や処理関数を持ちます。  
CPlayerとCEnemyが継承します。

##### Battle/Species.h
CSpeciesと、その子クラスCPlayerSpeciesとCEnemySpeciesを定義しています。  
プレイヤーキャラやエネミーの種類ごとにインスタンス化され、戦闘に関するステータスを持ちます。  
戦闘開始時にPlayerやEnemyに、ステータスを渡します。

##### Battle/SpeciesManager.h
プレイヤーキャラ情報（CPlayerSpecies）とエネミー情報（CEnemySpecies）をそれぞれ配列として管理します。  
CPlayerSpeciesやCEnemySpeciesの各インスタンスへはこのマネージャーを介してアクセスします。

##### Battle/Player.h
戦闘に参加するプレイヤーキャラのクラスです。
CActorとCPlayerSpeceisを継承します。

##### Battle/Enemy.h
戦闘に参加するエネミーのクラスです。
CActorとCEnemySpeceisを継承します。

##### Battle/EnemyPlanManager.h
エネミーのAIを管理します。

##### Battle/BattleCalculator.h
戦闘に関する各種計算を受け持ちます。  
将来的に戦闘計算は複雑化することが予想されるので、計算関数だけ分離していく予定です。

##### Battle/BImgBank.h
戦闘に使用する各種画像を管理します。

##### Battle/TrickManager.h
戦闘で使用される技の情報を管理します。


## おまけ
#### 序幕
物語の舞台は、海に浮かぶ島、ソラリシア。  
この島では八百万の土着宗教が息づき、多くの民族が静かに暮らしている。

少年は今日も、輝く星々を屋根の上から見ていた。  
星の民、風上冬仁。まだ世界を知らない、未来の英雄。
***
この島ではかつて、無数の少数民族が部落をつくってひっそりと暮らしていた。  
彼らはそれぞれ独自の生活様式と宗教を持ち、自然とともに生活を送っていた。  
そのなかで平原の民だけは、いくつかの部落を束ねることで一つの国家を生み出した。  
ソラリシア帝国と名乗り“太平の教え”を掲げるその国はいつしか暴走を始める。  
平和布教の御旗のもとに、帝国は他の民の暮らす部落に介入を開始したのだ。  
進んだ科学技術と強大な軍事力を背景に事実上の侵攻を始めた帝国に対し、  
多くの民はなすすべを持たなかったが、帝国と戦うことを選んだ者たちもいた。  

それから20年。帝国による異教徒弾圧が続く中、  
いままた一人の星の民の少年が、父親を奪った帝国への憎悪を燃やし戦いに身を投じる。  
やがて多くの出会いの中で戦いの真の意味を見つけた彼は、  
家族を守るため、国を変えるため、仲間を率いて立ち上がることを決意する。  
20年前に散った、あの英雄のように。  

<center>_失われた自由を、取り戻せ。_  
_満天の星空を、取り戻せ。_</center>
