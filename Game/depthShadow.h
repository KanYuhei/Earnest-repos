//--------------------------------------------------------------------------------------
//  �f�v�X�V���h�E   ( sceneModel.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _DEPTH_SHADOW_H_
#define _DEPTH_SHADOW_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  �f�v�X�V���h�E�N���X�̒�`
//--------------------------------------------------------------------------------------
class DepthShadow
{
public:
	DepthShadow(  );													//  �f�t�H���g�R���X�g���N�^		
	~DepthShadow( );													//  �f�X�g���N�^

	static HRESULT				Init( void );							//  ������
	static void					Uninit( void );							//  �I��
	static void					DrawClearBuffer( void );				//  �o�b�t�@�̃N���A
	static void					SetRendererTarget( void );				//  �����_�[�^�[�Q�b�g�̐ݒ�
	static void					SetDepthSerface( void );				//  �[�x�o�b�t�@�̐ݒ�
	static LPDIRECT3DTEXTURE9	GetRendereTargetTexture( void );		//  �����_�[�^�[�Q�b�g�e�N�X�`���̎擾

	static DepthShadow*			Create( void );							//  ����

private:
	static LPDIRECT3DTEXTURE9	m_depthShadowTexture;					//  �e�N�X�`��
	static LPDIRECT3DSURFACE9	m_depthShadowSurface;					//  �����_�[�^�[�Q�b�g�p�T�[�t�F�C�X
	static LPDIRECT3DSURFACE9	m_shadowMap;							//  �e�N�X�`���p�̐[�x�o�b�t�@�[
};

#endif