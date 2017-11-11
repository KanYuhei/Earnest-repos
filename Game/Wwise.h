
#ifndef _WWISE_H_
#define _WWISE_H_

#include <windows.h>
#include <unordered_map>
#include <assert.h>

// Release時に定義させる
#ifndef _DEBUG
#define AK_OPTIMIZED
#endif

// Wwise(サウンドエンジン)を使用するために必要
#include <AK/SoundEngine/Common/AkSoundEngine.h>		// サウンドエンジン
#include <AK/IBytes.h>

// #include <AK/Plugin/AkCompressorFXFactory.h>			// プラグイン

#include <AK/SoundEngine/Platforms/Windows/AkTypes.h>	// タイプ

#include "Wwise_IDs.h"									// イベントのID

using namespace AK;
using namespace AK::SoundEngine;

// ======== ======== ======== ======== ======== ======== ======== ========
// 構造体
// -------- -------- -------- -------- -------- -------- -------- --------
// ゲームオブジェクト
struct WWISE_GAMEOBJ
{
	D3DXVECTOR3 position, rot;		// 座標, 回転角
	D3DXVECTOR3 vel;			// 速度(ドップラー効果で必要？)
};

// 前方宣言
class CAkFilePackageLowLevelIOBlocking;

// ======== ======== ======== ======== ======== ======== ======== ========
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
// 
// 使い方！
// ① CreateNoListenerGameObj()でゲームオブジェクトを作成！
// ② SetEvent()でイベントを再生！
// ③ Stop()でイベントの停止！
//
// ・ Init(), Uninit(), Update() はSceneManager.cppに書いて下さい
// ・ リスナー(m_pMainListener)は、プレイヤー等の位置や向きに合わせて毎フレーム設定し直して下さい
// 
class Wwise
{
public:
	Wwise(){}
	~Wwise(){}

	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新

	// ゲームオブジェクトの生成
	WWISE_GAMEOBJ *CreateGameObj(D3DXVECTOR3 position, D3DXVECTOR3 rot, char *name);				// Listener付き
	WWISE_GAMEOBJ *CreateNoListenerGameObj(D3DXVECTOR3 position, D3DXVECTOR3 rot, char *name);	// Listener無し
	void DeleteGameObj(std::string gameSceneName);

	// イベント送信
	void MainListenerGameObjEvent(int eventID);				// メインリスナー(プレイヤー)にイベントを送信
	void SetEvent(int eventID, D3DXVECTOR3 *loc = 0);		// ワールド座標の指定位置にIDでイベントを送信
	void SetEvent(char *name, D3DXVECTOR3 *loc = 0);		// ワールド座標の指定位置に名前でイベントを送信
	void SetEvent(int eventID, std::string GameSceneName);	// 指定ゲームオブジェクトにIDでイベントを送信
	void SetEvent(char *name, std::string GameSceneName);	// 指定ゲームオブジェクトに名前でイベントを送信

	// ゲームオブジェクトの再生を停止
	void Stop(std::string name);
	void StopMainListener();

	// イベントの状態を変更
	void SetState(int stateGroup, int stateID);			// ステートの設定
	void SetGameParam(int rtpcID, float value);			// ゲームパラメーター(RTPC)の設定

	// マテリアル
	int GetMaterialId(char *name);						// 名前からマテリアルIDの取得
//	void SetMaterial(void *gameScene, int materialid);	// ゲームオブジェクトにマテリアルIDを設定

	// メインリスナー系
	void SetPos(const D3DXVECTOR3 &position) { m_pMainListener->position = position; }		// 座標
	void SetRot(const D3DXVECTOR3 &rot) { m_pMainListener->rot = rot; }		// 回転
	void SetVel(const D3DXVECTOR3 &vel) { m_pMainListener->vel = vel; }		// 速度

	// 各ゲームオブジェクト
	void SetPos(const std::string &name, const D3DXVECTOR3 &position) {			// 座標
		if (m_umapGameObj.count(name) < 1) { assert(!"引数の名前が不正"); }
		m_umapGameObj[name]->position = position;
	}
	void SetRot(const std::string &name, const D3DXVECTOR3 &rot) {			// 回転
		if (m_umapGameObj.count(name) < 1) { assert(!"引数の名前が不正"); }
		m_umapGameObj[name]->rot = rot;
	}
	void SetVel(const std::string &name, const D3DXVECTOR3 &vel) {			// 速度
		if (m_umapGameObj.count(name) < 1) { assert(!"引数の名前が不正"); }
		m_umapGameObj[name]->vel = vel;
	}

	// キャプチャリストへエラーメッセージを送る
	void PostMessage(char *szMessage);

	// D3DXVECTOR3とAkVectorの変換
	static void VecToAkVector(const D3DXVECTOR3 &vec, AkVector &vector);

private:
	// ゲームオブジェクトの登録(直接は呼び出さない)
	void registPtr(void *gameScene, char *name = NULL);
	void unregistPtr(void *gameScene);

	// 低レベルI/O
	static CAkFilePackageLowLevelIOBlocking m_lowLevelIO;			

	// ゲームオブジェクト
	std::unordered_map<std::string, WWISE_GAMEOBJ*> m_umapGameObj;	// ゲームオブジェクト
	WWISE_GAMEOBJ *m_pMainListener;									// メインリスナーへのポインタ

	// ダミーコールバック
	static void dummyCallback(AkCallbackType type, AkCallbackInfo* pCallbackInfo);
	static std::list<int> dummies;
	static CAkLock dummiesLock;

	/*	void soundvol(int vol);
	void musicvol(int vol);
	void voicevol(int vol);*/				
};

#endif
