//--------------------------------------------------------------------------------------
//  ルートノードAIクラス   ( rootNodeAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ROOT_NODE_AI_H_
#define _ROOT_NODE_AI_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "nodeAI.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ルートノードAIクラスの定義
//--------------------------------------------------------------------------------------
class RootNodeAI : public NodeAI
{
public: 
	RootNodeAI( ){ }							//  コンストラクタ
	
	void AllJudgeActive( void );				//  全てのノードの実行可能状態の判断					

protected:
};

#endif
