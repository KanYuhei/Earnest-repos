//--------------------------------------------------------------------------------------
//  �V�F�[�_�[�v���O����   ( shader.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_H_
#define _SHADER_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �V�F�[�_�[�N���X�̒�`
//--------------------------------------------------------------------------------------
class Shader
{
public: 
	enum class TYPE
	{
		TYPE_NORMAL = 0 ,
		TYPE_MAX ,
	};

	Shader( ){ }										//  �R���X�g���N�^
	~Shader( ){ }										//  �f�X�g���N�^

	virtual void	Init( void ) = 0;					//  ������						
	virtual void	Uninit( void ) = 0;					//  �I��
	virtual void	Update( void ) = 0;					//  �X�V
	virtual void	Draw( void ) = 0;					//  �`��
private:
};

#endif