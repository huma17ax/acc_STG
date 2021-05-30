#include "DxLib.h"
#include <math.h>
#include <stdio.h>

#define PI 3.141592

//戻り値あり
int ChkKey();
int PosOnScreenX(int x);//オーバーロード
int PosOnScreenX(double x);
int PosOnScreenY(int y);//オーバーロード
int PosOnScreenY(double y);
double Distance(double x1, double y1, double x2, double y2);

//戻り値なし
void Player_Move();
void Enemy_Move();
void DrawPulseLine(double x1, double y1, double x2, double y2);
void DrawPulseCircle(double x, double y, double r);

typedef enum {
	way_4,
	way_1_rot
}move1;

typedef enum {
	ID,
	Angle
}type1;

typedef enum {
	Title,
	Tutorial,
	Pre,
	Exec,
	Result
}scene;

typedef struct StructA {//自機　軌跡
	int x;
	int y;
	StructA() {//コンストラクタで初期化
		x = 0;
		y = 0;
	}
}pos;

typedef struct StructB {//敵機　弾
	double x;
	double y;
	bool exist;
	int type;
	int angle;
	StructB() {//コンストラクタで初期化
		x = 0;
		y = 0;
		exist = FALSE;
		type = 1;
		angle = 0;
	}
	void Draw() {
		DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 5, GetColor(255, 0, 0), TRUE);
	}
}shot1;

typedef struct StructD {//自機　弾
	double x;
	double y;
	bool exist;
	int type;
	int angle, angle2;
	double sp;
	int min_id;
	double pow;
	StructD() {//コンストラクタで初期化
		x = 0;
		y = 0;
		exist = FALSE;
		type = 1;
		angle = 0;
		angle2 = 0;
		sp = 0;
		min_id = 0;
	}
	void Draw() {
		DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 5, GetColor(0, 255, 0), TRUE);
		//DrawFormatString(PosOnScreenX(x), PosOnScreenY(y),GetColor(255,0,0),"%d",min_id);
	}
}shot2;

typedef struct StructE {//スター
	double x;
	double y;
	bool exist;
	double sp;
	int angle;
	char type;
	StructE() {
		x = 0;
		y = 0;
		exist = FALSE;
		sp = 0;
		angle = 0;
	}
}shot3;

typedef struct StructF {//ボス
	double x;
	double y;
	bool exist;
	double sp;
	int hp;
	int angle;
	int appTime;
	int eff_ang;
	int MoveType;
	int MoveI;
	StructF() {
		x = 0;
		y = 0;
		exist = FALSE;
		sp = 0;
		angle = 0;
		appTime = 1;
		eff_ang = 0;
		MoveType = 0;
		MoveI = 0;
	}
	void Draw() {
		if (hp > 0) {
			DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 60, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 20, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60.0 * cos(PI * 2 * (eff_ang) / 360), PosOnScreenY(y) + 60.0 * sin(PI * 2 * (eff_ang) / 360), 20, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 90.0) / 360), 20, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 180) / 360.0), 20, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60 * cos(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenY(y) + 60 * sin(PI * 2.0 * (eff_ang + 270) / 360), 20, GetColor(255, 0, 0), false);
			DrawLine(PosOnScreenX(x) + 60.0 * cos(PI * 2 * (eff_ang) / 360), PosOnScreenY(y) + 60.0 * sin(PI * 2 * (eff_ang) / 360), PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 90.0) / 360), GetColor(255, 0, 0));
			DrawLine(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 180) / 360.0), GetColor(255, 0, 0));
			DrawLine(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenX(x) + 60 * cos(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenY(y) + 60 * sin(PI * 2.0 * (eff_ang + 270) / 360), GetColor(255, 0, 0));
			DrawLine(PosOnScreenX(x) + 60 * cos(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenY(y) + 60 * sin(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenX(x) + 60.0 * cos(PI * 2 * (eff_ang) / 360), PosOnScreenY(y) + 60.0 * sin(PI * 2 * (eff_ang) / 360), GetColor(255, 0, 0));
		}
		if (hp <= 0 && hp > -50) {
			DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 60 - 50 - hp, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 20, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60.0 * cos(PI * 2 * (eff_ang) / 360), PosOnScreenY(y) + 60.0 * sin(PI * 2 * (eff_ang) / 360), 20, GetColor(255 - 200 - hp * 4, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 90.0) / 360), 20, GetColor(255 - 200 - hp * 4, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 180) / 360.0), 20, GetColor(255 - 200 - hp * 4, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60 * cos(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenY(y) + 60 * sin(PI * 2.0 * (eff_ang + 270) / 360), 20, GetColor(255 - 200 - hp * 4, 0, 0), false);
		}
		if (hp <= -50) {
			DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 60, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 20, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60.0 * cos(PI * 2 * (eff_ang) / 360), PosOnScreenY(y) + 60.0 * sin(PI * 2 * (eff_ang) / 360), 20, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 90.0) / 360), 20, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 180) / 360.0), 20, GetColor(255, 0, 0), false);
			DrawCircle(PosOnScreenX(x) + 60 * cos(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenY(y) + 60 * sin(PI * 2.0 * (eff_ang + 270) / 360), 20, GetColor(255, 0, 0), false);
			DrawLine(PosOnScreenX(x) + 60.0 * cos(PI * 2 * (eff_ang) / 360), PosOnScreenY(y) + 60.0 * sin(PI * 2 * (eff_ang) / 360), PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 90.0 - 180 - hp / 50.0 * 90) / 360), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 90 - 180 - hp / 50.0 * 90) / 360), GetColor(255, 0, 0));
			DrawLine(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 180 - 180 - hp / 50.0 * 90) / 360.0), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 180 - 180 - hp / 50.0 * 90) / 360.0), GetColor(255, 0, 0));
			DrawLine(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenX(x) + 60 * cos(PI * 2.0 * (eff_ang + 270 - 180 - hp / 50.0 * 90) / 360), PosOnScreenY(y) + 60 * sin(PI * 2.0 * (eff_ang + 270 - 180 - hp / 50.0 * 90) / 360), GetColor(255, 0, 0));
			DrawLine(PosOnScreenX(x) + 60 * cos(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenY(y) + 60 * sin(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenX(x) + 60.0 * cos(PI * 2 * (eff_ang - 180 - hp / 50.0 * 90) / 360), PosOnScreenY(y) + 60.0 * sin(PI * 2 * (eff_ang - 180 - hp / 50.0 * 90) / 360), GetColor(255, 0, 0));
			//DrawLine(PosOnScreenX(x) + 60.0 * cos(PI * 2 * (eff_ang) / 360), PosOnScreenY(y) + 60.0 * sin(PI * 2 * (eff_ang) / 360), PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 90.0) / 360), GetColor(255, 0, 0));
			//DrawLine(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 90.0) / 360), PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 180) / 360.0), GetColor(255, 0, 0));
			//DrawLine(PosOnScreenX(x) + 60 * cos(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenY(y) + 60 * sin(PI * 2 * (eff_ang + 180) / 360.0), PosOnScreenX(x) + 60 * cos(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenY(y) + 60 * sin(PI * 2.0 * (eff_ang + 270) / 360), GetColor(255, 0, 0));
			//DrawLine(PosOnScreenX(x) + 60 * cos(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenY(y) + 60 * sin(PI * 2.0 * (eff_ang + 270) / 360), PosOnScreenX(x) + 60.0 * cos(PI * 2 * (eff_ang) / 360), PosOnScreenY(y) + 60.0 * sin(PI * 2 * (eff_ang) / 360), GetColor(255, 0, 0));
		}
	}
}boss;

typedef struct StructG {//ボス　弾
	double x;
	double y;
	int type;
	bool exist;
	double sp;
	int angle;
	StructG() {
		exist = FALSE;
	}
	void Draw() {
		DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 5, GetColor(255, 0, 0), TRUE);
	}
}BossShot;

typedef struct StructC {//敵機
	double x;
	double y;
	bool exist;
	double sp;
	int angle;
	int hp;
	int dens;//弾密度
	int type;
	StructC() {//コンストラクタで初期化
		x = 0;
		y = 0;
		exist = TRUE;
		sp = 0;
		angle = 0;
		hp = 0;
		dens = 0;
		type = 0;
	}
	void Draw() {
		DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 20, GetColor(255, 0, 0), FALSE);
		DrawCircle(PosOnScreenX(x), PosOnScreenY(y), 16, GetColor(255, 0, 0), FALSE);
		//DrawFormatString(PosOnScreenX(x), PosOnScreenY(y), GetColor(255, 0, 0), "%d : %d", type, dens);
		if (type == 0 || type == 2 || type == 6) {
			DrawBox(PosOnScreenX(x), PosOnScreenY(y + 8), PosOnScreenX(x + 30), PosOnScreenY(y + 14), GetColor(0, 0, 0), true);
			DrawBox(PosOnScreenX(x), PosOnScreenY(y + 8), PosOnScreenX(x + hp / 10), PosOnScreenY(y + 14), GetColor(255, 50, 50), true);
			DrawBox(PosOnScreenX(x), PosOnScreenY(y + 8), PosOnScreenX(x + 30), PosOnScreenY(y + 14), GetColor(255, 255, 255), false);
		}
		if (type == 1 || type == 4 || type == 5) {
			DrawBox(PosOnScreenX(x), PosOnScreenY(y + 8), PosOnScreenX(x + 30), PosOnScreenY(y + 14), GetColor(0, 0, 0), true);
			DrawBox(PosOnScreenX(x), PosOnScreenY(y + 8), PosOnScreenX(x + hp / 11.666), PosOnScreenY(y + 14), GetColor(255, 50, 50), true);
			DrawBox(PosOnScreenX(x), PosOnScreenY(y + 8), PosOnScreenX(x + 30), PosOnScreenY(y + 14), GetColor(255, 255, 255), false);
		}
		if (type == 3) {
			DrawBox(PosOnScreenX(x), PosOnScreenY(y + 8), PosOnScreenX(x + 30), PosOnScreenY(y + 14), GetColor(0, 0, 0), true);
			DrawBox(PosOnScreenX(x), PosOnScreenY(y + 8), PosOnScreenX(x + hp / 13.333), PosOnScreenY(y + 14), GetColor(255, 50, 50), true);
			DrawBox(PosOnScreenX(x), PosOnScreenY(y + 8), PosOnScreenX(x + 30), PosOnScreenY(y + 14), GetColor(255, 255, 255), false);
		}
	}
}enemy;

int NearEnemy(type1 type, enemy ene[]);

double plyX = 320, plyY = 240;

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(640, 480, 16);
	ChangeWindowMode(TRUE);
	SetBackgroundColor(0, 0, 0);

	if (DxLib_Init() == -1)	return -1;// 初期化//エラーが起きたら直ちに終了

	SetDrawScreen(DX_SCREEN_BACK);

	CreateMaskScreen();

	double ply_sp = 4;
	double ply_sp_min = 2, ply_sp_max = 7;
	int ply_ang = 0;//進行角度　0～359
	int ply_ang_sp = 10;//角速度
	double ply_acc = 0.2;//加速
	double ply_frc = 0.1;//自然減速
	move1 ply_move = way_1_rot;
	int ply_hp = 1000, ply_over = 497;
	int ply_ult = 360;
	bool ply_ult_flag = FALSE;
	int ply_ult_cnt = 0;

	shot2 Pshot[100];//自機弾
	for (int i = 0; i < 20; i++) {
		Pshot[i].sp = 10;
	}
	int shot_num = 0;

	enemy ene[20];
	/*
	for (int i = 0; i < 20; i++) {
		ene[i].x = GetRand(3200) - 320;
		ene[i].y = GetRand(2400) - 240;
		ene[i].angle = GetRand(359);
		ene[i].sp = 3;
		ene[i].hp = 300;
		ene[i].type = 1;
	}
	*/

	shot1 Eshot[500];//敵機弾
	int e_shot_num = 0;

	shot3 star[80];
	int starNum = 0;

	boss Boss;
	BossShot Bshot[500];
	int BshotNum = 0;

	int trj = LoadGraph("trj.png");
	int name = LoadGraph("name.png");
	int hoge = LoadGraph("hoge.png");
	int start = LoadGraph("start.png");
	int manual = LoadGraph("manual.png");
	int point = LoadGraph("point.png");
	int gage = LoadGraph("gage.png");
	int bar = LoadGraph("bar.png");
	int bar2 = LoadGraph("bar2.png");
	int bar2_ef = LoadGraph("bar2_ef.png");
	int shader = LoadGraph("shader.png");
	int bosss = LoadGraph("boss_shot.png");

	int select[2];
	LoadDivGraph("select.png", 2, 2, 1, 31, 29, select);

	int Mask = LoadMask("gageMask.png");
	int gageCopy = MakeScreen(640, 40, true);
	int gage_ef = 0;

	pos trj_pos[20];//軌跡描画用
	int eff_i = 0;//自機デザ用

	scene scene = Title;//シーン遷移

	int title_pos = -320;
	int title_type = 0;
	char title_sel = 0;

	int tuto_type = 0;
	int tuto_time = 0;
	int del_time = 0;

	int pre_time0 = -30;
	int pre_time1 = 16;
	int pre_time2 = 0;
	int pre_type = 0;

	int Score = 0;
	int highScore[3] = { 0,0,0 };
	int Stage = 1;

	//ファイル読み込み
	FILE *fp;

	fp = fopen("score.bin", "rb");
	fread(&highScore[0], sizeof(int), 1, fp);
	fread(&highScore[1], sizeof(int), 1, fp);
	fread(&highScore[2], sizeof(int), 1, fp);
	fclose(fp);

	//BGM
	int BGM = LoadSoundMem("tw012.mp3");
	int SEL = LoadSoundMem("select01.mp3");
	int DEL = LoadSoundMem("Button80.mp3");
	int SLK = LoadSoundMem("strike02.mp3");
	int STR = LoadSoundMem("button70.mp3");
	int ELE = LoadSoundMem("electro.mp3");
	ChangeVolumeSoundMem(180, BGM);
	ChangeVolumeSoundMem(180, SLK);
	ChangeVolumeSoundMem(120, ELE);
	PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);

	while (1) {
		SetBackgroundColor(0, 0, 0);
		ClearDrawScreen();

		switch (scene)
		{
		case Title:
			DrawString(400, 450, "Delete Score → DeleteKey", GetColor(255, 255, 255));
			DrawString(0, 410, "スコアが30000を超えると次のStageが解放されます", GetColor(255, 255, 255));

			if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
				tuto_time++;
			}
			else {
				tuto_time = 0;
			}
			if (CheckHitKey(KEY_INPUT_DELETE) == 1) {
				del_time++;
			}
			else {
				del_time = 0;
			}
			if (del_time == 1) {
				PlaySoundMem(DEL, DX_PLAYTYPE_BACK);
				highScore[0] = 0;
				highScore[1] = 0;
				highScore[2] = 0;
				title_sel = 0;
			}

			if (title_type == 1) DrawBox(0, 0, title_pos + 160, 159, GetColor(80, 80, 130), true);
			if (title_type == 2) {
				DrawBox(0, 0, 640, 159, GetColor(80, 80, 130), true);
			}

			DrawGraph(title_pos, 0, hoge, true);
			title_pos += 12;
			if (title_pos > 640) {
				title_pos = -320;
				if (title_type == 1) title_type = 2;
				if (title_type == 0) title_type = 1;
			}

			DrawGraph(0, 0, name, true);
			DrawGraph(85, 175, start, true);
			DrawGraph(330, 175, manual, true);

			//DrawFormatString(120, 360, GetColor(255, 255, 255), "high score : %d", highScore);
			if (title_sel == 0) {
				DrawFormatString(120, 290, GetColor(255, 255, 0), "Stage 1　　　　　　　　　　%d", highScore[0]);
			}
			else {
				DrawFormatString(120, 290, GetColor(255, 255, 255), "Stage 1　　　　　　　　　　%d", highScore[0]);
			}
			if (title_sel == 1) {
				DrawFormatString(120, 330, GetColor(255, 255, 0), "Stage 2　　　　　　　　　　%d", highScore[1]);
			}
			else {
				DrawFormatString(120, 330, GetColor(255, 255, 255), "Stage 2　　　　　　　　　　%d", highScore[1]);
			}
			if (title_sel == 3) {
				DrawFormatString(120, 370, GetColor(255, 255, 0), "Stage 3　　　　　　　　　　%d", highScore[2]);
			}
			else {
				DrawFormatString(120, 370, GetColor(255, 255, 255), "Stage 3　　　　　　　　　　%d", highScore[2]);
			}

			DrawString(300, 260, "High Score", GetColor(255, 255, 255));

			if (highScore[0] < 30000) DrawLine(110, 338, 195, 338, GetColor(255, 255, 255));
			if (highScore[1] < 30000) DrawLine(110, 378, 195, 378, GetColor(255, 255, 255));

			//DrawString(370, 220, "Stage2が解放されました。MANUALにStage2の内容が追加されました", GetColor(255, 255, 255));

			if (title_sel == 0) {//play選択(Stage1)
				DrawGraph(50, 194, select[0], true);
				DrawGraph(243, 194, select[1], true);
				DrawTriangle(105, 290, 105, 302, 117, 296, GetColor(255, 255, 0), true);
				if (tuto_time == 1) {
					PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
					Stage = 1;
					scene = Pre;
					pre_type = 0;
					pre_time0 = -30;
					pre_time1 = 16;
					pre_time2 = 0;
					plyX = 1280;
					plyY = 960;
					ply_sp = 4;
					ply_ang = 0;
					ply_hp = 1000;
					ply_ult = 200;
					Score = 0;
					for (int i = 0; i < 20; i++) {
						ene[i].x = GetRand(3200) - 320;
						ene[i].y = GetRand(2400) - 240;
						ene[i].angle = GetRand(359);
						ene[i].sp = 3;
						ene[i].hp = 300;
						ene[i].dens = 20;
						ene[i].type = 0;
					}
					Boss.exist = FALSE;
				}
			}
			if (title_sel == 1 || title_sel == 2) {//play選択(Stage2)
				DrawGraph(50, 194, select[0], true);
				DrawGraph(243, 194, select[1], true);
				DrawTriangle(105, 330, 105, 342, 117, 336, GetColor(255, 255, 0), true);
				if (tuto_time == 1) {
					PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
					Stage = 2;
					scene = Pre;
					pre_type = 0;
					pre_time0 = -30;
					pre_time1 = 16;
					pre_time2 = 0;
					plyX = 1280;
					plyY = 960;
					ply_sp = 4;
					ply_ang = 0;
					ply_hp = 1000;
					ply_ult = 200;
					Score = 0;
					for (int i = 0; i < 10; i++) {
						ene[i].x = GetRand(3200) - 320;
						ene[i].y = GetRand(2400) - 240;
						ene[i].angle = GetRand(359);
						ene[i].sp = 3.5;
						ene[i].hp = 350;
						ene[i].dens = 20;
						ene[i].type = 1;
					}
					for (int i = 10; i < 20; i++) {
						ene[i].x = GetRand(3200) - 320;
						ene[i].y = GetRand(2400) - 240;
						ene[i].angle = GetRand(359);
						ene[i].sp = 3;
						ene[i].hp = 300;
						ene[i].dens = 20;
						ene[i].type = 2;
					}
					Boss.exist = FALSE;
				}
			}
			if (title_sel == 3) {//play選択(Stage3)
				DrawGraph(50, 194, select[0], true);
				DrawGraph(243, 194, select[1], true);
				DrawTriangle(105, 370, 105, 382, 117, 376, GetColor(255, 255, 0), true);
				if (tuto_time == 1) {
					PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
					Stage = 3;
					scene = Pre;
					pre_type = 0;
					pre_time0 = -30;
					pre_time1 = 16;
					pre_time2 = 0;
					plyX = 1280;
					plyY = 960;
					ply_sp = 4;
					ply_ang = 0;
					ply_hp = 1000;
					ply_ult = 200;
					Score = 0;
					for (int i = 0; i < 5; i++) {
						ene[i].x = GetRand(3200) - 320;
						ene[i].y = GetRand(2400) - 240;
						ene[i].angle = GetRand(359);
						ene[i].sp = 4;
						ene[i].hp = 400;
						ene[i].dens = 20;
						ene[i].type = 3;
					}
					for (int i = 5; i < 10; i++) {
						ene[i].x = GetRand(3200) - 320;
						ene[i].y = GetRand(2400) - 240;
						ene[i].angle = GetRand(359);
						ene[i].sp = 3.5;
						ene[i].hp = 350;
						ene[i].dens = 20;
						ene[i].type = 4;
					}
					for (int i = 10; i < 15; i++) {
						ene[i].x = GetRand(3200) - 320;
						ene[i].y = GetRand(2400) - 240;
						ene[i].angle = GetRand(359);
						ene[i].sp = 3.5;
						ene[i].hp = 350;
						ene[i].dens = 20;
						ene[i].type = 5;
					}
					for (int i = 15; i < 20; i++) {
						ene[i].x = GetRand(3200) - 320;
						ene[i].y = GetRand(2400) - 240;
						ene[i].angle = GetRand(359);
						ene[i].sp = 3;
						ene[i].hp = 300;
						ene[i].dens = 20;
						ene[i].type = 6;
					}
					Boss.exist = FALSE;
				}
			}
			if (title_sel == 4) {//manual選択
				DrawGraph(295, 194, select[0], true);
				DrawGraph(545, 194, select[1], true);
				if (tuto_time == 1) {
					PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
					scene = Tutorial;
					tuto_type = 0;
					plyX = 320;
					plyY = 240;
					ply_sp = 2;
					ply_ang = 0;
					ply_hp = 1000;
					ply_ult = 0;
				}
			}
			//DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", title_sel);
			//一度押すと2に　→　離すと1で、次の入力を受けつける
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && title_sel != 0) {
				title_sel = 0;
				PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
			}
			if (title_sel == 0 && CheckHitKey(KEY_INPUT_DOWN) == 1 && highScore[0] > 30000) {
				title_sel = 2;
				PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
			}
			if (title_sel == 2 && CheckHitKey(KEY_INPUT_DOWN) == 0 && CheckHitKey(KEY_INPUT_UP) == 0) title_sel = 1;
			if (title_sel == 1 && CheckHitKey(KEY_INPUT_DOWN) == 1 && highScore[1] > 30000) {
				title_sel = 3;
				PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
			}
			if (title_sel == 3 && CheckHitKey(KEY_INPUT_UP) == 1) {
				title_sel = 2;
				PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
			}
			if (title_sel == 2 && CheckHitKey(KEY_INPUT_UP) == 0 && CheckHitKey(KEY_INPUT_DOWN) == 0) title_sel = 1;
			if (title_sel == 1 && CheckHitKey(KEY_INPUT_UP) == 1) {
				title_sel = 0;
				PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && title_sel != 4) {
				title_sel = 4;
				PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
			}

			break;
		case Tutorial:
			if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
				tuto_time++;
			}
			else {
				tuto_time = 0;
			}
			if (tuto_type == 0) {
				DrawString(0, 0, "＃ 自機の動き ＃", GetColor(255, 255, 255));
				DrawString(0, 30, "十字キー 右　…　右旋回", GetColor(255, 255, 255));
				DrawString(0, 50, "十字キー 左　…　左旋回", GetColor(255, 255, 255));
				DrawString(0, 70, "十字キー 上　…　前進", GetColor(255, 255, 255));
				DrawString(100, 90, "↓体力ゲージ", GetColor(255, 255, 255));
				DrawString(100, 150, "↑必殺技ゲージ", GetColor(255, 255, 255));
				DrawString(300, 320, "壁にぶつかるとダメージ", GetColor(255, 255, 255));
				DrawString(300, 340, "敵機への衝突でもダメージ", GetColor(255, 255, 255));
				DrawString(450, 450, "Next → EnterKey", GetColor(255, 255, 255));

				if (tuto_time == 1) {
					PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
					tuto_type = 1;
					tuto_time++;
					plyX = 200;
					plyY = 240;
					ply_ang = 0;
					ply_hp = 1000;
					ply_ult = 200;
					ply_sp = 2;
				}

				DrawCircle(plyX, plyY, 20, GetColor(0, 255, 0), false);
				DrawTriangle(plyX + 20 * cos(PI * 2 * ply_ang / 360), plyY + 20 * sin(PI * 2 * ply_ang / 360), plyX + 20 * cos(PI * 2 * (ply_ang + 120) / 360), plyY + 20 * sin(PI * 2 * (ply_ang + 120) / 360), plyX + 20 * cos(PI * 2 * (ply_ang + 240) / 360), plyY + 20 * sin(PI * 2 * (ply_ang + 240) / 360), GetColor(0, 255, 0), FALSE);
				DrawCircle(plyX + 25 * cos(PI * 2 * ply_ang / 360), plyY + 25 * sin(PI * 2 * ply_ang / 360), 2, GetColor(0, 255, 0), TRUE);

				if (CheckHitKey(KEY_INPUT_UP) == 1) {
					if (ply_sp < ply_sp_max) ply_sp += ply_acc;
				}
				else {
					if (ply_sp > ply_sp_min) ply_sp -= ply_frc;
				}
				if (CheckHitKey(KEY_INPUT_RIGHT) == 0
					&& CheckHitKey(KEY_INPUT_LEFT) == 1) ply_ang -= ply_ang_sp / 2;
				if (CheckHitKey(KEY_INPUT_RIGHT) == 1
					&& CheckHitKey(KEY_INPUT_LEFT) == 0) ply_ang += ply_ang_sp / 2;

				if (ply_ang >= 360) ply_ang -= 360;
				if (ply_ang <= -1) ply_ang += 360;

				if ((ply_ang >= 90 && ply_ang <= 270 && plyX > 10)
					|| ((ply_ang <= 90 || ply_ang >= 270) && plyX < 630)) {
					plyX += ply_sp*cos(PI * 2 * ply_ang / 360);
				}
				else {
					ply_hp--;
				}
				if ((ply_ang >= 0 && ply_ang <= 180 && plyY < 470)
					|| (ply_ang >= 180 && ply_ang <= 360 && plyY > 10)) {
					plyY += ply_sp*sin(PI * 2 * ply_ang / 360);
				}
				else {
					ply_hp--;
				}

				SetDrawScreen(gageCopy);
				DrawBox(0, 0, 640, 40, GetColor(255, 255, 255), true);
				//DrawBox(0, 0, ply_hp / 1000.0 * 600, 20, GetColor(0, 255, 0), true);
				DrawGraph((ply_hp / 1000.0 - 1)*580.0, 0, bar, true);
				DrawGraph((ply_ult / 400.0 - 1)*330.0, 18, bar2, true);
				if (ply_ult == 400) DrawGraph(gage_ef, 18, bar2_ef, true);
				if (ply_sp <= 7) DrawCircle(20, 19, 19, GetColor(45 + ply_sp * 30, 45 + ply_sp * 30, 45 + ply_sp * 30), true);
				if (ply_sp > 7) DrawCircle(20, 19, 19, GetColor(255, 255, 255), true);
				DrawGraph(0, 0, gage, true);

				SetDrawScreen(DX_SCREEN_BACK);
				DrawMask(0, 110, Mask, DX_MASKTRANS_BLACK);
				DrawGraph(0, 110, gageCopy, true);
				FillMaskScreen(0);
			}
			if (tuto_type == 1) {
				DrawString(0, 0, "＃ 自機の攻撃 ＃", GetColor(255, 255, 255));
				DrawString(0, 30, "Ｚキー　…　攻撃", GetColor(255, 255, 255));
				DrawString(0, 50, "Ｘキー　…　必殺技", GetColor(255, 255, 255));
				DrawString(100, 90, "↓体力ゲージ", GetColor(255, 255, 255));
				DrawString(100, 150, "↑必殺技ゲージ", GetColor(255, 255, 255));
				DrawString(300, 320, "攻撃は画面内の一番近い敵へ", GetColor(255, 255, 255));
				DrawString(300, 340, "必殺技はゲージが貯まると撃てる", GetColor(255, 255, 255));
				DrawString(450, 450, "Next → EnterKey", GetColor(255, 255, 255));

				if (tuto_time == 1) {
					PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
					tuto_type = 2;
					tuto_time++;
					ply_ult = 0;
				}

				DrawCircle(plyX, plyY, 20, GetColor(0, 255, 0), false);
				DrawTriangle(plyX + 20 * cos(PI * 2 * ply_ang / 360), plyY + 20 * sin(PI * 2 * ply_ang / 360), plyX + 20 * cos(PI * 2 * (ply_ang + 120) / 360), plyY + 20 * sin(PI * 2 * (ply_ang + 120) / 360), plyX + 20 * cos(PI * 2 * (ply_ang + 240) / 360), plyY + 20 * sin(PI * 2 * (ply_ang + 240) / 360), GetColor(0, 255, 0), FALSE);
				DrawCircle(plyX + 25 * cos(PI * 2 * ply_ang / 360), plyY + 25 * sin(PI * 2 * ply_ang / 360), 2, GetColor(0, 255, 0), TRUE);

				if (ene[0].hp > 0) {
					DrawCircle(440, 240, 20, GetColor(255, 0, 0), false);
					DrawCircle(440, 240, 16, GetColor(255, 0, 0), false);
					DrawBox(440, 248, 470, 254, GetColor(0, 0, 0), true);
					DrawBox(440, 248, 440 + ene[0].hp / 10, 254, GetColor(255, 50, 50), true);
					DrawBox(440, 248, 470, 254, GetColor(255, 255, 255), false);
				}

				if (ene[0].hp == 0) {
					ene[0].hp = -40;
					double angle2 = GetRand(157) / 100.00;
					for (int ii = 0; ii < 4; ii++) {
						star[starNum].x = 440 + 20 * cos(PI*ii / 2 + angle2);
						star[starNum].y = 240 + 20 * sin(PI*ii / 2 + angle2);
						star[starNum].angle = PI*ii / 2 + angle2;
						star[starNum].exist = TRUE;
						star[starNum].sp = 7;
						star[starNum].type = 0;
						starNum++;
						if (starNum == 80) starNum = 0;
					}
				}

				if (ene[0].hp < 0) {
					ene[0].hp++;
					if (ene[0].hp == -1) ene[0].hp = 300;
				}

				if (CheckHitKey(KEY_INPUT_Z) == 1) {
					if (ene[0].hp > 0) {
						ene[0].hp -= 10;
						DrawPulseLine(plyX, plyY, 440, 240);
					}
					else {
						DrawPulseCircle(plyX, plyY, 20);
					}
				}

				for (int i = 0; i < 80; i++) {
					if (star[i].type == 0 && star[i].exist == TRUE) {
						double angle2 = atan((star[i].y - plyY) / (star[i].x - plyX));
						if (star[i].x > plyX) angle2 += PI;
						if (star[i].sp > 0) {
							star[i].x += star[i].sp * cos(star[i].angle);
							star[i].y += star[i].sp * sin(star[i].angle);
						}
						star[i].x += (5 - star[i].sp) * cos(angle2);
						star[i].y += (5 - star[i].sp) * sin(angle2);
						if (star[i].sp > 0) {
							star[i].sp -= 0.2;
						}
						else {
							star[i].sp -= 0.3;
						}
						if (Distance(plyX, plyY, star[i].x, star[i].y) < 20) {
							if (ply_ult < 400) ply_ult += 5;
							star[i].exist = FALSE;
						}
					}
				}
				for (int i = 0; i < 80; i++) {//スター描画
					if (star[i].type == 0 && star[i].exist == TRUE) {
						DrawGraph(star[i].x - 7, star[i].y - 7, point, true);
					}
				}

				gage_ef += 10;
				if (gage_ef > 460) gage_ef = -100;

				SetDrawScreen(gageCopy);
				DrawBox(0, 0, 640, 40, GetColor(255, 255, 255), true);
				//DrawBox(0, 0, ply_hp / 1000.0 * 600, 20, GetColor(0, 255, 0), true);
				DrawGraph((ply_hp / 1000.0 - 1)*580.0, 0, bar, true);
				DrawGraph((ply_ult / 400.0 - 1)*330.0, 18, bar2, true);
				if (ply_ult == 400) DrawGraph(gage_ef, 18, bar2_ef, true);
				if (ply_sp <= 7) DrawCircle(20, 19, 19, GetColor(45 + ply_sp * 30, 45 + ply_sp * 30, 45 + ply_sp * 30), true);
				if (ply_sp > 7) DrawCircle(20, 19, 19, GetColor(255, 255, 255), true);
				DrawGraph(0, 0, gage, true);

				SetDrawScreen(DX_SCREEN_BACK);
				DrawMask(0, 110, Mask, DX_MASKTRANS_BLACK);
				DrawGraph(0, 110, gageCopy, true);
				FillMaskScreen(0);
			}
			if (tuto_type == 2) {
				DrawString(0, 0, "＃ 敵機の攻撃 ＃", GetColor(255, 255, 255));
				DrawString(0, 30, "", GetColor(255, 255, 255));
				DrawString(0, 50, "", GetColor(255, 255, 255));
				DrawString(100, 90, "↓体力ゲージ", GetColor(255, 255, 255));
				DrawString(100, 150, "↑必殺技ゲージ", GetColor(255, 255, 255));
				DrawString(300, 320, "敵はステージによって動きが違ったり…", GetColor(255, 255, 255));
				DrawString(300, 340, "←ボスマーカー　　触れるとボスが出現", GetColor(255, 255, 255));
				DrawString(300, 360, "出すかどうかはプレーヤー次第", GetColor(255, 255, 255));
				DrawString(300, 360, "　　　　　　　(ボスの体力は表示されません)", GetColor(255, 255, 255));
				DrawString(450, 450, "End → EnterKey", GetColor(255, 255, 255));

				if (tuto_time == 1) {
					PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
					scene = Title;
					tuto_time++;
					ply_ult = 0;
				}

				DrawLine(100 + (40 - (40) * 2) * cos(PI * 2 * (0) / 360), 370 + (40 - (40) * 2) * sin(PI * 2 * (0) / 360), 100 + (40 - (40) * 2) * cos(PI * 2 * (90) / 360), 370 + (40 - (40) * 2) * sin(PI * 2 * (90) / 360), GetColor(232, 70, 232));
				DrawLine(100 + (40 - (40) * 2) * cos(PI * 2 * (90) / 360), 370 + (40 - (40) * 2) * sin(PI * 2 * (90) / 360), 100 + (40 - (40) * 2) * cos(PI * 2 * (180) / 360), 370 + (40 - (40) * 2) * sin(PI * 2 * (180) / 360), GetColor(232, 70, 232));
				DrawLine(100 + (40 - (40) * 2) * cos(PI * 2 * (180) / 360), 370 + (40 - (40) * 2) * sin(PI * 2 * (180) / 360), 100 + (40 - (40) * 2) * cos(PI * 2 * (270) / 360), 370 + (40 - (40) * 2) * sin(PI * 2 * (270) / 360), GetColor(232, 70, 232));
				DrawLine(100 + (40 - (40) * 2) * cos(PI * 2 * (270) / 360), 370 + (40 - (40) * 2) * sin(PI * 2 * (270) / 360), 100 + (40 - (40) * 2) * cos(PI * 2 * (0) / 360), 370 + (40 - (40) * 2) * sin(PI * 2 * (0) / 360), GetColor(232, 70, 232));
				DrawString(100 - 17, 370 - 20, "BOSS", GetColor(232, 70, 232));

				DrawCircle(plyX, plyY, 20, GetColor(0, 255, 0), false);
				DrawTriangle(plyX + 20 * cos(PI * 2 * ply_ang / 360), plyY + 20 * sin(PI * 2 * ply_ang / 360), plyX + 20 * cos(PI * 2 * (ply_ang + 120) / 360), plyY + 20 * sin(PI * 2 * (ply_ang + 120) / 360), plyX + 20 * cos(PI * 2 * (ply_ang + 240) / 360), plyY + 20 * sin(PI * 2 * (ply_ang + 240) / 360), GetColor(0, 255, 0), FALSE);
				DrawCircle(plyX + 25 * cos(PI * 2 * ply_ang / 360), plyY + 25 * sin(PI * 2 * ply_ang / 360), 2, GetColor(0, 255, 0), TRUE);

				DrawCircle(440, 240, 20, GetColor(255, 0, 0), false);
				DrawCircle(440, 240, 16, GetColor(255, 0, 0), false);
				DrawBox(440, 248, 470, 254, GetColor(0, 0, 0), true);
				DrawBox(440, 248, 440 + 300 / 10, 254, GetColor(255, 50, 50), true);
				DrawBox(440, 248, 470, 254, GetColor(255, 255, 255), false);

				if (GetRand(20) == 1) {
					Eshot[e_shot_num].exist = TRUE;
					Eshot[e_shot_num].x = 440;
					Eshot[e_shot_num].y = 240;
					/*if (ene[i].hp > 200) */Eshot[e_shot_num].angle = GetRand(359);
					//if (ene[i].hp <= 200) Eshot[e_shot_num].angle = atan((ene[i].y - plyY) / (ene[i].x - plyX)) * 360 / (PI * 2);
					//if (ene[i].x < plyX) Eshot[e_shot_num].angle = 180 + Eshot[e_shot_num].angle;
					//Eshot[e_shot_num].angle = Eshot[e_shot_num].angle + 180;
					e_shot_num++;
					if (e_shot_num == 500) e_shot_num = 0;
				}
				if (GetRand(40) == 1) {
					Eshot[e_shot_num].exist = TRUE;
					Eshot[e_shot_num].x = 440;
					Eshot[e_shot_num].y = 240;
					Eshot[e_shot_num].angle = 180;
					e_shot_num++;
					if (e_shot_num == 500) e_shot_num = 0;
				}

				for (int i = 0; i < 500; i++) {
					if (Eshot[i].exist == TRUE) {
						Eshot[i].x += 6 * cos(PI * 2 * Eshot[i].angle / 360);
						Eshot[i].y += 6 * sin(PI * 2 * Eshot[i].angle / 360);

						if (Distance(plyX, plyY, Eshot[i].x, Eshot[i].y) < 20) {
							ply_hp -= 10;
							Eshot[i].exist = FALSE;
						}

						if (Eshot[i].x < -320
							|| Eshot[i].x > 2880
							|| Eshot[i].y < -240
							|| Eshot[i].y > 2160) {
							Eshot[i].exist = FALSE;
						}

						DrawCircle(Eshot[i].x, Eshot[i].y, 5, GetColor(255, 0, 0), TRUE);

					}
				}

				SetDrawScreen(gageCopy);
				DrawBox(0, 0, 640, 40, GetColor(255, 255, 255), true);
				//DrawBox(0, 0, ply_hp / 1000.0 * 600, 20, GetColor(0, 255, 0), true);
				DrawGraph((ply_hp / 1000.0 - 1)*580.0, 0, bar, true);
				DrawGraph((ply_ult / 400.0 - 1)*330.0, 18, bar2, true);
				if (ply_ult == 400) DrawGraph(gage_ef, 18, bar2_ef, true);
				if (ply_sp <= 7) DrawCircle(20, 19, 19, GetColor(45 + ply_sp * 30, 45 + ply_sp * 30, 45 + ply_sp * 30), true);
				if (ply_sp > 7) DrawCircle(20, 19, 19, GetColor(255, 255, 255), true);
				DrawGraph(0, 0, gage, true);

				SetDrawScreen(DX_SCREEN_BACK);
				DrawMask(0, 110, Mask, DX_MASKTRANS_BLACK);
				DrawGraph(0, 110, gageCopy, true);
				FillMaskScreen(0);
			}


			break;
		case Pre:
			/*
			if (pre_type == 0) {
				plyX+=30;
				if (plyX > 2240) pre_type = 1;
			}
			if (pre_type == 1) {
				plyY+=30;
				if (plyY > 1680) pre_type = 2;
			}
			if (pre_type == 2) {
				plyX-=30;
				if (plyX < 320) pre_type = 3;
			}
			if (pre_type == 3) {
				plyY-=30;
				if (plyY < 960) pre_type = 4;
			}
			if (pre_type == 4) {
				plyX+=30;
			}*/
			/*
			if (pre_type <= 270) {
				pre_type += 2;
				plyX = 1280 + 720 * cos(2 * PI*pre_type / 360);
				plyY = 960 + 720 * sin(2 * PI*pre_type / 360);
			}
			if (pre_type > 270) {
				plyX = 1280;
				plyY++;
			}*/
			//背景描画処理
			for (int i = 0; i < 41; i++) {//縦線
				DrawLine(320 + (i - 20) * pre_time1, 0, 320 + (i - 20) * pre_time1, 480, GetColor(100, 100, 100));
				//DrawLineAA(320 + (i - 20) * pre_time1 + 1, 0, 320 + (i - 20) * pre_time1 + 1, 480, GetColor(100, 100, 100));
			}
			for (int i = 0; i < 31; i++) {//横線
				DrawLine(0, 240 + (i - 15) * pre_time1, 640, 240 + (i - 15) * pre_time1, GetColor(100, 100, 100));
				//DrawLineAA(0, 240 + (i - 15) * pre_time1 + 1, 640, 240 + (i - 15) * pre_time1 + 1, GetColor(100, 100, 100));
			}
			DrawLine(320 - 16 * pre_time1, 240 - 12 * pre_time1, 320 - 16 * pre_time1, 240 + 12 * pre_time1, GetColor(255, 0, 0));
			DrawLine(320 + 16 * pre_time1, 240 - 12 * pre_time1, 320 + 16 * pre_time1, 240 + 12 * pre_time1, GetColor(255, 0, 0));
			DrawLine(320 - 16 * pre_time1, 240 - 12 * pre_time1, 320 + 16 * pre_time1, 240 - 12 * pre_time1, GetColor(255, 0, 0));
			DrawLine(320 - 16 * pre_time1, 240 + 12 * pre_time1, 320 + 16 * pre_time1, 240 + 12 * pre_time1, GetColor(255, 0, 0));

			for (int i = 0; i < (pre_time0 / 4); i++) {//敵機
				DrawCircle(320 - 16 * pre_time1 + ene[i].x / (80.0 / pre_time1), 240 - 12 * pre_time1 + ene[i].y / (80.0 / pre_time1), 20 / (80.0 / pre_time1), GetColor(255, 0, 0), FALSE);
				DrawCircle(320 - 16 * pre_time1 + ene[i].x / (80.0 / pre_time1), 240 - 12 * pre_time1 + ene[i].y / (80.0 / pre_time1), 16 / (80.0 / pre_time1), GetColor(255, 0, 0), FALSE);
			}
			if (pre_time0 < 80) pre_time0++;
			if (pre_time0 == 80 && pre_time1 < 80) pre_time1++;
			if (pre_time1 == 80) pre_type = 1;
			if (pre_type == 1) {
				pre_time2++;
				if (pre_time2 <= 10) DrawCircle((int)320, (int)240, 30 - pre_time2, GetColor(0, 255, 0), FALSE);
				if (pre_time2 > 10) DrawCircle((int)320, (int)240, 20, GetColor(0, 255, 0), FALSE);
				if (pre_time2 > 10 && pre_time2 <= 30) {
					DrawLine(320 + 20 * cos(PI * 2 * (-30 + pre_time2) / 360), 240 + 20 * sin(PI * 2 * (-30 + pre_time2) / 360), 320 + 20 * cos(PI * 2 * ((-30 + pre_time2) + 90) / 360), 240 + 20 * sin(PI * 2 * ((-30 + pre_time2) + 90) / 360), GetColor(0, 255, 0));
					DrawLine(320 + 20 * cos(PI * 2 * ((-30 + pre_time2) + 90) / 360), 240 + 20 * sin(PI * 2 * ((-30 + pre_time2) + 90) / 360), 320 + 20 * cos(PI * 2 * ((-30 + pre_time2) + 180) / 360), 240 + 20 * sin(PI * 2 * ((-30 + pre_time2) + 180) / 360), GetColor(0, 255, 0));
					DrawLine(320 + 20 * cos(PI * 2 * ((-30 + pre_time2) + 180) / 360), 240 + 20 * sin(PI * 2 * ((-30 + pre_time2) + 180) / 360), 320 + 20 * cos(PI * 2 * ((-30 + pre_time2) + 270) / 360), 240 + 20 * sin(PI * 2 * ((-30 + pre_time2) + 270) / 360), GetColor(0, 255, 0));
					DrawLine(320 + 20 * cos(PI * 2 * ((-30 + pre_time2) + 270) / 360), 240 + 20 * sin(PI * 2 * ((-30 + pre_time2) + 270) / 360), 320 + 20 * cos(PI * 2 * (-30 + pre_time2) / 360), 240 + 20 * sin(PI * 2 * (-30 + pre_time2) / 360), GetColor(0, 255, 0));
				}
				if (pre_time2 > 30) {
					DrawLine(320 + 20 * cos(PI * 2 * 0), 240 + 20 * sin(PI * 2 * 0), 320 + 20 * cos(PI * 2 * 90 / 360), 240 + 20 * sin(PI * 2 * 90 / 360), GetColor(0, 255, 0));
					DrawLine(320 + 20 * cos(PI * 2 * 90 / 360), 240 + 20 * sin(PI * 2 * 90 / 360), 320 + 20 * cos(PI * 2 * 180 / 360), 240 + 20 * sin(PI * 2 * 180 / 360), GetColor(0, 255, 0));
					DrawLine(320 + 20 * cos(PI * 2 * 180 / 360), 240 + 20 * sin(PI * 2 * 180 / 360), 320 + 20 * cos(PI * 2 * 270 / 360), 240 + 20 * sin(PI * 2 * 270 / 360), GetColor(0, 255, 0));
					DrawLine(320 + 20 * cos(PI * 2 * 270 / 360), 240 + 20 * sin(PI * 2 * 270 / 360), 320 + 20 * cos(PI * 2 * 0 / 360), 240 + 20 * sin(PI * 2 * 0 / 360), GetColor(0, 255, 0));
				}
				if (pre_time2 > 30 && pre_time2 <= 40) {
					DrawTriangle(320 + (60 - pre_time2) * cos(PI * 2 * 0 / 360), 240 + (60 - pre_time2) * sin(PI * 2 * 0 / 360), 320 + (60 - pre_time2) * cos(PI * 2 * 120 / 360), 240 + (60 - pre_time2) * sin(PI * 2 * 120 / 360), 320 + (60 - pre_time2) * cos(PI * 2 * 240 / 360), 240 + (60 - pre_time2) * sin(PI * 2 * 240 / 360), GetColor(0, 255, 0), FALSE);
				}
				if (pre_time2 > 40) {
					DrawTriangle(320 + 20 * cos(PI * 2 * 0 / 360), 240 + 20 * sin(PI * 2 * 0 / 360), 320 + 20 * cos(PI * 2 * 120 / 360), 240 + 20 * sin(PI * 2 * 120 / 360), 320 + 20 * cos(PI * 2 * 240 / 360), 240 + 20 * sin(PI * 2 * 240 / 360), GetColor(0, 255, 0), FALSE);
				}
				if (pre_time2 > 40) {
					DrawFormatString(300, 200, GetColor(255, 255, 0), "START");
				}
				if (pre_time2 == 50) scene = Exec;
			}

			break;
		case Exec:

			if (ply_ult_flag == FALSE) {//ult時移動ストップ

				//自機移動処理
				switch (ply_move)
				{
				case way_4:

					if (ChkKey() == 1 || ChkKey() == 13) {//0°
						if (ply_ang > 180 && ply_ang <= 359) ply_ang += ply_ang_sp;
						if (ply_ang < 180 && ply_ang >0) ply_ang -= ply_ang_sp;
						if (ply_ang == 180) {
							if (GetRand(1) == 0) { ply_ang += ply_ang_sp; }
							else { ply_ang -= ply_ang_sp; }
						}
					}
					if (ChkKey() == 2 || ChkKey() == 14) {//180°
						if (ply_ang >= 1 && ply_ang < 180) ply_ang += ply_ang_sp;
						if (ply_ang <= 359 && ply_ang > 180) ply_ang -= ply_ang_sp;
						if (ply_ang == 0) {
							if (GetRand(1) == 0) { ply_ang += ply_ang_sp; }
							else { ply_ang -= ply_ang_sp; }
						}
					}
					if (ChkKey() == 3 || ChkKey() == 11) {//270°
						if (ply_ang > 90 && ply_ang < 270) ply_ang += ply_ang_sp;
						if (ply_ang < 90 && ply_ang >= 0) ply_ang -= ply_ang_sp;
						if (ply_ang <= 359 && ply_ang > 270) ply_ang -= ply_ang_sp;
						if (ply_ang == 90) {
							if (GetRand(1) == 0) { ply_ang += ply_ang_sp; }
							else { ply_ang -= ply_ang_sp; }
						}
					}
					if (ChkKey() == 4 || ChkKey() == 12) {//90°
						if (ply_ang > 90 && ply_ang < 270) ply_ang -= ply_ang_sp;
						if (ply_ang < 90 && ply_ang >= 0) ply_ang += ply_ang_sp;
						if (ply_ang <= 359 && ply_ang > 270) ply_ang += ply_ang_sp;
						if (ply_ang == 270) {
							if (GetRand(1) == 0) { ply_ang += ply_ang_sp; }
							else { ply_ang -= ply_ang_sp; }
						}
					}
					if (ChkKey() == 6) {//315°
						if (ply_ang > 135 && ply_ang < 315) ply_ang += ply_ang_sp;
						if (ply_ang < 135 && ply_ang >= 0) ply_ang -= ply_ang_sp;
						if (ply_ang <= 359 && ply_ang > 315) ply_ang -= ply_ang_sp;
						if (ply_ang == 135) {
							if (GetRand(1) == 0) { ply_ang += ply_ang_sp; }
							else { ply_ang -= ply_ang_sp; }
						}
					}
					if (ChkKey() == 7) {//45°
						if (ply_ang < 225 && ply_ang>45) ply_ang -= ply_ang_sp;
						if (ply_ang > 255 && ply_ang <= 359) ply_ang += ply_ang_sp;
						if (ply_ang < 45 && ply_ang >= 0) ply_ang += ply_ang_sp;
						if (ply_ang == 225) {
							if (GetRand(1) == 0) { ply_ang += ply_ang_sp; }
							else { ply_ang -= ply_ang_sp; }
						}
					}
					if (ChkKey() == 8) {//225°
						if (ply_ang > 45 && ply_ang < 225) ply_ang += ply_ang_sp;
						if (ply_ang > 225 && ply_ang <= 359) ply_ang -= ply_ang_sp;
						if (ply_ang >= 0 && ply_ang < 45) ply_ang -= ply_ang_sp;
						if (ply_ang == 45) {
							if (GetRand(1) == 0) { ply_ang += ply_ang_sp; }
							else { ply_ang -= ply_ang_sp; }
						}
					}
					if (ChkKey() == 9) {//135°
						if (ply_ang > 135 && ply_ang < 315) ply_ang -= ply_ang_sp;
						if (ply_ang > 315 && ply_ang <= 359) ply_ang += ply_ang_sp;
						if (ply_ang < 135 && ply_ang >= 0) ply_ang += ply_ang_sp;
						if (ply_ang == 315) {
							if (GetRand(1) == 0) { ply_ang += ply_ang_sp; }
							else { ply_ang -= ply_ang_sp; }
						}
					}
					if (ply_ang >= 360) ply_ang -= 360;
					if (ply_ang <= -1) ply_ang += 360;

					if (ChkKey() == 0) {
						if (ply_sp > ply_sp_min) ply_sp -= ply_frc;
					}
					else {
						if (ply_sp < ply_sp_max) ply_sp += ply_acc;
					}

					if ((ply_ang >= 90 && ply_ang <= 270 && plyX > 10)
						|| ((ply_ang <= 90 || ply_ang >= 270) && plyX < 2550)) plyX += ply_sp*cos(PI * 2 * ply_ang / 360);
					if ((ply_ang >= 0 && ply_ang <= 180 && plyY < 1910)
						|| (ply_ang >= 180 && ply_ang <= 360 && plyY > 10)) plyY += ply_sp*sin(PI * 2 * ply_ang / 360);

					break;
				case way_1_rot:

					if (CheckHitKey(KEY_INPUT_UP) == 1) {
						if (ply_sp < ply_sp_max) ply_sp += ply_acc;
					}
					else {
						if (ply_sp > ply_sp_min) ply_sp -= ply_frc;
					}
					if (CheckHitKey(KEY_INPUT_RIGHT) == 0
						&& CheckHitKey(KEY_INPUT_LEFT) == 1) ply_ang -= ply_ang_sp / 2;
					if (CheckHitKey(KEY_INPUT_RIGHT) == 1
						&& CheckHitKey(KEY_INPUT_LEFT) == 0) ply_ang += ply_ang_sp / 2;

					if (ply_ang >= 360) ply_ang -= 360;
					if (ply_ang <= -1) ply_ang += 360;

					if ((ply_ang >= 90 && ply_ang <= 270 && plyX > 10)
						|| ((ply_ang <= 90 || ply_ang >= 270) && plyX < 2550)) {
						plyX += ply_sp*cos(PI * 2 * ply_ang / 360);
					}
					else {
						ply_hp--;
					}
					if ((ply_ang >= 0 && ply_ang <= 180 && plyY < 1910)
						|| (ply_ang >= 180 && ply_ang <= 360 && plyY > 10)) {
						plyY += ply_sp*sin(PI * 2 * ply_ang / 360);
					}
					else {
						ply_hp--;
					}
					break;
				}

				//ゲーム終了処理
				if (ply_hp <= 0) {
					ply_hp = 0;
					scene = Result;
				}

				//自機弾移動処理(※使用中止※)
				for (int i = 0; i < 100; i++) {
					if (Pshot[i].exist == TRUE) {

						for (int j = 0; j < 20; j++) {
							if (Distance(Pshot[i].x, Pshot[i].y, ene[j].x, ene[j].y) < 20 && ene[j].exist == TRUE) {
								ene[j].hp -= 10;
								Pshot[i].exist = FALSE;
							}
							if (Distance(Pshot[i].x, Pshot[i].y, ene[j].x, ene[j].y) < 100 && ene[j].exist == TRUE) {
								Pshot[i].min_id = j;
							}
						}

						if (Pshot[i].min_id != -1) {
							Pshot[i].angle2 = atan((ene[Pshot[i].min_id].y - Pshot[i].y) / (ene[Pshot[i].min_id].x - Pshot[i].x)) * 360 / (PI * 2);
							if (ene[Pshot[i].min_id].x < Pshot[i].x) {
								Pshot[i].angle2 = 180 + Pshot[i].angle2;
							}
						}

						Pshot[i].x += Pshot[i].sp * cos(PI * 2 * Pshot[i].angle / 360);
						Pshot[i].y += Pshot[i].sp * sin(PI * 2 * Pshot[i].angle / 360);
						Pshot[i].x += (10 - Pshot[i].sp) * cos(PI * 2 * Pshot[i].angle2 / 360);
						Pshot[i].y += (10 - Pshot[i].sp) * sin(PI * 2 * Pshot[i].angle2 / 360);
						if (Pshot[i].sp > 0) Pshot[i].sp -= 0.2;
					}
				}

				//敵機移動処理
				for (int i = 0; i < 20; i++) {
					if (ene[i].exist == TRUE) {
						if (ene[i].angle >= 90 && ene[i].angle <= 270 && ene[i].x < -320) ene[i].angle = 180 - ene[i].angle;
						if ((ene[i].angle <= 90 || ene[i].angle >= 270) && ene[i].x > 2880) ene[i].angle = 180 - ene[i].angle;
						if (ene[i].angle >= 0 && ene[i].angle <= 180 && ene[i].y > 2160) ene[i].angle = -ene[i].angle;
						if (ene[i].angle >= 180 && ene[i].angle <= 359 && ene[i].y < -240) ene[i].angle = -ene[i].angle;

						if (ene[i].angle < 0) ene[i].angle += 360;
						if (ene[i].angle > 359) ene[i].angle -= 360;

						ene[i].x += ene[i].sp*cos(PI * 2 * ene[i].angle / 360);
						ene[i].y += ene[i].sp*sin(PI * 2 * ene[i].angle / 360);

						ene[i].dens = 20;

						if (Distance(plyX, plyY, ene[i].x, ene[i].y) < 25) {//自機との衝突
							PlaySoundMem(SLK, DX_PLAYTYPE_BACK);
							ply_hp -= ene[i].hp / 5;
							ene[i].hp = 0;

							star[starNum].x = ene[i].x;
							star[starNum].y = ene[i].y;
							star[starNum].angle = 0;
							star[starNum].exist = TRUE;
							star[starNum].sp = 0;
							star[starNum].type = 1;
							starNum++;
							if (starNum == 80) starNum = 0;

							{//リポップ
								ene[i].exist = TRUE;
								for (int ii = 0; ii < 1; ii++) {
									ene[i].x = GetRand(3200) - 320;
									ene[i].y = GetRand(2400) - 240;
									if (PosOnScreenX(ene[i].x) > 0
										&& PosOnScreenX(ene[i].x) < 640
										&& PosOnScreenY(ene[i].y) > 0
										&& PosOnScreenY(ene[i].y) < 480) {
										ii--;
									}
								}
								ene[i].angle = GetRand(359);
								ene[i].sp = 3;
								ene[i].hp = 300;
							}
						}

						if (ene[i].hp <= 0) {
							PlaySoundMem(SLK, DX_PLAYTYPE_BACK);
							Score += 1000;
							ene[i].exist = FALSE;
							double angle2 = GetRand(157) / 100.00;
							for (int ii = 0; ii < 4; ii++) {
								star[starNum].x = ene[i].x + 20 * cos(PI*ii / 2 + angle2);
								star[starNum].y = ene[i].y + 20 * sin(PI*ii / 2 + angle2);
								star[starNum].angle = PI*ii / 2 + angle2;
								star[starNum].exist = TRUE;
								star[starNum].sp = 7;
								star[starNum].type = 0;
								starNum++;
								if (starNum == 80) starNum = 0;
							}
							{//リポップ
								ene[i].exist = TRUE;
								for (int ii = 0; ii < 1; ii++) {
									ene[i].x = GetRand(3200) - 320;
									ene[i].y = GetRand(2400) - 240;
									if (PosOnScreenX(ene[i].x) > 0
										&& PosOnScreenX(ene[i].x) < 640
										&& PosOnScreenY(ene[i].y) > 0
										&& PosOnScreenY(ene[i].y) < 480) {
										ii--;
									}
								}
								ene[i].angle = GetRand(359);
								ene[i].sp = 3;
								ene[i].hp = 300;
								//typeはそのまま
							}
						}
					}
				}

				//スター移動処理
				for (int i = 0; i < 80; i++) {
					if (star[i].type == 0 && star[i].exist == TRUE) {
						double angle2 = atan((star[i].y - plyY) / (star[i].x - plyX));
						if (star[i].x > plyX) angle2 += PI;
						if (star[i].sp > 0) {
							star[i].x += star[i].sp * cos(star[i].angle);
							star[i].y += star[i].sp * sin(star[i].angle);
						}
						star[i].x += (5 - star[i].sp) * cos(angle2);
						star[i].y += (5 - star[i].sp) * sin(angle2);
						if (star[i].sp > 0) {
							star[i].sp -= 0.2;
						}
						else {
							star[i].sp -= 0.3;
						}
						if (Distance(plyX, plyY, star[i].x, star[i].y) < 20) {
							PlaySoundMem(STR, DX_PLAYTYPE_BACK);
							if (ply_ult < 400) ply_ult += 5;
							star[i].exist = FALSE;
						}
					}
					if (star[i].type == 1 && star[i].exist == TRUE) {
						DrawCircle(PosOnScreenX(star[i].x), PosOnScreenY(star[i].y), 20 + star[i].angle, GetColor(255, 0, 0), false);
						star[i].angle++;
						if (star[i].angle == 10) star[i].exist = FALSE;
					}
				}

				//敵機弾移動処理
				for (int i = 0; i < 500; i++) {
					if (Eshot[i].exist == TRUE) {
						Eshot[i].x += 6 * cos(PI * 2 * Eshot[i].angle / 360);
						Eshot[i].y += 6 * sin(PI * 2 * Eshot[i].angle / 360);

						if (Distance(plyX, plyY, Eshot[i].x, Eshot[i].y) < 20) {
							PlaySoundMem(SLK, DX_PLAYTYPE_BACK);
							ply_hp -= 10;
							Eshot[i].exist = FALSE;
						}

						if (Eshot[i].x < -320
							|| Eshot[i].x > 2880
							|| Eshot[i].y < -240
							|| Eshot[i].y > 2160) {
							Eshot[i].exist = FALSE;
						}
					}
				}

				//自機射撃処理
				/*
				if (CheckHitKey(KEY_INPUT_Z) == 1) {
					if (ply_over > 0) ply_over--;
				}
				else {
					if (ply_over < 497) ply_over += 2;//497で停止→496は射撃処理
				}
				if (CheckHitKey(KEY_INPUT_Z) == 1 && ply_over % 5 == 1) {
					Pshot[shot_num].x = plyX;
					Pshot[shot_num].y = plyY;
					Pshot[shot_num].sp = 0;
					Pshot[shot_num].exist = TRUE;
					Pshot[shot_num].angle = gun_ang;

					Pshot[shot_num].min_id = NearEnemy(ID, ene);
					if (Pshot[shot_num].min_id == -1) Pshot[shot_num].angle2 = gun_ang;

					if (ply_over > 100) Pshot[shot_num].pow = 1;
					if (ply_over <= 100) Pshot[shot_num].pow = 2;

					shot_num++;
					if (shot_num == 100) shot_num = 0;
				}*/

				if (CheckHitKey(KEY_INPUT_Z) == 1) {
					if (CheckSoundMem(ELE)==0) PlaySoundMem(ELE, DX_PLAYTYPE_LOOP);
					if (NearEnemy(ID, ene) == -1) {
						if (Boss.exist == TRUE && Boss.appTime == 0 && Boss.hp > 0
							&& PosOnScreenX(Boss.x) > 0 && PosOnScreenX(Boss.x) < 640
							&& PosOnScreenY(Boss.y) > 0 && PosOnScreenY(Boss.y) < 480) {
							DrawPulseLine(320, 240, PosOnScreenX(Boss.x), PosOnScreenY(Boss.y));
							Boss.hp -= 1;
							Score += 5;
						}
						else {
							DrawPulseCircle(320, 240, 20);
						}
					}
					else {
						if (Boss.exist == TRUE && Boss.appTime == 0 && Boss.hp > 0
							&& PosOnScreenX(Boss.x) > 0 && PosOnScreenX(Boss.x) < 640
							&& PosOnScreenY(Boss.y) > 0 && PosOnScreenY(Boss.y) < 480
							&& Distance(plyX, plyY, Boss.x, Boss.y) < Distance(plyX, plyY, ene[NearEnemy(ID, ene)].x, ene[NearEnemy(ID, ene)].y)) {
							DrawPulseLine(320, 240, PosOnScreenX(Boss.x), PosOnScreenY(Boss.y));
							if (Boss.hp > 0) Boss.hp -= 1;
							Score += 5;
						}
						else {
							DrawPulseLine(320, 240, PosOnScreenX(ene[NearEnemy(ID, ene)].x), PosOnScreenY(ene[NearEnemy(ID, ene)].y));
							ene[NearEnemy(ID, ene)].hp -= 3;
							if (ene[NearEnemy(ID, ene)].type == 2 || ene[NearEnemy(ID, ene)].type == 4 || ene[NearEnemy(ID, ene)].type == 6) {
								ene[NearEnemy(ID, ene)].dens = 5;
							}
							Score += 3;
						}
					}
				}
				else {
					if (CheckSoundMem(ELE) == 1) StopSoundMem(ELE);
				}

				//自機　必殺技突入処理
				if (CheckHitKey(KEY_INPUT_X) == 1 && ply_ult >= 400) {
					//ply_ult -= 400;
					ply_ult_flag = TRUE;
					ply_ult_cnt = 40;
				}

				//敵機射撃処理
				for (int i = 0; i < 20; i++) {
					if (ene[i].exist == TRUE) {
						if (GetRand(ene[i].dens) == 1) {
							Eshot[e_shot_num].exist = TRUE;
							Eshot[e_shot_num].x = ene[i].x;
							Eshot[e_shot_num].y = ene[i].y;
							if ((ene[i].type == 5 || ene[i].type == 6)
								&& PosOnScreenX(ene[i].x) > 0 && PosOnScreenX(ene[i].x) < 640
								&& PosOnScreenY(ene[i].y) > 0 && PosOnScreenY(ene[i].y) < 480) {
								Eshot[e_shot_num].angle = atan((ene[i].y - plyY) / (ene[i].x - plyX)) * 360 / (PI * 2);
								if (ene[i].x < plyX) Eshot[e_shot_num].angle = 180 + Eshot[e_shot_num].angle;
								Eshot[e_shot_num].angle = Eshot[e_shot_num].angle + 180;
							}
							else {
								Eshot[e_shot_num].angle = GetRand(359);
							}
							e_shot_num++;
							if (e_shot_num == 500) e_shot_num = 0;
						}
					}
				}

				//ボス出現処理
				if (Boss.exist == FALSE && GetRand(100) == 1) {
					Boss.exist = TRUE;
					Boss.appTime = 1000;
					Boss.x = GetRand(2560);
					Boss.y = GetRand(1920);
					Boss.MoveType = 1;
					Boss.hp = 700;
					Boss.angle = GetRand(360);
					Boss.sp = 1;
					Boss.MoveI = 0;
				}
				if (Boss.exist == TRUE) {
					/*
					if (Boss.appTime == 1000
						&& PosOnScreenX(Boss.x) > 0
						&& PosOnScreenX(Boss.x) < 640
						&& PosOnScreenY(Boss.y) > 0
						&& PosOnScreenY(Boss.y) < 480) {
						Boss.appTime = 999;
					}*/
					if (Boss.appTime > 0 && Boss.appTime <= 1000) {
						if (Distance(plyX, plyY, Boss.x, Boss.y) < 40) {
							Boss.appTime = -30;
						}
						Boss.appTime--;
						if (Boss.appTime == 1) Boss.exist = FALSE;
					}
					if (Boss.appTime < 0) {
						Boss.appTime++;
					}
				}

				//ボス移動処理
				if (Boss.exist == TRUE && Boss.appTime == 0) {
					if (Boss.MoveType == 1) {//通常移動

						if (Stage == 1 || Stage == 3) {
							if (Boss.angle >= 90 && Boss.angle <= 270 && Boss.x < 0) Boss.angle = 180 - Boss.angle;
							if ((Boss.angle <= 90 || Boss.angle >= 270) && Boss.x > 2560) Boss.angle = 180 - Boss.angle;
							if (Boss.angle >= 0 && Boss.angle <= 180 && Boss.y > 1920) Boss.angle = -Boss.angle;
							if (Boss.angle >= 180 && Boss.angle <= 359 && Boss.y < 0) Boss.angle = -Boss.angle;

							if (Boss.angle < 0) Boss.angle += 360;
							if (Boss.angle > 359) Boss.angle -= 360;

							Boss.sp = 1;
						}
						if (Stage == 2) {
							Boss.angle = atan((Boss.y - plyY) / (Boss.x - plyX)) * 360 / (PI * 2);
							if (Boss.x < plyX) Boss.angle = 180 + Boss.angle;
							Boss.angle = Boss.angle + 180;
							Boss.sp = 6;
						}
						if (Stage == 3) {
							if (!(PosOnScreenX(Boss.x) > 0 && PosOnScreenX(Boss.x) < 640
								&& PosOnScreenY(Boss.y) > 0 && PosOnScreenY(Boss.y) < 480)) {
								int rnd = GetRand(359);
								Boss.x = plyX + 70 * cos(PI * 2 * rnd / 360);
								Boss.y = plyY + 70 * sin(PI * 2 * rnd / 360);
								Boss.angle = GetRand(359);
							}
						}
						Boss.x += Boss.sp*cos(PI * 2 * Boss.angle / 360);
						Boss.y += Boss.sp*sin(PI * 2 * Boss.angle / 360);

						Boss.eff_ang++;
						if (Boss.eff_ang == 360) Boss.eff_ang = 0;

						if (GetRand(10) == 1) {
							for (int i = 0; i < 4; i++) {
								Bshot[BshotNum].exist = TRUE;
								Bshot[BshotNum].sp = 7;
								Bshot[BshotNum].type = 1;
								Bshot[BshotNum].angle = Boss.eff_ang + i * 90 + GetRand(180) - 90;
								Bshot[BshotNum].x = Boss.x + 60 * cos(PI * 2 * (Boss.eff_ang + i * 90) / 360);
								Bshot[BshotNum].y = Boss.y + 60 * sin(PI * 2 * (Boss.eff_ang + i * 90) / 360);

								BshotNum++;
								if (BshotNum == 500) BshotNum = 0;
							}
						}

						Boss.MoveI++;
						if (Boss.MoveI > 500 && GetRand(300) == 1) {
							Boss.MoveType = 2;
							Boss.MoveI = 0;
						}
					}
					if (Boss.MoveType == 2) {//射撃　溜め
						if (Boss.MoveI % 2 == 1) {
							for (int i = 0; i < 4; i++) {
								Bshot[BshotNum].exist = TRUE;
								Bshot[BshotNum].sp = 7;
								Bshot[BshotNum].type = 2;
								Bshot[BshotNum].angle = Boss.eff_ang + i * 90;
								Bshot[BshotNum].x = Boss.x + 60 * cos(PI * 2 * (Boss.eff_ang + i * 90) / 360);
								Bshot[BshotNum].y = Boss.y + 60 * sin(PI * 2 * (Boss.eff_ang + i * 90) / 360);

								BshotNum++;
								if (BshotNum == 500) BshotNum = 0;
							}
						}
						Boss.MoveI++;
						if (Boss.MoveI > 25) {
							Boss.MoveType = 3;
							Boss.MoveI = 0;
						}
					}
					if (Boss.MoveType == 3) {//ボス弾挙動切り替え用 一瞬
						Boss.MoveI++;
						if (Boss.MoveI == 2) {
							Boss.MoveType = 1;
							Boss.MoveI = 0;
						}
					}

					if (Boss.hp < 1 && Boss.MoveType != -1) {//ボス破壊
						Boss.hp = -100;
						Boss.MoveType = -1;
					}
					if (Boss.MoveType == -1) {//破壊時エフェクト
						Boss.hp += 2;
						if (Boss.hp >= -2) Boss.exist = FALSE;
					}
				}

				//ボス弾移動処理
				for (int i = 0; i < 500; i++) {
					if (Bshot[i].exist == TRUE) {
						Bshot[i].x += Bshot[i].sp * cos(PI * 2 * Bshot[i].angle / 360);
						Bshot[i].y += Bshot[i].sp * sin(PI * 2 * Bshot[i].angle / 360);

						if (Boss.MoveType == 3 && Bshot[i].type == 2) {//最初の一瞬のみ
							Bshot[i].angle = atan((Bshot[i].y - (plyY + 40 * sin(PI * 2 * Bshot[i].angle / 360))) / (Bshot[i].x - (plyX + 40 * cos(PI * 2 * Bshot[i].angle / 360)))) * 360 / (PI * 2);
							if (Bshot[i].x > plyX) Bshot[i].angle += 180;
						}

						if (Distance(plyX, plyY, Bshot[i].x, Bshot[i].y) < 20) {
							ply_hp -= 10;
							Bshot[i].exist = FALSE;
						}

						if (Bshot[i].x > -320 && Bshot[i].x<2880 && Bshot[i].y>-280 && Bshot[i].y < 2160) {
						}
						else {
							Bshot[i].exist = FALSE;
						}

					}
				}

				//ボスと自機の当たり判定
				if (Boss.exist == TRUE && Boss.hp > 0
					&& Distance(plyX, plyY, Boss.x, Boss.y) < 60) {
					ply_hp--;
				}

			}//ult時ストップフラグここまで

			if (ply_ult_flag == TRUE) {//自機　ult処理
				eff_i += 10;
				ply_ang -= 18;
				ply_ult_cnt--;
				ply_ult -= 10;
				DrawPulseCircle(320, 240, 8 * ply_ult_cnt);
				DrawPulseCircle(320, 240, 320 - 8 * ply_ult_cnt);
				for (int i = 0; i < 20; i++) {
					if (ene[i].exist == TRUE
						&& PosOnScreenX(ene[i].x) > 0
						&& PosOnScreenX(ene[i].x) < 640
						&& PosOnScreenY(ene[i].y) > 0
						&& PosOnScreenY(ene[i].y) < 480) {
						DrawPulseLine(320, 240, PosOnScreenX(ene[i].x), PosOnScreenY(ene[i].y));
						DrawPulseLine(320, 240, PosOnScreenX(ene[i].x), PosOnScreenY(ene[i].y));
						if (ene[i].hp > 0) ene[i].hp -= 10;
					}
				}
				if (ply_ult_cnt == 0) ply_ult_flag = FALSE;
			}

			//背景描画処理
			for (int i = 0; i < 41; i++) {//縦線
				if (i * 80 - plyX > 0 && i * 80 - plyX < 640) {
					DrawLine(i * 80 - plyX, -plyY, i * 80 - plyX, 2400 - plyY, GetColor(100, 100, 100));
					DrawLine(i * 80 - plyX + 1, -plyY, i * 80 - plyX + 1, 2400 - plyY, GetColor(100, 100, 100));
				}
			}
			for (int i = 0; i < 31; i++) {//横線
				if (i * 80 - plyY > 0 && i * 80 - plyY < 480) {
					DrawLine(-plyX, i * 80 - plyY, 3200 - plyX, i * 80 - plyY, GetColor(100, 100, 100));
					DrawLine(-plyX, i * 80 - plyY + 1, 3200 - plyX, i * 80 - plyY + 1, GetColor(100, 100, 100));
				}
			}
			DrawLine(320 - plyX, 240 - plyY, 320 - plyX, 2160 - plyY, GetColor(255, 0, 0));
			DrawLine(2880 - plyX, 240 - plyY, 2880 - plyX, 2160 - plyY, GetColor(255, 0, 0));
			DrawLine(320 - plyX, 240 - plyY, 2880 - plyX, 240 - plyY, GetColor(255, 0, 0));
			DrawLine(320 - plyX, 2160 - plyY, 2880 - plyX, 2160 - plyY, GetColor(255, 0, 0));

			//軌跡描画処理
			for (int i = 0; i < 19; i++) {
				trj_pos[i].x = trj_pos[i + 1].x;
				trj_pos[i].y = trj_pos[i + 1].y;
			}
			trj_pos[19].x = plyX + GetRand(6) - 3;
			trj_pos[19].y = plyY + GetRand(6) - 3;

			for (int i = 0; i < 19; i++) {
				DrawExtendGraph(trj_pos[i].x - (i / 2.0) + 320 - plyX, trj_pos[i].y - (i / 2.0) + 240 - plyY, trj_pos[i].x + (i / 2.0) + 320 - plyX, trj_pos[i].y + (i / 2.0) + 240 - plyY, trj, TRUE);
			}

			//ボス描画
			if (Boss.exist == TRUE) {
				if (Boss.appTime == 0) {
					Boss.Draw();
				}
				if (Boss.appTime > 0) {
					//DrawCircle(PosOnScreenX(Boss.x), PosOnScreenY(Boss.y), 30 - (Boss.appTime % 40) / 2, GetColor(255, 0, 0), false);
					//DrawCircle(PosOnScreenX(Boss.x), PosOnScreenY(Boss.y), 30 - ((Boss.appTime+10) % 40) / 2, GetColor(255, 0, 0), false);
					//DrawCircle(PosOnScreenX(Boss.x), PosOnScreenY(Boss.y), 30 - ((Boss.appTime + 20) % 40) / 2, GetColor(255, 0, 0), false);
					//DrawCircle(PosOnScreenX(Boss.x), PosOnScreenY(Boss.y), 30 - ((Boss.appTime + 30) % 40) / 2, GetColor(255, 0, 0), false);


					DrawLine(PosOnScreenX(Boss.x) + (40 - (Boss.appTime % 40) * 2) * cos(PI * 2 * (0) / 360), PosOnScreenY(Boss.y) + (40 - (Boss.appTime % 40) * 2) * sin(PI * 2 * (0) / 360), PosOnScreenX(Boss.x) + (40 - (Boss.appTime % 40) * 2) * cos(PI * 2 * (90) / 360), PosOnScreenY(Boss.y) + (40 - (Boss.appTime % 40) * 2) * sin(PI * 2 * (90) / 360), GetColor(232, 70, 232));
					DrawLine(PosOnScreenX(Boss.x) + (40 - (Boss.appTime % 40) * 2) * cos(PI * 2 * (90) / 360), PosOnScreenY(Boss.y) + (40 - (Boss.appTime % 40) * 2) * sin(PI * 2 * (90) / 360), PosOnScreenX(Boss.x) + (40 - (Boss.appTime % 40) * 2) * cos(PI * 2 * (180) / 360), PosOnScreenY(Boss.y) + (40 - (Boss.appTime % 40) * 2) * sin(PI * 2 * (180) / 360), GetColor(232, 70, 232));
					DrawLine(PosOnScreenX(Boss.x) + (40 - (Boss.appTime % 40) * 2) * cos(PI * 2 * (180) / 360), PosOnScreenY(Boss.y) + (40 - (Boss.appTime % 40) * 2) * sin(PI * 2 * (180) / 360), PosOnScreenX(Boss.x) + (40 - (Boss.appTime % 40) * 2) * cos(PI * 2 * (270) / 360), PosOnScreenY(Boss.y) + (40 - (Boss.appTime % 40) * 2) * sin(PI * 2 * (270) / 360), GetColor(232, 70, 232));
					DrawLine(PosOnScreenX(Boss.x) + (40 - (Boss.appTime % 40) * 2) * cos(PI * 2 * (270) / 360), PosOnScreenY(Boss.y) + (40 - (Boss.appTime % 40) * 2) * sin(PI * 2 * (270) / 360), PosOnScreenX(Boss.x) + (40 - (Boss.appTime % 40) * 2) * cos(PI * 2 * (0) / 360), PosOnScreenY(Boss.y) + (40 - (Boss.appTime % 40) * 2) * sin(PI * 2 * (0) / 360), GetColor(232, 70, 232));
					DrawString(PosOnScreenX(Boss.x) - 17, PosOnScreenY(Boss.y) - 20, "BOSS", GetColor(232, 70, 232));
					DrawFormatString(PosOnScreenX(Boss.x) - 4, PosOnScreenY(Boss.y) + 15, GetColor(232, 70, 232), "%d", Boss.appTime / 80 + 1);
				}
			}
			//ボス弾描画

			for (int i = 0; i < 500; i++) {
				if (Bshot[i].exist == TRUE) {
					if (Bshot[i].type == 1) {
						Bshot[i].Draw();
					}
					if (Bshot[i].type == 2) {
						DrawRotaGraph(PosOnScreenX(Bshot[i].x), PosOnScreenY(Bshot[i].y), 1, PI * 2 / 360 * Bshot[i].angle, bosss, false);
					}
				}
			}

			//自機デザ
			DrawCircle((int)320, (int)240, 20, GetColor(0, 255, 0), FALSE);
			DrawTriangle(320 + 20 * cos(PI * 2 * ply_ang / 360), 240 + 20 * sin(PI * 2 * ply_ang / 360), 320 + 20 * cos(PI * 2 * (ply_ang + 120) / 360), 240 + 20 * sin(PI * 2 * (ply_ang + 120) / 360), 320 + 20 * cos(PI * 2 * (ply_ang + 240) / 360), 240 + 20 * sin(PI * 2 * (ply_ang + 240) / 360), GetColor(0, 255, 0), FALSE);
			//DrawCircleAA(320,240-50,20,30,GetColor(100,255,100),FALSE);
			//DrawPixel(320 + 25 * cos(PI * 2 * ply_ang / 360), 240 + 25 * sin(PI * 2 * ply_ang / 360), GetColor(0, 255, 0));
			if (CheckHitKey(KEY_INPUT_RIGHT) == 0 && CheckHitKey(KEY_INPUT_LEFT) == 0 && CheckHitKey(KEY_INPUT_UP) == 0 && CheckHitKey(KEY_INPUT_DOWN) == 0) {
				DrawCircle((int)320, (int)240, 25, GetColor(0, 255, 0), FALSE);
			}
			else {
				if (ply_ult_flag == FALSE) DrawCircle(320 + 25 * cos(PI * 2 * ply_ang / 360), 240 + 25 * sin(PI * 2 * ply_ang / 360), 2, GetColor(0, 255, 0), TRUE);
				if (ply_ult_flag == TRUE) DrawCircle((int)320, (int)240, 25, GetColor(0, 255, 0), FALSE);
			}

			DrawLine(320 + 20 * cos(PI * 2 * eff_i / 360), 240 + 20 * sin(PI * 2 * eff_i / 360), 320 + 20 * cos(PI * 2 * (eff_i + 90) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 90) / 360), GetColor(0, 255, 0));
			DrawLine(320 + 20 * cos(PI * 2 * (eff_i + 90) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 90) / 360), 320 + 20 * cos(PI * 2 * (eff_i + 180) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 180) / 360), GetColor(0, 255, 0));
			DrawLine(320 + 20 * cos(PI * 2 * (eff_i + 180) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 180) / 360), 320 + 20 * cos(PI * 2 * (eff_i + 270) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 270) / 360), GetColor(0, 255, 0));
			DrawLine(320 + 20 * cos(PI * 2 * (eff_i + 270) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 270) / 360), 320 + 20 * cos(PI * 2 * eff_i / 360), 240 + 20 * sin(PI * 2 * eff_i / 360), GetColor(0, 255, 0));

			eff_i += ply_sp;
			if (eff_i > 360) eff_i -= 360;

			for (int i = 0; i < 80; i++) {//スター描画
				if (star[i].type == 0 && star[i].exist == TRUE) {
					DrawGraph(PosOnScreenX(star[i].x - 7), PosOnScreenY(star[i].y - 7), point, true);
				}
			}
			for (int i = 0; i < 100; i++) {//自機弾デザ
				if (Pshot[i].exist == TRUE) {
					Pshot[i].Draw();
				}
			}
			for (int i = 0; i < 20; i++) {//敵機デザ
				if (ene[i].exist == TRUE) {
					ene[i].Draw();
				}
			}
			for (int i = 0; i < 500; i++) {//敵機弾デザ
				if (Eshot[i].exist == TRUE
					&& PosOnScreenX(Eshot[i].x) > 0
					&& PosOnScreenX(Eshot[i].x) < 640
					&& PosOnScreenY(Eshot[i].y) > 0
					&& PosOnScreenY(Eshot[i].y) < 480) {
					Eshot[i].Draw();
				}
			}


			//パラメータゲージ描画

			gage_ef += 10;
			if (gage_ef > 460) gage_ef = -100;

			SetDrawScreen(gageCopy);
			DrawBox(0, 0, 640, 40, GetColor(255, 255, 255), true);
			//DrawBox(0, 0, ply_hp / 1000.0 * 600, 20, GetColor(0, 255, 0), true);
			DrawGraph((ply_hp / 1000.0 - 1)*580.0, 0, bar, true);
			DrawGraph((ply_ult / 400.0 - 1)*330.0, 18, bar2, true);
			if (ply_ult == 400) DrawGraph(gage_ef, 18, bar2_ef, true);
			if (ply_sp <= 7) DrawCircle(20, 19, 19, GetColor(45 + ply_sp * 30, 45 + ply_sp * 30, 45 + ply_sp * 30), true);
			if (ply_sp > 7) DrawCircle(20, 19, 19, GetColor(255, 255, 255), true);
			DrawGraph(0, 0, gage, true);

			SetDrawScreen(DX_SCREEN_BACK);
			DrawMask(0, 0, Mask, DX_MASKTRANS_BLACK);
			DrawGraph(0, 0, gageCopy, true);
			FillMaskScreen(0);

			//スコア加算
			Score += ply_sp;
			DrawFormatString(370, 22, GetColor(255, 255, 0), "score: %d", Score);

			//DrawFormatString(0, 450, GetColor(255, 255, 0), "boss: %f : %f : %d : %d", Boss.x, Boss.y, Boss.appTime, Boss.hp);
			//if (Boss.exist == FALSE) DrawFormatString(0, 450, GetColor(255, 255, 0), "aaa");


			break;
		case Result:

			//背景描画処理
			for (int i = 0; i < 41; i++) {//縦線
				if (i * 80 - plyX > 0 && i * 80 - plyX < 640) {
					DrawLine(i * 80 - plyX, -plyY, i * 80 - plyX, 2400 - plyY, GetColor(100, 100, 100));
					DrawLine(i * 80 - plyX + 1, -plyY, i * 80 - plyX + 1, 2400 - plyY, GetColor(100, 100, 100));
				}
			}
			for (int i = 0; i < 31; i++) {//横線
				if (i * 80 - plyY > 0 && i * 80 - plyY < 480) {
					DrawLine(-plyX, i * 80 - plyY, 3200 - plyX, i * 80 - plyY, GetColor(100, 100, 100));
					DrawLine(-plyX, i * 80 - plyY + 1, 3200 - plyX, i * 80 - plyY + 1, GetColor(100, 100, 100));
				}
			}
			DrawLine(320 - plyX, 240 - plyY, 320 - plyX, 2160 - plyY, GetColor(255, 0, 0));
			DrawLine(2880 - plyX, 240 - plyY, 2880 - plyX, 2160 - plyY, GetColor(255, 0, 0));
			DrawLine(320 - plyX, 240 - plyY, 2880 - plyX, 240 - plyY, GetColor(255, 0, 0));
			DrawLine(320 - plyX, 2160 - plyY, 2880 - plyX, 2160 - plyY, GetColor(255, 0, 0));

			//自機デザ
			DrawCircle((int)320, (int)240, 20, GetColor(0, 255, 0), FALSE);
			DrawTriangle(320 + 20 * cos(PI * 2 * ply_ang / 360), 240 + 20 * sin(PI * 2 * ply_ang / 360), 320 + 20 * cos(PI * 2 * (ply_ang + 120) / 360), 240 + 20 * sin(PI * 2 * (ply_ang + 120) / 360), 320 + 20 * cos(PI * 2 * (ply_ang + 240) / 360), 240 + 20 * sin(PI * 2 * (ply_ang + 240) / 360), GetColor(0, 255, 0), FALSE);
			//DrawCircleAA(320,240-50,20,30,GetColor(100,255,100),FALSE);
			//DrawPixel(320 + 25 * cos(PI * 2 * ply_ang / 360), 240 + 25 * sin(PI * 2 * ply_ang / 360), GetColor(0, 255, 0));
			/*if (CheckHitKey(KEY_INPUT_RIGHT) == 0 && CheckHitKey(KEY_INPUT_LEFT) == 0 && CheckHitKey(KEY_INPUT_UP) == 0 && CheckHitKey(KEY_INPUT_DOWN) == 0) {
				DrawCircle((int)320, (int)240, 25, GetColor(0, 255, 0), FALSE);
			}
			else {
				if (ply_ult_flag == FALSE) DrawCircle(320 + 25 * cos(PI * 2 * ply_ang / 360), 240 + 25 * sin(PI * 2 * ply_ang / 360), 2, GetColor(0, 255, 0), TRUE);
				if (ply_ult_flag == TRUE) DrawCircle((int)320, (int)240, 25, GetColor(0, 255, 0), FALSE);
			}*/DrawCircle((int)320, (int)240, 25, GetColor(0, 255, 0), FALSE);

			DrawLine(320 + 20 * cos(PI * 2 * eff_i / 360), 240 + 20 * sin(PI * 2 * eff_i / 360), 320 + 20 * cos(PI * 2 * (eff_i + 90) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 90) / 360), GetColor(0, 255, 0));
			DrawLine(320 + 20 * cos(PI * 2 * (eff_i + 90) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 90) / 360), 320 + 20 * cos(PI * 2 * (eff_i + 180) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 180) / 360), GetColor(0, 255, 0));
			DrawLine(320 + 20 * cos(PI * 2 * (eff_i + 180) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 180) / 360), 320 + 20 * cos(PI * 2 * (eff_i + 270) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 270) / 360), GetColor(0, 255, 0));
			DrawLine(320 + 20 * cos(PI * 2 * (eff_i + 270) / 360), 240 + 20 * sin(PI * 2 * (eff_i + 270) / 360), 320 + 20 * cos(PI * 2 * eff_i / 360), 240 + 20 * sin(PI * 2 * eff_i / 360), GetColor(0, 255, 0));

			//eff_i += ply_sp;
			//if (eff_i > 360) eff_i -= 360;

			for (int i = 0; i < 80; i++) {//スター描画
				if (star[i].type == 0 && star[i].exist == TRUE) {
					DrawGraph(PosOnScreenX(star[i].x - 7), PosOnScreenY(star[i].y - 7), point, true);
				}
			}
			for (int i = 0; i < 100; i++) {//自機弾デザ
				if (Pshot[i].exist == TRUE) {
					Pshot[i].Draw();
				}
			}
			for (int i = 0; i < 20; i++) {//敵機デザ
				if (ene[i].exist == TRUE) {
					ene[i].Draw();
				}
			}
			for (int i = 0; i < 500; i++) {//敵機弾デザ
				if (Eshot[i].exist == TRUE
					&& PosOnScreenX(Eshot[i].x) > 0
					&& PosOnScreenX(Eshot[i].x) < 640
					&& PosOnScreenY(Eshot[i].y) > 0
					&& PosOnScreenY(Eshot[i].y) < 480) {
					Eshot[i].Draw();
				}
			}

			gage_ef += 10;
			if (gage_ef > 460) gage_ef = -100;

			SetDrawScreen(gageCopy);
			DrawBox(0, 0, 640, 40, GetColor(255, 255, 255), true);
			//DrawBox(0, 0, ply_hp / 1000.0 * 600, 20, GetColor(0, 255, 0), true);
			DrawGraph((ply_hp / 1000.0 - 1)*580.0, 0, bar, true);
			DrawGraph((ply_ult / 400.0 - 1)*330.0, 18, bar2, true);
			if (ply_ult == 400) DrawGraph(gage_ef, 18, bar2_ef, true);
			if (ply_sp <= 7) DrawCircle(20, 19, 19, GetColor(45 + ply_sp * 30, 45 + ply_sp * 30, 45 + ply_sp * 30), true);
			if (ply_sp > 7) DrawCircle(20, 19, 19, GetColor(255, 255, 255), true);
			DrawGraph(0, 0, gage, true);

			SetDrawScreen(DX_SCREEN_BACK);
			DrawMask(0, 0, Mask, DX_MASKTRANS_BLACK);
			DrawGraph(0, 0, gageCopy, true);
			FillMaskScreen(0);

			DrawGraph(128, 96, shader, true);

			DrawString(160, 130, "FINISH", GetColor(255, 255, 0));
			DrawFormatString(160, 160, GetColor(255, 255, 255), "score : %d", Score);
			if (Score > highScore[Stage - 1]) {
				if (GetRand(10) < 5) {
					DrawFormatString(300, 160, GetColor(255, 0, 0), "HighScore!");
				}
				else {
					DrawFormatString(300, 160, GetColor(255, 255, 0), "HighScore!");
				}
			}
			DrawString(160, 210, "Return → EnterKey", GetColor(255, 255, 255));
			if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
				tuto_time++;
			}
			else {
				tuto_time = 0;
			}
			if (tuto_time == 1) {
				PlaySoundMem(SEL, DX_PLAYTYPE_BACK);
				if (Score > highScore[Stage - 1]) highScore[Stage - 1] = Score;
				scene = Title;
				tuto_time++;
				for (int i = 0; i < 500; i++) {
					Eshot[i].x = 0;
					Eshot[i].y = 0;
					Eshot[i].exist = FALSE;
					Eshot[i].type = 1;
					Eshot[i].angle = 0;
				}
				for (int i = 0; i < 80; i++) {
					star[i].x = 0;
					star[i].y = 0;
					star[i].exist = FALSE;
				}
			}


			break;
		}

		if (ProcessMessage() == -1) break;
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;
		ScreenFlip();
	}

	//ファイル書き込み
	fp = fopen("score.bin", "wb");
	fwrite(&highScore[0], sizeof(int), 1, fp);
	fwrite(&highScore[1], sizeof(int), 1, fp);
	fwrite(&highScore[2], sizeof(int), 1, fp);
	fclose(fp);

	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}

void Player_Move() {}

void Enemy_Move() {}

int NearEnemy(type1 type, enemy ene[]) {

	int min_dist = 160000;//画面内最大距離400
	int way_id = -1;
	for (int i = 0; i < 20; i++) {
		int dist;
		dist = (ene[i].x - plyX)*(ene[i].x - plyX) + (ene[i].y - plyY)*(ene[i].y - plyY);
		if (min_dist > dist && ene[i].exist == TRUE
			&& PosOnScreenX(ene[i].x) > 0 && PosOnScreenX(ene[i].x) < 640
			&& PosOnScreenY(ene[i].y) > 0 && PosOnScreenY(ene[i].y) < 480) {
			min_dist = dist; way_id = i;
		}
	}
	int angle = 0;
	if (way_id != -1) {
		angle = atan((ene[way_id].y - plyY) / (ene[way_id].x - plyX)) * 360 / (PI * 2);
		if (ene[way_id].x < plyX) angle = 180 + angle;
	}

	if (type == ID) return way_id;
	if (type == Angle) {
		if (way_id == -1) return -1;
		if (way_id != -1) return angle;
	}
	return 0;
}

int PosOnScreenX(int x) {
	return x - plyX + 320;
}
int PosOnScreenX(double x) {
	return (int)x - plyX + 320;
}
int PosOnScreenY(int y) {
	return y - plyY + 240;
}
int PosOnScreenY(double y) {
	return (int)y - plyY + 240;
}

double Distance(double x1, double y1, double x2, double y2) {

	double dist;
	dist = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	return dist;

}

void DrawPulseLine(double x1, double y1, double x2, double y2) {
	double angle = atan((y1 - y2) / (x1 - x2));
	if (x1 > x2) angle += PI;
	double dist = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	int Num = (int)(dist / 30) + 2;

	pos dot[16];

	for (int i = 0; i < Num; i++) {
		double fuga = cos(angle) * 30 * i;
		dot[i].x = x1 + fuga + GetRand(21) - 10;
		fuga = sin(angle) * 30 * i;
		dot[i].y = y1 + fuga + GetRand(21) - 10;
	}
	dot[0].x = x1;
	dot[0].y = y1;

	if (sqrt((x1 - dot[Num - 1].x)*(x1 - dot[Num - 1].x) + (y1 - dot[Num - 1].y)*(y1 - dot[Num - 1].y))) Num--;

	for (int i = 0; i < Num - 1; i++) {
		DrawLine(dot[i].x, dot[i].y, dot[i + 1].x, dot[i + 1].y, GetColor(0, 255, 0));
	}
	DrawLine(dot[Num - 1].x, dot[Num - 1].y, x2, y2, GetColor(0, 255, 0));

}

void DrawPulseCircle(double x, double y, double r) {
	float dist = PI * 2 * r;
	int Num = (int)(dist / 10);
	float Intvl = dist / Num;

	pos dot[300];

	for (int i = 0; i < Num; i++) {
		//double fuga = cos(angle) * 30 * i;
		dot[i].x = x + r*cos(PI * 2 / Num*i) + GetRand(21) - 10;
		//fuga = sin(angle) * 30 * i;
		dot[i].y = y + r*sin(PI * 2 / Num*i) + GetRand(21) - 10;
	}

	for (int i = 0; i < Num - 1; i++) {
		DrawLine(dot[i].x, dot[i].y, dot[i + 1].x, dot[i + 1].y, GetColor(0, 255, 0));
	}
	DrawLine(dot[0].x, dot[0].y, dot[Num - 1].x, dot[Num - 1].y, GetColor(0, 255, 0));
}

int ChkKey() {
	if (CheckHitKey(KEY_INPUT_RIGHT) == 0
		&& CheckHitKey(KEY_INPUT_LEFT) == 0
		&& CheckHitKey(KEY_INPUT_UP) == 0
		&& CheckHitKey(KEY_INPUT_DOWN) == 0) return 0;//なし
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1
		&& CheckHitKey(KEY_INPUT_LEFT) == 0
		&& CheckHitKey(KEY_INPUT_UP) == 0
		&& CheckHitKey(KEY_INPUT_DOWN) == 0) return 1;//右(0)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 0
		&& CheckHitKey(KEY_INPUT_LEFT) == 1
		&& CheckHitKey(KEY_INPUT_UP) == 0
		&& CheckHitKey(KEY_INPUT_DOWN) == 0) return 2;//左(180)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 0
		&& CheckHitKey(KEY_INPUT_LEFT) == 0
		&& CheckHitKey(KEY_INPUT_UP) == 1
		&& CheckHitKey(KEY_INPUT_DOWN) == 0) return 3;//上(270)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 0
		&& CheckHitKey(KEY_INPUT_LEFT) == 0
		&& CheckHitKey(KEY_INPUT_UP) == 0
		&& CheckHitKey(KEY_INPUT_DOWN) == 1) return 4;//下(90)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1
		&& CheckHitKey(KEY_INPUT_LEFT) == 1
		&& CheckHitKey(KEY_INPUT_UP) == 0
		&& CheckHitKey(KEY_INPUT_DOWN) == 0) return 5;//右・左
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1
		&& CheckHitKey(KEY_INPUT_LEFT) == 0
		&& CheckHitKey(KEY_INPUT_UP) == 1
		&& CheckHitKey(KEY_INPUT_DOWN) == 0) return 6;//右・上(315)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1
		&& CheckHitKey(KEY_INPUT_LEFT) == 0
		&& CheckHitKey(KEY_INPUT_UP) == 0
		&& CheckHitKey(KEY_INPUT_DOWN) == 1) return 7;//右・下(45)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 0
		&& CheckHitKey(KEY_INPUT_LEFT) == 1
		&& CheckHitKey(KEY_INPUT_UP) == 1
		&& CheckHitKey(KEY_INPUT_DOWN) == 0) return 8;//左・上(225)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 0
		&& CheckHitKey(KEY_INPUT_LEFT) == 1
		&& CheckHitKey(KEY_INPUT_UP) == 0
		&& CheckHitKey(KEY_INPUT_DOWN) == 1) return 9;//左・下(135)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 0
		&& CheckHitKey(KEY_INPUT_LEFT) == 0
		&& CheckHitKey(KEY_INPUT_UP) == 1
		&& CheckHitKey(KEY_INPUT_DOWN) == 1) return 10;//上・下
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1
		&& CheckHitKey(KEY_INPUT_LEFT) == 1
		&& CheckHitKey(KEY_INPUT_UP) == 1
		&& CheckHitKey(KEY_INPUT_DOWN) == 0) return 11;//右・左・上(270)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1
		&& CheckHitKey(KEY_INPUT_LEFT) == 1
		&& CheckHitKey(KEY_INPUT_UP) == 0
		&& CheckHitKey(KEY_INPUT_DOWN) == 1) return 12;//右・左・下(90)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1
		&& CheckHitKey(KEY_INPUT_LEFT) == 0
		&& CheckHitKey(KEY_INPUT_UP) == 1
		&& CheckHitKey(KEY_INPUT_DOWN) == 1) return 13;//右・上・下(0)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 0
		&& CheckHitKey(KEY_INPUT_LEFT) == 1
		&& CheckHitKey(KEY_INPUT_UP) == 1
		&& CheckHitKey(KEY_INPUT_DOWN) == 1) return 14;//左・上・下(180)
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1
		&& CheckHitKey(KEY_INPUT_LEFT) == 1
		&& CheckHitKey(KEY_INPUT_UP) == 1
		&& CheckHitKey(KEY_INPUT_DOWN) == 1) return 15;//右・左・上・下
	return 0;
}