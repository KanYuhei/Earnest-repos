//--------------------------------------------------------------------------------------
//  �e�X�g�v���O����   ( title.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "test.h"
#include "manager.h"
#include "sceneBG.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "fade.h"
#include "renderer.h"
#include "field.h"
#include "meshDome.h"
#include "titlePress.h"
#include "effekseerManager.h"
#include "meshField.h"
#include "objectFBX.h"
#include "sceneFBX.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
#define TITLE_PRESS_POS_X				( SCREEN_WIDTH * 0.5f )				//  ���000���WX
#define TITLE_PRESS_POS_Y				( SCREEN_HEIGHT * 0.8f )			//  ���000���WY
#define TITLE_PRESS_SIZE_X				( 580.0f )							//  ���000�T�C�YX
#define TITLE_PRESS_SIZE_Y				( 120.0f )							//  ���000�T�C�YY

static const float	ATMOSPHERE_SIZE = 15.0f; 
static const int	ATMOSPHERE_NUM = 1;

//--------------------------------------------------------------------------------------
//  �C���X�^���X����
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �e�X�g�N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
Test::Test( )
{

}

//--------------------------------------------------------------------------------------
//  �e�X�g�N���X�̃f�X�g���N�^
//--------------------------------------------------------------------------------------
Test::~Test( )
{

}

//--------------------------------------------------------------------------------------
//  �e�X�g�N���X�̏���������
//--------------------------------------------------------------------------------------
void Test::Init( void )
{
	//  �N���X�|�C���^�̏�����
	m_pCamera = NULL;

	//  �J�����N���X�|�C���^����̏ꍇ
	if( m_pCamera == NULL )
	{
		//  �J�����N���X�̐���
		m_pCamera = new Camera;
		m_pCamera->Init( D3DXVECTOR3( 0.0f , 10.0f , -50.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
						 D3DX_PI / 3.0f , 1.0f , 10000.0f );
		m_pCamera->ChangeMode( );
	}

	//  ���b�V���h�[���̐���
	MeshDome::Create( MeshDome::TYPE_COSMO ,
					  D3DXVECTOR3( 0.0f , -1000.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
					  D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( 0.0001f , 0.0f ) ,
					  32 , 32 );

	//  ���b�V���t�B�[���h�̐���
	MeshField::Create( MeshField::TYPE_FIELD , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
					   D3DXVECTOR3( 1000.0f , 0.0f , 1000.0f ) , D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) , 10 , 10 , 
					   D3DXVECTOR2( 0.0f , 0.0f ) );

	//  FBX�̐���
	//ObjectFBX* fbx = new ObjectFBX;
	//fbx->Init( );
	SceneFBX* fbx2 = new SceneFBX;
	fbx2->Init( );

	for( int i = 0; i < ATMOSPHERE_NUM; i++ )
	{
		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		/////////////////////////////////////////////////////
		//  ��1���C��
		/////////////////////////////////////////////////////

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE , 
								  D3DXVECTOR3( 0.0f , 0.0f , -500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 0.0f , 0.0f , 500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -500.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 500.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -500.0f , 0.0f , -500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -500.0f , 0.0f , 500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 500.0f , 0.0f , -500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 500.0f , 0.0f , 500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		/////////////////////////////////////////////////////
		//  ��2���C��
		/////////////////////////////////////////////////////

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE , 
								  D3DXVECTOR3( 0.0f , 0.0f , -250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 0.0f , 0.0f , 250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -250.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 250.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -250.0f , 0.0f , -250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -250.0f , 0.0f , 250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 250.0f , 0.0f , -250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 250.0f , 0.0f , 250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );
	}
}

//--------------------------------------------------------------------------------------
//  �e�X�g�N���X�̏I������
//--------------------------------------------------------------------------------------
void Test::Uninit( void )
{
	//  �I�u�W�F�N�g�N���X�̑S���
	Scene::ReleaseAll( );

	//  �J�����N���X�|�C���^����ł͂Ȃ��ꍇ
	if( m_pCamera != NULL )
	{
		//  �J�����N���X�̔j��
		m_pCamera->Uninit( );
		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  �e�X�g�N���X�̍X�V����
//--------------------------------------------------------------------------------------
void Test::Update( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard*			pKeyboard = SceneManager::GetKeyboard( );

	//  Xbox�R���g���[���[���̎擾
	XboxController*	pXInput = SceneManager::GetXInput( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller*		pPS4Input = SceneManager::GetPS4Input( );

	//  �J�����N���X�|�C���^����ł͂Ȃ��ꍇ
	if( m_pCamera != NULL )
	{
		m_pCamera->Update( );
		m_pCamera->SetCamera( );
	}

	if( Fade::GetFade( ) == Fade::FADE_NONE )
	{
		if( ( pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_CIRCLE ) || pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_OPTIONS ) ) ||
			 pKeyboard->GetKeyboardTrigger( DIK_RETURN ) || pKeyboard->GetKeyboardTrigger( DIK_SPACE ) )
		{
			//  �t�F�[�h�̐ݒ�
			Fade::SetFade( Fade::FADE_OUT , Mode::MODE_STAGE_SELECT , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
		}
	}
}

//--------------------------------------------------------------------------------------
//  �e�X�g�N���X�̕`�揈��
//--------------------------------------------------------------------------------------
void Test::Draw( void )
{
	Scene::DrawAll( );
}