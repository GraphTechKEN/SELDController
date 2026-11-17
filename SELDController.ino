//Arduino Micro または Leonard を使用してください

//簡単な説明
//コマンドに対応しました。デリミタ:CR、Baud:115200、DataBits:8、StopBit:1
//マスコン4段or5段、ブレーキX段+EBです。 ブレーキ段数は"WR 004 X(CR)"で段数(X)指定できます。
//ノッチの移動量に応じて、各キーボードコマンドを打ち込んでいることになりますので、BVEのキーアサイン設定はデフォルトに戻してください。
//マスコン側はMC53またはMC37の真理値に基づいてノッチ(N,P1～P(4)5)を指示します。レバーサも対応します。
//ブレーキ側はポテンショの値を角度換算し、ブレーキノッチ(N,B1～EB)を指示します。

//BVEゲーム開始時は、一旦ブレーキハンドルをN→常用最大(or EB)、マスコンノッチはN、レバーサハンドルをB→N→Fと動かす等してリセットします。
//※BveEx(旧AtsEx)を導入し、SELDController最新プラグインを適用すると上記を自動で調整します。

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
//V4.2.1.9 抑速段に入った場合でも1ノッチを投入する(PIN..,,ブレーキ弁EB入力対応、ATSMxコマンドの修正
//V4.2.2.2 事故モードを追加、マスコン、レバーサ無効、抑速/段処理軽量化、Serial1→USB通信修正
//V4.2.2.3 模型モード(modeN)トグルスイッチ修正、キーボード処理、マスコン処理周辺軽量化
//V4.2.2.4 模型モード修正、キーボード処理、マスコン処理周辺軽量化
//V4.2.2.5 USB入力とSerial入力判定部を共通化
//V4.2.2.6 設定データのReturnが認知できなかったエラーを修正
//V4.2.2.7 String周辺を軽量化、PAN下シーケンスを追加
//V4.2.2.8 非常の'/'連打をやめる
//V4.2.2.9 模型モード微修正(トグル操作orコマンド操作でB8、Pノッチリセット)
//V4.2.3.0 交直切替対応
//V4.2.3.1 容量削減のためライブラリの使用を停止
//V4.2.3.2 ノッチ自動合わせの選択肢にシナリオ開始時のみを追加、前照灯(PageUp)、減光(PageDown)試験実装、パンタ下ボタンに'Alt+F4’or'P'を選択式とした
//V4.2.3.3 B1接点情報伝送追加 076 4bit 1:ポテンショ 0:接点
//V4.2.3.4 ATS電源接点情報伝送追加 076 5bit  1:ポテンショ 0:接点 084:ATS電源角度(ME48)
//V4.2.3.5 B1接点情報のチャタリング防止追加、ATS復帰スイッチが動作しないバグ修正
//V4.2.3.6 自動帯から直通帯に戻した際ブレーキがかからなくなる重大インシデントを修正、USBシリアル通信非接続時の動作安定化
//V4.2.3.7 B1情報伝送コマンドを"B1 X"から"BDN X"に変更
//V4.2.3.8 MCP23S17処理部を修正、!0xFFFF時を削除
//V4.2.3.9 ブレーキ段数0段(直通帯無効)を仮設定、バージョン管理を追加
//V4.2.3.10 後段に非常状態を転送する機能を選択可とする 076 6bit:非常投入状態転送(36) 0:無効 1:有効
//V4.2.3.11 強制BVEモード追加、受信電文に対してLFの読み飛ばしを追加
//V4.2.3.12 リファクタリング(動作変更なし) EEPROM設定をテーブル化、設定ビット/電文位置/ピンを定数化、read_Break・keyboard_control・string_Analysisを関数分割、Stringを文字配列に置換、read_Breakのデバッグ表示の範囲外書込とSIMPLE構成のビルドエラーを修正
//V4.2.3.13 ブレーキポテンショのN位置とEB位置の大小を問わず動作するよう修正(N>EBでも可。ポテンショの+とGNDを逆に接続可能)。N位置とEB位置が同値のときの0除算も回避
//V4.2.3.14 ME38/49の抜取保ち位置での抜取(接点不使用時)に対応

// ------------------------------------------------
// バージョン情報の定義
// ------------------------------------------------
#define VERSION_MAJOR 4
#define VERSION_MINOR 2
#define VERSION_PATCH 3
#define VERSION_BUILD 14

/*set_InputFlip //074
  1bit:警報持続 0:A接点 1:B接点
  (2bit:ATS確認ボタン) 0:A接点 1:B接点
  3bit:ATS復帰ボタン 0:A接点 1:B接点 
  4bit:EB 0:A接点 1:B接点
  5bit:警笛1 0:A接点 1:B接点
  6bit:警笛2 0:A接点 1:B接点
  7bit:抑速 0:抑速 1:非常
  8bit:メーター確認 0:実行 2:なし
*/

/*Ats_Conf_flip //076
  1bit:ATS確認ボタン 0:A接点 1:B接点
  2bit:パンタ下ボタン 0:A接点 1:B接点
  3bit:上予備
  4bit:B1電源使用時(自動帯有効) 0:接点 1:POT 
  5bit:ATS電源 0:接点 1:POT 
  6bit:非常投入状態転送(36) 0:無効 1:有効
  7bit:強制BVEモード
*/

#include <EEPROM.h>
#include <Wire.h>
#include <Keyboard.h>
#include <SPI.h>

//#define SIMPLE MCP3008およびMCP4725を使用しない場合にコメントアウトを削除します

//↓デバッグのコメント(//)を解除するとシリアルモニタでデバッグできます
//#define DEBUG

// ================================================
// ピン・デバイス定義
// ================================================

//MCP23S17接続ピン定義(ビット位置)
//マスコン入力
constexpr uint8_t PIN_MC_1 = 0;
constexpr uint8_t PIN_MC_2 = 1;
constexpr uint8_t PIN_MC_3 = 2;
constexpr uint8_t PIN_MC_4 = 3;
constexpr uint8_t PIN_MC_5 = 4;
constexpr uint8_t PIN_MC_DEC = 5;
//レバーサ入力
constexpr uint8_t PIN_MC_DIR_F = 6;
constexpr uint8_t PIN_MC_DIR_B = 7;
//警笛入力
constexpr uint8_t PIN_HORN_1 = 8;
constexpr uint8_t PIN_HORN_2 = 9;
//ATS警報持続入力
constexpr uint8_t PIN_ATS_CONT = 10;
//スイッチボックス入力
constexpr uint8_t PIN_ATS_CONF = 11;
constexpr uint8_t PIN_ROOM_LIGHT = 12;
constexpr uint8_t PIN_LIGHT_DEF = 13;
constexpr uint8_t PIN_LIGHT_ON = 14;
constexpr uint8_t PIN_PANTO = 15;

//Arduino Micro接続ピン定義
constexpr uint8_t PIN_MCP3008_CS = 4;  //下の CS_BRK_* マクロ(PORTD bit4)と一致させること
constexpr uint8_t PIN_EB_SW = 5;
constexpr uint8_t PIN_DOOR_OUT = 8;
constexpr uint8_t PIN_ATS_REC = 9;
constexpr uint8_t PIN_BVE_MODE = 12;

//MCP3008使用時SPI_SS定義
#ifndef SIMPLE
// Arduino Micro Pin 4 = PORTD Bit 4 (PD4)
#define CS_BRK_LOW() (PORTD &= ~(1 << 4))  // 4番ピンをLOWにする
#define CS_BRK_HIGH() (PORTD |= (1 << 4))  // 4番ピンをHIGHにする
#endif

//SPI_SSピン(PB0)をレジスタで直接叩くためのマクロ
#define CS_LOW() PORTB &= ~_BV(PB0)
#define CS_HIGH() PORTB |= _BV(PB0)

//MCP4725デバイスのアドレス定義
constexpr uint8_t DAC1_ADDR = 0x60;  //速度計
constexpr uint8_t DAC2_ADDR = 0x61;  //電圧計/電流計

//MCP23S17レジスタ定義
constexpr uint8_t MCP23S17_OP_WRITE = 0x40;  //デバイスアドレス (A0-A2がGNDの場合)
constexpr uint8_t MCP23S17_OP_READ = 0x41;   //0x40 | 0x01
constexpr uint8_t MCP23S17_REG_IODIR = 0x00;
constexpr uint8_t MCP23S17_REG_GPPU = 0x0C;
constexpr uint8_t MCP23S17_REG_GPIO = 0x12;

//MCP3008チャンネル割り当て
constexpr uint8_t ADC_CH_BRAKE = 0;    //ブレーキ弁ポテンショ
constexpr uint8_t ADC_CH_ATS_POS = 1;  //ATS確認位置
constexpr uint8_t ADC_CH_B1 = 3;       //B1電源接点
constexpr uint8_t ADC_CH_EB = 4;       //EB接点
constexpr uint8_t ADC_CH_BTN_N = 5;    //ブレーキ位置調整ボタン(N)
constexpr uint8_t ADC_CH_BTN_EB = 6;   //ブレーキ位置調整ボタン(EB)

// ================================================
// 設定ビット位置
// ※ヘッダコメントの "Nbit" は1始まり。コード上のシフト量は N-1
// ================================================
enum InputFlipBit : uint8_t {  //074 set_InputFlip
  FLIP_ATS_CONT = 0,           //警報持続
                               //1: (ATS確認ボタン。現在は Ats_Conf_flip の bit0 を使用)
  FLIP_ATS_REC = 2,            //ATS復帰ボタン
  FLIP_EB = 3,                 //EB
  FLIP_HORN1 = 4,              //警笛1
  FLIP_HORN2 = 5,              //警笛2
  FLIP_DEC_IS_EB = 6,          //0:抑速 1:非常
  FLIP_SKIP_METER_TEST = 7     //0:メーター確認を実行 1:なし
};

enum AtsConfBit : uint8_t {     //076 Ats_Conf_flip
  ATSCONF_ATS_CONF = 0,         //ATS確認ボタン 0:A接点 1:B接点
  ATSCONF_PANTO_KEY_P = 1,      //パンタ下ボタン 0:Alt+F4 1:'P'
  ATSCONF_B1_FROM_POT = 3,      //B1電源 0:接点 1:POT(自動帯有効時)
  ATSCONF_ATSPWR_FROM_POT = 4,  //ATS電源 0:接点 1:POT
  ATSCONF_TRANSFER_EB = 5,      //非常投入状態転送(36) 0:無効 1:有効
  ATSCONF_MODE_SWITCH = 6       //0:常にBVEモード 1:PIN_BVE_MODE で模型/BVE切替
};

enum AutoAirBit : uint8_t {  //068 set_AutoAirBrake
  AAB_ENABLE = 0,            //自動帯使用可否
                             //1: AutoAirEX(未使用)
  AAB_BVEEX = 2,             //BveEX
  AAB_DIRECT_SEND = 3,       //直接伝送
  AAB_TS185 = 4              //TS185
};

// ================================================
// EEPROMアドレス
// ================================================
enum EepromAddr : uint8_t {
  EE_POT_N = 0,
  EE_POT_EB = 2,
  EE_BRAKE_NOTCH_NUM = 4,
  EE_BRAKE_SAP_ANGLE = 6,
  EE_BRAKE_EB_ANGLE = 8,
  EE_BRAKE_FULL_ANGLE = 10,
  EE_SPEED_ADJUST_FIRST = 12,  //12～42(2刻み) set_SpeedAdjust[1]～[16]
  EE_SPEED_ADJUST_LAST = 42,
  EE_SPEED_LIMIT = 44,
  EE_KAISEI_BRAKE = 46,
  EE_CURRENT_METER_MODE = 48,
  EE_CURRENT_LIMIT = 50,
  EE_VEHICLE_RESISTANCE = 52,
  EE_CHATTERING_FILTER = 54,
  EE_BRAKE_67DEG = 56,
  EE_BRAKE_10DEG = 58,
  EE_AUTOAIR_1 = 60,
  EE_AUTOAIR_2 = 62,
  EE_BP_DOWN_SPAN = 64,
  EE_BP_UP_SPAN = 66,
  EE_AUTO_AIR_BRAKE = 68,
  EE_MC_NOTCH_CONSOLE = 70,
  EE_MC_NOTCH_BVE = 72,
  EE_INPUT_FLIP = 74,
  EE_ATS_CONF_FLIP = 76,
  EE_AUTO_NOTCH_ADJUST = 78,
  EE_AAB_REAL_AIR = 80,
  EE_ATS_CONTACT = 82,
  EE_ATS_DENGEN_ANGLE = 84,
  EE_ID = 90,          //SELD"C"ontroller
  EE_VERSION = 92,     //VersionData(4byte)
  EE_INIT_FLAG = 100,   //初回書き込みフラグ
  EE_ME48_EB_EN = 120  // ★新規追加：ME48非常抜取機能有効化(WR 120)
};

// ================================================
// 電文(strbve)のレイアウト
// "0000/1/ 00000/100000/0000000000000000000001/NN0B08M780C440F490S440P490/"
// ================================================
constexpr uint8_t MSG_SPEED = 0;        //速度(4桁)
constexpr uint8_t MSG_DOOR = 5;         //戸閉 '1':閉
constexpr uint8_t MSG_CURRENT = 7;      //電流(5桁)
constexpr uint8_t MSG_LAMP_SAP = 17;    //直通ランプ
constexpr uint8_t MSG_LAMP_DYN = 18;    //電制ランプ
constexpr uint8_t MSG_EB = 36;          //非常
constexpr uint8_t MSG_DIR = 44;         //レバーサ F/B/N
constexpr uint8_t MSG_NOTCH_MARK = 47;  //'B':ブレーキノッチ情報あり
constexpr uint8_t MSG_NOTCH_BRK = 48;   //ブレーキノッチ(16進3桁)
constexpr uint8_t MSG_BC = 55;          //BC圧(3桁)
constexpr uint8_t MSG_ER = 59;          //ER圧(3桁)
constexpr uint8_t MSG_SAP = 63;         //SAP圧(3桁)
constexpr uint8_t MSG_BP = 67;          //BP圧(3桁)
constexpr uint8_t MSG_MIN_LEN_PRESSURE = 67;

// ================================================
// 型定義 (※関数より前に置くこと: Arduinoのプロトタイプ自動生成対策)
// ================================================

struct VersionData {
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
  uint8_t build;
};

//EEPROM保存対象の設定項目
struct SettingEntry {
  uint8_t addr;
  bool writable;  //false:WRコマンドで変更不可
  uint16_t *var;
};

//チャタリング防止用カウンタ
struct DebounceCounter {
  uint8_t on;
  uint8_t off;
};

//ブレーキ位置調整ボタンの状態
struct PotCalibState {
  uint16_t adj;
  uint8_t mode;  //0:待機 1:平均化中 2:確定
};

// ================================================
// グローバル変数
// ================================================

//MCP3008/23S17用SPI設定定義
const SPISettings mcp3008settings = SPISettings(1000000, MSBFIRST, SPI_MODE0);   //1MHz MAX3.6MHz
const SPISettings mcp23S17settings = SPISettings(4000000, MSBFIRST, SPI_MODE0);  //MAX10MHz

uint16_t ioexp_1_AB = 0;
uint16_t bve_speed = 0;
int8_t notch_mc = 0;                //マスコンノッチ
char notch_prefix = 'P';            //マスコンノッチ名称の先頭文字 P:力行 H:抑速
uint8_t notch_brk = 0;              //ブレーキノッチ
uint8_t notch_brk_latch = 0;        //ブレーキノッチ格納 ※自動ノッチ合わせ機構でも使用するためグローバル変数
uint8_t sap_notch_brk = 0;          //ブレーキノッチ(直通帯)
char notch_brk_name[6] = "";        //ブレーキノッチ名称 B0～B255,A0～A2,EB
char notch_brk_name_latch[6] = "";  //ブレーキノッチ名称格納

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

//以下速度計補正値
uint16_t set_SpeedAdjust[18] = { 0, 150, 400, 680, 1010, 1330, 1650, 2000, 2340, 2680, 3020, 3340, 3650, 4000, 4095, 4095, 4095, 4095 };
uint16_t set_SpeedLimit = 120;  //044 速度上限

uint16_t use_KaiseiBrake = true;       //046 回生モード true:有効　false:無効
uint16_t set_CurrentMeterMode = true;  //048 計器モード true:電圧計 false:電流計
uint16_t set_CurrentLimit = 750;       //050 電流上限
uint16_t set_VehicleResistance = 500;  //052 列車抵抗
uint16_t set_ChatteringFilter = 1;     //054 チャタリングフィルタ[°]

int8_t iDir = 0;          //レバーサ
int8_t iDir_latch = 0;    //レバーサ格納 ※自動ノッチ合わせ機構でも使用するためグローバル変数
char dir_prefix_N = ' ';  //模型モード用レバーサ名称 L:前進 R:後進 N:切 (送信時は後ろに空白1文字を付ける)

uint16_t set_InputFlip = 0;       //074 ボタン反転(旧警報持続ボタン反転 0:B接点 1以上:A接点)0:B接点 1以上:A接点
uint16_t Ats_Conf_flip = 0;       //076 ATS確認ボタン反転 0:B接点 1以上:A接点
uint16_t use_AtsContact = 0;      //082 ATS接点判定使用
uint16_t set_AtsDengenAngle = 0;  //084 ATS電源接点角度(ME48)
uint16_t use_Me48Eb = 0;          //120 ME48非常抜取機能有効化 (0:無効 1:有効)

//以下ブレーキ位置調整用
uint16_t set_POT_N = 0;     //000
uint16_t set_POT_EB = 512;  //002

bool mode_POT = false;

//運転モード
bool modeBVE = true;
bool modeN = false;

//自動ブレーキ帯
uint8_t bp_span = 20;
uint8_t autoair_notch_brk = 0;
uint16_t set_BrakeBPDownSpan = 20;  //064 自動帯減圧インターバル
uint16_t set_BrakeBPUpSpan = 20;    //066 自動帯増圧インターバル
uint16_t set_AutoAirBrake = 1;      //068 自動帯使用可否 0bit:使用可否 bit1:AutoAirEX bit2:BveEX bit3:直接伝送 bit4:TS185//V4.2.1.1変更
bool notch_BrakeAAB = false;

bool autoair_dir_mask = false;               //自動帯使用時方向切替をマスク
uint16_t BC_press = 0;                       //自動帯他基板より入力された値を格納
uint16_t press_BC_Sim = 0;                   //自動帯シミュレータのBC圧を格納
uint16_t bve_BC_press = 0;                   //USBより入力されたBC値を格納
uint16_t bve_SAP_press = 0;                  //USBより入力されたSAP値を格納
uint16_t use_AAB_RealAir = 1;                //080 実際のエアー圧で自動帯再現
uint16_t set_MCNotchNumConsole = 5;          //070マスコンノッチ最大数
uint16_t set_MCNotchNumBVE = 5;              //072マスコンノッチ数(車両)
uint16_t Auto_Notch_Adjust = 1;              //078自動ノッチ合わせ機構
bool EB_latch = false;                       //非常ラッチ　自動帯で解除されないようにする
bool deadman = false;                        //デッドマン
bool flg_set_BrakeNotchNum_changed = false;  //ブレーキ段数変更フラグ(0段/それ以外のB1電源制御用)

bool Pan_Mode = true;        //PAN 1:通電 PAN 0:停電
bool Accident_Mode = false;  //ACD 1:事故 0:解除

bool EB_JR_move_E = false;       //非常ブレーキ
bool transfer_EB_state = false;  //後段に非常投入状態を転送(0:しない 1:する) ※keyboard_control()で更新

const char strbve0[] = "0000/1/ 00000/100000/0000000000000000000001/NN0B08M780C440F490S440P490/";
String strbve = strbve0;
String str = strbve0;
String str_latch = str;

bool flgFirstSend = false;  //078自動ノッチ合わせモード2のときに使用、004を受信時にtrue
int16_t bve_current = 0;

// #define の値を構造体にセット
VersionData currentVersion = {
  VERSION_MAJOR,
  VERSION_MINOR,
  VERSION_PATCH,
  VERSION_BUILD
};

//EEPROM保存対象の設定項目表(Flash上に配置)
//※変数の初期値が工場出荷値。追加する場合はここに1行足す
const SettingEntry SETTING_TABLE[] PROGMEM = {
  { EE_POT_N, true, &set_POT_N },
  { EE_POT_EB, true, &set_POT_EB },
  { EE_BRAKE_NOTCH_NUM, true, &set_BrakeNotchNum },
  { EE_BRAKE_SAP_ANGLE, true, &set_BrakeSAPAngle },
  { EE_BRAKE_EB_ANGLE, true, &set_BrakeEBAngle },
  { EE_BRAKE_FULL_ANGLE, true, &set_BrakeFullAngle },
  { 12, true, &set_SpeedAdjust[1] },
  { 14, true, &set_SpeedAdjust[2] },
  { 16, true, &set_SpeedAdjust[3] },
  { 18, true, &set_SpeedAdjust[4] },
  { 20, true, &set_SpeedAdjust[5] },
  { 22, true, &set_SpeedAdjust[6] },
  { 24, true, &set_SpeedAdjust[7] },
  { 26, true, &set_SpeedAdjust[8] },
  { 28, true, &set_SpeedAdjust[9] },
  { 30, true, &set_SpeedAdjust[10] },
  { 32, true, &set_SpeedAdjust[11] },
  { 34, true, &set_SpeedAdjust[12] },
  { 36, true, &set_SpeedAdjust[13] },
  { 38, true, &set_SpeedAdjust[14] },
  { 40, true, &set_SpeedAdjust[15] },
  { 42, true, &set_SpeedAdjust[16] },
  { EE_SPEED_LIMIT, true, &set_SpeedLimit },
  { EE_KAISEI_BRAKE, true, &use_KaiseiBrake },
  { EE_CURRENT_METER_MODE, true, &set_CurrentMeterMode },
  { EE_CURRENT_LIMIT, false, &set_CurrentLimit },  //※WRコマンドでは変更不可(従来仕様)
  { EE_VEHICLE_RESISTANCE, true, &set_VehicleResistance },
  { EE_CHATTERING_FILTER, true, &set_ChatteringFilter },
  { EE_BRAKE_67DEG, true, &set_Brake67DegAngl },
  { EE_BRAKE_10DEG, true, &set_Brake10DegAngl },
  { EE_AUTOAIR_1, true, &set_BrakeAutoAir1Angle },
  { EE_AUTOAIR_2, true, &set_BrakeAutoAir2Angle },
  { EE_BP_DOWN_SPAN, true, &set_BrakeBPDownSpan },
  { EE_BP_UP_SPAN, true, &set_BrakeBPUpSpan },
  { EE_AUTO_AIR_BRAKE, true, &set_AutoAirBrake },
  { EE_MC_NOTCH_CONSOLE, true, &set_MCNotchNumConsole },
  { EE_MC_NOTCH_BVE, true, &set_MCNotchNumBVE },
  { EE_INPUT_FLIP, true, &set_InputFlip },
  { EE_ATS_CONF_FLIP, true, &Ats_Conf_flip },
  { EE_AUTO_NOTCH_ADJUST, true, &Auto_Notch_Adjust },
  { EE_AAB_REAL_AIR, true, &use_AAB_RealAir },
  { EE_ATS_CONTACT, true, &use_AtsContact },
  { EE_ATS_DENGEN_ANGLE, true, &set_AtsDengenAngle },
  { EE_ME48_EB_EN, true, &use_Me48Eb },  // ★新規追加
};
constexpr uint8_t SETTING_COUNT = sizeof(SETTING_TABLE) / sizeof(SettingEntry);

// ================================================
// 汎用ヘルパー
// ================================================

inline bool bitOf(uint16_t value, uint8_t bit) {
  return (value >> bit) & 1;
}
inline bool inputFlip(InputFlipBit bit) {
  return bitOf(set_InputFlip, bit);
}
inline bool atsConfFlag(AtsConfBit bit) {
  return bitOf(Ats_Conf_flip, bit);
}

//自動帯設定(068)から取り出す各フラグ(loop() の先頭で更新。1周の間は値を固定する従来動作)
struct AutoAirFlags {
  bool use;         //自動帯使用可否
  bool bveEX;       //BveEX
  bool directSend;  //直接伝送
  bool ts185;       //TS185
};
AutoAirFlags autoAirFlags = { true, false, false, false };  //set_AutoAirBrake 初期値(1)に対応

void updateAutoAirFlags() {
  autoAirFlags.use = bitOf(set_AutoAirBrake, AAB_ENABLE);
  autoAirFlags.bveEX = bitOf(set_AutoAirBrake, AAB_BVEEX);
  autoAirFlags.directSend = bitOf(set_AutoAirBrake, AAB_DIRECT_SEND);
  autoAirFlags.ts185 = bitOf(set_AutoAirBrake, AAB_TS185);
}
inline bool use_AutoAirBrake() {
  return autoAirFlags.use;
}
inline bool use_BveEX() {
  return autoAirFlags.bveEX;
}
inline bool use_AutoAirBrakeDirectSend() {
  return autoAirFlags.directSend;
}
inline bool mode_TS185() {
  return autoAirFlags.ts185;
}


//MCP23S17の入力が有効(Lowアクティブ)か
inline bool pinActive(uint8_t pin) {
  return (~ioexp_1_AB >> pin) & 1;
}

//USBシリアルが送信可能か
inline bool usbWritable() {
  return Serial && Serial.availableForWrite() > 32;
}

//値が前回から変化したか(変化時は latch を更新)
inline bool changed(bool now, bool &latch) {
  if (now == latch) return false;
  latch = now;
  return true;
}

//チャタリング防止: input が limit 回連続で同じ値になったら output を更新
void debounce(bool input, bool &output, DebounceCounter &c, uint8_t limit) {
  if (input) {
    c.on++;
    c.off = 0;
  } else {
    c.off++;
    c.on = 0;
  }
  if (c.on >= limit) {
    c.on = limit;
    output = true;
  } else if (c.off >= limit) {
    c.off = limit;
    output = false;
  }
}

//ブレーキノッチ名称の設定
void setBrakeName(const char *name) {
  strncpy(notch_brk_name, name, sizeof(notch_brk_name) - 1);
  notch_brk_name[sizeof(notch_brk_name) - 1] = '\0';
}
void setBrakeNameNumbered(uint8_t n) {  //"B"+段数
  notch_brk_name[0] = 'B';
  itoa(n, notch_brk_name + 1, 10);
}

//Serial1へ送信(必要ならUSBへモニター出力)
void Serial1Print(const char *command, bool monitor) {
  Serial1.print(command);
  Serial1.print('\r');
  if (monitor && usbWritable()) {
    Serial.println(command);
  }
}
void Serial1Print(const String &command, bool monitor) {
  Serial1Print(command.c_str(), monitor);
}

//"TAG 0"/"TAG 1" 形式の電文を送信
void Serial1PrintFlag(const char *tag, bool value, bool monitor) {
  char buf[8];
  uint8_t n = 0;
  while (*tag && n < 4) {
    buf[n++] = *tag++;
  }
  buf[n++] = ' ';
  buf[n++] = value ? '1' : '0';
  buf[n] = '\0';
  Serial1Print(buf, monitor);
}

void Keyboard_Press_Release_BVE(bool pressed, uint8_t keycode) {
  if (modeBVE) {
    if (pressed) {
      Keyboard.press(keycode);
    } else {
      Keyboard.release(keycode);
    }
  }
}

void Keyboard_Write_Repeat(uint8_t keycode, uint8_t times) {
  while (times--) {
    Keyboard.write(keycode);
  }
}

// ================================================
// setup / loop
// ================================================

void setup() {
  pinMode(PIN_MCP3008_CS, OUTPUT);   //MCP3008
  pinMode(SS, OUTPUT);               //MCP23S17
  pinMode(PIN_EB_SW, INPUT_PULLUP);  //EBスイッチ
  pinMode(PIN_DOOR_OUT, OUTPUT);     //BVE_Door
  digitalWrite(PIN_DOOR_OUT, 0);
  pinMode(LED_BUILTIN, OUTPUT);  //BVE_Door_Monitor
  digitalWrite(LED_BUILTIN, LOW);

#ifdef SIMPLE
  pinMode(A0, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
#endif

  pinMode(PIN_ATS_REC, INPUT_PULLUP);   //ATS復帰
  pinMode(10, INPUT_PULLUP);            //予備
  pinMode(11, INPUT_PULLUP);            //予備SW1
  pinMode(PIN_BVE_MODE, INPUT_PULLUP);  //予備SW2(模型/BVE切替)

  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.setTimeout(10);
  Serial1.setTimeout(10);
#ifndef SIMPLE
  Wire.begin();
  Wire.setClock(400000);  // 高速モード
#endif

  Keyboard.begin();

  // SPIの初期化
  SPI.begin();
  CS_HIGH();  // SSピンをHIGH（非選択状態）にする

  // MCP23S17 の全ピン設定 (16ピン一括)
  mcpWrite16(MCP23S17_REG_IODIR, 0xFFFF);  //全入力
  mcpWrite16(MCP23S17_REG_GPPU, 0xFFFF);   //全プルアップ有効

  loadSettings();

  EEPROM.put(EE_ID, 'C');  //SELD"C"ontroller
  EEPROM.put(EE_VERSION, currentVersion);

  modeN = atsConfFlag(ATSCONF_MODE_SWITCH) ? digitalRead(PIN_BVE_MODE) : false;
  modeBVE = !modeN;

  //速度計テスト
  if (!inputFlip(FLIP_SKIP_METER_TEST)) {
    disp_SpeedMeter(set_SpeedLimit * 10);
    delay(1500);
  }
  disp_SpeedMeter(0);
  strbve.reserve(128);
}

void loop() {
  updateAutoAirFlags();      //自動帯設定フラグ更新
  read_USB();                //USB読込ルーチン
  read_Serial1();            //Serial1読込ルーチン
  read_ModeN();              //模型モード制御
  string_Analysis();         //電文解析
  read_IOexp();              //IOエキスパンダ読込ルーチン
  read_Light_Def();          //減光ライト読込ルーチン
  read_Light();              //前照灯読込ルーチン
  read_MC();                 //マスコンノッチ読込ルーチン
  read_Dir();                //マスコンレバーサ読込ルーチン
  read_Break();              //ブレーキハンドル読込ルーチン
  read_Break_Setting();      //ブレーキハンドル位置調整ルーチン
  read_Horn();               //ホーンペダル読込ルーチン
  read_Ats();                //ATS確認・警報持続読込ルーチン
  read_Panto();              //強制終了ルーチン
  read_EB();                 //EBスイッチ読込ルーチン
  read_B1();                 //B1電源制御
  keyboard_control(strbve);  //キーボード(HID)アウトプットルーチン
  comm_Pressure();           //圧力伝送制御(Sim)

  delay(10);
  if (!modeBVE) {
    delay(10);
  }
}

// ================================================
// EEPROM設定の読み書き
// ================================================

//初回起動時は現在値(工場出荷値)を書き込み、2回目以降はEEPROMから読み出す
void loadSettings() {
  int16_t initialized = 0;
  EEPROM.get(EE_INIT_FLAG, initialized);
  const bool firstRun = (initialized != 1);

  SettingEntry e;
  for (uint8_t i = 0; i < SETTING_COUNT; i++) {
    memcpy_P(&e, &SETTING_TABLE[i], sizeof(SettingEntry));
    if (firstRun) {
      EEPROM.put(e.addr, *e.var);
    } else {
      EEPROM.get(e.addr, *e.var);
    }
  }
  if (firstRun) {
    EEPROM.put(EE_INIT_FLAG, (int16_t)1);  //初回書き込みフラグセット
  }
}

bool findSetting(uint8_t addr, SettingEntry &out) {
  for (uint8_t i = 0; i < SETTING_COUNT; i++) {
    memcpy_P(&out, &SETTING_TABLE[i], sizeof(SettingEntry));
    if (out.addr == addr) return true;
  }
  return false;
}

//WRコマンドの値が不正か
bool isSettingRejected(uint8_t addr, uint16_t num) {
  switch (addr) {
    case EE_POT_N:
    case EE_POT_EB:
      return num > 1023;
    case EE_BRAKE_NOTCH_NUM:
      return num > 255;
    case EE_BRAKE_SAP_ANGLE:
      return num == 0 || num > set_BrakeEBAngle;
    case EE_BRAKE_EB_ANGLE:
      return num == 0 || num > set_BrakeFullAngle;
    case EE_BRAKE_FULL_ANGLE:
      return num == 0 || num > 255;
    case EE_SPEED_LIMIT:
      return num == 0;
    case EE_KAISEI_BRAKE:
    case EE_CURRENT_METER_MODE:
      return num > 1;
    case EE_VEHICLE_RESISTANCE:
      return num == 0;
    case EE_CHATTERING_FILTER:
      return num > 3;
    case EE_BRAKE_67DEG:
      return num == 0 || num > set_BrakeSAPAngle;
    case EE_BRAKE_10DEG:
      return num == 0 || num > set_Brake67DegAngl;
    case EE_AUTOAIR_1:
    case EE_AUTOAIR_2:
      return num < set_Brake67DegAngl || num > set_BrakeEBAngle;
    case EE_BP_DOWN_SPAN:
    case EE_BP_UP_SPAN:
      return num == 0 || num > 100;
    case EE_AUTO_AIR_BRAKE:
      return num > 255;
    case EE_AUTO_NOTCH_ADJUST:
      return num > 2;
    case EE_AAB_REAL_AIR:
    case EE_ATS_CONTACT:
    case EE_ME48_EB_EN:  // ★新規追加（0か1のみ許可）
      return num > 1;
    case EE_ATS_DENGEN_ANGLE:
      return num > 255;
    default:  //速度計補正、マスコンノッチ数、入力反転など(範囲チェックなし)
      return false;
  }
}

//EEPROM読書用
String rw_eeprom(uint8_t dev, uint16_t n, uint16_t &param, bool write, bool NGcondition) {
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

//WRコマンド処理
String set_Settings(uint8_t device, uint16_t num) {
  SettingEntry e;
  if (!findSetting(device, e) || !e.writable) {
    return "E0";
  }

  String s = rw_eeprom(device, num, *e.var, true, isSettingRejected(device, num));

  switch (device) {
    case EE_BRAKE_NOTCH_NUM:
      if (Auto_Notch_Adjust == 2) {
        flgFirstSend = true;
      }
      flg_set_BrakeNotchNum_changed = true;
      break;
    case EE_AUTO_NOTCH_ADJUST:
      //開始時または変更時にフラグをtrue　AutoNotch内でフラグ解除
      if (Auto_Notch_Adjust == 2) {
        flgFirstSend = true;
      }
      break;
    default:
      break;
  }

  //速度計調整時は、調整中の速度を表示
  if (device >= EE_SPEED_ADJUST_FIRST && device <= EE_SPEED_ADJUST_LAST) {
    bve_speed = ((device - 10) / 2) * 100;
  }
  return s;
}

void modeN_set() {
  set_BrakeNotchNum = 8;
  EEPROM.put(EE_BRAKE_NOTCH_NUM, set_BrakeNotchNum);
  EEPROM.get(EE_MC_NOTCH_CONSOLE, set_MCNotchNumConsole);
  set_MCNotchNumBVE = set_MCNotchNumConsole;
  EEPROM.put(EE_MC_NOTCH_BVE, set_MCNotchNumBVE);
}

// ================================================
// ハードウェア入出力
// ================================================

// 16bit一括読み取り用の軽量関数
uint16_t mcpRead16(uint8_t reg) {
  SPI.beginTransaction(mcp23S17settings);
  CS_LOW();
  SPI.transfer(MCP23S17_OP_READ);
  SPI.transfer(reg);            // 読み出し開始レジスタ
  uint8_t a = SPI.transfer(0);  // Port A
  uint8_t b = SPI.transfer(0);  // Port B
  CS_HIGH();
  SPI.endTransaction();
  return (uint16_t)a | ((uint16_t)b << 8);
}

// Flash節約用の軽量書き込み関数
void mcpWrite16(uint8_t reg, uint16_t data) {
  SPI.beginTransaction(mcp23S17settings);
  CS_LOW();
  SPI.transfer(MCP23S17_OP_WRITE);
  SPI.transfer(reg);          // 書き込み開始レジスタ
  SPI.transfer(data & 0xFF);  // Port A
  SPI.transfer(data >> 8);    // Port B (自動インクリメント)
  CS_HIGH();
  SPI.endTransaction();
}

// チャタリング防止付きの実行部分
void read_IOexp() {
  uint16_t current = mcpRead16(MCP23S17_REG_GPIO);

  // 3回連続一致を確認して確定(0xFFFFも含め全状態で確認)
  for (uint8_t i = 0; i < 2; i++) {
    if (mcpRead16(MCP23S17_REG_GPIO) != current) return;
  }

  ioexp_1_AB = current;
}

//MCP3008ADコンバータ読取
uint16_t adcRead(uint8_t ch) {  // 0 .. 7
#ifndef SIMPLE
  byte channelData = (ch + 8) << 4;
  SPI.beginTransaction(mcp3008settings);
  CS_BRK_LOW();
  SPI.transfer(0b00000001);                   // Start bit 1
  byte highByte = SPI.transfer(channelData);  // singleEnd
  byte lowByte = SPI.transfer(0x00);          // dummy
  CS_BRK_HIGH();
  SPI.endTransaction();
  return ((highByte & 0x03) << 8) + lowByte;
#else
  switch (ch) {
    case ADC_CH_BRAKE:
      return analogRead(A0);
    case ADC_CH_ATS_POS:
      return analogRead(A3);
    default:  //SIMPLE構成に存在しないチャンネルは「未入力(最大値)」を返す
      return 1023;
  }
#endif
}

//DAC1:速度計 (12bit)
void outputSpeedMeter(uint16_t value) {
#ifndef SIMPLE
  setVoltage(DAC1_ADDR, value);
#else
  analogWrite(10, value >> 4);
#endif
}

//DAC2:電圧計/電流計 (12bit)
void outputCurrentMeter(uint16_t value) {
#ifndef SIMPLE
  setVoltage(DAC2_ADDR, value);
#else
  analogWrite(11, value >> 4);
#endif
}

void setVoltage(uint8_t address, uint16_t voltage) {
  // 範囲外の値を防ぐ（0〜4095）
  if (voltage > 4095) voltage = 4095;

  Wire.beginTransmission(address);

  // Fast Mode (上位4bitに0x00) + 電圧上位4bit
  Wire.write((voltage >> 8) & 0x0F);
  // 電圧下位8bit
  Wire.write(voltage & 0xFF);

  Wire.endTransmission();
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
  outputSpeedMeter(volt);
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
    outputCurrentMeter(map(v, 0, 2000, 0, 4095));
    //電流計モードの時
  } else {
    //電流計は正のみ(現状)
    int curr = abs(current);
    //電流が電流上限を超えた時は電流上限に抑える
    if (curr > set_CurrentLimit) {
      curr = set_CurrentLimit;
    }
    outputCurrentMeter(map(curr, 0, set_CurrentLimit, 0, 4095));
  }
}

// ================================================
// マスコン・レバーサ
// ================================================

//自動帯使用時の方向切替マスク
void AutoAirMask() {
  //マスコンノッチが0、BVEモード時でBveEXモード以外とTS185以外のとき、
  //レバーサ0、レバーサマスクをtrue
  if (use_AutoAirBrake() && notch_mc <= 0 && modeBVE && !use_BveEX() && !mode_TS185()) {
    autoair_dir_mask = true;
    iDir = 0;
    //マスコンノッチが0以外の時はレバーサマスクをfalse
  } else {
    autoair_dir_mask = false;
  }
}

//抑速ノッチ(MC53)デコード: ioexp bit1～4 の反転値 → ノッチ
int8_t decodeDecelNotch(uint8_t decBit) {
  switch (decBit) {
    case 0b0001:
      return -3;
    case 0b0010:
      return -2;
    case 0b0101:
      return -4;
    case 0b0110:
      return -5;
    case 0b1000:
      return -1;
    default:
      return -1;
  }
}

//MCP23S17マスコンノッチ状態読込 (MC53抑速ブレーキ対応)
void read_MC(void) {
  if (Accident_Mode) {
    notch_mc = 0;
  } else if ((ioexp_1_AB >> PIN_MC_DEC) & 1) {
    //力行ノッチ
    if (inputFlip(FLIP_DEC_IS_EB) && deadman) {  //非常有効時
      deadman = false;
    }
    autoair_dir_mask = false;
    if (pinActive(PIN_MC_5)) {
      if (set_MCNotchNumConsole == 5) {
        notch_mc = set_MCNotchNumBVE;
      }
    } else if (pinActive(PIN_MC_4)) {
      if (set_MCNotchNumConsole == 4) {
        notch_mc = set_MCNotchNumBVE;
      } else if (set_MCNotchNumConsole == 5 && set_MCNotchNumBVE != 4) {
        notch_mc = 4;
      }
    } else if (pinActive(PIN_MC_3)) {
      notch_mc = 3;
    } else if (pinActive(PIN_MC_2)) {
      notch_mc = 2;
    } else if (pinActive(PIN_MC_1)) {
      notch_mc = 1;
    } else {
      notch_mc = 0;
      //マスコンノッチが0でブレーキが自動帯にあるときはレバーサ0、レバーサマスクをtrue、ただしBveEXモード時を除く
      if (brk_angl > set_BrakeSAPAngle && brk_angl < set_BrakeEBAngle) {
        AutoAirMask();
      }
    }
  } else if (inputFlip(FLIP_DEC_IS_EB) && !deadman) {  //非常有効時
    //マスコンデッドマン仮実装
    notch_mc = 0;
    if (set_BrakeNotchNum > 0) {
      notch_brk = set_BrakeNotchNum + 1;
    }
    setBrakeName("EB");
    autoair_dir_mask = false;
    deadman = true;
  } else if (!autoair_dir_mask) {  //抑速有効時(自動ブレーキ使用時は抑速使用不可)
    notch_mc = decodeDecelNotch((~ioexp_1_AB >> 1) & 0x0F);
  }

  notch_prefix = (notch_mc >= 0) ? 'P' : 'H';
}

//マスコンレバーサ読取
void read_Dir(void) {
  if (!autoair_dir_mask) {
    if (pinActive(PIN_MC_DIR_F) && !Accident_Mode) {
      iDir = 1;
      dir_prefix_N = 'L';
    } else if (pinActive(PIN_MC_DIR_B) && !Accident_Mode) {
      iDir = -1;
      dir_prefix_N = 'R';
    } else {
      iDir = 0;
      dir_prefix_N = 'N';
    }
  }
}

// ================================================
// ブレーキ弁
// ================================================

//ブレーキ弁ADC値 → 角度[°]
//N位置とEB位置の大小は問わない(ポテンショの+とGNDを逆に接続した場合は N > EB となる)
uint8_t adcToBrakeAngle(uint16_t adc) {
  if (set_POT_N == set_POT_EB) {  //未調整(同値)のときは0除算を避ける
    return 0;
  }
  const uint16_t potMin = (set_POT_N < set_POT_EB) ? set_POT_N : set_POT_EB;
  const uint16_t potMax = (set_POT_N < set_POT_EB) ? set_POT_EB : set_POT_N;
  if (adc < potMin) {
    adc = potMin;
  } else if (adc > potMax) {
    adc = potMax;
  }
  return static_cast<uint8_t>(map(adc, set_POT_N, set_POT_EB, 0, set_BrakeFullAngle));
}

//ポテンショ生データ表示モード: ADC値と角度
void debugPrintPot(uint16_t adc_raw) {
  if (!usbWritable()) return;
  char sPot[7];
  itoa(adc_raw + 10000, sPot, 10);  //"1xxxx" の先頭を '0' にして5桁ゼロ埋め
  sPot[0] = '0';
  char sDeg[6];
  itoa(brk_angl + 1000, sDeg, 10);  //"1xxx" の先頭を '0' にして4桁ゼロ埋め
  sDeg[0] = '0';
  Serial.print(" Pot1: ");
  Serial.print(sPot);
  Serial.print(" Deg: ");
  Serial.println(sDeg);
}

//ポテンショ生データ表示モード: ノッチとBP
void debugPrintNotch() {
  if (!usbWritable()) return;
  Serial.print(" Notch: ");
  Serial.print(notch_brk_name);
  Serial.print(" BP: ");
  Serial.print(BP_press);
  Serial.print(" BP_notch: ");
  Serial.print(autoair_notch_brk);
}

//直通帯の処理
void updateNotchSap() {
  EB_latch = false;  //非常ラッチ解除 V4.1.2.8位置移動
  if (set_BrakeNotchNum > 0) {
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
      if (mode_TS185() && notch_BrakeAAB) {
        Keyboard.write('K');
      }
      notch_brk = set_BrakeNotchNum;
      sap_notch_brk = set_BrakeNotchNum;
      autoair_dir_mask = false;
      notch_BrakeAAB = false;
    }
  } else {
    notch_brk = 0;
    sap_notch_brk = 0;
    autoair_dir_mask = false;
    notch_BrakeAAB = false;
  }
  setBrakeNameNumbered(notch_brk);
}

//自動帯の処理
void updateNotchAutoAir() {
  //自動帯非使用時は直通帯常用最大扱い
  if (!use_AutoAirBrake()) {
    notch_BrakeAAB = false;
    notch_brk = set_BrakeNotchNum;
    autoair_dir_mask = false;
    return;
  }

  //抜き取り・保ち位置
  if (brk_angl < set_BrakeAutoAir1Angle) {
    if (mode_TS185()) {
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
    setBrakeName("A0");
    AutoAirMask();
    return;
  }

  //自動帯減圧位置(弱:A1 / 強:A2)
  if (modeN || mode_POT) {
    setBrakeName(brk_angl < set_BrakeAutoAir2Angle ? "A1" : "A2");
  } else {
    if (mode_TS185()) {
      Keyboard.press('.');
    }
    //ブレーキ弁が自動帯にあり、マスコンノッチが0のときレバーサ0、レバーサマスクをtrueに、ただし模型モード以外またはBveEXモード以外のときを除く
    AutoAirMask();
  }
}

//非常位置の処理
void updateNotchEmergency() {
  notch_brk = set_BrakeNotchNum + 1;
  setBrakeName("EB");
  autoair_dir_mask = false;
  EB_latch = true;
}

//ブレーキ角度読取
void read_Break() {
  if (deadman) return;

  uint16_t adc_raw = adcRead(ADC_CH_BRAKE);
  brk_angl = adcToBrakeAngle(adc_raw);

  if (mode_POT) {
    debugPrintPot(adc_raw);
  }

  static uint8_t brk_angl_latch = brk_angl;
  static bool notch_BrakeAAB_latch = notch_BrakeAAB;
  //前回ブレーキ弁角度と今回ブレーキ弁角度がチャタリングフィルタ以上の場合
  if (abs(brk_angl - brk_angl_latch) >= set_ChatteringFilter) {
    if (brk_angl < set_BrakeSAPAngle) {
      updateNotchSap();
    } else if (brk_angl < set_BrakeEBAngle) {
      updateNotchAutoAir();
    } else {
      updateNotchEmergency();
    }

    if ((notch_BrakeAAB != notch_BrakeAAB_latch) && !mode_TS185() && use_BveEX()) {
      if (usbWritable()) {
        Serial.print("AAB ");
        Serial.println(notch_BrakeAAB, BIN);
      }
    }
    notch_BrakeAAB_latch = notch_BrakeAAB;
    brk_angl_latch = brk_angl;
  }

  if (use_AutoAirBrake()) {
    BP_Sim();
  }

  if (mode_POT) {
    debugPrintNotch();
  }
}

//ブレーキ位置調整ボタン1個分の処理。メッセージを作った場合 true
bool updatePotCalib(bool pressed, PotCalibState &st, unsigned long &iniMillis, uint16_t &target,
                    uint8_t eepromAddr, const char *label, String &msg) {
  if (!pressed) {
    st.mode = 0;
    return false;
  }

  uint16_t value = 0;
  if (st.mode == 0) {
    st.adj = adcRead(ADC_CH_BRAKE);
    st.mode = 1;
    iniMillis = millis();
    msg = label;
    msg += " = ";
    msg += EEPROM.get(eepromAddr, value);
    msg += " ADC = ";
    msg += st.adj;
    return true;
  } else if (st.mode == 1) {
    if (millis() - iniMillis > 3000) {
      st.mode = 2;
    }
    st.adj = (st.adj * 9 + adcRead(ADC_CH_BRAKE)) * 0.1;
  } else if (st.mode == 2) {
    st.mode = 0;
    target = st.adj;
    EEPROM.put(eepromAddr, target);
    msg = "NEW ";
    msg += label;
    msg += " = ";
    msg += target;
    return true;
  }
  return false;
}

//ブレーキ角度調整
void read_Break_Setting(void) {
  static PotCalibState stN = { 0, 0 };
  static PotCalibState stEB = { 0, 0 };
  static unsigned long iniMillis = 0;
  String s = "";
#ifndef SIMPLE
  bool btn_n = (adcRead(ADC_CH_BTN_N) < 512);
  bool btn_eb = (adcRead(ADC_CH_BTN_EB) < 512);
#else
  bool btn_n = !digitalRead(3);
  bool btn_eb = !digitalRead(2);
#endif
  updatePotCalib(btn_n, stN, iniMillis, set_POT_N, EE_POT_N, "POT_N", s);
  updatePotCalib(btn_eb, stEB, iniMillis, set_POT_EB, EE_POT_EB, "POT_EB", s);

  if (s.length() > 0 && usbWritable()) {
    Serial.println(s);
  }
}

// ================================================
// キーボード(HID)出力
// ================================================

//マスコンノッチが前回と異なるとき
void sendMasconKeys() {
  static int8_t notch_mc_latch = notch_mc;
  if (notch_mc != notch_mc_latch) {
    if (modeBVE) {
      uint8_t d = abs(notch_mc - notch_mc_latch);
      if (notch_mc >= 0 && notch_mc_latch >= 0) {
        //力行ノッチ 進段:Z 戻し:A
        Keyboard_Write_Repeat((notch_mc > notch_mc_latch) ? 'Z' : 'A', d);
      } else {
        //抑速ノッチ 進段:Q 戻し:A
        Keyboard_Write_Repeat((notch_mc < notch_mc_latch) ? 'Q' : 'A', d);
      }
      if (notch_mc >= 0 && notch_mc <= 3) {
        char msg[6] = "ATSM0";
        msg[4] = '0' + notch_mc;
        Serial1Print(msg, false);
      }
    } else {
      //modeN
      if (notch_brk == 0) {  //ブレーキ緩解
        char msg[8];
        msg[0] = notch_prefix;
        itoa(abs(notch_mc), msg + 1, 10);
        Serial1Print(msg, true);
      }
    }
  }
  notch_mc_latch = notch_mc;
}

//ブレーキノッチ(角度)が前回と異なるとき
void sendBrakeKeys(String &str) {
  if (notch_brk != notch_brk_latch || strcmp(notch_brk_name, notch_brk_name_latch) != 0) {
    //ブレーキノッチ(非常を除く) 戻し:','(TS185:'K') ブレーキ:'.'(TS185:'L')
    if (modeBVE && notch_brk < (set_BrakeNotchNum + 1)) {
      uint8_t d = abs(notch_brk - notch_brk_latch);
      if (notch_brk < notch_brk_latch) {
        Keyboard_Write_Repeat(mode_TS185() ? 'K' : ',', d);
      } else if (notch_brk > notch_brk_latch) {
        Keyboard_Write_Repeat(mode_TS185() ? 'L' : '.', d);
      }
    }

    //非常
    bool condition_EB_state = notch_brk == set_BrakeNotchNum + 1;
    static bool condition_EB_state_latch = false;
    if (changed(condition_EB_state, condition_EB_state_latch)) {
      if (condition_EB_state) {
        Keyboard.write('/');
      }
      if (transfer_EB_state) {
        str.setCharAt(MSG_EB, condition_EB_state + '0');
        send_Serial1(str);
      }
    }

    if (modeN) {
      Serial1Print(notch_brk_name, true);
    }
  }
  notch_brk_latch = notch_brk;
  strcpy(notch_brk_name_latch, notch_brk_name);
}

//レバーサが前回と異なるとき
void sendReverserKeys() {
  if (iDir != iDir_latch) {
    if (modeBVE) {
      //前進:↑ 後進:↓
      Keyboard_Write_Repeat((iDir > iDir_latch) ? KEY_UP_ARROW : KEY_DOWN_ARROW, abs(iDir - iDir_latch));
    } else {
      //modeN (電文は2文字: 名称+空白)
      char msg[3] = { dir_prefix_N, ' ', '\0' };
      Serial1Print(msg, true);
    }
  }
  iDir_latch = iDir;
}

//キーボード(HID)出力
void keyboard_control(String &str) {
  transfer_EB_state = atsConfFlag(ATSCONF_TRANSFER_EB);
  sendMasconKeys();
  sendBrakeKeys(str);
  sendReverserKeys();
}

void KeyPut(String &str) {
  if (str.length() > 4) {
    if (str.indexOf("0x") > 0) {
      char c_str[5];
      str.substring(4, 8).toCharArray(c_str, 5);
      Keyboard.write(static_cast<uint8_t>(strtol(c_str, NULL, 0)));
    } else {
      char c = str.charAt(4);
      Keyboard.write(c);
    }
  }
}

// ================================================
// スイッチ類
// ================================================

void read_Horn(void) {
  bool Horn_1 = pinActive(PIN_HORN_1) != inputFlip(FLIP_HORN1);  //警笛1
  static bool Horn_1_latch = Horn_1;
  if (changed(Horn_1, Horn_1_latch)) {
    if (!mode_TS185()) {
      Keyboard_Press_Release_BVE(Horn_1, KEY_RETURN);  //Enter:0xB0
    } else {
      Keyboard_Press_Release_BVE(Horn_1, KEY_BACKSPACE);  //BackSpace:0xB2
    }
    Serial1PrintFlag("HN1", Horn_1, false);  //モニターしない
  }

  bool Horn_2 = pinActive(PIN_HORN_2) != inputFlip(FLIP_HORN2);  //警笛2
  static bool Horn_2_latch = Horn_2;
  if (changed(Horn_2, Horn_2_latch)) {
    Keyboard_Press_Release_BVE(Horn_2, KEY_KP_PLUS);  //+:0xDF
    Serial1PrintFlag("HN2", Horn_2, false);           //モニターしない
  }
}

void read_Ats(void) {
  //ATS確認位置(誤作動防止のためチャタリング防止)
  static bool Ats_Pos = false;
  static bool Ats_Pos_latch = false;
  static DebounceCounter Ats_Pos_cnt = { 0, 0 };
  debounce(adcRead(ADC_CH_ATS_POS) < 1, Ats_Pos, Ats_Pos_cnt, 10);

  //ATS確認位置転送
  if (use_AtsContact) {
    if (Ats_Pos != Ats_Pos_latch) {
      Serial1PrintFlag("ATS", Ats_Pos, true);
    }
    Ats_Pos_latch = Ats_Pos;
  }

  //EB
  static DebounceCounter EB_cnt = { 0, 0 };
  static bool EB_Pos = false;
  debounce(adcRead(ADC_CH_EB) < 1, EB_Pos, EB_cnt, 10);

  if (EB_Pos) {
    notch_brk = set_BrakeNotchNum + 1;
    setBrakeName("EB");
    autoair_dir_mask = false;
    EB_latch = true;
  }

  //ATS警報持続
  bool Ats_Cont = pinActive(PIN_ATS_CONT) != inputFlip(FLIP_ATS_CONT);  //警報持続スイッチ
  static bool Ats_Cont_latch = Ats_Cont;
  if (changed(Ats_Cont, Ats_Cont_latch)) {
    Serial1PrintFlag("ACT", Ats_Cont, true);
    Keyboard_Press_Release_BVE(Ats_Cont, KEY_INSERT);  //Insert:0xD1
  }

  //ATS確認
  bool Ats_Conf = pinActive(PIN_ATS_CONF) != atsConfFlag(ATSCONF_ATS_CONF);  //ATS確認ボタン
  static bool Ats_Conf_latch = Ats_Conf;
  if (changed(Ats_Conf, Ats_Conf_latch)) {
    Serial1PrintFlag("ACF", Ats_Conf, true);
    Keyboard_Press_Release_BVE(Ats_Conf, 0x20);  //Space:Keyboard.hに定義なし
  }

  //ATS復帰
  bool Ats_Rec = !digitalRead(PIN_ATS_REC) != inputFlip(FLIP_ATS_REC);
  static bool Ats_Rec_latch = Ats_Rec;
  if (changed(Ats_Rec, Ats_Rec_latch)) {
    Keyboard_Press_Release_BVE(Ats_Rec, KEY_HOME);
  }

  //ME48非常抜取 / ME49電源判定
  bool ME48_EbPull = false;
  if (atsConfFlag(ATSCONF_ATSPWR_FROM_POT)) {
    if (use_Me48Eb != 0) {
      // WR 120 1 : ME48非常抜取モード（設定角度未満で有効）
      ME48_EbPull = (brk_angl < set_AtsDengenAngle);
    } else {
      // WR 120 0 : ME49モード（直通帯以下 または 自動帯A1以上で有効、間のA0帯で無効）
      ME48_EbPull = (brk_angl <= set_BrakeSAPAngle) || (brk_angl >= set_BrakeAutoAir1Angle);
    }
  }

  static bool ME48_EbPull_latch = ME48_EbPull;
  if (changed(ME48_EbPull, ME48_EbPull_latch)) {
    Serial1PrintFlag("ADN", ME48_EbPull, true);
  }
}

void read_Panto(void) {
  bool Panto = pinActive(PIN_PANTO);
  static bool Panto_latch = false;
  if (changed(Panto, Panto_latch)) {
    if (atsConfFlag(ATSCONF_PANTO_KEY_P)) {
      if (Panto) Keyboard.write('P');
    } else {
      Keyboard_Press_Release_BVE(Panto, KEY_LEFT_ALT);  //Alt:0x82
      Keyboard_Press_Release_BVE(Panto, KEY_F4);        //F40xC5
      str = strbve0;
      Serial1Print("PAN 0", false);
      outputCurrentMeter(0);
    }
  }
}

void read_Light_Def(void) {
  bool Light_Def = pinActive(PIN_LIGHT_DEF);
  static bool Light_Def_latch = Light_Def;
  if (changed(Light_Def, Light_Def_latch)) {
    Keyboard.write(KEY_PAGE_DOWN);
  }
}

void read_Light(void) {
  bool Light_On = pinActive(PIN_LIGHT_ON);
  static bool Light_On_latch = Light_On;
  if (changed(Light_On, Light_On_latch)) {
    Keyboard.write(KEY_PAGE_UP);
  }
}

void read_EB(void) {
  bool EB_SW = !digitalRead(PIN_EB_SW) != inputFlip(FLIP_EB);
  static bool EB_SW_latch = false;
  if (changed(EB_SW, EB_SW_latch)) {
    Keyboard_Press_Release_BVE(EB_SW, KEY_DELETE);  //Delete:0xD4
  }
}

void read_B1() {
  static bool B1_Dengen = false;
  static bool B1_Dengen_latch = B1_Dengen;
  if (atsConfFlag(ATSCONF_B1_FROM_POT)) {
    static uint8_t brk_angl_latch = brk_angl;
    //前回ブレーキ弁角度と今回ブレーキ弁角度がチャタリングフィルタ以上の場合
    if (abs(brk_angl - brk_angl_latch) >= set_ChatteringFilter || flg_set_BrakeNotchNum_changed) {
      flg_set_BrakeNotchNum_changed = false;
      if (set_BrakeNotchNum > 0) {
        if (use_AutoAirBrake()) {
          B1_Dengen = brk_angl <= set_BrakeSAPAngle;
        } else {
          B1_Dengen = brk_angl <= set_BrakeEBAngle;
        }
      } else {
        B1_Dengen = false;
      }
      brk_angl_latch = brk_angl;
    }
  } else {
    static DebounceCounter B1_cnt = { 0, 0 };
    debounce(adcRead(ADC_CH_B1) < 5, B1_Dengen, B1_cnt, 10);
  }

  if (B1_Dengen_latch != B1_Dengen) {
    Serial1PrintFlag("BDN", B1_Dengen && (set_BrakeNotchNum > 0), true);
    send_Serial1(strbve);
  }
  B1_Dengen_latch = B1_Dengen;
}

void read_ModeN() {
  bool tmp_mode_n = !digitalRead(PIN_BVE_MODE);  //BVE模型切替SW BVE:1 模型:0
  static bool tmp_mode_n_latch = tmp_mode_n;
  if (changed(tmp_mode_n, tmp_mode_n_latch)) {
    modeN = !tmp_mode_n;
    modeBVE = tmp_mode_n;
    if (modeN) {
      modeN_set();
    }
  }
}

// ================================================
// 自動ブレーキ帯シミュレーション
// ================================================

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

//BP,ER圧のシミュレーション
void simulateBpEr() {
  //BPの増減圧インターバルを設定(重なり全開位置より手前は増圧、以降は減圧)
  bp_span = (brk_angl < set_BrakeAutoAir1Angle) ? set_BrakeBPUpSpan : set_BrakeBPDownSpan;

  static unsigned long bp_millis = 0;
  static unsigned long er_millis = 0;
  if (EB_JR_move_E) {
    BP_ER_EB_Control(bp_millis, er_millis);
    return;
  }

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
  } else {
    BP_ER_EB_Control(bp_millis, er_millis);
  }
}

//BC圧をBP圧より生成
void calcBcFromBp() {
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
}

void BP_Sim() {
  if (!use_AAB_RealAir) {
    simulateBpEr();
    calcBcFromBp();
  } else {
    press_BC_Sim = BC_press;
  }

  //自動帯使用時に圧力をノッチに変換(BveEXを使用しない場合)
  //BveEX使用時、ノッチはB0固定
  if (!use_BveEX()) {
    //BC圧からブレーキノッチに変換
    if (EB_latch) {
      autoair_notch_brk = set_BrakeNotchNum + 1;
    } else {
      if (set_BrakeNotchNum > 0) {
        autoair_notch_brk = map(press_BC_Sim, 0, 440, 0, set_BrakeNotchNum);
      }
    }
  }

  //自動帯圧力優先シーケンス
  if (brk_angl <= set_Brake10DegAngl) {
    //N位置: 自動ブレーキの段数を採用
    notch_brk = autoair_notch_brk;
    if (!use_BveEX()) {
      strbve.setCharAt(MSG_LAMP_SAP, '0');  //直通ランプ消灯
    }
  } else if (brk_angl < set_BrakeSAPAngle) {
    //直通帯位置: 自動ブレーキ帯の段数が高いとき
    if (notch_brk < autoair_notch_brk) {
      notch_brk = autoair_notch_brk;
      if (!use_BveEX()) {
        strbve.setCharAt(MSG_LAMP_SAP, '1');  //直通ランプ点灯
      }
    }
  } else if (brk_angl < set_BrakeEBAngle && !use_BveEX()) {
    //自動帯(BveEX非使用時のとき): 自動ブレーキ帯の段数が高いとき
    if (notch_brk < autoair_notch_brk) {
      notch_brk = autoair_notch_brk;
    }
  }
}

//圧力値が変動時、下位および上位に伝送
void comm_Pressure() {
  static uint16_t BC_press_latch = BC_press;
  static uint16_t BP_press_latch = BP_press;
  static uint16_t ER_press_latch = ER_press;
  if (BC_press != BC_press_latch || BP_press != BP_press_latch || ER_press != ER_press_latch) {
    send_Serial1(strbve);
    //V4.2.1.1追加 実際のエアー圧を使用しない場合(TS185でなく、modeBVEのとき)はBCの変化時にPCへ伝送
    if (use_AutoAirBrake() && !use_AAB_RealAir && !mode_TS185() && modeBVE) {
      if (BC_press != BC_press_latch) {
        if (usbWritable()) {
          Serial.print("BC ");
          Serial.println(BC_press);
        }
      }
    }
    BC_press_latch = BC_press;
    BP_press_latch = BP_press;
    ER_press_latch = ER_press;
  }
}

// ================================================
// シリアル通信・電文処理
// ================================================

//直前の読み取りでCRの次にLFが続く場合(CRLF)、1文字読み飛ばす
void skipLF(Stream &port) {
  if (port.peek() == '\n') {
    port.read();
  }
}

void read_Serial1() {
  if (Serial1.available() > 0) {
    str = Serial1.readStringUntil('\r');
    str.trim();
    skipLF(Serial1);
  }
}

void read_USB() {
  if (Serial.available()) {
    str = Serial.readStringUntil('\r');
  }
  str.trim();
  skipLF(Serial);
}

//3桁の数値を電文の指定位置へ書き込む
void setStringAt(uint8_t startIndex, String &str, uint16_t value) {
  uint16_t d[3];
  d[0] = value / 100 + 0x30;
  d[1] = value / 10 % 10 + 0x30;
  d[2] = value % 10 + 0x30;

  if (str.length() >= MSG_MIN_LEN_PRESSURE) {
    for (int i = 0; i < 3; i++) {
      str.setCharAt(startIndex + i, char(d[i]));
    }
  }
}

//自動帯シミュレーション時、BC、BP、SAPの演算結果を電文へ反映する(直接転送モード以外)
void applyAutoAirPressure(String &str) {
  //"0000/1/ 00000/100000/0000000000000000000001/NN0B08M780C440E490S440P490/";
  static bool sap_auto_mask = false;
  BC_press = press_BC_Sim;  //BCの演算結果を代入

  //現在BC圧よりもBVEからのBC圧が高い場合はBVEを優先し
  //逆の場合で直通帯のときBVEの圧力を優先する
  if (!sap_auto_mask && (bve_BC_press > BC_press || brk_angl < set_BrakeSAPAngle)) {
    BC_press = bve_BC_press;
  }

  setStringAt(MSG_BC, str, BC_press);
  setStringAt(MSG_BP, str, BP_press);
  setStringAt(MSG_ER, str, ER_press);

  //SAP
  static uint16_t sap_press_latch = bve_SAP_press;
  static unsigned long sap_millis = millis();  //※更新されないため、起動20ms後以降は常に条件成立(従来動作)
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
        if (set_BrakeNotchNum > 0) {
          uint16_t tmp_sap_press = map(sap_notch_brk, 0, set_BrakeNotchNum, 0, 440);
          if (tmp_sap_press > bve_SAP_press) {
            sap_auto_mask = false;
          }
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
  setStringAt(MSG_SAP, str, sap_press_latch);
}

void send_Serial1(String &str) {
  if (use_AutoAirBrake()) {
    //自動帯有効で自動帯投入時、直通ランプと電制を無効とする
    if (brk_angl > set_BrakeSAPAngle) {
      if (str.length() > MSG_LAMP_DYN) {
        if (!use_BveEX()) {
          str.setCharAt(MSG_LAMP_SAP, '0');  //直通ランプ
          str.setCharAt(MSG_LAMP_DYN, '0');  //電制ランプ
        }
      }
    }
    if (!use_AAB_RealAir && !use_AutoAirBrakeDirectSend()) {
      applyAutoAirPressure(str);
    }
  }
  //非常
  if (transfer_EB_state && notch_brk == set_BrakeNotchNum + 1) {
    str.setCharAt(MSG_EB, '1');
  }

  Serial1.print(str);
  Serial1.print('\r');
}

//ノッチ自動調整 BVEからの入力に対して現在状態の比較を行い、ラッチに格納することでメインループで自動的に修正する
void AutoNotch(String &str) {
  if (str.length() > 49 && (Auto_Notch_Adjust == 1 || (Auto_Notch_Adjust == 2 && flgFirstSend)) && !autoair_dir_mask) {
    if (str.charAt(MSG_NOTCH_MARK) == 'B') {
      switch (str.charAt(MSG_DIR)) {
        case 'F':
          iDir_latch = 1;
          break;
        case 'B':
          iDir_latch = -1;
          break;
        default:
          iDir_latch = 0;
      }
      char Buf[4];
      Buf[0] = str.charAt(MSG_NOTCH_BRK);
      Buf[1] = str.charAt(MSG_NOTCH_BRK + 1);
      Buf[2] = str.charAt(MSG_NOTCH_BRK + 2);
      Buf[3] = '\0';
      notch_brk_latch = strtol(Buf, NULL, 16);  //16進数→10進数に変換
    }
    flgFirstSend = false;
  }
}

//"WR DDD NNNNN": 設定書込
void cmdWrite(const String &str) {
  if (str.length() > 7) {
    uint8_t device = str.substring(3, 6).toInt();
    uint16_t num = str.substring(7, 12).toInt();
    if (device <= 100) {
      Serial1Print(set_Settings(device, num), true);
    } else if (device == 120) {
      set_Settings(device, num);
    }
  }
}

//"RD DDD": 設定読出
void cmdRead(const String &str) {
  if (str.length() > 5) {
    uint8_t device = str.substring(3, 6).toInt();
    if (device <= 100) {
      uint16_t value = 0;
      Serial1Print(rw_eeprom(device, 0, value, false, false), true);
    } 
  }
}

//"MD ..."(模型モード/ポテンショ読取モード)
void cmdMode(const String &str) {
  String s = "";
  if (str.indexOf("N   ") > 0) {
    //模型モード
    if (str.length() > 7) {
      int16_t num = str.substring(7, 12).toInt();
      s = "OK N   ";
      if (num) {
        modeN = true;
        modeBVE = false;
        modeN_set();
        s += "ON";
      } else {
        modeN = false;
        modeBVE = true;
        s += "OFF";
      }
    }
  } else if (str.indexOf("POT ") > 0) {
    //ポテンショ読取モード
    if (str.length() > 7) {
      int16_t num = str.substring(7, 12).toInt();
      s = "OK POT ";
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
  if (usbWritable()) {
    Serial.println(s);
  }
}

//通常電文：速度、戸閉、電流、BC、SAP、非常を抽出
void parseBveFrame(const String &str) {
  if (str.length() > 11) {
    strbve = str;
    bve_speed = strbve.substring(MSG_SPEED, MSG_SPEED + 4).toInt();

    //戸閉灯指示
    bool doorClosed = (strbve.charAt(MSG_DOOR) == '1');
    digitalWrite(PIN_DOOR_OUT, !doorClosed);
    digitalWrite(LED_BUILTIN, !doorClosed);

    bve_current = strbve.substring(MSG_CURRENT, MSG_CURRENT + 5).toInt();

    //自動ノッチ合わせ機構
    AutoNotch(strbve);

    //BC抽出
    if (strbve.length() >= MSG_MIN_LEN_PRESSURE) {
      bve_BC_press = strbve.substring(MSG_BC, MSG_BC + 3).toInt();
      bve_SAP_press = strbve.substring(MSG_SAP, MSG_SAP + 3).toInt();
      //非常ブレーキ抽出
      EB_JR_move_E = strbve.substring(MSG_EB, MSG_EB + 1).toInt();
    }
  }
}

//電文解析
void string_Analysis() {
  if (str == str_latch) return;

  if (str.startsWith("WR ")) {
    cmdWrite(str);  //設定モード
  } else if (str.startsWith("RD ")) {
    cmdRead(str);
  } else if (str.startsWith("BC ")) {
    if (use_AutoAirBrake() && !mode_TS185() && use_AAB_RealAir) {
      BC_press = str.substring(3, 6).toInt();
      if (usbWritable()) {
        Serial.println(str);
      }
    }
  } else if (str.startsWith("KEY")) {
    KeyPut(str);  //キーボードモード 4.2.1.6追加
  } else if (str.startsWith("PAN")) {
    if (str.length() > 4) Pan_Mode = str.charAt(4) == '1';  //PANモード 4.2.1.6追加
  } else if (str.startsWith("ACD")) {
    if (str.length() > 4) Accident_Mode = str.charAt(4) == '1';  //事故モード 4.2.2.2追加 マスコンレバーサ無効
  } else if (str.startsWith("FV") || str.startsWith("OK") || str.startsWith("E1") || str.startsWith("SW")) {
    //圧力モニタ、交直切替4.2.3.0追加
    if (str.length() > 4 && usbWritable()) {
      Serial.println(str);
    }
  } else if (str.startsWith("MD ")) {
    cmdMode(str);
  } else {
    parseBveFrame(str);
  }

  //速度計表示補正 調整時表示のためここ
  disp_SpeedMeter(bve_speed);

  //電流計
  disp_CurrentMeter(bve_current);

  //Serial1転送
  send_Serial1(str);
  str_latch = str;
}
