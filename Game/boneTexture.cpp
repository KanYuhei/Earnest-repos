//--------------------------------------------------------------------------------------
//  ボーン情報格納テクスチャ   ( boneTexture.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "boneTexture.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "shaderManager.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9	BoneTexture::m_boneInfoTexture = nullptr;			//  テクスチャ
const UINT			BoneTexture::TEXTURE_WIDTH = 256;					//  テクスチャの幅
const UINT			BoneTexture::TEXTURE_HEIGHT = 256;					//  テクスチャの高さ

//--------------------------------------------------------------------------------------
//  ボーン情報格納テクスチャクラスのコンストラクタ
//--------------------------------------------------------------------------------------
BoneTexture::BoneTexture( )
{

}

//--------------------------------------------------------------------------------------
//  ボーン情報格納テクスチャのデストラクタ
//--------------------------------------------------------------------------------------
BoneTexture::~BoneTexture( )
{

}

//--------------------------------------------------------------------------------------
//  ボーン情報格納テクスチャの初期化処理
//--------------------------------------------------------------------------------------
HRESULT BoneTexture::Init( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  レンダーターゲット用の空のテクスチャ生成
	D3DXCreateTexture( pDevice ,						//  デバイス
					   TEXTURE_WIDTH ,					//  スクリーンの幅
					   TEXTURE_HEIGHT ,					//  スクリーンの高さ
					   1 ,								//  ミップマップの数
					   D3DUSAGE_DYNAMIC ,				//  使用用途
					   D3DFMT_A32B32G32R32F ,			//  色の要素
					   D3DPOOL_DEFAULT ,				//  メモリの配置方法
					   &m_boneInfoTexture );			//	テクスチャ格納場所

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  ボーン情報格納テクスチャの終了処理
//--------------------------------------------------------------------------------------
void BoneTexture::Uninit( void )
{
	//  ボーン情報格納テクスチャの破棄
	if( m_boneInfoTexture != nullptr )
	{
		m_boneInfoTexture->Release( );
		m_boneInfoTexture = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  ボーン情報格納テクスチャ取得
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 BoneTexture::GetBoneInfoTexture( void )
{
	return m_boneInfoTexture;
}