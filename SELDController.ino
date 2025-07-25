//Arduino Micro または Leonard を使用してください

//Adafruit MCP23017 Arduino Library を導入してください。
//Adafruit_MCP4725 Arduino Library は使用しません
//導入方法：ライブラリマネージャーから上記を検索してインストールします


//簡単な説明
//コマンドに対応しました。デリミタ:CR、Baud:115200、DataBits:8、StopBit:1
//マスコン4段or5段、ブレーキX段+EBです。 ブレーキ段数は"WR 004 X(CR)"で段数(X)指定できます。
//ノッチの移動量に応じて、各キーボードコマンドを打ち込んでいることになりますので、BVEのキーアサイン設定はデフォルトに戻してください。
//マスコン側はMC53またはMC37の真理値に基づいてノッチ(N,P1～P(4)5)を指示します。レバーサも対応します。
//ブレーキ側はポテンショの値を角度換算し、ブレーキノッチ(N,B1～EB)を指示します。

//BVEゲーム開始時は、一旦ブレーキハンドルをN→常用最大(or EB)、マスコンノッチはN、レバーサハンドルをB→N→Fと動かす等してリセットします。
//※AtsEXを導入し、最新プラグインを適用すると上記を自動で調整します。

//更新履歴
//V2   ブレーキ弁角度と段数を計算で処理するようにした
//V3   速度計を調整可能にした
//V3.5 電圧計を電流に応じて動かすようにした
//V3.6 ブレーキ弁段数を変更できるようにした
//V3.6.1 電流計を絶対値表示にした、レバーサ不具合修正、ブレーキ角度をPOT_NとPOT_EB間の範囲とした
//V3.6.2 Arduino Pin9 に ATS復帰(内房線用)　"Homeキー" 追加
//V3.6.3 他基板連動対応(Serial1送信対応)
//V3.6.3.2 他基板からキーボードコマンド受付対応、ATS確認をSpacebar(0x20)に変更、Pin6をEBに、チャタリング防止機能テスト
//V3.6.3.3 速度調整修正、常用最大位置を設定可能とする(デフォルト67°)
//V3.6.3.4 速度計修正、微修正
//V3.6.3.5 ブレーキ弁調整モード
//V3.6.3.8 ブレーキ弁調整モード修正
//V3.6.3.9 直通帯最小位置を設定可能とする(デフォルト3°)
//V3.7 簡易自動帯再現
//V4.0.0.0 コマンド番号化
//V4.1.0.0 自動帯を追加
//V4.1.0.1 速度計調整時速度計が動かないバグ修正
//V4.1.0.2 BPの増減圧インターバルを追加
//V4.1.0.3 自動帯の使用可否の選択機能を追加
//V4.1.0.4 自動帯有効時、電制を無効とする
//V4.1.0.5 自動帯有効時、レバーサをNとする
//V4.1.0.6 自動帯有効時、マスコンノッチ投入でF/B対応、NでレバーサNとする
//V4.1.0.7 個別読出追加、微修正、最大ノッチ指定追加
//V4.1.0.8 最大ノッチ条件判定微修正
//V4.1.0.9 EEPROM書き込み関数修正(int→uint16_t)、ATS_ConfとATS_Contを反転機能追加、
//V4.1.0.10 小修正、他基板対応修正
//V4.1.1.0 自動ブレーキ基板対応 ATS警報持続、確認ボタン基板転送対応
//V4.1.1.3 自動ノッチ合わせ機構対応
//V4.1.1.5 速度計補正配列化
//V4.1.1.6 速度計補正最適化
//V4.1.1.7 ブレーキノッチ逆転
//V4.1.1.8_simple MCP3008とMCP4725を使用しない定義を追加
//V4.1.1.9 微修正
//V4.1.2.1 警報持続スイッチ反転を他スイッチに拡張(ATS確認を除く)、抑速-非常接点切替対応、ME38自動帯非常抜取対応
//V4.1.2.2 実際のエアー圧使用時にもME38非常抜取対応
//V4.1.2.3 非常ラッチ(EB_latch)解除位置が常用最大手前だったものを修正
//V4.1.2.4 EBスイッチとATS復帰が反転していたので修正
//V4.1.2.5 POT_NとPOT_EBの上限修正
//V4.2.0.1 下位に自動帯情報を伝達、抑速ノッチを廃止(マスコンノッチに統合)
//V4.2.0.2 ツリアイ管(ER)を追加
//V4.2.0.3 圧力計の針の動きをソフトにする
//V4.2.0.4 自動帯から直通帯に動かした時の針の動きを滑らかにする
//V4.2.0.5 自動帯で直通ランプを消灯させる
//V4.2.0.6 N位置で自動ブレーキ作動時は直通ランプを消灯、直通帯で自動ブレーキ作動時は直通ランプを点灯させる
//V4.2.0.7 調整モード(模型運転モード)でノッチオフをP0、緩解をB0、自動ブレーキ時のレバーサ切替を無効とする
//V4.2.1.1 BveEX起動時、自動帯をB0とし、下位からのBC圧を伝達する
//V4.2.1.2 BveEX起動時、B0位置で直通ランプが点灯しない不具合修正
//V4.2.1.3 BveEX起動時、直通帯でBCが動かない不具合修正
//V4.2.1.4 JRETS185系に仮対応
//V4.2.1.5 マスコンEBに対応
//V4.2.1.6 外部からのキー入力に対応("KEY c"c:char or "KEY 0xXX")
//V4.2.1.7 軽量化および文字列のポインタ→参照渡しへ修正、ブレーキ弁調整モードを削除、エアー不使用時にATS直下など非常ブレーキ動作を導入
//V4.2.1.8 BveEX起動時、直通帯でEB解除とならなかったため修正
//V4.2.1.9 抑速段に入った場合でも1ノッチを投入する(PIN_DECの判定のみ)
//V4.2.2.1 ブレーキ弁EB入力対応、ATSMxコマンドの修正
//V4.2.2.2 事故モードを追加、マスコン、レバーサ無効、抑速段処理軽量化、Serial1→USB通信修正
//V4.2.2.3 模型モード(modeN)トグルスイッチ修正、キーボード処理、マスコン処理周辺軽量化
//V4.2.2.4 模型モード修正、キーボード処理、マスコン処理周辺軽量化
//V4.2.2.5 USB入力とSerial入力判定部を共通化

/*set_InputFlip
  1bit:警報持続 0:A接点 1:B接点
  (2bit:ATS確認ボタン) 0:A接点 1:B接点
  3bit:ATS復帰ボタン 0:A接点 1:B接点 
  4bit:EB 0:A接点 1:B接点
  5bit:警笛1 0:A接点 1:B接点
  6bit:警笛2 0:A接点 1:B接点
  7bit:抑速 0:抑速 1:非常
  8bit:メーター確認 0:実行 2:なし
  */

#include <Adafruit_MCP23X17.h>
#include <Adafruit_MCP4725.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Keyboard.h>

//#define SIMPLE MCP3008およびMCP4725を使用しない定義用

//MCP23S17接続ピン定義
//マスコン入力
#define PIN_MC_1 0
#define PIN_MC_2 1
#define PIN_MC_3 2
#define PIN_MC_4 3
#define PIN_MC_5 4
#define PIN_MC_DEC 5
//レバーサ入力
#define PIN_MC_DIR_F 6
#define PIN_MC_DIR_B 7
//警笛入力
#define PIN_HORN_1 8
#define PIN_HORN_2 9
//ATS警報持続入力
#define PIN_ATS_CONT 10
//スイッチボックス入力
#define PIN_ATS_CONF 11
#define PIN_ROOM_LIGHT 12
#define PIN_LIGFT_DEF 13
#define PIN_LIGHT_ON 14
#define PIN_PANTO 15
//MCP23S17接続ピン定義

//Arduino Micro接続ピン定義
#define PIN_EB_SW 5
#define PIN_DOOR_OUT 8
#define PIN_ATS_REC 9
#define PIN_BVE_MODE 12
//Arduino Micro接続ピン定義

//MCP3008/MCP23S17不使用時SPI_SS定義
#ifndef SIMPLE
#define SS_Brk 4  //MCP3008_Brk
#define SS_Mc SS  //MCP23S17_MC
#endif
//MCP3008/MCP23S17不使用時SPI_SS定義

//↓デバッグのコメント(//)を解除するとシリアルモニタでデバッグできます
//#define DEBUG

Adafruit_MCP23X17 mcp;
#ifndef SIMPLE
Adafruit_MCP4725 dac;
Adafruit_MCP4725 dac2;
#endif

SPISettings settings = SPISettings(1000000, MSBFIRST, SPI_MODE0);

uint16_t ioexp_1_AB = 0;
uint16_t bve_speed = 0;
int8_t notch_mc = 0;               //マスコンノッチ
char notch_name[3];                //マスコンノッチ名称
uint8_t notch_brk = 0;             //ブレーキノッチ
uint8_t notch_brk_latch = 0;       //ブレーキノッチ格納 ※自動ノッチ合わせ機構でも使用するためグローバル変数
uint8_t sap_notch_brk = 0;         //ブレーキノッチ(直通帯)
String notch_brk_name = "";        //ブレーキノッチ名称
String notch_brk_name_latch = "";  //ブレーキノッチ名称格納
//以下ブレーキ設定値

uint16_t set_BrakeNotchNum = 8;         //004 常用ブレーキ段数
uint16_t set_BrakeSAPAngle = 80;        //006 直通帯の角度
uint16_t set_BrakeEBAngle = 150;        //008 非常位置
uint16_t set_BrakeFullAngle = 165;      //010 ブレーキ全体角度
uint16_t set_Brake67DegAngl = 67;       //056 直通帯最大角度
uint16_t set_Brake10DegAngl = 3;        //058 直通帯最小角度
uint16_t set_BrakeAutoAir1Angle = 130;  //060 重なり全開位置
uint16_t set_BrakeAutoAir2Angle = 135;  //062 重なり開始位置
uint16_t BP_press = 490;                //BP管圧力
uint16_t ER_press = 490;                //ER管圧力
uint8_t brk_angl = 0;                   //ブレーキ弁角度
//以上ブレーキ設定値

//以下速度計補正値
uint16_t set_SpeedAdjust[18] = { 0, 150, 400, 680, 1010, 1330, 1650, 2000, 2340, 2680, 3020, 3340, 3650, 4000, 4095, 4095, 4095, 4095 };
uint16_t set_SpeedLimit = 120;  //044 速度上限
//以上速度計補正値

uint16_t use_KaiseiBrake = true;       //046 回生モード true:有効　false:無効
uint16_t set_CurrentMeterMode = true;  //048 計器モード true:電圧計 false:電流計
uint16_t set_CurrentLimit = 750;       //050 電流上限
uint16_t set_VehicleResistance = 500;  //052 列車抵抗
uint16_t set_ChatteringFilter = 1;     //054 チャタリングフィルタ[°]

int8_t iDir = 0;        //レバーサ
int8_t iDir_latch = 0;  //レバーサ格納 ※自動ノッチ合わせ機構でも使用するためグローバル変数
char cDir[2] = "  ";
char cDir_N[2] = "  ";

//ATS
bool Ats_Pos = 0;              //ATS確認位置
bool Ats_Pos_latch = 0;        //ATS確認位置
uint8_t Ats_In_Count_On = 0;   //ATS確認位置チャタリング防止用
uint8_t Ats_In_Count_Off = 0;  //ATS確認位置チャタリング防止用
uint16_t set_InputFlip = 0;    //074 ボタン反転(旧警報持続ボタン反転 0:B接点 1以上:A接点)0:B接点 1以上:A接点
uint16_t Ats_Conf_flip = 0;    //076 ATS確認ボタン反転 0:B接点 1以上:A接点
uint16_t use_AtsContact = 0;   //082 ATS接点判定使用
//ATS

//以下ブレーキ位置調整用
uint16_t set_POT_N = 0;     //000
uint16_t set_POT_EB = 512;  //002
//以上ブレーキ位置調整用

bool mode_POT = false;

//運転モード
bool modeBVE = true;
bool modeN = false;

//自動ブレーキ帯

uint8_t bp_span = 20;
uint8_t autoair_notch_brk = 0;
uint8_t autoair_notch_brk_latch = 0;
uint16_t set_BrakeBPDownSpan = 20;  //064 自動帯減圧インターバル
uint16_t set_BrakeBPUpSpan = 20;    //066 自動帯増圧インターバル
uint16_t set_AutoAirBrake = 1;      //068 自動帯使用可否 0bit:使用可否 bit1:AutoAirEX bit2:BveEX bit3:直接伝送 bit4:TS185//V4.2.1.1変更
bool use_AutoAirBrake = set_AutoAirBrake & 1;
bool use_BveEX = set_AutoAirBrake >> 2 & 1;
bool mode_TS185 = set_AutoAirBrake >> 4 & 1;
bool use_AutoAirBrakeDirectSend = set_AutoAirBrake >> 3 & 1;
bool notch_BrakeAAB = false;

bool autoair_dir_mask = false;       //自動帯使用時方向切替をマスク
uint16_t BC_press = 0;               //自動帯他基板より入力された値を格納
uint16_t press_BC_Sim = 0;           //自動帯シミュレータのBC圧を格納
uint16_t bve_BC_press = 0;           //USBより入力されたBC値を格納
uint16_t bve_SAP_press = 0;          //USBより入力されたSAP値を格納
uint16_t use_AAB_RealAir = 1;        //080 実際のエアー圧で自動帯再現
uint16_t set_MCNotchNumConsole = 5;  //070マスコンノッチ最大数
uint16_t set_MCNotchNumBVE = 5;      //072マスコンノッチ数(車両)
uint16_t Auto_Notch_Adjust = 1;      //078自動ノッチ合わせ機構
bool EB_latch = false;               //非常ラッチ　自動帯で解除されないようにする
bool deadman = false;                //デッドマン

bool Pan_Mode = true;        //PAN 1:通電 PAN 0:停電
bool Accident_Mode = false;  //ACD 1:事故 0:解除

bool EB_JR_move_E = false;  //非常ブレーキ

String tmp_ser1_s = "";

String strbve = "0000/1/ 00000/100000/0000000000000000000001/NN0B08M780C440F490S440P490/";
String str = strbve;
String str_latch = str;

void setup() {
  pinMode(SS_Brk, OUTPUT);           //MCP3008
  pinMode(SS_Mc, OUTPUT);            //MCP23S17_MC
  pinMode(PIN_EB_SW, INPUT_PULLUP);  //EBスイッチ
  pinMode(PIN_DOOR_OUT, OUTPUT);     //BVE_Door
  digitalWrite(PIN_DOOR_OUT, 0);

#ifdef SIMPLE
  pinMode(A0, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
#endif

  pinMode(PIN_ATS_REC, INPUT_PULLUP);   //ATS復帰
  pinMode(10, INPUT_PULLUP);            //予備
  pinMode(11, INPUT_PULLUP);            //予備SW1
  pinMode(PIN_BVE_MODE, INPUT_PULLUP);  //BVE模型切替SW BVE:1 模型:0

  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.setTimeout(10);
  Serial1.setTimeout(10);
#ifndef SIMPLE
  dac.begin(0x60);
  dac2.begin(0x61);
#endif

  Keyboard.begin();

  if (mcp.begin_SPI(SS_Mc)) {
    // マスコンスイッチを全てプルアップ
    for (uint8_t i = 0; i < 16; i++) {
      mcp.pinMode(i, INPUT_PULLUP);
    }
  } /*else{
Serial.println("Error.");
  }*/

  //初回書き込みチェック
  int16_t b = 0;
  EEPROM.get(100, b);
  if (b != 1) {
    EEPROM.put(0, set_POT_N);                        //POT_N
    EEPROM.put(2, set_POT_EB);                       //POT_EB
    EEPROM.put(4, set_BrakeNotchNum);                //ブレーキ段数設定
    EEPROM.put(6, set_BrakeSAPAngle);                //直通帯範囲
    EEPROM.put(8, set_BrakeEBAngle);                 //非常位置
    EEPROM.put(10, set_BrakeFullAngle);              //ブレーキ最大角度
    for (int i = 1; i <= 16; i++) {                  //速度計補正値
      EEPROM.put((i * 2) + 10, set_SpeedAdjust[i]);  //
    }                                                //
    EEPROM.put(44, set_SpeedLimit);                  //速度計上限
    EEPROM.put(46, use_KaiseiBrake);                 //回生モード
    EEPROM.put(48, set_CurrentMeterMode);            //計器モード
    EEPROM.put(50, set_CurrentLimit);                //電流値上限
    EEPROM.put(52, set_VehicleResistance);           //列車抵抗
    EEPROM.put(54, set_ChatteringFilter);            //チャタリング
    EEPROM.put(56, set_Brake67DegAngl);              //直通帯最大角度
    EEPROM.put(58, set_Brake10DegAngl);              //直通帯最小角度
    EEPROM.put(60, set_BrakeAutoAir1Angle);          //自動帯重なり開始位置
    EEPROM.put(62, set_BrakeAutoAir2Angle);          //自動帯常用全開角度
    EEPROM.put(64, set_BrakeBPDownSpan);             //自動帯減圧インターバル
    EEPROM.put(66, set_BrakeBPUpSpan);               //自動帯増圧インターバル
    EEPROM.put(68, set_AutoAirBrake);                //自動帯使用可否 0bit:使用可否 bit1:AutoAirEX bit2:BveEX bit3:直接伝送 bit4:TS185//V4.2.1.1変更
    EEPROM.put(70, set_MCNotchNumConsole);           //マスコンノッチ最大数
    EEPROM.put(72, set_MCNotchNumBVE);               //マスコンノッチ数(車両)
    EEPROM.put(74, set_InputFlip);                   //入力反転(旧警報持続ボタン反転) 0:B接点 1以上:A接点
    EEPROM.put(76, Ats_Conf_flip);                   //ATS確認ボタン反転 0:B接点 1以上:A接点
    EEPROM.put(78, Auto_Notch_Adjust);               //自動ノッチ合わせ
    EEPROM.put(80, use_AAB_RealAir);                 //実際のエアー圧で自動帯再現
    EEPROM.put(82, use_AtsContact);                  //ATS接点情報を他基板へ伝送
    //初回書き込みフラグセット
    EEPROM.put(100, 1);
  } else {
    EEPROM.get(0, set_POT_N);                        //POT_N
    EEPROM.get(2, set_POT_EB);                       //POT_EB
    EEPROM.get(4, set_BrakeNotchNum);                //ブレーキ段数
    EEPROM.get(6, set_BrakeSAPAngle);                //直通帯幅
    EEPROM.get(8, set_BrakeEBAngle);                 //非常位置
    EEPROM.get(10, set_BrakeFullAngle);              //ブレーキ最大角度
    for (int i = 1; i <= 16; i++) {                  //速度計補正値
      EEPROM.get((i * 2) + 10, set_SpeedAdjust[i]);  //
    }                                                //
    EEPROM.get(44, set_SpeedLimit);                  //速度計上限
    EEPROM.get(46, use_KaiseiBrake);                 //回生モード
    EEPROM.get(48, set_CurrentMeterMode);            //計器モード
    EEPROM.get(50, set_CurrentLimit);                //電流値上限
    EEPROM.get(52, set_VehicleResistance);           //列車抵抗
    EEPROM.get(54, set_ChatteringFilter);            //チャタリング
    EEPROM.get(56, set_Brake67DegAngl);              //直通帯最大角度
    EEPROM.get(58, set_Brake10DegAngl);              //直通帯最小角度
    EEPROM.get(60, set_BrakeAutoAir1Angle);          //自動帯重なり開始位置
    EEPROM.get(62, set_BrakeAutoAir2Angle);          //自動帯常用全開角度
    EEPROM.get(64, set_BrakeBPDownSpan);             //自動帯減圧インターバル
    EEPROM.get(66, set_BrakeBPUpSpan);               //自動帯増圧インターバル
    EEPROM.get(68, set_AutoAirBrake);                //自動帯使用可否 0bit:使用可否 bit1:AutoAirEX bit2:BveEX bit3:直接伝送 bit4:TS185//V4.2.1.1変更
    EEPROM.get(70, set_MCNotchNumConsole);           //マスコンノッチ最大数
    EEPROM.get(72, set_MCNotchNumBVE);               //マスコンノッチ数(車両)
    EEPROM.get(74, set_InputFlip);                   //入力反転(旧警報持続ボタン反転) 0:B接点 1以上:A接点
    EEPROM.get(76, Ats_Conf_flip);                   //ATS確認ボタン反転 0:B接点 1以上:A接点
    EEPROM.get(78, Auto_Notch_Adjust);               //自動ノッチ合わせ
    EEPROM.get(80, use_AAB_RealAir);                 //実際のエアー圧で自動帯再現
    EEPROM.get(82, use_AtsContact);                  //ATS接点情報を他基板へ伝送
  }
  modeN = digitalRead(PIN_BVE_MODE);
  modeBVE = !modeN;
  //速度計テスト
  if (!(set_InputFlip >> 7 & 1)) {
    disp_SpeedMeter(set_SpeedLimit * 10);
    delay(1500);
  }
  disp_SpeedMeter(0);
}

void loop() {
  static int16_t bve_current = 0;
  static uint16_t BC_press_latch = BC_press;
  static uint16_t BP_press_latch = BP_press;
  static uint16_t ER_press_latch = ER_press;
  use_AutoAirBrake = set_AutoAirBrake & 1;
  use_BveEX = set_AutoAirBrake >> 2 & 1;
  use_AutoAirBrakeDirectSend = set_AutoAirBrake >> 3 & 1;
  mode_TS185 = set_AutoAirBrake >> 4 & 1;
  read_USB();
  read_Serial1();

  bool tmp_mode_n = !digitalRead(PIN_BVE_MODE);  //BVE模型切替SW BVE:1 模型:0
  static bool tmp_mode_n_latch = tmp_mode_n;
  if (tmp_mode_n != tmp_mode_n_latch) {
    modeN = !tmp_mode_n;
    modeBVE = tmp_mode_n;
    tmp_mode_n_latch = tmp_mode_n;
  }

  if (str != str_latch) {
    uint8_t i = 0;
    String s = "";
    if (str.startsWith("WR ")) {
      //設定モード
      if (str.length() > 7) {
        uint8_t device = str.substring(3, 6).toInt();
        uint16_t num = str.substring(7, 12).toInt();
        if (device < 100) {
          set_Settings(device, num);
        }
      }

    } else if (str.startsWith("RD ")) {
      if (str.length() > 5) {
        uint8_t device = str.substring(3, 6).toInt();
        if (device < 100) {
          uint16_t nnn = 0;
          Serial.println(rw_eeprom(device, nnn, nnn, false, false));
        }
      }

    } else if (str.startsWith("BC ")) {
      if (use_AutoAirBrake && !mode_TS185) {
        if (use_AAB_RealAir) {
          BC_press = str.substring(3, 6).toInt();
          Serial.println(str);
        }
      }

      //キーボードモード 4.2.1.6追加
    } else if (str.startsWith("KEY")) {
      KeyPut(str);

      //PANモード 4.2.1.6追加
    } else if (str.startsWith("PAN")) {
      if (str.length() > 4) {
        Pan_Mode = str.charAt(4) == '1';
      }

      //事故モード 4.2.2.2追加 マスコンレバーサ無効
    } else if (str.startsWith("ACD")) {
      if (str.length() > 4) {
        Accident_Mode = str.charAt(4) == '1';
      }

      //圧力モニタ
    } else if (str.startsWith("FV")) {
      if (str.length() > 4) {
        Serial.println(str);
      }
    } else if (str.startsWith("MD ")) {
      //模型モード
      if (str.indexOf("N   ") > 0) {
        s = "OK N   ";
        if (str.length() > 7) {
          int16_t num = str.substring(7, 12).toInt();
          if (num) {
            modeN = true;
            modeBVE = false;
            set_BrakeNotchNum = 8;
            s += "ON";
          } else {
            modeN = false;
            modeBVE = true;
            s += "OFF";
          }
        }
      }

      //ポテンショ読取モード
      else if (str.indexOf("POT ") > 0) {
        s = "OK POT ";
        if (str.length() > 7) {
          int16_t num = str.substring(7, 12).toInt();
          if (num) {
            mode_POT = true;
            s += "ON";
          } else {
            mode_POT = false;
            s += "OFF";
          }
        }
      } else {
        s = "E0";
      }
      Serial.println(s);

    } else {
      //通常モード：速度、戸閉、電流抽出
      if (str.length() > 11) {
        strbve = str;
        bve_speed = strbve.substring(0, 4).toInt();

        //戸閉灯指示
        digitalWrite(PIN_DOOR_OUT, strbve.charAt(5) != '1');

        bve_current = strbve.substring(7, 12).toInt();

        //自動ノッチ合わせ機構
        AutoNotch(strbve);

        //BC抽出
        if (strbve.length() >= 67) {
          bve_BC_press = strbve.substring(55, 58).toInt();
          bve_SAP_press = strbve.substring(63, 66).toInt();
          //非常ブレーキ抽出
          EB_JR_move_E = strbve.substring(36, 37).toInt();
        }
      }
    }

    //速度計表示補正 調整時表示のためここ
    disp_SpeedMeter(bve_speed);

    //電流計
    disp_CurrentMeter(bve_current);


    //Serial1転送
    send_Serial1(str);
    str_latch = str;
  }

  read_IOexp();              //IOエキスパンダ読込ルーチン
  read_Light_Def();          //減光ライト読込ルーチン
  read_Light();              //前照灯読込ルーチン
  read_MC();                 //マスコンノッチ読込ルーチン
  read_Dir();                //マスコンレバーサ読込ルーチン
  read_Break();              //ブレーキハンドル読込ルーチン
  read_Break_Setting();      //ブレーキハンドル読込ルーチン(未実装)
  read_Horn();               //ホーンペダル読込ルーチン
  read_Ats();                //ATS確認・警報持続読込ルーチン
  read_Panto();              //強制終了ルーチン
  read_EB();                 //EBスイッチ読込ルーチン
  keyboard_control(strbve);  //キーボード(HID)アウトプットルーチン

  //圧力値が変動時、下位および上位に伝送
  if (BC_press != BC_press_latch || BP_press != BP_press_latch || ER_press != ER_press_latch) {
    send_Serial1(strbve);
    //V4.2.1.1追加 実際のエアー圧を使用しない場合(TS185でなく、modeBVEのとき)はBCの変化時にPCへ伝送
    if (use_AutoAirBrake && !use_AAB_RealAir && !mode_TS185 && modeBVE) {
      if (BC_press != BC_press_latch) {
        Serial.print("BC ");
        Serial.println(BC_press);
      }
    }
    BC_press_latch = BC_press;
    BP_press_latch = BP_press;
    ER_press_latch = ER_press;
  }

  delay(10);
  if (!modeBVE) {
    delay(10);
  }
}

//MCP23S17読込
//値が0でないとき　かつ n 回連続で同じ値を出力したときに値を変更する
void read_IOexp() {
  uint16_t temp_ioexp1_ini = mcp.readGPIOAB();
  uint8_t n = 3;
  if (temp_ioexp1_ini != 0) {
    for (uint8_t i = 0; i < n; i++) {
      uint16_t temp_ioexp1 = mcp.readGPIOAB();
      if (temp_ioexp1 != 0) {
        if (temp_ioexp1_ini == temp_ioexp1) {
          if (i == n - 1) {
            ioexp_1_AB = temp_ioexp1;
          }
        } else {
          break;
        }
      } else {
        break;
      }
    }
  }
}

//MCP3008ADコンバータ読取
uint16_t adcRead(uint8_t ch) {  // 0 .. 7
#ifndef SIMPLE
  byte channelData = (ch + 8) << 4;
  //  Serial.println(String(channelData, BIN));
  SPI.beginTransaction(settings);
  digitalWrite(SS_Brk, LOW);
  delayMicroseconds(100);
  SPI.transfer(0b00000001);                   // Start bit 1
  byte highByte = SPI.transfer(channelData);  // singleEnd
  byte lowByte = SPI.transfer(0x00);          // dummy
  delayMicroseconds(100);
  digitalWrite(SS_Brk, HIGH);
  SPI.endTransaction();
  return ((highByte & 0x03) << 8) + lowByte;
#else
  switch (ch) {
    case 0:
      return analogRead(A0);
      break;
    case 1:
      return analogRead(A3);
      break;
  }
#endif
}

//MCP23S17マスコンノッチ状態読込 (MC53抑速ブレーキ対応)
void read_MC(void) {
  if (!Accident_Mode) {
    if (ioexp_1_AB >> PIN_MC_DEC & 1) {
      if (set_InputFlip >> 6 & 1 && deadman) {  //非常有効時
        deadman = false;
      }
      autoair_dir_mask = false;
      if (~ioexp_1_AB >> PIN_MC_5 & 1) {
        if (set_MCNotchNumConsole == 5) {
          notch_mc = set_MCNotchNumBVE;
        }
      } else if (~ioexp_1_AB >> PIN_MC_4 & 1) {
        if (set_MCNotchNumConsole == 4) {
          notch_mc = set_MCNotchNumBVE;
        } else if (set_MCNotchNumConsole == 5 && set_MCNotchNumBVE != 4) {
          notch_mc = 4;
        }
      } else if (~ioexp_1_AB >> PIN_MC_3 & 1) {
        notch_mc = 3;
      } else if (~ioexp_1_AB >> PIN_MC_2 & 1) {
        notch_mc = 2;
      } else if (~ioexp_1_AB >> PIN_MC_1 & 1) {
        notch_mc = 1;
      } else {
        notch_mc = 0;
        //マスコンノッチが0でブレーキが自動帯にあるときはレバーサ0、レバーサマスクをtrue、ただしBveEXモード時を除く
        if (brk_angl > set_BrakeSAPAngle && brk_angl < set_BrakeEBAngle) {
          AutoAirMask();
        }
      }
    } else {
      if (set_InputFlip >> 6 & 1 && !deadman) {  //非常有効時
        //マスコンデッドマン仮実装
        notch_mc = 0;
        notch_brk = set_BrakeNotchNum + 1;
        notch_brk_name = "EB";
        autoair_dir_mask = false;
        deadman = true;
      } else {                    //抑速有効時
        if (!autoair_dir_mask) {  //自動ブレーキ使用時は抑速使用不可
          notch_mc = -1;
          uint8_t decBit = ~ioexp_1_AB >> 1 & B1111;
          switch (decBit) {
            case B0001:
              notch_mc = -3;
              break;
            case B0010:
              notch_mc = -2;
              break;
            case B0101:
              notch_mc = -4;
              break;
            case B0110:
              notch_mc = -5;
              break;
            case B1000:
              notch_mc = -1;
              break;
          }
        }
      }
    }
  } else {
    notch_mc = 0;
  }
  notch_name[0] = (notch_mc >= 0) ? 'P' : 'H';
  notch_name[1] = char(abs(notch_mc));
}

//マスコンレバーサ読取
void read_Dir(void) {
  if (!autoair_dir_mask) {
    if ((~ioexp_1_AB >> PIN_MC_DIR_F & 1) && !Accident_Mode) {
      iDir = 1;
      cDir[0] = 'F';
      cDir_N[0] = 'L';
    } else if ((~ioexp_1_AB >> PIN_MC_DIR_B & 1) && !Accident_Mode) {
      iDir = -1;
      cDir[0] = 'B';
      cDir_N[0] = 'R';
    } else {
      iDir = 0;
      cDir[0] = 'N';
      cDir_N[0] = 'N';
    }
  }
}

//ブレーキ角度読取
uint16_t read_Break() {
  if (!deadman) {
    uint16_t adc_raw = adcRead(0);
    static uint16_t adc_latch = 0;
    uint16_t adc = adc_raw;
    if (set_POT_N < set_POT_EB) {
      if (adc < set_POT_N) {
        adc = set_POT_N;
      } else if (adc > set_POT_EB) {
        adc = set_POT_EB;
      }
    }
    brk_angl = map(adc, set_POT_N, set_POT_EB, 0, set_BrakeFullAngle);

    if (mode_POT) {
      String pot = " Pot1: ";
      pot += String(adc_raw + 10000);
      pot.setCharAt(7, '0');
      Serial.print(pot);
      String deg = " Deg: ";
      deg += String(brk_angl + 1000);
      deg.setCharAt(6, '0');
      Serial.println(deg);
    }

    static uint8_t brk_angl_latch = brk_angl;
    static bool notch_BrakeAAB_latch = notch_BrakeAAB;
    //前回ブレーキ弁角度と今回ブレーキ弁角度がチャタリングフィルタ以上の場合
    if (abs(brk_angl - brk_angl_latch) >= set_ChatteringFilter) {

      //直通帯の処理
      if (brk_angl < set_BrakeSAPAngle) {
        EB_latch = false;  //非常ラッチ解除 V4.1.2.8位置移動
        //N位置
        if (brk_angl <= set_Brake10DegAngl) {
          notch_brk = 0;
          sap_notch_brk = 0;

          //直通帯位置(※常用最大67°位置まで)
        } else if (brk_angl < set_Brake67DegAngl) {
          uint16_t temp_notch_brk = round((float)(brk_angl - set_Brake10DegAngl) / (float)(set_Brake67DegAngl - set_Brake10DegAngl) * (set_BrakeNotchNum - 1) + 0.5);
          notch_brk = temp_notch_brk;
          sap_notch_brk = notch_brk;

          //常用最大67°位置～直通帯範囲まで
        } else {
          if (mode_TS185 && notch_BrakeAAB) {
            Keyboard.write('K');
          }
          notch_brk = set_BrakeNotchNum;
          sap_notch_brk = set_BrakeNotchNum;
          autoair_dir_mask = false;
          notch_BrakeAAB = false;
        }
        notch_brk_name = "B";
        notch_brk_name += notch_brk;
      }

      //自動帯の処理
      else if (brk_angl < set_BrakeEBAngle) {
        if (use_AutoAirBrake) {
          //抜き取り・保ち位置
          if (brk_angl < set_BrakeAutoAir1Angle) {
            if (mode_TS185) {
              if (!notch_BrakeAAB) {
                Keyboard.write(',');
              }
              Keyboard.release('.');
            } else {
              //4.2.1.8 TS185モード時以外、非常投入から抜取位置にしても非常を維持するよう変更
              if (!EB_latch) {
                notch_brk = 0;
              }
            }
            notch_BrakeAAB = true;
            notch_brk_name = "A0";
            AutoAirMask();

            //自動帯減圧位置(弱)
          } else if (brk_angl < set_BrakeAutoAir2Angle) {
            if (modeN || mode_POT) {
              notch_brk_name = "A1";
            } else {
              if (mode_TS185) {
                Keyboard.press('.');
              }
              //ブレーキ弁が自動帯にあり、マスコンノッチが0のときレバーサ0、レバーサマスクをtrueに、ただし模型モード以外またはBveEXモード以外のときを除く
              AutoAirMask();
            }
            //自動帯減圧位置(強)
          } else {
            if (modeN || mode_POT) {
              notch_brk_name = "A2";
            } else {
              if (mode_TS185) {
                Keyboard.press('.');
              }
              AutoAirMask();
            }
          }
          //自動帯非使用時は直通帯常用最大扱い
        } else {
          notch_BrakeAAB = false;
          notch_brk = set_BrakeNotchNum;
          autoair_dir_mask = false;
        }


        //非常位置
      } else {
        notch_brk = set_BrakeNotchNum + 1;
        notch_brk_name = "EB";
        autoair_dir_mask = false;
        EB_latch = true;
      }
      if ((notch_BrakeAAB != notch_BrakeAAB_latch) && !mode_TS185 && use_BveEX) {
        //Serial.print();
        Serial.print("AAB ");
        Serial.println(notch_BrakeAAB, BIN);
      }
      notch_BrakeAAB_latch = notch_BrakeAAB;
      brk_angl_latch = brk_angl;
    }

    if (use_AutoAirBrake) {
      BP_Sim();
    }

    //ポテンショ生データ表示モード
    if (mode_POT) {
      Serial.print(" Notch: ");
      Serial.print(notch_brk_name);
      Serial.print(" BP: ");
      Serial.print(BP_press);
      Serial.print(" BP_notch: ");
      Serial.print(autoair_notch_brk);
    }
    adc_latch = adc;
  }
}

//キーボード(HID)出力
void keyboard_control(String &str) {
  //マスコンノッチが前回と異なるとき
  static int8_t notch_mc_latch = notch_mc;
  if (notch_mc != notch_mc_latch) {
    if (modeBVE) {
      uint8_t d = abs(notch_mc - notch_mc_latch);
      //力行ノッチ
      if (notch_mc >= 0 && notch_mc_latch >= 0) {
        for (uint8_t i = 0; i < d; i++) {
          //進段
          if ((notch_mc - notch_mc_latch) > 0) {
            Keyboard.write('Z');
          } else {
            Keyboard.write('A');
          }
        }
      } else {
        //抑速ノッチ
        //if (notch_mc <= 0 && notch_mc_latch <= 0 && notch_mc >= -5 && notch_mc_latch >= -5) {
        //進段
        for (uint8_t i = 0; i < d; i++) {
          if ((notch_mc - notch_mc_latch) < 0) {
            Keyboard.write('Q');
            //戻し
          } else {
            Keyboard.write('A');
          }
        }
      }
      if (notch_mc >= 0 && notch_mc <= 3) {
        tmp_ser1_s = "ATSM";
        tmp_ser1_s += notch_mc;
        Serial1Print(tmp_ser1_s, false);
      }
      //modeN
    } else {
      if (notch_brk == 0) {  //ブレーキ緩解
        Serial1Print(String(notch_name[0]) + String(abs(notch_mc)), true);
      }
    }
  }

  //ブレーキノッチ(角度)が前回と異なるとき
  if (notch_brk != notch_brk_latch || notch_brk_name != notch_brk_name_latch) {
    uint8_t d = abs(notch_brk - notch_brk_latch);
    //ブレーキノッチ
    if (notch_brk >= 0 && notch_brk_latch >= 0 && notch_brk < (set_BrakeNotchNum + 1)) {
      for (uint8_t i = 0; i < d; i++) {
        //戻し
        if ((notch_brk - notch_brk_latch) < 0) {
          if (modeBVE) {
            if (!mode_TS185) {
              Keyboard.write(',');
            } else {
              Keyboard.write('K');
            }
          }
        }
      }
      //ブレーキ
      if ((notch_brk - notch_brk_latch) > 0) {
        if (modeBVE) {
          if (!mode_TS185) {
            Keyboard.write('.');
          } else {
            Keyboard.write('L');
          }
        }
      }
    }
    //非常
    static bool EB_serial_write = false;
    bool condition = notch_brk == set_BrakeNotchNum + 1;
    Keyboard_Press_Release_BVE(condition, '/');
    if (condition) {
      str.setCharAt(36, '1');
      send_Serial1(str);
      EB_serial_write = true;
    } else {
      if (EB_serial_write) {
        str.setCharAt(36, '0');
        send_Serial1(str);
        EB_serial_write = false;
      }
    }
    if (modeN) {
      Serial1Print(notch_brk_name, true);
    }
  }

  //レバーサが前回と異なるとき
  if (iDir != iDir_latch) {
    if (modeBVE) {
      uint8_t d = abs(iDir - iDir_latch);
      for (uint8_t i = 0; i < d; i++) {
        //前進
        if ((iDir - iDir_latch) > 0) {
          Keyboard.write(0xDA);  //"↑"
          //後進
        } else {
          Keyboard.write(0xD9);  //"↓"
        }
      }
    } else {
      //modeN
      Serial1Print(String(cDir_N[0]) + String(cDir_N[1]), true);
    }
  }
  notch_mc_latch = notch_mc;
  notch_brk_latch = notch_brk;
  notch_brk_name_latch = notch_brk_name;
  autoair_notch_brk_latch = autoair_notch_brk;
  iDir_latch = iDir;
}

//ブレーキ角度調整
void read_Break_Setting(void) {
  static uint16_t adj_N = 0;
  static uint16_t adj_EB = 0;
  static unsigned long iniMillis_N = 0;
  static unsigned long iniMillis_EB = 0;
  static uint8_t setMode_N = 0;
  static uint8_t setMode_EB = 0;
  String s = "";
  uint16_t value = 0;
#ifndef SIMPLE
  bool btn_n = (adcRead(5) < 512);
  bool btn_eb = (adcRead(6) < 512);
#else
  bool btn_n = !digitalRead(3);
  bool btn_eb = !digitalRead(2);
#endif
  if (btn_n) {
    if (setMode_N == 0) {
      adj_N = adcRead(0);
      setMode_N = 1;
      iniMillis_N = millis();
      s = "POT_N = ";
      s += EEPROM.get(0, value);
      s += " ADC = ";
      s += adj_N;
    } else if (setMode_N == 1) {
      if (millis() - iniMillis_N > 3000) {
        setMode_N = 2;
      }
      adj_N = (adj_N * 9 + adcRead(0)) * 0.1;
    } else if (setMode_N == 2) {
      setMode_N = 0;
      set_POT_N = adj_N;
      EEPROM.put(0, set_POT_N);
      s = "NEW POT_N = ";
      s += set_POT_N;
    }
  } else {
    setMode_N = 0;
  }

  if (btn_eb) {
    if (setMode_EB == 0) {
      adj_EB = adcRead(0);
      setMode_EB = 1;
      iniMillis_EB = millis();
      s = "POT_EB = ";
      s += EEPROM.get(2, value);
      s += " ADC = ";
      s += adj_EB;
    } else if (setMode_EB == 1) {
      if (millis() - iniMillis_EB > 3000) {
        setMode_EB = 2;
      }
      adj_EB = (adj_EB * 9 + adcRead(0)) * 0.1;
    } else if (setMode_EB == 2) {
      setMode_EB = 0;
      set_POT_EB = adj_EB;
      EEPROM.put(2, set_POT_EB);
      s = "NEW POT_EB = ";
      s += set_POT_EB;
    }
  } else {
    setMode_EB = 0;
  }
  if (s != "") {
    Serial.println(s);
  }
}

void read_Horn(void) {
  bool Horn_1 = (~ioexp_1_AB >> PIN_HORN_1 & 1) ^ (set_InputFlip >> 4 & 1);  //警笛1
  static bool Horn_1_latch = Horn_1;
  if (Horn_1 != Horn_1_latch) {
    if (!mode_TS185) {
      Keyboard_Press_Release_BVE(Horn_1, KEY_RETURN);  //Enter:0xB0
    } else {
      Keyboard_Press_Release_BVE(Horn_1, KEY_BACKSPACE);  //BackSpace:0xB2
    }
    tmp_ser1_s = "HN1 ";
    tmp_ser1_s += String(Horn_1);
    Serial1Print(tmp_ser1_s, false);  //モニターしない
    Horn_1_latch = Horn_1;
  }

  bool Horn_2 = (~ioexp_1_AB >> PIN_HORN_2 & 1) ^ (set_InputFlip >> 5 & 1);  //警笛2
  static bool Horn_2_latch = Horn_2;
  if (Horn_2 != Horn_2_latch) {
    Keyboard_Press_Release_BVE(Horn_2, KEY_KP_PLUS);  //+:0xDF
    tmp_ser1_s = "HN2 ";
    tmp_ser1_s += String(Horn_2);
    Serial1Print(tmp_ser1_s, false);  //モニターしない
    Horn_2_latch = Horn_2;
  }
}

bool In_and_Out(bool input, bool &output, uint8_t cycle_limit, uint8_t &cycle_on, uint8_t &cycle_off) {
  if (input) {
    cycle_on++;
    cycle_off = 0;
  } else {
    cycle_off++;
    cycle_on = 0;
  }
  if (cycle_on >= cycle_limit) {
    cycle_on = cycle_limit;
    output = true;
  } else if (cycle_off >= cycle_limit) {
    cycle_off = cycle_limit;
    output = false;
  }
}

void read_Ats(void) {
  //ATS誤作動防止
  In_and_Out(adcRead(1) < 1, Ats_Pos, 10, Ats_In_Count_On, Ats_In_Count_Off);

  //ATS確認位置転送
  if (use_AtsContact) {
    if (Ats_Pos != Ats_Pos_latch) {
      tmp_ser1_s = "ATS ";
      tmp_ser1_s += String(Ats_Pos);
      Serial1Print(tmp_ser1_s, true);
    }
    Ats_Pos_latch = Ats_Pos;
  }

  //EB
  static uint8_t EB_In_Count_On = 0;
  static uint8_t EB_In_Count_Off = 0;
  static bool EB_Pos = false;
  In_and_Out(adcRead(4) < 1, EB_Pos, 10, EB_In_Count_On, EB_In_Count_Off);

  if (EB_Pos) {
    notch_brk = set_BrakeNotchNum + 1;
    notch_brk_name = "EB";
    autoair_dir_mask = false;
    EB_latch = true;
  }

  //ATS警報持続
  bool Ats_Cont = (~ioexp_1_AB >> PIN_ATS_CONT & 1) ^ (set_InputFlip & 1);  //警報持続スイッチ
  static bool Ats_Cont_latch = Ats_Cont;                                    //警報持続スイッチ
  if (Ats_Cont != Ats_Cont_latch) {
    tmp_ser1_s = "ACT ";
    tmp_ser1_s += String(Ats_Cont);
    Serial1Print(tmp_ser1_s, true);
    Keyboard_Press_Release_BVE(Ats_Cont, KEY_INSERT);  //Insert:0xD1
    Ats_Cont_latch = Ats_Cont;
  }

  //ATS確認
  bool Ats_Conf = (~ioexp_1_AB >> PIN_ATS_CONF & 1) ^ (Ats_Conf_flip);  //ATS確認ボタン
  static bool Ats_Conf_latch = Ats_Conf;                                //ATS確認ボタン
  if (Ats_Conf != Ats_Conf_latch) {
    tmp_ser1_s = "ACF ";
    tmp_ser1_s += String(Ats_Conf);
    Serial1Print(tmp_ser1_s, true);
    Keyboard_Press_Release_BVE(Ats_Conf, 0x20);  //Space:Keyboard.hに定義なし
    Ats_Conf_latch = Ats_Conf;
  }

  //ATS復帰
  bool Ats_Rec = !digitalRead(PIN_ATS_REC) ^ (set_InputFlip >> 2 & 1);
  static bool Ats_Rec_latch = Ats_Rec;
  if (Ats_Rec != Ats_Rec_latch) {
    Keyboard_Press_Release_BVE(Ats_Rec, KEY_HOME);  //Home:0xD2
    Ats_Rec_latch = Ats_Rec;
  }
}

void read_Panto(void) {
  bool Panto = ~ioexp_1_AB >> PIN_PANTO & 1;
  static bool Panto_latch = false;
  if (Panto != Panto_latch) {
    Keyboard_Press_Release_BVE(Panto, KEY_LEFT_ALT);  //Alt:0x82
    Keyboard_Press_Release_BVE(Panto, KEY_F4);        //F40xC5
    Panto_latch = Panto;
  }
}

void read_Light_Def(void) {
  /*  Light_Def = ~ioexp_1_AB & (1 << PIN_LIGFT_DEF);
    if ( Light_Def != Light_Def_latch )
    {
    modeBVE = Light_Def;
    if (!Light_Def) {
    set_BrakeNotchNum = 8;
    }
    }
    Light_Def_latch = Light_Def;*/
}

void read_Light(void) {
  /*Light_On = ~ioexp_1_AB & (1 << PIN_LIGHT_ON);
    if ( Light_On != Light_On_latch )
    {
    modeBVE = Light_On;
    if (!Light_On) {
    set_BrakeNotchNum = 8;
    }
    }
    Light_On_latch = Light_On;*/
}

void read_EB(void) {
  bool EB_SW = !digitalRead(PIN_EB_SW) ^ (set_InputFlip >> 3 & 1);
  static bool EB_SW_latch = false;
  if (EB_SW != EB_SW_latch) {
    Keyboard_Press_Release_BVE(EB_SW, KEY_DELETE);  //Delete:0xD4
    EB_SW_latch = EB_SW;
  }
}

void disp_SpeedMeter(uint16_t spd) {
  //速度計のリミットを適用
  if (spd > (set_SpeedLimit * 10)) {
    spd = (set_SpeedLimit * 10);
  }

  uint16_t volt = 0;
  for (int i = 1; i < 18; i++) {
    if (spd <= (i * 100)) {
      volt = map(spd, (i - 1) * 100, i * 100, set_SpeedAdjust[i - 1], set_SpeedAdjust[i]);
      break;
    }
  }
#ifndef SIMPLE
  dac.setVoltage(volt, false);
#else
  analogWrite(10, volt >> 4);
#endif
}

void BP_Sim() {
  if (!use_AAB_RealAir) {
    //BPの増減圧インターバルを設定
    if (brk_angl < set_BrakeAutoAir1Angle) {
      bp_span = set_BrakeBPUpSpan;
    } else if ((brk_angl >= set_BrakeAutoAir1Angle) && (brk_angl <= set_BrakeAutoAir2Angle)) {
      bp_span = map(brk_angl, set_BrakeAutoAir1Angle, set_BrakeAutoAir2Angle, set_BrakeBPDownSpan, set_BrakeBPDownSpan);
    } else if (brk_angl > set_BrakeAutoAir2Angle) {
      bp_span = set_BrakeBPDownSpan;
    }

    static unsigned long bp_millis = 0;
    static unsigned long er_millis = 0;
    if (EB_JR_move_E) {
      BP_ER_EB_Control(bp_millis, er_millis);
    } else {
      //直通帯(運転位置)でBP,ERを加圧
      if (brk_angl < set_BrakeSAPAngle) {
        if ((millis() - bp_millis) > bp_span) {
          if (BP_press < 490) {
            BP_press++;
          }
          if (ER_press < 490) {
            ER_press++;
          }
          bp_millis = millis();
        }
        //重なり位置でBP,ERは維持
      } else if (brk_angl < set_BrakeAutoAir1Angle) {

        //常用位置でBPとER圧を減圧
      } else if (brk_angl < set_BrakeEBAngle) {
        if ((millis() - bp_millis) > bp_span) {
          if (BP_press > 0) {
            BP_press--;
          }
          if (ER_press > 0) {
            ER_press--;
          }
          bp_millis = millis();
        }

        //非常位置
      } else if (brk_angl >= set_BrakeEBAngle) {
        BP_ER_EB_Control(bp_millis, er_millis);
      }
    }

    //BC圧をBP圧より生成
    press_BC_Sim = (490 - BP_press) * 3.3;  //暫定
    //非常吐出なく、自動帯常用でBC圧力が440kPa以上のとき440kPaとする
    if (press_BC_Sim > 440) {
      if (brk_angl < set_BrakeEBAngle && !EB_latch) {
        press_BC_Sim = 440;
      }
    }
    //非常動作時
    if (press_BC_Sim > 490) {  //計算上のBC圧が490より上のとき
      if (brk_angl >= set_BrakeEBAngle || EB_latch) {
        press_BC_Sim = 490;
      }
    }
  } else {
    press_BC_Sim = BC_press;
  }  ////////

  //自動帯使用時に圧力をノッチに変換(BveEXを使用しない場合)
  //BveEX使用時、ノッチはB0固定
  if (!use_BveEX) {
    //BC圧からブレーキノッチに変換
    if (EB_latch) {
      autoair_notch_brk = set_BrakeNotchNum + 1;
    } else {
      autoair_notch_brk = map(press_BC_Sim, 0, 440, 0, set_BrakeNotchNum);
    }
  }
  //自動帯圧力優先シーケンス
  //N位置
  if (brk_angl <= set_Brake10DegAngl) {
    //自動ブレーキ
    if (autoair_notch_brk >= 0) {  //自動ブレーキ帯の段数が(N位置より)高いとき
      notch_brk = autoair_notch_brk;
      if (!use_BveEX) {
        strbve.setCharAt(17, '0');  //直通ランプ消灯
      }
    }
    //直通帯位置
  } else if (brk_angl < set_BrakeSAPAngle) {
    //自動ブレーキ
    if (notch_brk < autoair_notch_brk) {  //自動ブレーキ帯の段数が高いとき
      notch_brk = autoair_notch_brk;
      if (!use_BveEX) {
        strbve.setCharAt(17, '1');  //直通ランプ点灯
      }
    }

    //自動帯(BveEX非使用時のとき)
  } else if (brk_angl < set_BrakeEBAngle && !use_BveEX) {  //自動ブレーキ
    if (notch_brk < autoair_notch_brk) {                   //自動ブレーキ帯の段数が高いとき
      notch_brk = autoair_notch_brk;
    }
  }
}

void BP_ER_EB_Control(unsigned long &bp_millis, unsigned long &er_millis) {
  //BPを急減圧
  if ((millis() - bp_millis) > 20) {
    if (BP_press > 0) {
      BP_press -= 10;
      if (BP_press < 10) {
        BP_press = 0;
      }
    }
    bp_millis = millis();
  }
  //ER圧を減圧
  if ((millis() - er_millis) > bp_span) {
    if (ER_press > 0) {
      ER_press--;
    }
    er_millis = millis();
  }
}

//EEPROM読書用
String rw_eeprom(uint8_t &dev, uint16_t &n, uint16_t &param, bool write, bool NGcondition) {
  String s = "OK ";
  if (!NGcondition) {
    if (write) {
      param = n;
      EEPROM.put(dev, param);
    } else {
      EEPROM.get(dev, param);
    }

    if (dev < 100) {
      s += "0";
    }
    if (dev < 10) {
      s += "0";
    }
    s += dev;
    s += " ";
    s += param;
  } else {
    s = "E1 ";
    s += dev;
  }
  return s;
}

//ノッチ自動調整 BVEからの入力に対して現在状態の比較を行い、ラッチに格納することでメインループで自動的に修正する
void AutoNotch(String &str) {
  if (str.length() > 49 && Auto_Notch_Adjust && !autoair_dir_mask) {
    if (str.charAt(47) == 'B') {
      int bve_rev = 0;
      if (str.charAt(44) == 'F') {
        iDir_latch = 1;
      } else if (str.charAt(44) == 'B') {
        iDir_latch = -1;
      }
      char Buf[4];
      Buf[0] = str.charAt(48);
      Buf[1] = str.charAt(49);
      Buf[2] = str.charAt(50);
      notch_brk_latch = strtol(Buf, NULL, 16);  //16進数→10進数に変換
    }
  }
}

//電圧電流計表示用
void disp_CurrentMeter(int16_t current) {
  //回生モードでないとき、負電流は0
  if (!use_KaiseiBrake && (current < 0)) {
    current = 0;
  }
  //電圧計モードのとき計算後2000Vを超えた時は2000Vに固定
  if (set_CurrentMeterMode) {
    uint16_t v = 0;
    if (Pan_Mode) {
      v = 1500 - (current * (set_VehicleResistance / 1000.0));
      if (v > 2000) {
        v = 2000;
      }
    }
    //出力
#ifndef SIMPLE
    dac2.setVoltage(map(v, 0, 2000, 0, 4095), false);
#else
    //analogWrite(11, map(v, 0, 2000, 0, 4095) / 4);
    analogWrite(11, map(v, 0, 2000, 0, 4095) >> 4);
#endif
    //電流計モードの時
  } else {
    //電流計は正のみ(現状)
    int curr = abs(current);
    //電流が電流上限を超えた時は電流上限に抑える
    if (curr > set_CurrentLimit) {
      curr = set_CurrentLimit;
    }
    //出力
#ifndef SIMPLE
    dac2.setVoltage(map(curr, 0, set_CurrentLimit, 0, 4095), false);
#else
    //analogWrite(11, map(curr, 0, set_CurrentLimit, 0, 4095) / 4));
    analogWrite(11, map(curr, 0, set_CurrentLimit, 0, 4095) >> 4));
#endif
  }
}

void read_Serial1() {
  if (Serial1.available() > 0) {
    str = Serial1.readStringUntil('\r');
  }
}

void read_USB() {
  if (Serial.available()) {
    str = Serial.readStringUntil('\r');
  }
}

void send_Serial1(String &str) {
  if (use_AutoAirBrake) {
    //自動帯有効で自動帯投入時、直通ランプと電制を無効とする
    if (brk_angl > set_BrakeSAPAngle) {
      if (str.length() > 18) {
        if (!use_BveEX) {
          str.setCharAt(17, '0');  //直通ランプ
          str.setCharAt(18, '0');  //電制ランプ
        }
      }
    }
    //自動帯シミュレーション時、BC、BP、SAPの演算結果をSerial1へ転送する(直接転送モード以外)
    if (!use_AAB_RealAir && !use_AutoAirBrakeDirectSend) {
      static bool sap_auto_mask = false;
      //"0000/1/ 00000/100000/0000000000000000000001/NN0B08M780C440E490S440P490/";
      BC_press = press_BC_Sim;  //BCの演算結果を代入

      //現在BC圧よりもBVEからのBC圧が高い場合はBVEを優先し
      //逆の場合で直通帯のときBVEの圧力を優先する
      if (bve_BC_press > BC_press) {
        if (!sap_auto_mask) {
          BC_press = bve_BC_press;
        }
      } else {
        if (!sap_auto_mask) {
          if (brk_angl < set_BrakeSAPAngle) {
            BC_press = bve_BC_press;
          }
        }
      }

      setStringAt(55, str, BC_press);
      //BP
      setStringAt(67, str, BP_press);
      //ER
      setStringAt(59, str, ER_press);

      //SAPread_Serial1
      static uint16_t sap_press_latch = bve_SAP_press;
      static unsigned long sap_millis = millis();
      static bool sap_latch = false;
      if (brk_angl <= set_BrakeSAPAngle) {
        if (sap_latch && sap_press_latch <= 435) {
          if ((bve_SAP_press - sap_press_latch) > 50) {
            sap_press_latch += 5;
          } else {
            sap_latch = false;
          }
        }
        if (!sap_latch) {
          if (!sap_auto_mask) {
            sap_press_latch = bve_SAP_press;
          } else {
            uint16_t tmp_sap_press = map(sap_notch_brk, 0, set_BrakeNotchNum, 0, 440);
            if (tmp_sap_press > bve_SAP_press) {
              sap_auto_mask = false;
            }
          }
        }
      }
      if (brk_angl > set_BrakeSAPAngle) {
        sap_latch = true;
        sap_auto_mask = true;
        if (millis() - sap_millis > 20) {
          if (sap_press_latch >= 5) {
            sap_press_latch -= 5;
          }
          if (sap_press_latch < 5) {
            sap_press_latch = 0;
          }
        }
      }
      setStringAt(63, str, sap_press_latch);
    }
  }
  //非常
  if (notch_brk == set_BrakeNotchNum + 1) {
    str.setCharAt(36, '1');
  }

  Serial1.print(str);
  Serial1.print('\r');
}

void set_Settings(uint8_t &device, uint16_t &num) {
  String s = "";
  switch (device) {
    //緩め位置設定
    case 0:
      s = rw_eeprom(device, num, set_POT_N, true, num < 0 || num > 1023);
      break;

    //非常位置設定
    case 2:
      s = rw_eeprom(device, num, set_POT_EB, true, num < 0 || num > 1023);
      break;

    //ブレーキ段数
    case 4:
      s = rw_eeprom(device, num, set_BrakeNotchNum, true, num == 0 || num > 255);
      break;

    //直通帯幅
    case 6:
      s = rw_eeprom(device, num, set_BrakeSAPAngle, true, num == 0 || num > set_BrakeEBAngle);

      break;

    //非常位置
    case 8:
      s = rw_eeprom(device, num, set_BrakeEBAngle, true, num == 0 || num > set_BrakeFullAngle);
      break;

    //ブレーキ最大角度
    case 10:
      s = rw_eeprom(device, num, set_BrakeFullAngle, true, num == 0 || num > 255);
      break;

    //速度計調整
    case 12:
    case 14:
    case 16:
    case 18:
    case 20:
    case 22:
    case 24:
    case 26:
    case 28:
    case 30:
    case 32:
    case 34:
    case 36:
    case 38:
    case 40:
    case 42:
      s = rw_eeprom(device, num, set_SpeedAdjust[(device - 10) / 2], true, false);
      bve_speed = ((device - 10) / 2) * 100;
      break;

    //最高速度設定
    case 44:
      s = rw_eeprom(device, num, set_SpeedLimit, true, num == 0);
      break;

    //回生モード
    case 46:
      s = rw_eeprom(device, num, use_KaiseiBrake, true, num > 1);
      break;

    //計器モード
    case 48:
      s = rw_eeprom(device, num, set_CurrentMeterMode, true, num > 1);
      break;
    case 52:  //列車抵抗
      s = rw_eeprom(device, num, set_VehicleResistance, true, num == 0);
      break;
    case 54:  //チャタリング
      s = rw_eeprom(device, num, set_ChatteringFilter, true, num < 0 || num > 3);
      break;
    case 56:  //常用最大角度
      s = rw_eeprom(device, num, set_Brake67DegAngl, true, num == 0 || num > set_BrakeSAPAngle);
      break;
    case 58:  //直通帯最小角度
      s = rw_eeprom(device, num, set_Brake10DegAngl, true, num == 0 || num > set_Brake67DegAngl);
      break;
    case 60:  //自動帯常用開始角度
      s = rw_eeprom(device, num, set_BrakeAutoAir1Angle, true, num < set_Brake67DegAngl || num > set_BrakeEBAngle);
      break;
    case 62:  //自動帯常用全開角度
      s = rw_eeprom(device, num, set_BrakeAutoAir2Angle, true, num < set_Brake67DegAngl || num > set_BrakeEBAngle);
      break;
    case 64:  //自動帯減圧インターバル
      s = rw_eeprom(device, num, set_BrakeBPDownSpan, true, num == 0 || num > 100);
      break;
    case 66:  //自動帯増圧インターバル
      s = rw_eeprom(device, num, set_BrakeBPUpSpan, true, num == 0 || num > 100);
      break;
    case 68:  //自動帯使用可否
      s = rw_eeprom(device, num, set_AutoAirBrake, true, num < 0 || num > 255);
      break;
    case 70:  //マスコンノッチ最大数
      s = rw_eeprom(device, num, set_MCNotchNumConsole, true, num < 0);
      break;
    case 72:  //マスコンノッチ数(車両)
      s = rw_eeprom(device, num, set_MCNotchNumBVE, true, num < 0);
      break;
    case 74:  //警報持続ボタン反転 0:B接点 1以上:A接点
      s = rw_eeprom(device, num, set_InputFlip, true, num < 0 || num > 65535);
      break;
    case 76:  //ATS確認ボタン反転 0:B接点 1以上:A接点
      s = rw_eeprom(device, num, Ats_Conf_flip, true, num < 0 || num > 1);
      break;
    case 78:  //自動ノッチ合わせ
      s = rw_eeprom(device, num, Auto_Notch_Adjust, true, num < 0 || num > 1);
      break;
    case 80:  //実際のエアー圧で自動帯再現
      s = rw_eeprom(device, num, use_AAB_RealAir, true, num < 0 || num > 1);
      break;
    case 82:  //ATS接点情報を他基板へ伝送
      s = rw_eeprom(device, num, use_AtsContact, true, num < 0 || num > 1);
      break;

    default:
      s = "E0";
      break;
  }
  Serial.println(s);
}

void setStringAt(uint8_t startIndex, String &str, uint16_t &value) {
  uint16_t d[3];
  d[0] = value / 100 + 0x30;
  d[1] = value / 10 % 10 + 0x30;
  d[2] = value % 10 + 0x30;

  if (str.length() >= 67) {
    for (int i = 0; i < 3; i++) {
      str.setCharAt(startIndex + i, char(d[i]));
    }
  }
}

void Serial1Print(String &command, bool monitor) {
  Serial1.print(command);
  Serial1.print('\r');
  if (monitor) {
    Serial.println(command);
  }
}

void KeyPut(String &str) {
  if (str.length() > 4) {
    if (str.indexOf("0x") > 0) {
      char c_str[5];
      str.substring(4, 8).toCharArray(c_str, 5);
      Keyboard.write(strtol(c_str, NULL, 0));
    } else {
      char c = str.charAt(4);
      Keyboard.write(c);
    }
  }
}

void Keyboard_Press_Release_BVE(bool &condition, byte keycode) {
  if (modeBVE) {
    if (condition) {
      Keyboard.press(keycode);
    } else {
      Keyboard.release(keycode);
    }
  }
}

void AutoAirMask() {
  //マスコンノッチが0、BVEモード時でBveEXモード以外とTS185以外のとき、
  //レバーサ0、レバーサマスクをtrue
  if (use_AutoAirBrake && notch_mc <= 0 && modeBVE && !use_BveEX && !mode_TS185) {
    autoair_dir_mask = true;
    iDir = 0;
    //マスコンノッチが0以外の時はレバーサマスクをfalse
  } else {
    autoair_dir_mask = false;
  }
}