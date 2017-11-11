
// -------- -------- -------- -------- -------- -------- -------- --------
// インクルード
// -------- -------- -------- -------- -------- -------- -------- --------

#include "main.h"
#include "Wwise.h"

// メモリマネージャに必要
#include <AK/SoundEngine/Common/AkMemoryMgr.h>				// メモリマネージャ
#include <AK/SoundEngine/Common/AkModule.h>					// デフォルトメモリ, ストリームマネージャ

// ストリーミング再生に必要
#include <AK/SoundEngine/Common/IAkStreamMgr.h>				// ストリームマネージャ
#include <AK/Tools/Common/AkPlatformFuncs.h>				// スレッド
#include "Wwise_Win32/AkFilePackageLowLevelIOBlocking.h"	// 低レベルI/O

// ミュージックエンジンに必要
#include <AK/MusicEngine/Common/AkMusicEngine.h>			// ミュージックエンジン

// Wwiseとゲームの間の通信に必要 - リリースバージョンでは不要
#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

// ======== ======== ======== ======== ======== ======== ======== ========
//	メモリフック
// -------- -------- -------- -------- -------- -------- -------- --------
// カスタムalloc/free関数。
// これらはAkMemoryMgr.hの "extern"として宣言されている
// -------- -------- -------- -------- -------- -------- -------- --------
namespace AK
{
#ifdef WIN32
	void *AllocHook(size_t in_size) { return malloc(in_size); }

	void FreeHook(void *in_ptr) { free(in_ptr); }

	void *VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}

	void VirtualFreeHook(void *in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

#ifdef AK_MOTION
#include <AK/MotionEngine/Common/AkMotionEngine.h>	// Motion Engine (required only for playback of Motion objects)
#endif

static const AkGameObjectID LISTENER_ID = 10001;

// #include <AK/Plugin/AllPluginsRegistrationHelpers.h>

// ======== ======== ======== ======== ======== ======== ======== ========
// 名前省略
using namespace std;

// ======== ======== ======== ======== ======== ======== ======== ========
// マクロ
#define CHECK_SOUND_ENGINE if( !SoundEngine::IsInitialized() ) return;
#define CHECK_SOUND_ENGINE0 if( !SoundEngine::IsInitialized() ) return 0;
#define CHECK_NULLPTR(p) { if(p == nullptr) return; }

// ======== ======== ======== ======== ======== ======== ======== ========
// 静的メンバ
CAkFilePackageLowLevelIOBlocking Wwise::m_lowLevelIO;
std::list<int> Wwise::dummies;
CAkLock Wwise::dummiesLock;

// ======== ======== ======== ======== ======== ======== ======== ========
// 初期化
void Wwise::Init()
{
	// -------- -------- -------- --------
	// メモリマネージャ
	//
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (MemoryMgr::Init(&memSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the memory manager.");
		return;
	}

	// -------- -------- -------- --------
	// ストリーミングマネージャ
	//
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	if (!StreamMgr::Create(stmSettings))
	{
		AKASSERT(!"Could not create the Stream SceneManager");
		return;
	}

	// -------- -------- -------- --------
	// ストリーミングデバイス
	//
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (m_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the streaming device and Low-Level I/O system");
		return;
	}

	// -------- -------- -------- --------
	// サウンドエンジン
	//
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	// Setting pool sizes for this game. Here, allow for user content; every game should determine its own optimal values.
	initSettings.uDefaultPoolSize = 2 * 1024 * 1024;
	platformInitSettings.uLEngineDefaultPoolSize = 4 * 1024 * 1024;

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return;
	}

	// -------- -------- -------- --------
	// ミュージックエンジン
	//
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return;
	}

	/*
	// -------- -------- -------- --------
	// プラグイン
	//
	/// Note: This a convenience method for rapid prototyping. 
	/// To reduce executable code size register/link only the plug-ins required by your game 
	if (AK::SoundEngine::RegisterAllPlugins() != AK_Success)
	{
		AKASSERT(!"Error while registering plug-ins");
		return;
	}
	*/

	// -------- -------- -------- --------
	// 通信の初期化
	//
#ifndef AK_OPTIMIZED
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return;
	}
#endif // AK_OPTIMIZED

	// -------- -------- -------- --------
	// 低レベルI/O
	//
	// バンクスクリプトのパス
	m_lowLevelIO.SetBasePath(AKTEXT("data/wwisePrj_InitialT/GeneratedSoundBanks/Windows/"));

	// ゲームの現在の言語の指定
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	// -------- -------- -------- --------
	// バンクの読み込み
	//
	AkBankID bankID;
	AKRESULT retValue;

	// 初期化バンク
	retValue = SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	// メインバンク
	retValue = SoundEngine::LoadBank("main.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	/*
	// ボリュームの初期化
	musicvol(255);
	soundvol(255);
	voicevol(255);
	*/

	/*
	// Wwiseモーション
#ifdef AK_MOTION
	MotionEngine::AddPlayerMotionDevice(0, AKCOMPANYID_AUDIOKINETIC, AKMOTIONDEVICEID_RUMBLE, nullptr);
	MotionEngine::SetPlayerListener(0, 0);
	SoundEngine::SetListenerPipeline(0, true, true);
#endif
	*/

	// メインリスナーの設定
	m_pMainListener = Wwise::CreateGameObj(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), "MainListener");	// ゲームオブジェクトとして登録
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 終了
void Wwise::Uninit()
{
	// ゲームオブジェクトの消去
	for (auto gameObject : m_umapGameObj)
	{
		if (gameObject.second != nullptr)
		{
			Wwise::unregistPtr(gameObject.second);	// ゲームオブジェクトの登録取り消し
			delete gameObject.second;
			gameObject.second = nullptr;
		}
	}
	m_umapGameObj.clear();

	// 通信
#ifndef AK_OPTIMIZED
	Comm::Term();
#endif // AK_OPTIMIZED

	// ミュージックエンジン
	MusicEngine::Term();

	// サウンドエンジン
	SoundEngine::Term();

	// 低レベル入出力
	m_lowLevelIO.Term();
	if (IAkStreamMgr::Get())
		IAkStreamMgr::Get()->Destroy();

	// メモリマネージャ
	MemoryMgr::Term();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 更新
void Wwise::Update()
{
	CHECK_SOUND_ENGINE;

	// ======== ======== ======== ========
	// リスナーの設定
	AkSoundPosition MainListener;

	float fRadPitch = m_pMainListener->rot.z;
	float fCosPitch = cos(fRadPitch);
	float fSinPitch = sin(fRadPitch);

	float fRadYaw = m_pMainListener->rot.y - D3DX_PI * 0.5f;
	float fCosYaw = cos(fRadYaw);
	float fSinYaw = sin(fRadYaw);

	// 前方向ベクトル
	AkVector front;
	front.X = fCosYaw * fCosPitch;
	front.Y = fSinYaw * fCosPitch;
	front.Z = fSinPitch;

	// 上方向ベクトル
	AkVector top;
	top.X = -fCosYaw * fSinPitch;
	top.Y = -fSinYaw * fSinPitch;
	top.Z = fCosPitch;

	// 前方向、上方向ベクトルの設定
	MainListener.SetOrientation(front, top);

	// 座標設定
	AkVector position;
	VecToAkVector(m_pMainListener->position, position);
	MainListener.SetPosition(position);

	// リスナーの設定
	SoundEngine::SetPosition(LISTENER_ID, MainListener);

	// ======== ======== ======== ========
	// 各ゲームオブジェクトの設定
	AkSoundPosition soundPos;
	for (auto gameObject : m_umapGameObj)
	{
		fRadPitch = gameObject.second->rot.z;
		fCosPitch = cos(fRadPitch);
		fRadYaw = gameObject.second->rot.y - D3DX_PI * 0.5f;

		// 前方向ベクトル
		front.X = (float)(cos(fRadYaw) * fCosPitch);
		front.Y = (float)(sin(fRadYaw) * fCosPitch);
		front.Z = (float)(sin(fRadPitch));

		// 上方向ベクトル
		top = { 0, 1.f, 0 };
		soundPos.SetOrientation(front, top);

		// 座標設定
		VecToAkVector(gameObject.second->position, position);
		soundPos.SetPosition(position);

		// ゲームオブジェクトの設定
		SoundEngine::SetPosition((AkGameObjectID)gameObject.second, soundPos);
	}

	// 設定されたイベントの実行
	SoundEngine::RenderAudio();
};

// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトを登録
WWISE_GAMEOBJ *Wwise::CreateGameObj(D3DXVECTOR3 position, D3DXVECTOR3 rot, char *pName)
{
	WWISE_GAMEOBJ *pGameObject = new WWISE_GAMEOBJ;
	pGameObject->position = position;							// 座標
	pGameObject->rot = rot;							// 角度
	pGameObject->vel = D3DXVECTOR3(0.f, 0.f, 0.f);	// 速度(ドップラー用)
	
	// エンジンへ登録
	Wwise::registPtr(pGameObject, pName);

	// リストへ追加
	if (m_umapGameObj.count(pName) >= 1)
	{
		delete m_umapGameObj[pName];
		m_umapGameObj[pName] = nullptr;
	}
	m_umapGameObj[pName] = pGameObject;
	AK::SoundEngine::RegisterGameObj(LISTENER_ID, "Listener (Default)");
	AK::SoundEngine::SetDefaultListeners(&LISTENER_ID, 1);

	return pGameObject;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// リスナー無しゲームオブジェクトを登録
WWISE_GAMEOBJ *Wwise::CreateNoListenerGameObj(D3DXVECTOR3 position, D3DXVECTOR3 rot, char *pName)
{
	WWISE_GAMEOBJ *pGameObject = new WWISE_GAMEOBJ;
	pGameObject->position = position;							// 座標
	pGameObject->rot = rot;							// 角度
	pGameObject->vel = D3DXVECTOR3(0.f, 0.f, 0.f);	// 速度(ドップラー用)

	// エンジンへ登録
	Wwise::registPtr(pGameObject, pName);

	// リストへ追加
	if (m_umapGameObj.count(pName) >= 1)
	{
		delete m_umapGameObj[pName];
		m_umapGameObj[pName] = nullptr;
	}
	m_umapGameObj[pName] = pGameObject;

	return pGameObject;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトの消去
void Wwise::DeleteGameObj(std::string gameObjectName)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(gameObjectName) == 0) { return; }

	// 指定ゲームオブジェクトの消去
	Wwise::unregistPtr(m_umapGameObj[gameObjectName]);
	delete m_umapGameObj[gameObjectName];
	m_umapGameObj[gameObjectName] = nullptr;
	m_umapGameObj.erase(gameObjectName);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトの登録(エンジンに登録)
void Wwise::registPtr(void *gameObject, char *pName)
{
	CHECK_SOUND_ENGINE;

	if (pName == nullptr)
	{
		static int cEntity = 0;
		char name[256];
		sprintf(name, "Entity #%i", (int) ++cEntity);

		SoundEngine::RegisterGameObj((AkGameObjectID)gameObject, name);
	}
	else
	{
		SoundEngine::RegisterGameObj((AkGameObjectID)gameObject, pName);
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトの消去(エンジンから削除)
void Wwise::unregistPtr(void *gameObject)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::UnregisterGameObj((AkGameObjectID)gameObject);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 
// イベントの設定
// 
// -------- -------- -------- -------- -------- -------- -------- --------
// メインリスナーにイベントを送信
void Wwise::MainListenerGameObjEvent(int eventID)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::PostEvent(eventID, (AkGameObjectID)m_pMainListener);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ID, ワールド座標 (ワールド座標未設定の場合は自動的にプレイヤーへのイベント設定となる)
void Wwise::SetEvent(int eventID, D3DXVECTOR3 *loc)
{
	CHECK_SOUND_ENGINE;

	if (loc == nullptr)
		SoundEngine::PostEvent(eventID, (AkGameObjectID)m_pMainListener);
	else
	{
		// ダミーを使用
		dummiesLock.Lock();			// ロック
		dummies.push_back(0);			// ダミーを登録
		int &dummy = dummies.back();	// 取得
		dummiesLock.Unlock();			// アンロック

		// ダミーをイベントへ登録
		SoundEngine::RegisterGameObj((AkGameObjectID)&dummy, "PlayAtLocation");

		// ダミーの姿勢と座標を設定
		AkVector front = { 0, 0, 1.f };
		AkVector top = { 0, 1.f, 0 };
		AkVector position;
		VecToAkVector(*loc, position);

		// 座標をダミーに登録
		AkSoundPosition soundPos;
		soundPos.Set(position, front, top);
		SoundEngine::SetPosition((AkGameObjectID)&dummy, soundPos);

		// イベントの送信
		AkPlayingID id = SoundEngine::PostEvent(eventID, (AkGameObjectID)&dummy, AK_EndOfEvent, dummyCallback);
		if (id != AK_INVALID_PLAYING_ID)
		{
			// IDが無効でなければ登録
			dummy = id;
		}
		else
		{
			// IDが無効であれば、コンテナからダミーを取り出す
			SoundEngine::UnregisterGameObj((AkGameObjectID)&dummy);
			dummiesLock.Lock();		// ロック
			dummies.pop_back();			// 取り出し
			dummiesLock.Unlock();		// アンロック
		}
	}
};

// ======== ======== ======== ======== ======== ======== ======== ========
// イベント名, ワールド座標
void Wwise::SetEvent(char *pName, D3DXVECTOR3 *loc)
{
	CHECK_SOUND_ENGINE;

	AkUniqueID eventID = SoundEngine::GetIDFromString(pName);
	if (eventID != AK_INVALID_UNIQUE_ID)
	{
		Wwise::SetEvent((int)eventID, loc);
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ID, ゲームオブジェクト名
void Wwise::SetEvent(int eventID, std::string GameObjectName)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(GameObjectName) == 0) { return; }

	SoundEngine::PostEvent(eventID, (AkGameObjectID)m_umapGameObj[GameObjectName]);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// イベント名, ゲームオブジェクト名
void Wwise::SetEvent(char *pName, std::string GameObjectName)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(GameObjectName) == 0) { return; }

	SoundEngine::PostEvent(pName, (AkGameObjectID)m_umapGameObj[GameObjectName]);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 再生中イベントの停止
void Wwise::Stop(std::string name)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(name) == 0) { assert(!"引数の名前が不正"); }

	SoundEngine::StopAll((AkGameObjectID)m_umapGameObj[name]);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// メインリスナーの再生中イベントの停止
void Wwise::StopMainListener()
{
	CHECK_SOUND_ENGINE;
	if (m_pMainListener == nullptr) { return; }

	SoundEngine::StopAll((AkGameObjectID)m_pMainListener);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ステートの設定
void Wwise::SetState(int stateGroup, int stateID)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetState(stateGroup, stateID);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// RTPCの設定
void Wwise::SetGameParam(int rtpcID, float value)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(rtpcID, value);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// wwiseキャプチャーリストへエラーメッセージを送る
void Wwise::PostMessage(char *szMessage)
{
	Monitor::PostString(szMessage, Monitor::ErrorLevel_Message);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ダミーコールバック
void Wwise::dummyCallback(AkCallbackType type, AkCallbackInfo *pCallbackInfo)
{
	UNREFERENCED_PARAMETER(type);
	AkEventCallbackInfo *pInfo = (AkEventCallbackInfo *)pCallbackInfo;
	AkGameObjectID id = pInfo->gameObjID;

	dummiesLock.Lock();		// ロック

	// IDの有効性を確かめる
	auto it = std::find(dummies.begin(), dummies.end(), static_cast<int>(pInfo->playingID));
	if (it != dummies.end())
	{
		dummies.erase(it);
		AK::SoundEngine::UnregisterGameObj(id);
	}
	dummiesLock.Unlock();	// アンロック
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 名前からマテリアルIDの取得
int Wwise::GetMaterialId(char *pName)
{
	CHECK_SOUND_ENGINE0;

	return SoundEngine::GetIDFromString(pName);
}

/*
// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトにマテリアルIDを設定
void Wwise::SetMaterial(void *gameObject, int materialid)
{
CHECK_SOUND_ENGINE;

SoundEngine::SetSwitch(SWITCHES::MATERIAL::GROUP, materialid, (AkGameObjectID)gameObject);
}
*/

// ======== ======== ======== ======== ======== ======== ======== ========
// DirectXのVec3をWwiseのVec3に変換
void Wwise::VecToAkVector(const D3DXVECTOR3 & vec, AkVector & vector)
{
	vector.X = vec.x;
	vector.Y = vec.y;
	vector.Z = vec.z;
}

/*
// ======== ======== ======== ======== ======== ======== ======== ========
// 
// スクリプトコマンド
// 
// -------- -------- -------- -------- -------- -------- -------- --------
// ======== ======== ======== ======== ======== ======== ======== ========
// ボリューム設定 (0-255)
void Wwise::musicvol(int vol)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(GAME_PARAMETERS::MUSICVOLUME, (AkRtpcValue)vol);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// SFX(エフェクト)ボリューム設定 (0-255)
void Wwise::soundvol(int vol)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(GAME_PARAMETERS::SFXVOLUME, (AkRtpcValue)vol);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ボイスボリューム設定 (0-255)
void Wwise::voicevol(int vol)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(GAME_PARAMETERS::VOICEVOLUME, (AkRtpcValue)vol);
}
*/
