//--------------------------------------------------------------------------------------
//  レンダリング処理   ( sceneModelGL.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _CSCENE_FBX_H_
#define _CSCENE_FBX_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <string>
#include <fbxsdk.h>
#include <vector>
#include <list>
#include "scene.h"
#include "math.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class SenceFBX : public Sence
{
public: 
	struct myNode
	{
		std::vector<myNode*> childlen;
		FbxString name;
		std::vector<FbxString> attributeNames;
		FbxDouble3 translation;
		FbxDouble3 rotation;
		FbxDouble3 scaling;
	};

	struct Face
	{
		unsigned int v;
		unsigned int vt;
		unsigned int vn;
	};

	// マテリアルタイプ取得。 
	enum eMATERIAL_TYPE 
	{ 
		MATERIAL_LAMBERT = 0, 
		MATERIAL_PHONG, 
	}; 

	typedef struct Point2DF 
	{
		float x, y;

		bool operator==(Point2DF& val) 
		{
			if(this->x == val.x && this->y == val.y) 
			{
				return true;
			}

			return false;
		}
	} point2;

	typedef struct __UV_SET__ 
	{
		std::string uvSetName;
		std::list<std::string> textures;
		std::vector<point2> uvBuffer;
	} UvSet;

	SenceFBX( );													//  コンストラクタ
	~SenceFBX( );													//  デストラクタ

	HRESULT					Init( void );							//  初期化
	void					Uninit( void );							//  終了
	void					Update( void );							//  更新
	void					Draw( void );							//  描画

	void					SetPos( D3DXVECTOR3 position );				//  座標の代入
	void					SetScale( D3DXVECTOR3 scale );				//  大きさの代入
	void					SetRot( float fRot );					//  回転角の代入
	void					SetScale( float fScale );				//  大きさ倍率の代入

	void					MovePos( D3DXVECTOR3 movePos );			//  座標の移動
	void					ChangeRot( float fChangeRot );			//  回転角の変化
	void					ChangeScale( float fChangeScale );		//  大きさ倍率の変化

	static SenceFBX*		Create( D3DXVECTOR3 position ,
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 scale );			//  生成

private:
	int						GetStrToken( FILE* pFile ,
										 const char* pToken ,
										 char* pBuf );
	int						GetStrCount( FILE* pFile ,
										 const char* pToken ,
										 char* pStr );
	void					MakeVertex( void );
	void					SetVertex( void );

	void					PrintTabs( void ); 
	FbxString				GetAttributeTypeName( FbxNodeAttribute::EType type );
	void					PrintAttribute( FbxNodeAttribute* pAttribute );
	void					PrintNode( FILE* pFile , FbxNode* pNode );
	myNode*					RecursiveNode( FbxNode* pNode );
	void					AnalizePosition( FbxMesh* pMesh );
	void					AnalizeNormalize( FbxMesh* pMesh );
	void					AnalizeMaterial( FbxMesh* pMesh );
	void					AnalizeCluster( FbxMesh* pMesh );
	void					AnalizeUV( FbxMesh* pMesh );
	void					AnalizeTextureName( FbxMesh* pMesh );

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;								//  頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;							//  インデックスバッファインターフェースへのポインタ

	D3DXVECTOR3				m_position;									//  座標
	D3DXVECTOR3				m_scale;									//  大きさ
	D3DXVECTOR3				m_rot;									//  回転
	float					m_fRot;									//  回転角度
	float					m_fRot2;								//  
	float					m_fScale;								//  大きさの倍率
	int						m_nTexID;								//  テクスチャ識別番号
	int						m_nCntV;								//  頂点数
	int						m_nCntVt;								//  UV座標数
	int						m_nCntVn;								//  法線の数
	int						m_nCntF;								//  面の数
	Vector3D*				m_pV;									//  頂点のポインタ
	Vector2D*				m_pVt;									//  UV座標のポインタ
	Vector3D*				m_pVn;									//  法線のポインタ
	Face*					m_pFace;								//  面のポインタ

	std::string				m_fileName;								//  ファイル名

	int						numTabs; 

	UvSet					m_uvSet;
};

#endif