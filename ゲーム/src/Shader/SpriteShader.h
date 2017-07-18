#ifndef __SpriteShader_h__
#define __SpriteShader_h__

//=========================================================================================
//
// 2D�`��V�F�[�_
//  ��YskLib��YsSprite�N���X�������͈����Ǝv���B2D�`��̎Q�l�p�Ƃ��Č��Ă��������B
//    2D�������ς��o���ꍇ��YsSprite�N���X��������(DirectX Tool Kit��SpriteBatch�@�\���g���Ă܂�)
// Sprite.hlsl���g�p���܂�
// 
//=========================================================================================
class SpriteShader {
public:
	//=============================================
	// �������E���
	//=============================================
	// ������
	void Init();
	// ���
	void Release();

	//=============================================
	// �`��n
	//=============================================

	// 2D�`��J�n
	void Begin();
	// 2D�`��I��
	void End();

	// 2D�`��
	// ��Begin()��ǂ�ł��Ȃ��ꍇ�͌�����������������̂ŁA��ʂɌĂт��ꍇ��Begin()�`End()�̊ԂŌĂԂ��Ƃ𐄏�
	void Draw2D(ID3D11ShaderResourceView* srcTex, float x, float y, float w, float h, const YsVec4* color = &YsVec4(1, 1, 1, 1));
	void Draw2D(YsTexture& srcTex, float x, float y, float w, float h, const YsVec4* color = &YsVec4(1, 1, 1, 1)){
		Draw2D(srcTex.GetTex(), x, y, w, h, color);
	}

	// 2D���`��
	void DrawLine2D(const YsVec3& p1, const YsVec3& p2, const YsVec4* color = &YsVec4(1, 1, 1, 1));

	//=============================================
	// ���̑�
	//=============================================

	// �R���p�C���ς݃V�F�[�_���t�@�C���ɕۑ�
	void SaveCsoToFile(){
		m_VS.SaveToFile();
		m_PS.SaveToFile();
	}

	// 
	SpriteShader() : m_bBegin(false){}
	~SpriteShader() { Release(); }

private:

	// �e�V�F�[�_�I�u�W�F�N�g
	YsVertexShader		m_VS;		// ���_�V�F�[�_
	YsPixelShader		m_PS;		// �s�N�Z���V�F�[�_

	// ���I���_�o�b�t�@�p(���`��ȂǂŎg�p)
	YsPolygon			m_poly;

	// �萔�o�b�t�@
	struct cbPerObject {
		YsVec4	m_Color;		// �F
	};
	YsConstantBuffer<cbPerObject>	m_cb1_PerObject;

	// �X�e�[�g
	YsDepthStencilState	m_dsBackUp;		// Depth�X�e�[�g�L���p

	// Flag
	bool			m_bBegin;
};

#endif
