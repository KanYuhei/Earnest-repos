﻿//--------------------------------------------------------------------------------------
//  ライト  ( light.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _LIGHT_H_
#define _LIGHT_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  カメラクラスの定義
//--------------------------------------------------------------------------------------
class Light
{
public:
	Light( );													//  コンストラクタ
	~Light( );													//  デストラクタ

	HRESULT				Init( void );							//  初期化
	void				Uninit( void );							//  終了
	void				Update( void );							//  更新

	void				SetDiffuseColor( D3DXCOLOR color ,
										 int nNo = 0 );
	void				LightOn( void );						//  ライト点ける
	void				LightOff( void );						//  ライト消す

private:

};

#endif

