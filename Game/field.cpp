﻿//--------------------------------------------------------------------------------------
//  フィールド  ( field.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "field.h"
#include "hitField.h"
#include "game.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  フィールドクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Field::Field( )
{

}

//--------------------------------------------------------------------------------------
//  フィールドクラスのデストラクタ
//--------------------------------------------------------------------------------------
Field::~Field( )
{

}

//--------------------------------------------------------------------------------------
//  フィールドの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Field::Init( void )
{
	//  シーンモデル
	SenceModel::Init( );

	//  物体の種類の設定
	Sence::SetObjType( Sence::OBJTYPE_FIELD );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  プレイヤーの終了処理
//--------------------------------------------------------------------------------------
void Field::Uninit( void )
{
	//  シーンモデル
	SenceModel::Uninit( );
}

//--------------------------------------------------------------------------------------
//  プレイヤーの更新処理
//--------------------------------------------------------------------------------------
void Field::Update( void )
{
	//  シーンモデル
	SenceModel::Update( );
}

//--------------------------------------------------------------------------------------
//  プレイヤーの描画処理
//--------------------------------------------------------------------------------------
void Field::Draw( void )
{
	//  シーンモデル
	SenceModel::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Field* Field::Create( Field::TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	Field* pField;

	//  インスタンス生成
	pField = new Field;

	//  座標の代入
	pField->m_position = position;

	//  回転角の代入
	pField->m_rot = rot;

	//  大きさ倍率の代入
	pField->m_scale = scale;

	if( type == Field::TYPE_STAGE000 )
	{
		//  種類の代入
		pField->m_type = SenceModel::TYPE_FIELD000;

		//  当たり判定用フィールドの生成
		HitField::Create( HitField::TYPE_HIT_STAGE000 , position , rot , scale );
	}
	else if( type == Field::TYPE_STAGE001 )
	{
		//  種類の代入
		pField->m_type = SenceModel::TYPE_FIELD001;

		//  当たり判定用フィールドの生成
		HitField::Create( HitField::TYPE_HIT_STAGE001 , position , rot , scale );
	}

	//  初期化
	pField->Init( );

	return pField;
}