//--------------------------------------------------------------------------------------
//  �I�u�W�F�N�gFBX  ( objectFBX.cpp )
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
	FbxManager* lSdkManager = FbxManager::Create(); 
 
	// Create the IO settings object. 
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT); lSdkManager->SetIOSettings(ios); 
 
	// Create an importer using the SDK manager. 
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager,""); 
 
	// Use the first argument as the filename for the importer. 
	if(!lImporter->Initialize("data/FBX/male_model2.fbx", -1, lSdkManager->GetIOSettings())) 
	{      
		char buf[256];     
		sprintf(buf, "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" ,lImporter->GetStatus().GetErrorString());     
		MessageBox(NULL, buf, "error", MB_OK);     
		return E_FAIL; 
	}  
 
	// Create a new scene so that it can be populated by the imported file. 
	FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene"); 
 
	// Import the contents of the file into the scene. 
	lImporter->Import(lScene); 
 
	FbxGeometryConverter converter(lSdkManager); 
	
	// �O�p�|���S���� 
	converter.Triangulate(lScene, true); 
	
	// �k�ރ��b�V�����폜 
	converter.RemoveBadPolygonsFromMeshes(lScene); 
	
	// �}�e���A�����ƂɃ��b�V������ 
	converter.SplitMeshesPerMaterial(lScene, true); 

	//  �A�j���[�V�������̎擾
	lImporter->GetAnimStackCount( );

	//  �A�j���[�V�����ꗗ�̎擾
	FbxArray< FbxString* > animationNames;
	lScene->FillAnimStackNameArray( animationNames );

	//  �A�j���[�V�������̎擾
	FbxTakeInfo* pTakeInfo = lScene->GetTakeInfo( animationNames[ 0 ]->Buffer( ) );

	//  �A�j���[�V�����̊J�n�E�I�����Ԃ̎擾
	FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
	FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop( );
 
	//  ���b�V�����̎擾
	int numberMesh = lScene->GetMemberCount< FbxMesh >( );

	//  �}�e���A�����̎擾
	int numberMaterial = lScene->GetMaterialCount( );

	//  ���b�V���̐����̃��[�v
	for( int countMesh = 0; countMesh < numberMesh; ++countMesh )
	{
		//  �w�胁�b�V���̏��̎擾
		FbxMesh* mesh = lScene->GetSrcObject< FbxMesh >( countMesh );

		//  ���W�̎擾
		AnalyzePosition( mesh );

		//  �@�����                 
		AnalyzeNormal( mesh );                 
				
		// UV���                 
		AnalyzeTexcoord( mesh , true );   

		// �}�e���A����́i�Q�Ə��̎擾�j                 
		AnalyzeMaterial( mesh ); 

		//  �C���f�b�N�X
		FbxLayerElementArrayTemplate< int >* index;

		//  
		if( mesh->GetMaterialIndices( &index ) && index->GetCount( ) > 0 )
		{
			// �}�e���A�����̎擾     
			FbxSurfaceMaterial* material = mesh->GetNode( )->GetMaterial( index->GetAt( 0 ) ); 
		}
	}

	//  �}�e���A���̐����̃��[�v
	for( int countMaterial = 0; countMaterial < numberMaterial; ++countMaterial )
	{
		// �}�e���A�����̎擾     
		FbxSurfaceMaterial* pMaterial = lScene->GetMaterial( countMaterial ); 

		//  �}�e���A�����̎擾
		std::string materialName = pMaterial->GetName( );

		// �f�B�t���[�Y���̎擾     
		FbxProperty diffuseProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse); 
 
		// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N     
		int layeredTextureCount = diffuseProperty.GetSrcObjectCount<FbxLayeredTexture>(); 
 
		// ���C���[�h�e�N�X�`����������Βʏ�e�N�X�`��     
		if ( layeredTextureCount == 0 ) 
		{ 
			// �ʏ�e�N�X�`���̖������`�F�b�N         
			int textureCount = diffuseProperty.GetSrcObjectCount<FbxFileTexture>(); 
   
			// �e�e�N�X�`���ɂ��ăe�N�X�`�������Q�b�g         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i�Ԗڂ̃e�N�X�`���I�u�W�F�N�g�擾         
				FbxFileTexture* pTexture = diffuseProperty.GetSrcObject<FbxFileTexture>(i); 
 
				// �e�N�X�`���t�@�C�����̎擾         
				// std::string fileName = pTexture->GetFileName();         
				std::string relFileName = pTexture->GetRelativeFileName(); 
 
				// UVSet���̎擾         
				std::string uvSetName = pTexture->UVSet.Get().Buffer(); 
 
				// �����o�^         
				// texfile2UVset[relFileName] = uvSetName;         
				std::string strPathName = "data/dude/" + relFileName;        

				//  �e�N�X�`���N���X�̎擾
				Texture* pTexture2 = SceneManager::GetTexture( );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�         
				textures.push_back( strPathName );    
			}         
		}     
		else 
		{ 
			// ���C���[�h�e�N�X�`������         
			MessageBox(NULL, "���C���[�h�e�N�X�`��", "�}�e���A���̎擾", MB_OK);     
		}     
	}

	//  ���b�V���̐���
	for( auto itm = meshes.begin(); itm != meshes.end(); ++itm ) 
	{  
		if( itm->matrixes.empty() ) 
		{    
		}      
		else 
		{
			//  ������( �܂胏���X�L���Ȃ� )
			MakeVertex( itm->positionIndices.size( ) );
		}
	}

	lImporter->Destroy(); 
	lScene->Destroy(); 
	lSdkManager->Destroy(); 

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
	//  �ċA�I�ɕ`��
    recursiveDraw( ); 
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɕ`�揈��
//--------------------------------------------------------------------------------------
void ObjectFBX::recursiveDraw( void )
{ 
    for( auto itm = meshes.begin(); itm != meshes.end(); ++itm ) 
	{  
		if( itm->matrixes.empty() ) 
		{
			//// ���Ȃ��i�܂荄�̂̉�j   
			//for( size_t i = 0; i < itm->positionIndices.size(); i++ )
			//{ 
			//	if( !itm->normals.empty()   ) 
			//		glNormal3fv(itm->normals[itm->normalIndices[i]]);                 
			//		
			//	if( !itm->texcoords.empty() ) 
			//		glTexCoord2fv(itm->texcoords[itm->texcoordIndices[i]]);           
			//	
			//	glVertex3fv(itm->points[itm->positionIndices[i]].positions);            
			//}       
		}      
		else 
		{
			// ������i�܂胏���X�L���Ȃǁj 
 
			// ���_�̍��W�ϊ�             
			std::vector<D3DXVECTOR3> positions;            
			positions.reserve(itm->points.size()); 
 
			D3DXMATRIX mtx;             
				
			for( auto it = itm->points.begin(); it != itm->points.end(); ++it ) 
			{                
				ZeroMemory(&mtx, sizeof(D3DXMATRIX));   

				FbxTime allFrame = ( m_endTime - m_startTime );
				int nAllFrame = allFrame.GetFrameCount( );
					
				for( auto itb = it->bornRefarences.begin(); itb != it->bornRefarences.end(); ++itb ) 
				{                    
					//mtx += itm->matrixes[itb->index] * itb->weight;
					mtx += itm->matrixes[itb->index][ m_currentFrame % 73 ] * itb->weight;
				}

				D3DXVECTOR3 pos;                 
				D3DXVec3TransformCoord(&pos, &it->positions, &mtx);                
				positions.push_back(pos);             
			} 

			VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^

			if( m_pVtxBuff != NULL )
			{
				//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
				m_pVtxBuff->Lock( 0 , 0 ,									//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
								  ( void** )&pVtx ,							//  �A�h���X�������ꂽ�������̃A�h���X
								  0 );										//  ���b�N�̎��

				for( size_t i = 0; i < itm->positionIndices.size( ); i++ ) 
				{             
					//  ���_���W�̐ݒ�( 3D���W �E �E��� )
					pVtx[ i ].position = positions[ itm->positionIndices[ i ] ];

					//  �@���̎w��
					pVtx[ i ].normal = itm->normals[itm->normalIndices[ i ] ];

					//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
					pVtx[ i ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

					//  UV���W�̎w��
					pVtx[ i ].tex = itm->texcoords[ itm->texcoordIndices[ i ] ];
				}

				//  ���_�o�b�t�@�̃A�����b�N
				m_pVtxBuff->Unlock( );     
			}

			LPDIRECT3DDEVICE9 pDevice;

			D3DXMATRIX mtxWorld;							//  ���[���h�s��
			D3DXMATRIX mtxTrans;							//  ���s�ړ��s��
			D3DXMATRIX mtxScale;							//  �g��s��
			D3DXMATRIX mtxRot;								//  �r���[���W�ϊ��s��

			//  �f�o�C�X���̎擾
			pDevice = SceneManager::GetRenderer( )->GetDevice( );

			//  �e�N�X�`���N���X�̎擾
			Texture* pTexture = SceneManager::GetTexture( );

			D3DXMatrixIdentity( &mtxWorld );
			D3DXMatrixIdentity( &mtxScale );
			D3DXMatrixIdentity( &mtxRot );

			//  �g��s��̍쐬
			D3DXMatrixScaling( &mtxScale , 0.3f , 0.3f , 0.3f );

			//  �g��s��̊|���Z
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

			//  ���s�ړ��s��̍쐬
			D3DXMatrixTranslation( &mtxTrans , 0.0f , 0.0f , 0.0f );

			//  ���s�ړ��s��̊|���Z
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

			//  ���[���h���W�ϊ�
			pDevice->SetTransform( D3DTS_WORLD , &mtxWorld );

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource( 0 ,								//  �p�C�v���C���ԍ�
									  m_pVtxBuff ,						//  ���_�o�b�t�@�̃A�h���X
									  0 ,								//  �I�t�Z�b�g( byte )
									  sizeof( VERTEX_3D ) );			//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF( FVF_VERTEX_3D );

			std::string fileName = textures[ itm->materialIndex ];

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( textures[ itm->materialIndex ].c_str( ) ) ); 

			// �|���S���̕`��
			pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  �v���~�e�B�u�̎��
									0 ,										//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
									itm->positionIndices.size( ) / 3 );		//  �v���~�e�B�u��
		}
	}
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzePosition( FbxMesh* pMesh ) 
{     
	// �R���g���[���|�C���g���̎擾     
	int controlPointsCount = pMesh->GetControlPointsCount( ); 
 
    // ���_���W�p�R���e�i�̗̈�\��     
	meshes.back( ).points.reserve( controlPointsCount ); 
 
    // ���_�f�[�^�̎擾     
	FbxVector4* pP = pMesh->GetControlPoints( );     
	
	for( int i = 0; i < pMesh->GetControlPointsCount( ); i++ ) 
	{         
		meshes.back( ).points.push_back( D3DXVECTOR3( static_cast< float >( pP[ i ][ 0 ] ) ,                          
													  static_cast< float >( pP[ i ][ 1 ] ) ,                          
													  static_cast< float >( pP[ i ][ 2 ] ) ) );     
	} 
 
    /* ���_�C���f�b�N�X�̎擾 */ 
 
    // �C���f�b�N�X�����擾     
	int polygonVertexCount = pMesh->GetPolygonVertexCount( ); 
 
    // ���_���W�C���f�b�N�X�p�R���e�i�̗̈�\��     
	meshes.back( ).positionIndices.reserve( polygonVertexCount );          
	
	// �C���f�b�N�X�o�b�t�@�̎擾     
	for( int i = 0; i < polygonVertexCount; i++ )
	{         
		meshes.back( ).positionIndices.push_back( static_cast< unsigned short >( pMesh->GetPolygonVertices( )[ i ] ) );     
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
		meshes.back( ).normals.reserve( normalCount );
 
        // �@���f�[�^�̎擾         
		for( int i = 0; i < normalCount; i++ ) 
		{             
			meshes.back( ).normals.push_back( D3DXVECTOR3( static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 0 ] ) ,                 
														   static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 1 ] ) ,                 
														   static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 2 ] ) ) );         
		} 
 
        // �}�b�s���O���[�h�E���t�@�����X���[�h�擾         
		FbxLayerElement::EMappingMode mappingMode = pElementNormal->GetMappingMode( );         
		FbxLayerElement::EReferenceMode referenceMode = pElementNormal->GetReferenceMode( ); 
 
        switch(mappingMode) 
		{ 
			case FbxLayerElement::eNone:  
			{
				MessageBox(NULL, "Normal MappingMode = mappingMode", "������", MB_OK);      
				break; 
			}
			case FbxLayerElement::eByControlPoint: 
			{
				// ���_�o�b�t�@�Ɠ����C���f�b�N�X���g�p 
				if( referenceMode == FbxLayerElement::eDirect ) 
				{                 
					// �@���C���f�b�N�X�i�[�p�R���e�i�̗̈�\��                 
					meshes.back().normalIndices.reserve(meshes.back().points.size());                 
				
					// ���_�o�b�t�@�Ɠ����C���f�b�N�X���R�s�[                 
					meshes.back().normalIndices.assign( meshes.back().positionIndices.begin(), meshes.back().positionIndices.end());             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{                 
					MessageBox(NULL,  "Normal ReferenceMode = eIndexToDirect or eIndex, MappingMode = eByControlPoint",  "������", MB_OK);             
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
					meshes.back().normalIndices.reserve(normalCount);                 
				
					// �@���C���f�b�N�X�̍쐬                 
					for( int i = 0; i < normalCount; i++ ) 
					{                     
						meshes.back().normalIndices.push_back(i);                 
					}             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{ 
					// �Ǝ��C���f�b�N�X������ 
 
					// �C���f�b�N�X���̎擾                 
					int normalIndexCount = pElementNormal->GetIndexArray().GetCount();                 
				
					// �@���C���f�b�N�X�i�[�p�R���e�i�̗̈�\��                 
					meshes.back().normalIndices.reserve(normalIndexCount);                 
				
					// �@���C���f�b�N�X�̎擾                 
					for( int i = 0; i < normalIndexCount; i++ ) 
					{                     
						meshes.back().normalIndices.push_back(pElementNormal->GetIndexArray()[i]);                 
					}             
				}

				break; 
			}
			case FbxLayerElement::eByPolygon:       
			{
				MessageBox(NULL, "Normal MappingMode = eByPolygon", "������", MB_OK);             
				break;       
			}
			case FbxLayerElement::eByEdge: 
			{
				MessageBox(NULL, "Normal MappingMode = eByEdge", "������", MB_OK);             
				break;         
			}
			case FbxLayerElement::eAllSame:  
			{
				MessageBox(NULL, "Normal MappingMode = eAllSame", "������", MB_OK);            
				break;   
			}
			default: 
			{
				MessageBox(NULL, "Normal ???", "������", MB_OK);             
				break;         
			}     
		}
	} 
} 
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeTexcoord(FbxMesh* pMesh, bool bRevers) 
{ 
	int layerCount = pMesh->GetLayerCount(); 
 
	if( !layerCount ) 
	{ 
		MessageBox(NULL, "���C���[�������Ă��Ȃ����b�V�����m�F", "Analyze Texcoord", MB_OK); 
		return; 
	} 
 
	for( int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // UV�̎擾         
		FbxLayerElementUV* pElementUV = pMesh->GetLayer(layer)->GetUVs(); 
 
        if( !pElementUV ) 
		{             
			MessageBox(NULL, "...UV�̂Ȃ����b�V�����C���[���m�F", "Analyze Texcoord", MB_OK);             
			continue;         
		} 
 
        // UV�Z�b�g�����擾         
		// = pElementUV->GetName(); 
 
        // �}�b�s���O���[�h�E���t�@�����X���[�h�擾         
		FbxLayerElement::EMappingMode mappingMode = pElementUV->GetMappingMode();         
		FbxLayerElement::EReferenceMode referenceMode = pElementUV->GetReferenceMode(); 
 
        if( mappingMode == FbxLayerElement::eByPolygonVertex ) 
		{             
			if( referenceMode == FbxLayerElement::eIndexToDirect || referenceMode == FbxLayerElement::eIndex ) 
			{                 
				int uvIndexCount = pElementUV->GetIndexArray().GetCount();                
				meshes.back().texcoordIndices.reserve(uvIndexCount); 
 
                for( int i = 0; i < uvIndexCount; i++ ) 
				{                     
					meshes.back().texcoordIndices.push_back(pElementUV->GetIndexArray().GetAt(i));                 
				} 
 
                int uvCount = pElementUV->GetDirectArray().GetCount();      

				meshes.back().texcoords.reserve(uvCount); 
 
                for( int i = 0; i < uvCount; i++ ) 
				{ 
                    meshes.back().texcoords.push_back(D3DXVECTOR2( static_cast<float>(pElementUV->GetDirectArray().GetAt(i)[0]),                         
																   static_cast<float>(bRevers ? 1 - pElementUV->GetDirectArray().GetAt(i)[1] : pElementUV->GetDirectArray().GetAt(i)[1])));                 
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
			MessageBox(NULL, "...���Ή��}�b�s���O���[�h[eByControlPoint]���擾����", "Analyze Texcoord", MB_OK);         
		}         
		else if( mappingMode == FbxLayerElement::eByPolygon) 
		{             
			MessageBox(NULL, "...���Ή��}�b�s���O���[�h[eByPolygon]���擾����", "Analyze Texcoord", MB_OK);         
		}         
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{             
			MessageBox(NULL, "...���Ή��}�b�s���O���[�h[eByEdge]���擾����", "Analyze Texcoord", MB_OK);         
		}         
		else 
		{             
			MessageBox(NULL, "...�m��Ȃ��}�b�s���O���[�h���擾����", "Analyze Texcoord", MB_OK);         
		} 
 
        break; // �Ƃ肠�����P�߂���     
	} 
}
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeMaterial(FbxMesh* pMesh) 
{     
	int layerCount = pMesh->GetLayerCount(); 
 
    for(int layer = 0; layer < layerCount; layer++) 
	{ 
		FbxLayerElementMaterial* pElementMaterial = pMesh->GetLayer(layer)->GetMaterials();

		if( !pElementMaterial ) 
		{ 
			continue; 
		} 
 
		int materialIndexCount = pElementMaterial->GetIndexArray().GetCount(); 
 
		if( materialIndexCount == 0 ) 
		{ 
			continue; 
		} 
 
		FbxLayerElement::EMappingMode mappingMode = pElementMaterial->GetMappingMode();     
		FbxLayerElement::EReferenceMode referenceMode = pElementMaterial->GetReferenceMode(); 
 
		if( mappingMode == FbxLayerElement::eAllSame ) 
		{         
			if( referenceMode == FbxLayerElement::eIndexToDirect ) 
			{        
				// ���b�V���S�������̃}�e���A���C���f�b�N�X         
				meshes.back().materialIndex = pElementMaterial->GetIndexArray().GetAt(0);         
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
			meshes.back().materialIndex = pElementMaterial->GetIndexArray().GetAt(0);     
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
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
// Get the geometry offset to a node. It is never inherited by the children. 
FbxAMatrix ObjectFBX::GetGeometry(FbxNode* pNode) 
{     
	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);     
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);     
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot); 
 
    return FbxAMatrix(lT, lR, lS); 
} 
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void ObjectFBX::AnalyzeCluster(FbxMesh* pMesh) 
{     
	D3DXMATRIX mtxIdentitiy;     
	D3DXMatrixIdentity(&mtxIdentitiy); 
 
    // �X�L���̐����擾     
	int skinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin); 
 
    for( int skinNum = 0; skinNum < skinCount; skinNum++ ) 
	{ 
        // �X�L�����擾         
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(skinNum, FbxDeformer::eSkin); 
 
        // �N���X�^�[�̐����擾         
		int clusterCount = pSkin->GetClusterCount(); 
 
        for( int clusterNum = 0; clusterNum < clusterCount; clusterNum++ ) 
		{ 
			// �N���X�^���擾             
			FbxCluster* pCluster = pSkin->GetCluster(clusterNum); 
 
			// ���̃N���X�^���e�����y�ڂ����_�C���f�b�N�X�̌����擾             
			int pointIndexCount = pCluster->GetControlPointIndicesCount(); 
 
			meshes.back( ).matrixes.emplace_back( 0 );

			if( !pointIndexCount ) 
			{                 
				// ���̃��b�V���ɂ����āA���̃N���X�^�͖������Ă����Ǝv��...                 
				meshes.back().matrixes.back().push_back(mtxIdentitiy);         

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
 
            pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);             
			
			// lReferenceGlobalCurrentPosition = pGlobalPosition; // <- ���Ԃ񃏁[���h���W�ϊ��s��ł͂Ȃ�����                          
			
			// Multiply lReferenceGlobalInitPosition by Geometric Transformation             
			lReferenceGeometry = GetGeometry(pMesh->GetNode());             
			lReferenceGlobalInitPosition *= lReferenceGeometry; 
 
            // Get the link initial global position and the link current global position.             
			pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);             
			
			// lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose); // <- �|�[�Y�s��̎����H�H�H             
			
			FbxTime oneFrameTime;
			oneFrameTime.SetTime( 0 , 0 , 0 , 1 , 0 , 0 , FbxTime::eFrames60 );

			for( FbxTime currentTime = m_startTime; currentTime < m_endTime; currentTime += oneFrameTime )
			{
				lClusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(currentTime); 
 
				// Compute the initial position of the link relative to the reference.             
				lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition; 
 
				// Compute the current position of the link relative to the reference.             
				lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition; 
 
				// Compute the shift of the link relative to the reference.            
				FbxAMatrix VertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition; 
				// �� �����p���s����l�����ꂽ���[�V�����{�[���s��Ȃ̂ŁA����Œ��_���W��ϊ����邾���ŗǂ� 
 
				D3DXMATRIX d3dMtx; 
			
				for( int y = 0; y < 4; y++ ) 
				{                 
					for( int x = 0; x < 4; x++ ) 
					{ 
						d3dMtx(x, y) = (float)VertexTransformMatrix.Get(x, y);                 
					}  
				} 	
 
				meshes.back().matrixes.back().push_back(d3dMtx); 
			}
 
			int* pointIndexArray = pCluster->GetControlPointIndices();             
			double* weightArray = pCluster->GetControlPointWeights(); 
 
			for( int i = 0 ; i < pointIndexCount; i++ ) 
			{                 
				meshes.back().points[pointIndexArray[i]].bornRefarences.push_back(BornRefarence(clusterNum, static_cast<float>(weightArray[i])));            
			}     
		}		
	} 
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
	m_pVtxBuff = nullptr;

	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ���_�o�b�t�@�̍쐬
	if( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * size ,				//  �쐬���������_�o�b�t�@�̃T�C�Y
											 D3DUSAGE_WRITEONLY ,				//  �g�p���@
											 FVF_VERTEX_3D ,					//  
											 D3DPOOL_MANAGED ,					//  �������Ǘ����@( MANAGED �� ���C�� )
											 &m_pVtxBuff ,						//  �o�b�t�@
											 NULL ) )
	{
		MessageBox( NULL , "���_�o�b�t�@�C���^�[�t�F�[�X�𐳂����擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );

		return;
	}
}