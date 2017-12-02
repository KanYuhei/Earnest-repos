//--------------------------------------------------------------------------------------
//  ObjectFBX  ( objectFBX.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "objectFBX.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "test.h"
#include "utility.h"
#include "shaderManager.h"
#include "light.h"
#include <algorithm>

//--------------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------------
ObjectFBX::ObjectFBX( )
{
	m_startTime = 0;
	m_endTime = 0;
	m_currentFrame = 0;
	m_allTime = 0;
	m_scale = 1.0f;
}

//--------------------------------------------------------------------------------------
//  �f�X�X�g���N�^
//--------------------------------------------------------------------------------------
ObjectFBX::~ObjectFBX( )
{

}

//--------------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------------
HRESULT ObjectFBX::Init( void )
{
	//  FBX���f���ǂݍ��ݎ����p�v���O����
	FbxManager* lSdkManager = FbxManager::Create( ); 
 
	// Create the IO settings object. 
	FbxIOSettings *ios = FbxIOSettings::Create( lSdkManager , IOSROOT ); lSdkManager->SetIOSettings( ios ); 
 
	// Create an importer using the SDK manager. 
	FbxImporter* lImporter = FbxImporter::Create( lSdkManager ,"" ); 
 
	// Use the first argument as the filename for the importer. 
	if( !lImporter->Initialize( "data/FBX/player.fbx" , -1 , lSdkManager->GetIOSettings( ) ) ) 
	{      
		char buf[ 256 ];     
		sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
		MessageBox( NULL , buf , "error" , MB_OK );     
		return E_FAIL; 
	}

	// Create a new scene so that it can be populated by the imported file. 
	FbxScene* lScene = FbxScene::Create( lSdkManager , "myScene" ); 
 
	// Import the contents of the file into the scene. 
	lImporter->Import( lScene ); 
 
	FbxGeometryConverter converter( lSdkManager ); 
	
	// �O�p�|���S���� 
	converter.Triangulate( lScene , true );
	
	// �k�ރ��b�V�����폜 
	converter.RemoveBadPolygonsFromMeshes( lScene ); 
	
	// �}�e���A�����ƂɃ��b�V������ 
	converter.SplitMeshesPerMaterial( lScene , true ); 

	//  �A�j���[�V�������̎擾
	lImporter->GetAnimStackCount( );
 
	//  ���b�V�����̎擾
	int numberMesh = lScene->GetMemberCount< FbxMesh >( );

	//  �}�e���A�����̎擾
	int numberMaterial = lScene->GetMaterialCount( );

	//  ���b�V���̐����̃��[�v
	for( int countMesh = 0; countMesh < numberMesh; ++countMesh )
	{
		m_meshes.push_back( Mesh( ) );

		//  �w�胁�b�V���̏��̎擾
		FbxMesh* mesh = lScene->GetSrcObject< FbxMesh >( countMesh );

		//  ���W�̎擾
		AnalyzePosition( mesh );

		//  �@�����                 
		AnalyzeNormal( mesh );                 
				
		//  UV���                 
		AnalyzeTexcoord( mesh , true );   

		//  �}�e���A����́i�Q�Ə��̎擾�j                 
		AnalyzeMaterial( mesh ); 

		//  �{�[�����
		AnalyzeCluster( lScene , mesh );

		//  �C���f�b�N�X
		FbxLayerElementArrayTemplate< int >* index;

		//  
		if( mesh->GetMaterialIndices( &index ) && index->GetCount( ) > 0 )
		{
			// �}�e���A�����̎擾     
			FbxSurfaceMaterial* material = mesh->GetNode( )->GetMaterial( index->GetAt( 0 ) ); 

			//  �}�e���A�����̕ۑ�
			m_meshes.back( ).materialName = material->GetName( ); 
		}
	}

	//  �e�N�X�`�����̎擾
	AnalyzeTexture( lScene , numberMaterial );
	
	//  ���_�f�[�^�̕ϊ�
	ConvertVertex( );

	//  ���b�V�����̒��_�o�b�t�@�𐶐�
	CreateMeshVertex( );

	lImporter->Destroy( ); 
	lScene->Destroy( ); 
	lSdkManager->Destroy( ); 

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------------
void ObjectFBX::Uninit( void )
{
	Scene::Release( );
}

//--------------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------------
void ObjectFBX::Update( void )
{
	m_currentFrame += 1;
}

//--------------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------------
void ObjectFBX::Draw( void )
{  
	Camera* camera = SceneManager::GetCamera( );

	D3DXVECTOR3 cameraVectorDirect;
	D3DXVECTOR3 cameraToPlayer;

	if( camera != nullptr )
	{
		D3DXVECTOR3 cameraPosition = camera->GetCameraPosEye( );
		D3DXVECTOR3 playerPosition = m_position;
		cameraPosition.y = 0.0f;
		playerPosition.y = 0.0f;

		cameraToPlayer = playerPosition - cameraPosition;
		D3DXVec3Normalize( &cameraToPlayer , &cameraToPlayer );
		cameraVectorDirect = camera->GetCameraVecDirect( );
		cameraVectorDirect.y = 0.0f;
		D3DXVec3Normalize( &cameraVectorDirect , &cameraVectorDirect );
	}

	if( Utility::IntoView( cameraVectorDirect , cameraToPlayer , D3DX_PI * 0.5f ) )
	{
		//  �ċA�I�ɕ`��
		recursiveDraw( ); 
	}
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɕ`�揈��
//--------------------------------------------------------------------------------------
void ObjectFBX::recursiveDraw( void )
{ 
	D3DXMATRIX mtxWorld;							//  ���[���h�s��
	D3DXMATRIX mtxTrans;							//  ���s�ړ��s��
	D3DXMATRIX mtxScale;							//  �g��s��
	D3DXMATRIX mtxRot;								//  �r���[���W�ϊ��s��

	int countLoop = 0;

	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  �V�F�[�_�[���̎擾
	Shader3D* shader3D = ( Shader3D* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D );

	if( shader3D != nullptr )
	{
		//  �e�N�X�`���N���X�̎擾
		Texture* pTexture = SceneManager::GetTexture( );

		for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
		{  
			if( itm->matrixes.empty( ) ) 
			{
				// ���_�̍��W�ϊ�             
				std::vector< D3DXVECTOR3 > positions;
				int size = itm->points.size( );

				positions.reserve( size ); 
 
				D3DXMATRIX mtx;             
				
				for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
				{                
					ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

					D3DXVECTOR3 pos = it->positions;                              
					positions.push_back( pos );             
				} 

				VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^

				if( m_pVtxBuffers.at( countLoop ) != NULL )
				{
					//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
					m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
														 ( void** )&pVtx ,				//  �A�h���X�������ꂽ�������̃A�h���X
														 0 );							//  ���b�N�̎��

					int size = itm->positionIndices.size( );

					for( int countIndex = 0; countIndex < size; ++countIndex )
					{
						//  ���_���W�̐ݒ�( 3D���W �E �E��� )
						pVtx[ countIndex ].position = positions[ itm->positionIndices[ countIndex ] ];

						//  �@���̎w��
						pVtx[ countIndex ].normal = itm->normals[ itm->normalIndices[ countIndex ] ];

						//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
						pVtx[ countIndex ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

						//  UV���W�̎w��
						pVtx[ countIndex ].texcoord = itm->texcoords[ itm->texcoordIndices[ countIndex ] ];
					}

					//  ���_�o�b�t�@�̃A�����b�N
					m_pVtxBuffers.at( countLoop )->Unlock( );     
				}

				D3DXMatrixIdentity( &mtxWorld );
				D3DXMatrixIdentity( &mtxScale );
				D3DXMatrixIdentity( &mtxRot );

				//  �g��s��̍쐬
				D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

				//  �g��s��̊|���Z
				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

				//  ���s�ړ��s��̍쐬
				D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

				//  ���s�ړ��s��̊|���Z
				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

				//  ���C�g�̕����x�N�g�����擾����
				D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( );
				D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );

				//  ���[�J���P�ʃx�N�g���ɕύX
				D3DXVECTOR3 lightDirectLocal;

				D3DXMATRIX worldInverseMatrix;
				D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
				D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
				D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );

				Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
				D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
				D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
				D3DXCOLOR lightDiffuseColor = SceneManager::GetLight( )->GetLight( 0 ).Diffuse;

				//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
				shader3D->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor );

				// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource( 0 ,								//  �p�C�v���C���ԍ�
										  m_pVtxBuffers.at( countLoop ) ,	//  ���_�o�b�t�@�̃A�h���X
										  0 ,								//  �I�t�Z�b�g( byte )
										  sizeof( VERTEX_3D ) );			//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture( 0 , pTexture->GetTextureImage( "data/FBX/texture/Head.png" ) );

				//  �V�F�[�_�[3D�̕`��J�n
				shader3D->DrawBegin( );

				// �|���S���̕`��
				pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  �v���~�e�B�u�̎��
										0 ,										//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
										itm->positionIndices.size( ) / 3 );		//  �v���~�e�B�u��  

				//  �V�F�[�_�[3D�̕`��I��
				ShaderManager::DrawEnd( );
			}      
			else 
			{
				// ������i�܂胏���X�L���Ȃǁj 
 
				// ���_�̍��W�ϊ�             
				std::vector< D3DXVECTOR3 > positions;
				int size = itm->points.size( );

				positions.reserve( size ); 
 
				D3DXMATRIX mtx;             
				
				for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
				{                
					ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

					for( auto itb = it->bornRefarences.begin( ); itb != it->bornRefarences.end( ); ++itb ) 
					{                    
						mtx += itm->matrixes[ itb->index ][ m_currentFrame % m_allTime ] * itb->weight;
					}

					D3DXVECTOR3 pos = it->positions;                 
					D3DXVec3TransformCoord( &pos , &it->positions , &mtx );                
					positions.push_back( pos );             
				} 

				VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^

				if( m_pVtxBuffers.at( countLoop ) != NULL )
				{
					//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
					m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
														 ( void** )&pVtx ,				//  �A�h���X�������ꂽ�������̃A�h���X
														 0 );							//  ���b�N�̎��

					for( size_t i = 0; i < itm->positionIndices.size( ); i++ ) 
					{             
						//  ���_���W�̐ݒ�( 3D���W �E �E��� )
						pVtx[ i ].position = positions[ itm->positionIndices[ i ] ];

						//  �@���̎w��
						pVtx[ i ].normal = itm->normals[ itm->normalIndices[ i ] ];

						//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
						pVtx[ i ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

						//  UV���W�̎w��
						pVtx[ i ].texcoord = itm->texcoords[ itm->texcoordIndices[ i ] ];
					}

					//  ���_�o�b�t�@�̃A�����b�N
					m_pVtxBuffers.at( countLoop )->Unlock( );     
				}

				D3DXMatrixIdentity( &mtxWorld );
				D3DXMatrixIdentity( &mtxScale );
				D3DXMatrixIdentity( &mtxRot );

				//  �g��s��̍쐬
				D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

				//  �g��s��̊|���Z
				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

				//  ���s�ړ��s��̍쐬
				D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

				//  ���s�ړ��s��̊|���Z
				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

				//  ���_�t�H�[�}�b�g�̐ݒ�
				shader3D->SetVertexDeclaration( );

				//  ���[���h�s��̐ݒ�
				shader3D->SetWorldMatrix( mtxWorld );

				//  ���C�g�̕����x�N�g�����擾����
				D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( );
				D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );

				//  ���[�J���P�ʃx�N�g���ɕύX
				D3DXVECTOR3 lightDirectLocal;

				D3DXMATRIX worldInverseMatrix;
				D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
				D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
				D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );

				Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
				D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
				D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
				D3DXCOLOR lightDiffuseColor = SceneManager::GetLight( )->GetLight( 0 ).Diffuse;

				//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
				shader3D->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor );

				// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource( 0 ,								//  �p�C�v���C���ԍ�
										  m_pVtxBuffers.at( countLoop ) ,	//  ���_�o�b�t�@�̃A�h���X
										  0 ,								//  �I�t�Z�b�g( byte )
										  sizeof( VERTEX_3D ) );			//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_textures[ itm->materialIndex ].c_str( ) ) );

				//  �V�F�[�_�[3D�̕`��J�n
				shader3D->DrawBegin( );

				// �|���S���̕`��
				pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  �v���~�e�B�u�̎��
										0 ,										//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
										itm->positionIndices.size( ) / 3 );		//  �v���~�e�B�u��

				//  �V�F�[�_�[3D�̕`��I��
				ShaderManager::DrawEnd( );
			}

			//  ���[�v�J�E���g
			countLoop++;
		}
	}
}

//--------------------------------------------------------------------------------------
//  FBX�̃f�v�X�l�������ݏ���
//--------------------------------------------------------------------------------------
void ObjectFBX::DrawDepth( void )
{
	D3DXMATRIX mtxWorld;							//  ���[���h�s��
	D3DXMATRIX mtxTrans;							//  ���s�ړ��s��
	D3DXMATRIX mtxScale;							//  �g��s��
	D3DXMATRIX mtxRot;								//  �r���[���W�ϊ��s��

	int countLoop = 0;

	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
	{  
		if( itm->matrixes.empty( ) ) 
		{
			// ���_�̍��W�ϊ�             
			std::vector< D3DXVECTOR3 > positions;
			int size = itm->points.size( );

			positions.reserve( size ); 
 
			D3DXMATRIX mtx;             
				
			for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
			{                
				ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

				D3DXVECTOR3 pos = it->positions;                              
				positions.push_back( pos );             
			} 

			VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^

			if( m_pVtxBuffers.at( countLoop ) != NULL )
			{
				//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
				m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
														( void** )&pVtx ,				//  �A�h���X�������ꂽ�������̃A�h���X
														0 );							//  ���b�N�̎��

				int size = itm->positionIndices.size( );

				for( int countIndex = 0; countIndex < size; ++countIndex )
				{
					//  ���_���W�̐ݒ�( 3D���W �E �E��� )
					pVtx[ countIndex ].position = positions[ itm->positionIndices[ countIndex ] ];

					//  �@���̎w��
					pVtx[ countIndex ].normal = itm->normals[ itm->normalIndices[ countIndex ] ];

					//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
					pVtx[ countIndex ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

					//  UV���W�̎w��
					pVtx[ countIndex ].texcoord = itm->texcoords[ itm->texcoordIndices[ countIndex ] ];
				}

				//  ���_�o�b�t�@�̃A�����b�N
				m_pVtxBuffers.at( countLoop )->Unlock( );     
			}

			D3DXMatrixIdentity( &mtxWorld );
			D3DXMatrixIdentity( &mtxScale );
			D3DXMatrixIdentity( &mtxRot );

			//  �g��s��̍쐬
			D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

			//  �g��s��̊|���Z
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

			//  ���s�ړ��s��̍쐬
			D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

			//  ���s�ړ��s��̊|���Z
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

			//  �V�F�[�_�[���̎擾
			Shader3DShadowMap* shader3DShadowMap = ( Shader3DShadowMap* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_SHADOW_MAP );

			//  �J�����N���X�̎擾
			Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

			D3DXMATRIX viewMatrix = SceneManager::GetLight( )->GetViewMatrix( );
			D3DXMATRIX projectionMatrix = SceneManager::GetLight( )->GetProjectionMatrix( );

			shader3DShadowMap->SetShaderInfo( mtxWorld , viewMatrix * projectionMatrix );

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource( 0 ,								//  �p�C�v���C���ԍ�
										m_pVtxBuffers.at( countLoop ) ,	//  ���_�o�b�t�@�̃A�h���X
										0 ,								//  �I�t�Z�b�g( byte )
										sizeof( VERTEX_3D ) );			//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture( 0 , nullptr );

			//  �V�F�[�_�[�`��J�n
			shader3DShadowMap->DrawBegin( );

			// �|���S���̕`��
			pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  �v���~�e�B�u�̎��
									0 ,										//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
									itm->positionIndices.size( ) / 3 );		//  �v���~�e�B�u��  

			//  �`��I��
			ShaderManager::DrawEnd( );
		}      
		else 
		{
			// ������i�܂胏���X�L���Ȃǁj 
 
			// ���_�̍��W�ϊ�             
			std::vector< D3DXVECTOR3 > positions;
			int size = itm->points.size( );

			positions.reserve( size ); 
 
			D3DXMATRIX mtx;             
				
			for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
			{                
				ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

				for( auto itb = it->bornRefarences.begin( ); itb != it->bornRefarences.end( ); ++itb ) 
				{                    
					mtx += itm->matrixes[ itb->index ][ m_currentFrame % m_allTime ] * itb->weight;
				}

				D3DXVECTOR3 pos = it->positions;                 
				D3DXVec3TransformCoord( &pos , &it->positions , &mtx );                
				positions.push_back( pos );             
			} 

			VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^

			if( m_pVtxBuffers.at( countLoop ) != NULL )
			{
				//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
				m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
														( void** )&pVtx ,				//  �A�h���X�������ꂽ�������̃A�h���X
														0 );							//  ���b�N�̎��

				for( size_t i = 0; i < itm->positionIndices.size( ); i++ ) 
				{             
					//  ���_���W�̐ݒ�( 3D���W �E �E��� )
					pVtx[ i ].position = positions[ itm->positionIndices[ i ] ];

					//  �@���̎w��
					pVtx[ i ].normal = itm->normals[ itm->normalIndices[ i ] ];

					//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
					pVtx[ i ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

					//  UV���W�̎w��
					pVtx[ i ].texcoord = itm->texcoords[ itm->texcoordIndices[ i ] ];
				}

				//  ���_�o�b�t�@�̃A�����b�N
				m_pVtxBuffers.at( countLoop )->Unlock( );     
			}

			D3DXMatrixIdentity( &mtxWorld );
			D3DXMatrixIdentity( &mtxScale );
			D3DXMatrixIdentity( &mtxRot );

			//  �g��s��̍쐬
			D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

			//  �g��s��̊|���Z
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

			//  ���s�ړ��s��̍쐬
			D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

			//  ���s�ړ��s��̊|���Z
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource( 0 ,								//  �p�C�v���C���ԍ�
										m_pVtxBuffers.at( countLoop ) ,	//  ���_�o�b�t�@�̃A�h���X
										0 ,								//  �I�t�Z�b�g( byte )
										sizeof( VERTEX_3D ) );			//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

			//  �V�F�[�_�[���̎擾
			Shader3DShadowMap* shader3DShadowMap = ( Shader3DShadowMap* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_SHADOW_MAP );

			//  �J�����N���X�̎擾
			Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

			D3DXMATRIX viewMatrix = SceneManager::GetLight( )->GetViewMatrix( );
			D3DXMATRIX projectionMatrix = SceneManager::GetLight( )->GetProjectionMatrix( );

			shader3DShadowMap->SetShaderInfo( mtxWorld , viewMatrix * projectionMatrix );

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture( 0 , nullptr );

			//  �V�F�[�_�[�`��J�n
			shader3DShadowMap->DrawBegin( );

			// �|���S���̕`��
			pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  �v���~�e�B�u�̎��
									0 ,										//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
									itm->positionIndices.size( ) / 3 );		//  �v���~�e�B�u��

			//  �`��I��
			ShaderManager::DrawEnd( );
		}

		//  ���[�v�J�E���g
		countLoop++;
	}
}

////--------------------------------------------------------------------------------------
////  �m�[�h��H���čċA�I�ɕ`�揈��
////--------------------------------------------------------------------------------------
//void ObjectFBX::recursiveDraw( void )
//{ 
//	D3DXMATRIX mtxWorld;							//  ���[���h�s��
//	D3DXMATRIX mtxTrans;							//  ���s�ړ��s��
//	D3DXMATRIX mtxScale;							//  �g��s��
//	D3DXMATRIX mtxRot;								//  �r���[���W�ϊ��s��
//
//	int countLoop = 0;
//
//	//  �f�o�C�X���̎擾
//	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );
//
//	//  �V�F�[�_�[���̎擾
//	Shader3D* shader3D = ( Shader3D* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D );
//
//	if( shader3D != nullptr )
//	{
//		//  �e�N�X�`���N���X�̎擾
//		Texture* pTexture = SceneManager::GetTexture( );
//
//		for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
//		{  
//			//if( itm->matrixes.empty( ) ) 
//			//{
//			//	// ���_�̍��W�ϊ�             
//			//	std::vector< D3DXVECTOR3 > positions;
//			//	int size = itm->points.size( );
//
//			//	positions.reserve( size ); 
// 
//			//	D3DXMATRIX mtx;             
//			//	
//			//	for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
//			//	{                
//			//		ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   
//
//			//		D3DXVECTOR3 pos = it->positions;                              
//			//		positions.push_back( pos );             
//			//	} 
//
//			//	VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^
//
//			//	if( m_pVtxBuffers.at( countLoop ) != NULL )
//			//	{
//			//		//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
//			//		m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
//			//											 ( void** )&pVtx ,				//  �A�h���X�������ꂽ�������̃A�h���X
//			//											 0 );							//  ���b�N�̎��
//
//			//		int size = itm->positionIndices.size( );
//
//			//		for( int countIndex = 0; countIndex < size; ++countIndex )
//			//		{
//			//			//  ���_���W�̐ݒ�( 3D���W �E �E��� )
//			//			pVtx[ countIndex ].position = positions[ itm->positionIndices[ countIndex ] ];
//
//			//			//  �@���̎w��
//			//			pVtx[ countIndex ].normal = itm->normals[ itm->normalIndices[ countIndex ] ];
//
//			//			//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
//			//			pVtx[ countIndex ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
//
//			//			//  UV���W�̎w��
//			//			pVtx[ countIndex ].texcoord = itm->texcoords[ itm->texcoordIndices[ countIndex ] ];
//			//		}
//
//			//		//  ���_�o�b�t�@�̃A�����b�N
//			//		m_pVtxBuffers.at( countLoop )->Unlock( );     
//			//	}
//
//			//	D3DXMatrixIdentity( &mtxWorld );
//			//	D3DXMatrixIdentity( &mtxScale );
//			//	D3DXMatrixIdentity( &mtxRot );
//
//			//	//  �g��s��̍쐬
//			//	D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );
//
//			//	//  �g��s��̊|���Z
//			//	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );
//
//			//	//  ���s�ړ��s��̍쐬
//			//	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );
//
//			//	//  ���s�ړ��s��̊|���Z
//			//	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );
//
//			//	//  ���C�g�̕����x�N�g�����擾����
//			//	D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( 0 );
//			//	D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );
//
//			//	//  ���[�J���P�ʃx�N�g���ɕύX
//			//	D3DXVECTOR3 lightDirectLocal;
//
//			//	D3DXMATRIX worldInverseMatrix;
//			//	D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
//			//	D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
//			//	D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );
//
//			//	//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
//			//	shader3D->SetShaderInfo( mtxWorld , lightDirectLocal );
//
//			//	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//			//	pDevice->SetStreamSource( 0 ,								//  �p�C�v���C���ԍ�
//			//							  m_pVtxBuffers.at( countLoop ) ,	//  ���_�o�b�t�@�̃A�h���X
//			//							  0 ,								//  �I�t�Z�b�g( byte )
//			//							  sizeof( VERTEX_3D ) );			//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )
//
//			//	// �e�N�X�`���̐ݒ�
//			//	pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_textures[ itm->materialIndex ].c_str( ) ) );
//
//			//	//  �V�F�[�_�[3D�̕`��J�n
//			//	shader3D->DrawBegin( );
//
//			//	// �|���S���̕`��
//			//	pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  �v���~�e�B�u�̎��
//			//							0 ,										//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
//			//							itm->positionIndices.size( ) / 3 );		//  �v���~�e�B�u��  
//
//			//	//  �V�F�[�_�[3D�̕`��I��
//			//	ShaderManager::DrawEnd( );
//			//}      
//			//else 
//			{
//				// ������i�܂胏���X�L���Ȃǁj 
// 
//				//// ���_�̍��W�ϊ�             
//				//std::vector< D3DXVECTOR3 > positions;
//				//int size = itm->points.size( );
//
//				//positions.reserve( size ); 
// 
//				//D3DXMATRIX mtx;            
//
//				//for( auto it = itm->vertexList.begin( ); it != itm->vertexList.end( ); ++it )
//				//{
//				//	ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   
//
//				//	for( auto itb = it->bornReference.begin( ); itb != it->bornReference.end( ); ++itb ) 
//				//	{                    
//				//		mtx += itm->matrixes[ itb->index ][ m_currentFrame % m_allTime ] * itb->weight;
//				//	}
//
//				//	D3DXVECTOR3 pos = it->vertex.position;                 
//				//	D3DXVec3TransformCoord( &pos , &pos , &mtx );                
//				//	positions.push_back( pos );
//				//}
//
//				VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^
//
//				if( m_pVtxBuffers.at( countLoop ) != NULL )
//				{
//					//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
//					m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
//														 ( void** )&pVtx ,				//  �A�h���X�������ꂽ�������̃A�h���X
//														 0 );							//  ���b�N�̎��
//
//					int i = 0;
//
//					for( int index = 0; index < itm->indices.size( ); ++index )
//					{      
//						//  ���_���W�̐ݒ�( 3D���W �E �E��� )
//						pVtx[ itm->indices[ index ] ].position = itm->vertexList[ itm->indices[ index ] ].vertex.position;
//
//						//  �@���̎w��
//						pVtx[ itm->indices[ index ] ].normal = itm->vertexList[ itm->indices[ index ] ].vertex.normal;
//
//						//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
//						pVtx[ itm->indices[ index ] ].color = itm->vertexList[ itm->indices[ index ] ].vertex.color;
//
//						//  UV���W�̎w��
//						pVtx[ itm->indices[ index ] ].texcoord = itm->vertexList[ itm->indices[ index ] ].vertex.texcoord;
//					}
//
//					//  ���_�o�b�t�@�̃A�����b�N
//					m_pVtxBuffers.at( countLoop )->Unlock( );     
//				}
//
//				D3DXMatrixIdentity( &mtxWorld );
//				D3DXMatrixIdentity( &mtxScale );
//				D3DXMatrixIdentity( &mtxRot );
//
//				//  �g��s��̍쐬
//				D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );
//
//				//  �g��s��̊|���Z
//				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );
//
//				//  ���s�ړ��s��̍쐬
//				D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );
//
//				//  ���s�ړ��s��̊|���Z
//				D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );
//
//				//  ���_�t�H�[�}�b�g�̐ݒ�
//				shader3D->SetVertexDeclaration( );
//
//				//  ���[���h�s��̐ݒ�
//				shader3D->SetWorldMatrix( mtxWorld );
//
//				//  ���C�g�̕����x�N�g�����擾����
//				D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( 0 );
//				D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );
//
//				//  ���[�J���P�ʃx�N�g���ɕύX
//				D3DXVECTOR3 lightDirectLocal;
//
//				D3DXMATRIX worldInverseMatrix;
//				D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
//				D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
//				D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );
//
//				//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
//				shader3D->SetShaderInfo( mtxWorld , lightDirectLocal );
//
//				// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//				pDevice->SetStreamSource( 0 ,								//  �p�C�v���C���ԍ�
//										  m_pVtxBuffers.at( countLoop ) ,	//  ���_�o�b�t�@�̃A�h���X
//										  0 ,								//  �I�t�Z�b�g( byte )
//										  sizeof( VERTEX_3D ) );			//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )
//
//				// �e�N�X�`���̐ݒ�
//				pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_textures[ itm->materialIndex ].c_str( ) ) );
//
//				//  �V�F�[�_�[3D�̕`��J�n
//				shader3D->DrawBegin( );
//
//				// �|���S���̕`��
//				pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  �v���~�e�B�u�̎��
//										0 ,										//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
//										itm->positionIndices.size( ) / 3 );		//  �v���~�e�B�u��
//
//				//  �V�F�[�_�[3D�̕`��I��
//				ShaderManager::DrawEnd( );
//			}
//
//			//  ���[�v�J�E���g
//			countLoop++;
//		}
//	}
//}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzePosition( FbxMesh* pMesh ) 
{     
	// �R���g���[���|�C���g���̎擾     
	int controlPointsCount = pMesh->GetControlPointsCount( ); 
 
    // ���_���W�p�R���e�i�̗̈�\��     
	m_meshes.back( ).points.reserve( controlPointsCount ); 
 
    // ���_�f�[�^�̎擾     
	FbxVector4* pP = pMesh->GetControlPoints( );     
	
	for( int i = 0; i < pMesh->GetControlPointsCount( ); i++ ) 
	{         
		m_meshes.back( ).points.push_back( D3DXVECTOR3( static_cast< float >( pP[ i ][ 0 ] ) ,                          
													    static_cast< float >( pP[ i ][ 1 ] ) ,                          
													    static_cast< float >( pP[ i ][ 2 ] ) ) );     
	} 
 
    /* ���_�C���f�b�N�X�̎擾 */ 
 
    // �C���f�b�N�X�����擾     
	int polygonVertexCount = pMesh->GetPolygonVertexCount( ); 
 
    // ���_���W�C���f�b�N�X�p�R���e�i�̗̈�\��     
	m_meshes.back( ).positionIndices.reserve( polygonVertexCount );          
	
	// �C���f�b�N�X�o�b�t�@�̎擾     
	for( int i = 0; i < polygonVertexCount; i++ )
	{         
		m_meshes.back( ).positionIndices.push_back( static_cast< unsigned short >( pMesh->GetPolygonVertices( )[ i ] ) );     
	} 
} 
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeNormal( FbxMesh* pMesh ) 
{     
	// ���C���[���̎擾     
	int layerCount = pMesh->GetLayerCount( ); 
 
    for(int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // �@���̎擾         
		FbxLayerElementNormal* pElementNormal = pMesh->GetElementNormal( layer ); 
 
        if( !pElementNormal ) 
		{ 
			continue; 
		} 
 
        // �@�����̎擾         
		int normalCount = pElementNormal->GetDirectArray( ).GetCount( ); 
 
        // �@���i�[�p�R���e�i�̗̈�\��         
		m_meshes.back( ).normals.reserve( normalCount );
 
        // �@���f�[�^�̎擾         
		for( int i = 0; i < normalCount; i++ ) 
		{             
			m_meshes.back( ).normals.push_back( D3DXVECTOR3( static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 0 ] ) ,                 
														     static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 1 ] ) ,                 
														     static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 2 ] ) ) );         
		} 
 
        // �}�b�s���O���[�h�E���t�@�����X���[�h�擾         
		FbxLayerElement::EMappingMode mappingMode = pElementNormal->GetMappingMode( );         
		FbxLayerElement::EReferenceMode referenceMode = pElementNormal->GetReferenceMode( ); 
 
        switch( mappingMode ) 
		{ 
			case FbxLayerElement::eNone:  
			{
				MessageBox( NULL , "Normal MappingMode = mappingMode" , "������" , MB_OK );      
				break; 
			}
			case FbxLayerElement::eByControlPoint: 
			{
				// ���_�o�b�t�@�Ɠ����C���f�b�N�X���g�p 
				if( referenceMode == FbxLayerElement::eDirect ) 
				{                 
					// �@���C���f�b�N�X�i�[�p�R���e�i�̗̈�\��                 
					m_meshes.back( ).normalIndices.reserve( m_meshes.back( ).points.size( ) );                 
				
					// ���_�o�b�t�@�Ɠ����C���f�b�N�X���R�s�[                 
					m_meshes.back( ).normalIndices.assign( m_meshes.back( ).positionIndices.begin( ) , m_meshes.back( ).positionIndices.end( ) );             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{                 
					MessageBox( NULL ,  "Normal ReferenceMode = eIndexToDirect or eIndex, MappingMode = eByControlPoint" ,  "������" , MB_OK );             
				}             
			
				break; 
			}
			case FbxLayerElement::eByPolygonVertex:
			{
				/* �@���Ǝ��̃C���f�b�N�X���g�p */ 
 
				if( referenceMode == FbxLayerElement::eDirect ) 
				{                                  
					// �C���f�b�N�X�Q�Ƃ̕K�v�Ȃ� => �C���f�b�N�X���쐬 
 
					// �@���C���f�b�N�X�i�[�p�R���e�i�̗̈�\��                 
					m_meshes.back( ).normalIndices.reserve( normalCount );                 
				
					// �@���C���f�b�N�X�̍쐬                 
					for( int i = 0; i < normalCount; i++ ) 
					{                     
						m_meshes.back( ).normalIndices.push_back( i );                 
					}             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{ 
					// �Ǝ��C���f�b�N�X������ 
 
					// �C���f�b�N�X���̎擾                 
					int normalIndexCount = pElementNormal->GetIndexArray( ).GetCount( );                 
				
					// �@���C���f�b�N�X�i�[�p�R���e�i�̗̈�\��                 
					m_meshes.back( ).normalIndices.reserve( normalIndexCount );                 
				
					// �@���C���f�b�N�X�̎擾                 
					for( int i = 0; i < normalIndexCount; i++ ) 
					{                     
						m_meshes.back( ).normalIndices.push_back( pElementNormal->GetIndexArray( )[ i ] );                 
					}             
				}

				break; 
			}
			case FbxLayerElement::eByPolygon:       
			{
				MessageBox( NULL , "Normal MappingMode = eByPolygon", "������" , MB_OK );             
				break;       
			}
			case FbxLayerElement::eByEdge: 
			{
				MessageBox( NULL , "Normal MappingMode = eByEdge" , "������" , MB_OK );             
				break;         
			}
			case FbxLayerElement::eAllSame:  
			{
				MessageBox( NULL , "Normal MappingMode = eAllSame" , "������" , MB_OK );            
				break;   
			}
			default: 
			{
				MessageBox( NULL , "Normal ???" , "������" , MB_OK );             
				break;         
			}     
		}
	} 
} 
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeTexcoord( FbxMesh* pMesh , bool bRevers ) 
{ 
	int layerCount = pMesh->GetLayerCount( ); 
 
	if( !layerCount ) 
	{ 
		MessageBox(NULL, "���C���[�������Ă��Ȃ����b�V�����m�F", "Analyze Texcoord", MB_OK); 
		return; 
	} 
 
	for( int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // UV�̎擾         
		FbxLayerElementUV* pElementUV = pMesh->GetLayer( layer )->GetUVs( ); 
 
        if( !pElementUV ) 
		{             
			MessageBox( NULL , "...UV�̂Ȃ����b�V�����C���[���m�F" , "Analyze Texcoord" , MB_OK );             
			continue;         
		} 
 
        // UV�Z�b�g�����擾         
		// = pElementUV->GetName(); 
 
        // �}�b�s���O���[�h�E���t�@�����X���[�h�擾         
		FbxLayerElement::EMappingMode mappingMode = pElementUV->GetMappingMode( );         
		FbxLayerElement::EReferenceMode referenceMode = pElementUV->GetReferenceMode( ); 
 
        if( mappingMode == FbxLayerElement::eByPolygonVertex ) 
		{             
			if( referenceMode == FbxLayerElement::eIndexToDirect || referenceMode == FbxLayerElement::eIndex ) 
			{                 
				int uvIndexCount = pElementUV->GetIndexArray( ).GetCount( );                
				m_meshes.back( ).texcoordIndices.reserve( uvIndexCount ); 
 
                for( int i = 0; i < uvIndexCount; i++ ) 
				{                     
					m_meshes.back( ).texcoordIndices.push_back( pElementUV->GetIndexArray( ).GetAt( i ) );                 
				} 
 
                int uvCount = pElementUV->GetDirectArray( ).GetCount( );      

				m_meshes.back().texcoords.reserve( uvCount ); 
 
                for( int i = 0; i < uvCount; i++ ) 
				{ 
                    m_meshes.back( ).texcoords.push_back( D3DXVECTOR2( static_cast< float >( pElementUV->GetDirectArray( ).GetAt( i )[ 0 ]),                         
																	   static_cast< float >( bRevers ? 1 - pElementUV->GetDirectArray( ).GetAt( i )[ 1]  : pElementUV->GetDirectArray( ).GetAt( i )[ 1] ) ) );                 
				}             																   
			}             
			else 
			{                 
				MessageBox(NULL, "Texcoord::���Ή��̃��t�@�����X���[�h���擾", "FbxLayerElement::eByPolygonVertex", MB_OK);                 
				break;             
			}         
		}         
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{             
			MessageBox( NULL , "...���Ή��}�b�s���O���[�h[eByControlPoint]���擾����", "Analyze Texcoord" , MB_OK );         
		}         
		else if( mappingMode == FbxLayerElement::eByPolygon) 
		{             
			MessageBox( NULL , "...���Ή��}�b�s���O���[�h[eByPolygon]���擾����", "Analyze Texcoord" , MB_OK );         
		}         
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{             
			MessageBox( NULL , "...���Ή��}�b�s���O���[�h[eByEdge]���擾����", "Analyze Texcoord" , MB_OK );         
		}         
		else 
		{             
			MessageBox( NULL , "...�m��Ȃ��}�b�s���O���[�h���擾����", "Analyze Texcoord" , MB_OK );         
		} 
 
        break; // �Ƃ肠�����P�߂���     
	} 
}
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeMaterial( FbxMesh* pMesh ) 
{     
	int layerCount = pMesh->GetLayerCount( ); 
 
    for(int layer = 0; layer < layerCount; layer++) 
	{ 
		FbxLayerElementMaterial* pElementMaterial = pMesh->GetLayer( layer )->GetMaterials( );

		if( !pElementMaterial ) 
		{ 
			continue; 
		} 
 
		int materialIndexCount = pElementMaterial->GetIndexArray( ).GetCount( ); 
 
		if( materialIndexCount == 0 ) 
		{ 
			continue; 
		} 
 
		FbxLayerElement::EMappingMode mappingMode = pElementMaterial->GetMappingMode( );     
		FbxLayerElement::EReferenceMode referenceMode = pElementMaterial->GetReferenceMode( ); 
 
		if( mappingMode == FbxLayerElement::eAllSame ) 
		{         
			if( referenceMode == FbxLayerElement::eIndexToDirect ) 
			{        
				// ���b�V���S�������̃}�e���A���C���f�b�N�X         
				m_meshes.back( ).materialIndex = pElementMaterial->GetIndexArray( ).GetAt( 0 );         
			}         
			else 
			{         
				MessageBox(NULL, "...���Ή��̃��t�@�����X���[�h���擾����", "Material MappingMode = eAllSame", MB_OK);         
			}     
		}     
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{         
			MessageBox(NULL, "...���Ή��̃}�b�s���O���[�h[eByControlPoint]���擾����", "Material MappingMode", MB_OK);     
		}     
		else if( mappingMode == FbxLayerElement::eByPolygon ) 
		{         
			// �}�e���A����������Ă���͂�������A��Ԃ͂��߂̂����ł���         
			m_meshes.back( ).materialIndex = pElementMaterial->GetIndexArray( ).GetAt( 0 );     
		}     
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{         
			MessageBox(NULL, "...���Ή��̃}�b�s���O���[�h[eByEdge]���擾����", "Material MappingMode", MB_OK);     
		}     
		else 
		{         
			MessageBox(NULL, "...���Ή��̃}�b�s���O���[�h���擾����", "Material MappingMode", MB_OK);     
		}     
	} 
}

//--------------------------------------------------------------------------------------
//  �e�N�X�`�����̎擾������֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeTexture( FbxScene* lScene , int numberMaterial )
{
	//  �}�e���A���̐����̃��[�v
	for( int countMaterial = 0; countMaterial < numberMaterial; ++countMaterial )
	{
		// �}�e���A�����̎擾     
		FbxSurfaceMaterial* pMaterial = lScene->GetMaterial( countMaterial ); 

		//  �}�e���A�����̎擾
		std::string materialName = pMaterial->GetName( );

		// �f�B�t���[�Y���̎擾     
		FbxProperty diffuseProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sDiffuse ); 
 
		// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N     
		int layeredTextureCount = diffuseProperty.GetSrcObjectCount< FbxLayeredTexture >( ); 
 
		// ���C���[�h�e�N�X�`����������Βʏ�e�N�X�`��     
		if ( layeredTextureCount == 0 ) 
		{ 
			// �ʏ�e�N�X�`���̖������`�F�b�N         
			int textureCount = diffuseProperty.GetSrcObjectCount< FbxFileTexture >( ); 
   
			// �e�e�N�X�`���ɂ��ăe�N�X�`�������Q�b�g         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i�Ԗڂ̃e�N�X�`���I�u�W�F�N�g�擾         
				FbxFileTexture* pTexture = diffuseProperty.GetSrcObject< FbxFileTexture >( i ); 

				// �e�N�X�`���t�@�C�����̎擾         
				//std::string fileName = pTexture->GetFileName( );         
				std::string relFileName = pTexture->GetRelativeFileName( ); 
 
				// UVSet���̎擾         
				std::string uvSetName = pTexture->UVSet.Get().Buffer( ); 

				char* fileName = ( char* )relFileName.c_str( );
				char aTextureFileName[ 128 ] = { };

				int index = 0;

				//  ���������̃��[�v
				for( int nCntLength = strlen( fileName ) - 1; nCntLength >= 0 ; nCntLength-- )
				{
					//  �e�N�X�`�����̎��o��
					aTextureFileName[ index ] = fileName[ nCntLength ];

					index++;

					//  \������ƃ��[�v�𔲂��o��
					if( fileName[ nCntLength ] == '\\' )
					{
						break;
					}
				}

				//  \0�̊i�[
				aTextureFileName[ index ] = '\0';

				//  �C���f�b�N�X�ԍ��̏�����
				index = 0;

				char aWork;											//  �P�������̊i�[�p
				int	 nLength = strlen( aTextureFileName );			//  �����̒���

				//  ���������̃��[�v
				for( int nCntLength = 0; nCntLength < nLength / 2; nCntLength++ )
				{
					//  �e�N�X�`�����̔��]
					aWork = aTextureFileName[ nCntLength ];
					aTextureFileName[ nCntLength ] = aTextureFileName[ nLength - nCntLength - 1 ];
					aTextureFileName[ nLength - nCntLength - 1 ] = aWork;
				}

				std::string file = aTextureFileName;
				std::string sub = file.substr( 1 , file.size( ) - 1 );

				// �����o�^            
				std::string strPathName = "data/FBX/texture/" + sub;        

				//  �e�N�X�`���N���X�̎擾
				Texture* pTexture2 = SceneManager::GetTexture( );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�         
				m_textures.push_back( strPathName );    
			}         
		}     
		else 
		{ 
			// ���C���[�h�e�N�X�`������         
			MessageBox( NULL , "���C���[�h�e�N�X�`��" , "�}�e���A���̎擾" , MB_OK );     
		}     
	}
}

//--------------------------------------------------------------------------------------
//  ���b�V�����̒��_�o�b�t�@�𐶐�������֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::CreateMeshVertex( void )
{
	//  ���b�V���̐���
	for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
	{  
		m_pVtxBuff = nullptr;
		m_pVtxBuffers.push_back( m_pVtxBuff );

		//  ������( �܂胏���X�L���Ȃ� )
		MakeVertex( itm->indices.size( ) );
	}
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
// Get the geometry offset to a node. It is never inherited by the children. 
FbxAMatrix ObjectFBX::GetGeometry(FbxNode* pNode) 
{     
	const FbxVector4 lT = pNode->GetGeometricTranslation( FbxNode::eSourcePivot );     
	const FbxVector4 lR = pNode->GetGeometricRotation( FbxNode::eSourcePivot );     
	const FbxVector4 lS = pNode->GetGeometricScaling( FbxNode::eSourcePivot ); 
 
    return FbxAMatrix( lT , lR , lS ); 
} 
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeCluster( FbxScene* scene , FbxMesh* pMesh ) 
{     
	D3DXMATRIX mtxIdentitiy;     
	D3DXMatrixIdentity( &mtxIdentitiy ); 
 
    // �X�L���̐����擾     
	int skinCount = pMesh->GetDeformerCount( FbxDeformer::eSkin ); 
 
    for( int skinNum = 0; skinNum < skinCount; skinNum++ ) 
	{ 
        // �X�L�����擾         
		FbxSkin* pSkin = ( FbxSkin* )pMesh->GetDeformer( skinNum , FbxDeformer::eSkin ); 
 
        // �N���X�^�[�̐����擾         
		int clusterCount = pSkin->GetClusterCount( ); 
 
        for( int clusterNum = 0; clusterNum < clusterCount; clusterNum++ ) 
		{ 
			// �N���X�^���擾             
			FbxCluster* pCluster = pSkin->GetCluster( clusterNum ); 
 
			// ���̃N���X�^���e�����y�ڂ����_�C���f�b�N�X�̌����擾             
			int pointIndexCount = pCluster->GetControlPointIndicesCount( ); 
 
			m_meshes.back( ).matrixes.emplace_back( 0 );

			if( !pointIndexCount ) 
			{                 
				// ���̃��b�V���ɂ����āA���̃N���X�^�͖������Ă����Ǝv��...                 
				m_meshes.back( ).matrixes.back( ).push_back( mtxIdentitiy );     

				continue;             
			} 
 
			// �����p���s��̎擾             
			FbxAMatrix lReferenceGlobalInitPosition;             
			FbxAMatrix lReferenceGlobalCurrentPosition;             
			FbxAMatrix lReferenceGeometry;             
			FbxAMatrix lClusterGlobalInitPosition;             
			FbxAMatrix lClusterGlobalCurrentPosition;             
			FbxAMatrix lClusterRelativeInitPosition;             
			FbxAMatrix lClusterRelativeCurrentPositionInverse; 
 
            pCluster->GetTransformMatrix( lReferenceGlobalInitPosition );             
			
			// lReferenceGlobalCurrentPosition = pGlobalPosition; // <- ���Ԃ񃏁[���h���W�ϊ��s��ł͂Ȃ�����                          
			
			// Multiply lReferenceGlobalInitPosition by Geometric Transformation             
			lReferenceGeometry = GetGeometry( pMesh->GetNode( ) );             
			lReferenceGlobalInitPosition *= lReferenceGeometry; 
 
            // Get the link initial global position and the link current global position.             
			pCluster->GetTransformLinkMatrix( lClusterGlobalInitPosition );             
			
			// lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose); // <- �|�[�Y�s��̎����H�H�H             
			
			FbxTime oneFrameTime;
			oneFrameTime.SetTime( 0 , 0 , 0 , 1 , 0 , 0 , FbxTime::eFrames60 );

			//  �A�j���[�V�����ꗗ�̎擾
			FbxArray< FbxString* > animationNames;
			scene->FillAnimStackNameArray( animationNames );

			//  �A�j���[�V�������̎擾
			FbxTakeInfo* pTakeInfo = scene->GetTakeInfo( animationNames[ 0 ]->Buffer( ) );

			//  �A�j���[�V�����̊J�n�E�I�����Ԃ̎擾
			FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
			FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop( );

			//  �����t���[�����̎擾
			m_allTime = ( int )( ( endTime - startTime ) / oneFrameTime ).Get( );

			for( FbxTime currentTime = m_startTime; currentTime < m_endTime; currentTime += oneFrameTime )
			{
				lClusterGlobalCurrentPosition = pCluster->GetLink( )->EvaluateGlobalTransform( currentTime ); 
 
				// Compute the initial position of the link relative to the reference.             
				lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse( ) * lReferenceGlobalInitPosition; 
 
				// Compute the current position of the link relative to the reference.             
				lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse( ) * lClusterGlobalCurrentPosition; 
 
				// Compute the shift of the link relative to the reference.            
				FbxAMatrix VertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition; 
				// �� �����p���s����l�����ꂽ���[�V�����{�[���s��Ȃ̂ŁA����Œ��_���W��ϊ����邾���ŗǂ� 
 
				D3DXMATRIX d3dMtx; 
			
				for( int y = 0; y < 4; y++ ) 
				{                 
					for( int x = 0; x < 4; x++ ) 
					{ 
						d3dMtx( x , y ) = ( float )VertexTransformMatrix.Get( x , y );                 
					}  
				} 	
 
				m_meshes.back( ).matrixes.back( ).push_back( d3dMtx ); 
			}
 
			int* pointIndexArray = pCluster->GetControlPointIndices( );             
			double* weightArray = pCluster->GetControlPointWeights( ); 
 
			for( int i = 0 ; i < pointIndexCount; i++ ) 
			{                 
				m_meshes.back( ).points[ pointIndexArray[ i ] ].bornRefarences.push_back( BornRefarence( clusterNum ,
																										 static_cast< float >( weightArray[ i ] ) ) );            
			}     
		}		
	} 
}

//--------------------------------------------------------------------------------------
//  ���_�f�[�^�̕ϊ��֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::ConvertVertex( void )  
{
	for( unsigned int countMesh = 0; countMesh < m_meshes.size( ); ++countMesh )
	{
		for( unsigned int countIndex = 0; countIndex < m_meshes[ countMesh ].positionIndices.size( ); ++countIndex )
		{
			//  �ϊ����_�̑��
			Vertex vertex;
			vertex.bornReference = m_meshes[ countMesh ].points[ m_meshes[ countMesh ].positionIndices[ countIndex ] ].bornRefarences;
			vertex.vertex.position = m_meshes[ countMesh ].points[ m_meshes[ countMesh ].positionIndices[ countIndex ] ].positions;
			vertex.vertex.normal = m_meshes[ countMesh ].normals[ m_meshes[ countMesh ].normalIndices[ countIndex ] ];
			vertex.vertex.color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
			vertex.vertex.texcoord = m_meshes[ countMesh ].texcoords[ m_meshes[ countMesh ].texcoordIndices[ countIndex ] ];

			int index = ChangeVertexIndex( vertex , m_meshes[ countMesh ].vertexList );

			if( index == -1 )
			{
				//  ���̊i�[
				m_meshes[ countMesh ].vertexList.push_back( vertex );
				m_meshes[ countMesh ].indices.push_back( m_meshes[ countMesh ].vertexList.size( ) );
			}
			else
			{
				//  ���̊i�[
				m_meshes[ countMesh ].indices.push_back( index );
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  ���_�C���f�b�N�X�ԍ��̕ύX
//--------------------------------------------------------------------------------------
int	ObjectFBX::ChangeVertexIndex( Vertex &vertex , std::vector< Vertex > &vertexList )
{
	int		countIndex = 0;
	bool	checkIndex = true;

	for( auto it = vertexList.begin( ); it != vertexList.end( ); ++it  )
	{
		if( vertex.vertex.position == it->vertex.position && 
			vertex.vertex.normal == it->vertex.normal &&
			vertex.vertex.texcoord == it->vertex.texcoord &&
			vertex.bornReference.size( ) == it->bornReference.size( ) )
		{
			for( unsigned int countBorn1 = 0; countBorn1 < vertex.bornReference.size( ); ++countBorn1 )
			{
				for( unsigned int countBorn2 = 0; countBorn2 < it->bornReference.size( ); ++countBorn2 )
				{
					if( vertex.bornReference[ countBorn1 ].index != it->bornReference[ countBorn2 ].index ||
						vertex.bornReference[ countBorn1 ].weight != it->bornReference[ countBorn2 ].weight )
					{
						checkIndex = false;
					}
				}
			}
		}
		else
		{
			checkIndex = false;
		}

		if( checkIndex == true )
		{
			return countIndex;
		}

		countIndex++;
	}

	return -1;
}

//--------------------------------------------------------------------------------------
//  ���݂̃t�@�C���|�C���^���炠�镶����̐����J�E���g����֐�
//--------------------------------------------------------------------------------------
std::string ObjectFBX::GetAttributeTypeName(FbxNodeAttribute::EType type) 
{ 
    switch(type) { 
        case FbxNodeAttribute::eUnknown: return "unidentified"; 
        case FbxNodeAttribute::eNull: return "null"; 
        case FbxNodeAttribute::eMarker: return "marker"; 
        case FbxNodeAttribute::eSkeleton: return "skeleton"; 
        case FbxNodeAttribute::eMesh: return "mesh"; 
        case FbxNodeAttribute::eNurbs: return "nurbs"; 
        case FbxNodeAttribute::ePatch: return "patch"; 
        case FbxNodeAttribute::eCamera: return "camera"; 
        case FbxNodeAttribute::eCameraStereo: return "stereo"; 
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher"; 
        case FbxNodeAttribute::eLight: return "light"; 
        case FbxNodeAttribute::eOpticalReference: return "optical reference"; 
        case FbxNodeAttribute::eOpticalMarker: return "marker"; 
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve"; 
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface"; 
        case FbxNodeAttribute::eBoundary: return "boundary"; 
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface"; 
        case FbxNodeAttribute::eShape: return "shape"; 
        case FbxNodeAttribute::eLODGroup: return "lodgroup"; 
        case FbxNodeAttribute::eSubDiv: return "subdiv"; 
        default: return "unknown"; 
    } 
}

//--------------------------------------------------------------------------------------
//  ���_�o�b�t�@���쐬����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::MakeVertex( int size )
{     
	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ���_�o�b�t�@�̍쐬
	if( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * size ,				//  �쐬���������_�o�b�t�@�̃T�C�Y
											 D3DUSAGE_WRITEONLY ,				//  �g�p���@
											 FVF_VERTEX_3D ,					//  
											 D3DPOOL_MANAGED ,					//  �������Ǘ����@( MANAGED �� ���C�� )
											 &m_pVtxBuffers.back( ) ,			//  �o�b�t�@
											 NULL ) )
	{
		MessageBox( NULL , "���_�o�b�t�@�C���^�[�t�F�[�X�𐳂����擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );

		return;
	}
}

//--------------------------------------------------------------------------------------
//  ��������֐�
//--------------------------------------------------------------------------------------
ObjectFBX* ObjectFBX::Create( D3DXVECTOR3 position , float scale )
{
	ObjectFBX* objectFBX = new ObjectFBX;

	//  ���W���
	objectFBX->m_position = position;

	//  �傫���{�����
	objectFBX->m_scale = scale;

	//  ������
	objectFBX->Init( );

	return objectFBX;
}