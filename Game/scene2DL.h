//--------------------------------------------------------------------------------------
//  レンダリング処理   ( scene2DL.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SCENE_2D_L_H_
#define _SCENE_2D_L_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class Sence2DL : public Sence
{
public:
	Sence2DL( );														//  デフォルトコンストラクタ
	Sence2DL( D3DXVECTOR3 position ,
			   D3DXVECTOR3 size ,
			   D3DXVECTOR2 divideUV ,
			   D3DXVECTOR2 posUV ,
			   D3DXCOLOR color );										//  コンストラクタ
	~Sence2DL( );														//  デストラクタ

	HRESULT					Init( void );								//  初期化
	void					Uninit( void );								//  終了
	void					Update( void );								//  更新
	void					Draw( void );								//  描画

	static Sence2DL*		Create( D3DXVECTOR3 position,					//  生成
									D3DXVECTOR3 size , 
									D3DXVECTOR2 posUV ,
									D3DXVECTOR2 divideUV );

	void					SetTextureName( char* pFileName );			//  テクスチャ名の設定
	void					SetScale( D3DXVECTOR2 scale );				//  大きさの倍率設定
	void					SetScaleX( float fScaleX );					//  X方向の大きさの倍率設定
	void					SetScaleY( float fScaleY );					//  Y方向の大きさの倍率設定

protected:
	D3DXVECTOR2				m_posUV;									//  UV座標
	D3DXVECTOR2				m_divideUV;									//  UV分割数
	D3DXVECTOR2				m_endUV;									//  UV座標
	D3DXVECTOR2				m_scale;									//  大きさの倍率

	char					m_aFileName[ 256 ];							//  ファイル名

private:
	void					SetPosUV( D3DXVECTOR2 size );				//  UV座標の設定
	D3DXVECTOR2				GetPosUV( void);							//  UV座標の取得

	void					SetDivideUV( D3DXVECTOR2 size );			//  UV分割数の設定
	D3DXVECTOR2				GetDivideUV( void);							//  UV分割数の取得

	HRESULT					MakeVertex( void );							//  頂点の作成
	void					SetVertex( void );							//  頂点の設定

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;									//  頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;								//  インデックスバッファインターフェースへのポインタ

	int						m_nAnimationShift;							//  アニメーション移行フレーム	

	float					m_fRot;										//  傾き
};

#endif
