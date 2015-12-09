
# include <Siv3D.hpp>

#define ICENUM 300//背景の画像数



/*グローーーバルな変数たち----------------------*/
int Screenpart = 0;//画面の状態変数 [0:タイトル　1:ゲーム画面]

int keyhitflag = 0;//キーが押されたら「1」
double y[ICENUM];//各氷背景のｙ座標
double vo;//速度変数

double gagebar = 0.0;//速度ゲージy座標
int gagemovement = 1;//速度ゲージ[1:上がる 0:下がる]
double gagespeed = 10.0; //速度ゲージの速さ

int brushsetflag = 0;//ブラシ移動準備フラグ[0:存在せず 1:移動完了]
int brushpos = 0;//ブラシの位置[0:手前　1:奥]
int brushmoveflag = 0;//ブラシ移動中フラグ[0:停止中 1:移動中]
int brushcount = 0;//ブラシポイントカウンター
int brushx = -650;//ブラシ画像ｘ座標

double sita = 0.0;//石の角度変数

bool finishflag = false;//finishpicの出現フラグ[false:ゲーム進行中 true:ゲーム終了中]
int finishcount = 0;//finishpicの出現時間カウント[300fまで]
int finishy = -200;//finishpicのy座標

int score = 0;//スコア変数
int bounus = 0;//難易度によるボーナスポイント

int i;//for用
/*----------------------------------------------*/



/*デバック用数値表示*/
void Debug()
{
	PutText(L"point:", y[0]).from(0, 0);
	PutText(L"brushcount:", brushcount).from(0, 20);
	PutText(L"vo:", vo).from(0, 40);
	PutText(L"gagebar:", gagebar).from(0, 60);
}

/*ゲージ処理*/
void SpeedSetBar()
{
	if (keyhitflag == 0)//ストーン発射前
	{
		if (gagemovement == 1)gagebar += gagespeed;//ゲージが上がる間、ゲージスピード分だけ＋
		if (gagemovement == 0)gagebar -= gagespeed;//ゲージが下がる間、ゲージスピード分だけー

		if (gagebar >= 200.0)gagemovement = 0;//ゲージが上がり切ったら下げる
		if (gagebar <= 0.0)gagemovement = 1;//ゲージが下がり切ったら上げる
	}
	if (keyhitflag == 1)//ストーン発射後
	{
		gagebar = vo * 4.0;//ストーンの現在の速度をゲージで表示
	}
}

/*ブラシ移動処理*/
void BrushMovement()
{
	/*
		後ほど//if (Input::MouseR.pressed){}を削除してください	
	*/


	if (brushsetflag == 1)
	{
		if ((Input::MouseL.pressed) && (brushpos == 1) && (brushmoveflag == 0)){//ブラシ引っ込み処理
			//if (Input::MouseR.pressed){}
			//else
			{
				brushpos = 0;//ブラシの位置変更
				brushmoveflag = 1;//ブラシ移動中フラグ
				brushcount++;//ブラシポイント加算
			}
		}
		if ((Input::MouseR.pressed) && (brushpos == 0) && (brushmoveflag == 0)){//ブラシ押し込み処理
			//if (Input::MouseL.pressed){}
			//else
			{
				brushpos = 1;//ブラシの位置変更
				brushmoveflag = 1;//ブラシ移動中フラグ
				brushcount++;//ブラシポイント加算
			}
		}

		if ((brushmoveflag == 1) && (brushpos == 0))//ブラシ引っ込みアニメーション処理
		{
			brushx -= 50;							//ブラシｘ座標移動
			if (brushx == -300)brushmoveflag = 0;	//ブラシが引っ込み終わったらブラシ移動中フラグを消す
		}

		if ((brushmoveflag == 1) && (brushpos == 1))//ブラシ押し込みアニメーション処理
		{
			brushx += 50;							//ブラシｘ座標移動
			if (brushx == -150)brushmoveflag = 0;	//ブラシが押し込み終わったらブラシ移動中フラグを消す
		}
	}
}

/*背景移動処理*/
void BackgroundMovement()
{
	/*Spaceが押されたらkeyhitflagを１にする*/
	if (Input::KeySpace.clicked)
	{
		keyhitflag = 1;		//カーリング石発射フラグを入れる
		vo = gagebar / 4.0;	//初速度設定（最低:0,最大:50)
	}

	/*keyhitflagが１の間,vo(速度）を与え、背景が動き続ける*/
	if (keyhitflag == 1)
	{
		for (i = 0; i < ICENUM; i++)y[i] += vo;//背景移動処理

		/*減速処理*/
		if (brushmoveflag == 1)vo -= 0.02;	//ブラシ移動中のみ減速の効果小
		else vo -= 0.05;					//通常減速

		sita += 0.1;//ストーン回転処理
	}

	/*速度が0以下になったらkeyhitflagを０にする*/
	if (vo < 0.0)
	{
		keyhitflag = 0;
		gagebar = 0.0;
		vo = 0.0;
		gagemovement = 1;
		brushsetflag = 0;//ブラシの動きをストップ
		finishflag = true;//finish画像出現フラグを入れる
	}
}

/*タイトル描写*/
void Title()
{
	PutText(L"楽しい（楽しい）カーリングゲ～～～ム").from(0, 0);
	PutText(L"[I]:EASY").from(0, 20);
	PutText(L"[O]:NORMAL").from(0, 40);
	PutText(L"[P]:HARD").from(0, 60);
	PutText(L"[L]:HELL").from(0, 80);

	if (Input::KeyI.clicked){
		gagespeed = 5.0;
		Screenpart = 1;
	}
	if (Input::KeyO.clicked){
		gagespeed = 10.0;
		Screenpart = 1;
	}
	if (Input::KeyP.clicked){
		gagespeed = 20.0;
		Screenpart = 1;
	}
	if (Input::KeyL.clicked){
		gagespeed = 40.0;
		Screenpart = 1;
	}
}

/*リザルト描写*/
void Result()
{
	PutText(L"距離:", (int)y[0]).from(0, 0);
	PutText(L"ブラシポイント:", brushcount).from(0, 20);
	PutText(L"難易度ボーナス:").from(0, 40);

	if (gagespeed == 5.0){
		PutText(L"+0").from(120, 40);
		bounus = 0;
	}
	if (gagespeed == 10.0){
		PutText(L"+100").from(120, 40);
		bounus = 100;
	}
	if (gagespeed == 20.0){
		PutText(L"+1000").from(120, 40);
		bounus = 1000;
	}
	if (gagespeed == 40.0){
		PutText(L"+10000").from(120, 40);
		bounus = 10000;
	}

	score = y[0] + (brushcount * 100) + bounus;

	PutText(L"スコア:", score).from(0, 60);

	if (Input::KeyQ.pressed)
	{
		keyhitflag = 0;
		gagebar = 0.0;
		gagemovement = 1;
		gagespeed = 10.0;
		brushsetflag = 0; 
		brushpos = 0;
		brushmoveflag = 0;
		brushcount = 0;
		brushx = -650;
		sita = 0.0;
		finishflag = false;
		finishcount = 0;
		finishy = -200;
		score = 0;
		bounus = 0;
		Screenpart = 0;
	}

}

/*メイン処理*/
void Main()
{

	for (i = 0; i < ICENUM; i++) y[i] = -480.0 * i;//背景座標設定

	Texture ice(L"Picture/ice(640×480).png");//背景画像読み込み
	Texture stone(L"Picture/stone.png");//カーリング石画像読み込み
	Texture gage(L"Picture/gage.png");//ゲージ画像読み込み
	Texture brush(L"Picture/brush.png");//ブラシ画像読み込み
	Texture finishpic(L"Picture/finishpic.png");//フィニッシュ画像読み込み
	const Font font(30);
	double radian = Radians(0.0);

	while (System::Update())
	{
		switch (Screenpart)
		{
		case 0://タイトル画面

			Title();

			break;

		case 1://ゲーム画面

			if (finishflag == false)
			{
				Debug();//デバック用

				BackgroundMovement();//背景移動処理

				SpeedSetBar();//ゲージ処理

				BrushMovement();//ブラシ移動処理

				/*氷の背景を描写*/
				for (i = 0; i < ICENUM; i++)ice.draw(0, y[i]);

				/*カーリング石設置*/
				stone(0, 0, 178, 178).rotateAt(89, 89, radian).draw(231, 220);
				radian = Radians(sita);//ストーン回転処理


				/*発射直後のブラシ出現アニメーション処理*/
				if ((keyhitflag == 1) && (brushsetflag == 0))
				{
					brushx += 50;
					if (brushx == -400)brushsetflag = 1;//ブラシセット完了設定
				}

				brush.draw(brushx, 90);//ブラシ描写

				/*ゲージ描写*/
				gage.draw(452, 240);
				Rect(520, 240, 42, 200).draw({ 0, 162, 232, 255 });
				Rect(520, 240, 42, 200 - gagebar).draw({ 0, 0, 0, 255 });

				/*if (Input::KeyR.clicked)//リセット[R]
				{
					keyhitflag = 0;
					gagebar = 0.0;
					vo = 0.0;
					gagemovement = 1;
					Screenpart = 0;
				}*/
			}
			else
			{

				Debug();

				/*氷の背景を描写*/
				for (i = 0; i < ICENUM; i++)ice.draw(0, y[i]);

				/*カーリング石描写*/
				stone(0, 0, 178, 178).rotateAt(89, 89, radian).draw(231, 220);
				radian = Radians(sita);//ストーン回転処理

				/*ブラシ描写*/
				brush.draw(brushx, 90);

				/*ゲージ描写*/
				gage.draw(452, 240);
				Rect(520, 240, 42, 200).draw({ 0, 0, 0, 255 });

				finishpic.draw(55, finishy);//フィニッシュ画像描写

				if (finishy < 150)finishy+=10;
				finishcount++;
				if (finishcount == 300)
				{
					finishflag = false;
					Screenpart = 2;
					finishcount = 0;
				}
			}

			break;

		case 2://リザルト画面

			Result();

			break;

		default:
			break;
		}
	}
}
