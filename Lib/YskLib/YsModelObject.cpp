#include "YskLib.h"

using namespace YskLib;


//====================================================================================================
//
// YsModelObject
//
//====================================================================================================
bool YsModelObject::LoadMesh(const std::string& filename)
{
	SPtr<YsGameModel> pSm = YsDx.GetResStg()->LoadMesh(filename);
	return SetModel(pSm);
}

bool YsModelObject::SetModel(SPtr<YsGameModel> pGameMesh, int animatorNum)
{
	// ���
	Release();

	// ���b�V����NULL�Ȃ牽�����Ȃ�
	if(pGameMesh == nullptr) {
		return false;
	}

	// YsGameModel�̃A�h���X�o�^
	m_pGameModel = pGameMesh;
	if (m_pGameModel->GetRoot() == nullptr)return false;

	// �{�[���c���[�쐬
	{
		// �ċA�֐�
		std::function<void(SPtr<YsGameModel::BoneNode>, SPtr<BoneNode>, int)> recCreateBoneTree = [this, &recCreateBoneTree](SPtr<YsGameModel::BoneNode> pBoneNode, SPtr<BoneNode> pBoneTreeNode, int Level)
		{

			// �c���[�ɓo�^
			m_BoneTree.push_back(pBoneTreeNode);

			// ���Z�b�g
			pBoneTreeNode->pSrcBoneNode = pBoneNode;
			pBoneTreeNode->Level = Level;

			// �q���ċA
			for (UINT i = 0; i<pBoneNode->Child.size(); i++) {
				// �q�o�^
				SPtr<BoneNode> ftn = std::make_shared<BoneNode>();
				ftn->Mother = pBoneTreeNode;
				pBoneTreeNode->Child.push_back(ftn);

				// �ċA
				recCreateBoneTree(pBoneNode->Child[i], ftn, Level + 1);
			}
		};
		// ���s
		recCreateBoneTree(m_pGameModel->GetBoneTree()[0], std::make_shared<BoneNode>(), 0);
	}

	// �Ƃ肠�����A�S�{�[���̃f�t�H���g�̕ϊ��s����R�s�[���Ƃ�
	for(UINT i=0;i<m_BoneTree.size();i++){
		m_BoneTree[i]->TransMat = m_BoneTree[i]->pSrcBoneNode->DefTransMat;
	}

	return true;
}

void YsModelObject::ResetDefaultTransMat()
{
	for(UINT i = 0; i<m_BoneTree.size(); i++) {
		m_BoneTree[i]->TransMat = m_BoneTree[i]->pSrcBoneNode->DefTransMat;
	}
}


void YsModelObject::CalcBoneMatrix(const YsMatrix* baseMat)
{
	if(m_BoneTree.empty())return;

	// �s��w�肪�Ȃ��ꍇ�́A�P�ʍs����g�p
	YsMatrix mTmp;
	if(baseMat == NULL)baseMat = &mTmp;

	// �S�{�[�����v�Z���Ă���
	recCalcBoneMatrix(*m_BoneTree[0], *baseMat);
}

void YsModelObject::CalcBoneMatrix_SkipRoot(const YsMatrix* baseMat)
{
	if (m_BoneTree.empty())return;

	// �s��w�肪�Ȃ��ꍇ�́A�P�ʍs����g�p
	YsMatrix mTmp;
	if (baseMat == NULL)baseMat = &mTmp;

	// Root���΂��A�S�{�[�����v�Z���Ă���
	for (UINT i = 0; i < m_BoneTree[0]->Child.size(); i++)
	{
		recCalcBoneMatrix(*m_BoneTree[0]->Child[i].get(), *baseMat);
	}
}

	void YsModelObject::recCalcBoneMatrix(BoneNode& node, const YsMatrix &parentMat)
	{
		// �v�Z
		// ���[���h�s��쐬
		D3DXMatrixMultiply(&node.WorldMat, &node.TransMat, &parentMat);

		// �q�ċA
		for(UINT i = 0; i<node.Child.size(); i++)
		{
			recCalcBoneMatrix(*node.Child[i].get(), node.WorldMat);
		}
	}

void YsModelObject::Release()
{
	m_BoneTree.clear();

	m_pGameModel = nullptr;
}

void YsModelObject::InitAnimator(YsAnimator& animator)
{
	// �A�j���[�^������
	animator.Init();

	// �A�j�����X�g�R�s�[
	if (m_pGameModel) {
		animator.CopyAnimationList(m_pGameModel->GetAnimeList());
	}

	// �t���[���A�j���f�[�^�\�z
	animator.ClearRefMatrix();
	for (UINT i = 0; i < m_BoneTree.size(); i++) {
		bool bBlendRootBone = (i == 0 ? false : true);	// Root�{�[���̂݁A�����g���b�N�u�����h�𖳌��ɂ���

		animator.AddRefMatrix(	&m_BoneTree[i]->TransMat, bBlendRootBone, bBlendRootBone, bBlendRootBone );
	}
}
