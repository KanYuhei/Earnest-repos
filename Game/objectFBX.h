//--------------------------------------------------------------------------------------
//  �����_�����O����   ( sceneFBX.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _OBJECT_FBX_H_
#define _OBJECT_FBX_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <string>
#include <fbxsdk.h>
#include <vector>
#include <list>
#include "scene.h"
#include "math.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class FBXNode;
class FBXMesh;

//--------------------------------------------------------------------------------------
//  �����_���[�N���X�̒�`
//--------------------------------------------------------------------------------------
class ObjectFBX : public Scene
{
public: 
	////////////////////////////////////////////////////////////////////////////////////////
	//  �@ �o�C�i���[�������ꍇ�́A�|�C���^�͊�{���Ԗڂ̃f�[�^( �z�� )�����������ق�������
	//  �A ��]�̏��Ԃ��s��( XYZ�̏��Ԃŉ�]�Ȃ�)
	////////////////////////////////////////////////////////////////////////////////////////

	struct BornRefarence 
	{     
		BornRefarence(unsigned char index, float weight) : index(index), weight(weight) {}     
		unsigned char index;     
		float weight; 
	}; 
 
	struct Point 
	{     
		Point(const D3DXVECTOR3& positions) : positions(positions) {}     
		D3DXVECTOR3 positions;     
		std::vector<BornRefarence> bornRefarences; 
	}; 
 
	struct Mesh 
	{     
		std::vector<Point> points;     
		std::vector<D3DXVECTOR3> normals;      
		std::vector<D3DXVECTOR2> texcoords; 
 
		std::vector<unsigned short> positionIndices;     
		std::vector<unsigned short> normalIndices;     
		std::vector<unsigned short> texcoordIndices;          
		int materialIndex; 
 
		D3DXMATRIX initInvMatrix;
		std::vector<std::vector<D3DXMATRIX>> matrixes;
	};

	struct MATERIAL
	{
		D3DXCOLOR	Diffuse;
		D3DXCOLOR	Ambient;
		D3DXCOLOR	Specular;
		D3DXCOLOR	Emission;
		float		Shininess;
	};
 
	ObjectFBX( );																//  �R���X�g���N�^
	~ObjectFBX( );																//  �f�X�g���N�^

	HRESULT						Init( void );											//  ������
	void						Uninit( void );											//  �I��
	void						Update( void );											//  �X�V
	void						Draw( void );											//  �`��

	void						SetPosition( D3DXVECTOR3 pos );								//  ���W�̑��
	void						SetSize( D3DXVECTOR3 size );								//  �傫���̑��
	void						SetRot( float fRot );									//  ��]�p�̑��
	void						SetScale( float fScale );								//  �傫���{���̑��

	void						MovePos( D3DXVECTOR3 movePos );							//  ���W�̈ړ�
	void						ChangeRot( float fChangeRot );							//  ��]�p�̕ω�
	void						ChangeScale( float fChangeScale );						//  �傫���{���̕ω�

private:
	void						AnalyzePosition(FbxMesh* pMesh);    
	void						AnalyzeNormal(FbxMesh* pMesh);     
	void						AnalyzeTexcoord(FbxMesh* pMesh, bool bRevers = false);     
	void						AnalyzeMaterial(FbxNode* pNode);     
	void						AnalyzeMaterial(FbxMesh* pMesh);     
	void						AnalyzeCluster(FbxMesh* pMesh);    

	FbxAMatrix					GetGeometry(FbxNode* pNode);
	void						MakeVertex( int size );

	static std::string			GetAttributeTypeName(FbxNodeAttribute::EType type);
 
/*	myNode*						recursiveNode( FbxManager* pManager, FbxNode* pNode, bool bTexcoordVReverse,
											   FbxTime startTime, FbxTime endTime );    */ 
		

	void						recursiveDraw( void );

	std::string					name;     
	std::vector<std::string>	attributeNames;     
	D3DXVECTOR3					translation;      
	D3DXVECTOR3					rotation;      
	D3DXVECTOR3					scaling;  


	FbxTime						m_startTime;
	FbxTime						m_endTime;
	int							m_currentFrame;
	int							m_allTime;
	bool						m_makeVertrx;
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;								//  ���_�o�b�t�@�ւ̃|�C���^
	std::vector< std::string >	textures;								//  �}�e���A��     
	std::vector< Mesh >			meshes; 
};

#endif