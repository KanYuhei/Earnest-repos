//--------------------------------------------------------------------------------------
//  ボーン情報格納テクスチャ   ( boneTexture.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _BONE_TEXTURE_H_
#define _BONE_TEXTURE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  ボーン情報格納テクスチャクラスの定義
//--------------------------------------------------------------------------------------
class BoneTexture
{
public:
	BoneTexture(  );													//  デフォルトコンストラクタ		
	~BoneTexture( );													//  デストラクタ

	static HRESULT				Init( void );							//  初期化
	static void					Uninit( void );							//  終了
	static LPDIRECT3DTEXTURE9	GetBoneInfoTexture( void );				//  ボーン情報格納テクスチャの取得

	static const UINT			TEXTURE_WIDTH;							//  デプスシャドウの幅
	static const UINT			TEXTURE_HEIGHT;							//  デプスシャドウの高さ

private:
	static LPDIRECT3DTEXTURE9	m_boneInfoTexture;						//  ボーン情報格納テクスチャ
};

#endif