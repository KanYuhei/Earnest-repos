//--------------------------------------------------------------------------------------
//  レンダリング処理   ( renderer.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _RENDERER_H_
#define _RENDERER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "Effekseer.h"
#include "EffekseerRendererDX9.h"
#include "EffekseerSoundXAudio2.h"

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class Fade;

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class Renderer
{
public: 
	Renderer( );													//  コンストラクタ
	~Renderer( );													//  デストラクタ
	HRESULT						Init( HWND hWnd, bool bWindow );	//  初期化
	void						Uninit( void );						//  終了
	void						Update( bool bUpdate );				//  更新
	void						Draw( void );						//  描画
	void						DrawBegin( void );					//  描画開始
	void						DrawClearBuffer( void );			//  バッファのクリア
	void						DrawEnd( void );					//  描画終了

	LPDIRECT3DDEVICE9			GetDevice( void );					//  デバイスの取得

private:

#ifdef _DEBUG

	void						DrawFPS(void);						//  FPS描画

	void						DrawField( void );					//  ツール時のフィールド情報描画

#endif

	LPDIRECT3D9					m_pD3D;								//  Direct3Dオブジェクト
	static LPDIRECT3DDEVICE9	m_pD3DDevice;						//  Deviceオブジェクト(描画に必要)

	static Fade*				m_pFade;							//  フェードクラスのポインタ

#ifdef _DEBUG

	LPD3DXFONT					m_pFont;							//  フォントへのポインタ
	int							m_nCountFPS;						//  FPSカウンタ

#endif

};

#endif
