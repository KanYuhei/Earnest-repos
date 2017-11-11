//--------------------------------------------------------------------------------------
//  シェーダープログラム   ( shader.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_H_
#define _SHADER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  シェーダークラスの定義
//--------------------------------------------------------------------------------------
class Shader
{
public: 
	enum class TYPE
	{
		TYPE_NORMAL = 0 ,
		TYPE_MAX ,
	};

	Shader( ){ }										//  コンストラクタ
	~Shader( ){ }										//  デストラクタ

	virtual void	Init( void ) = 0;					//  初期化						
	virtual void	Uninit( void ) = 0;					//  終了
	virtual void	Update( void ) = 0;					//  更新
	virtual void	Draw( void ) = 0;					//  描画
private:
};

#endif
