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
	// 解放
	Release();

	// メッシュがNULLなら何もしない
	if(pGameMesh == nullptr) {
		return false;
	}

	// YsGameModelのアドレス登録
	m_pGameModel = pGameMesh;
	if (m_pGameModel->GetRoot() == nullptr)return false;

	// ボーンツリー作成
	{
		// 再帰関数
		std::function<void(SPtr<YsGameModel::BoneNode>, SPtr<BoneNode>, int)> recCreateBoneTree = [this, &recCreateBoneTree](SPtr<YsGameModel::BoneNode> pBoneNode, SPtr<BoneNode> pBoneTreeNode, int Level)
		{

			// ツリーに登録
			m_BoneTree.push_back(pBoneTreeNode);

			// 情報セット
			pBoneTreeNode->pSrcBoneNode = pBoneNode;
			pBoneTreeNode->Level = Level;

			// 子を再帰
			for (UINT i = 0; i<pBoneNode->Child.size(); i++) {
				// 子登録
				SPtr<BoneNode> ftn = std::make_shared<BoneNode>();
				ftn->Mother = pBoneTreeNode;
				pBoneTreeNode->Child.push_back(ftn);

				// 再帰
				recCreateBoneTree(pBoneNode->Child[i], ftn, Level + 1);
			}
		};
		// 実行
		recCreateBoneTree(m_pGameModel->GetBoneTree()[0], std::make_shared<BoneNode>(), 0);
	}

	// とりあえず、全ボーンのデフォルトの変換行列もコピーしとく
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

	// 行列指定がない場合は、単位行列を使用
	YsMatrix mTmp;
	if(baseMat == NULL)baseMat = &mTmp;

	// 全ボーンを計算していく
	recCalcBoneMatrix(*m_BoneTree[0], *baseMat);
}

void YsModelObject::CalcBoneMatrix_SkipRoot(const YsMatrix* baseMat)
{
	if (m_BoneTree.empty())return;

	// 行列指定がない場合は、単位行列を使用
	YsMatrix mTmp;
	if (baseMat == NULL)baseMat = &mTmp;

	// Rootを飛ばし、全ボーンを計算していく
	for (UINT i = 0; i < m_BoneTree[0]->Child.size(); i++)
	{
		recCalcBoneMatrix(*m_BoneTree[0]->Child[i].get(), *baseMat);
	}
}

	void YsModelObject::recCalcBoneMatrix(BoneNode& node, const YsMatrix &parentMat)
	{
		// 計算
		// ワールド行列作成
		D3DXMatrixMultiply(&node.WorldMat, &node.TransMat, &parentMat);

		// 子再帰
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
	// アニメータ初期化
	animator.Init();

	// アニメリストコピー
	if (m_pGameModel) {
		animator.CopyAnimationList(m_pGameModel->GetAnimeList());
	}

	// フレームアニメデータ構築
	animator.ClearRefMatrix();
	for (UINT i = 0; i < m_BoneTree.size(); i++) {
		bool bBlendRootBone = (i == 0 ? false : true);	// Rootボーンのみ、複数トラックブレンドを無効にする

		animator.AddRefMatrix(	&m_BoneTree[i]->TransMat, bBlendRootBone, bBlendRootBone, bBlendRootBone );
	}
}
