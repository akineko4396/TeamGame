#include "YskLib.h"

using namespace YskLib;

// CAllocateHierarchy用ボーンデータ
class YsD3DXFRAME_EX : public D3DXFRAME {

};

// CAllocateHierarchy用メッシュコンテナデータ
class YsD3DXMESHCONTAINER_EX : public D3DXMESHCONTAINER {

};

//=================================================================================
// スキンメッシュXファイル読み込みクラス
//
// 読み込んだアニメーション付きXファイルの階層情報を元にフレーム(ボーン)情報や	\n
// メッシュコンテナオブジェクトの生成をするクラス
//
//=================================================================================
class YsAllocateHierarchy : public ID3DXAllocateHierarchy {
public:

	struct TmpMcData {
		YsD3DXMESHCONTAINER_EX*	pMC;
		SPtr<YsSingleModel>		pModel;
	};
	std::vector<TmpMcData>		m_TmpMcTbl;

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);


	// 
	YsAllocateHierarchy(const std::string& filePath) {
		m_FilePath = filePath;
	}


private:
	std::string			m_FilePath;

};


static bool CreateMeshFromD3DXMesh(YsMesh* outMesh, const LPD3DXMESH pMesh, UINT materialCnt, const LPD3DXSKININFO pSkinInfo, const std::vector<SPtr<YsGameModel::BoneNode>>& boneTree);
static void CreateAnimationFromAC(LPD3DXANIMATIONCONTROLLER pAC, const std::vector<std::string>& boneNameTbl, std::vector<SPtr<YsAnimationSet>>& outAnimeList);



// 
bool YskLib::YsLoadXFile(
	const std::string& fileName,
	std::vector<SPtr<YsSingleModel>>& outModelTbl,
	std::vector<SPtr<YsGameModel::BoneNode>>& outBoneTree,
	std::vector<SPtr<YsAnimationSet>>& outAnimeTbl
)
{
	// パス、ファイル名抽出
	char szDir  [_MAX_PATH]; 
	char szFname[_MAX_FNAME]; 
	char szExt  [_MAX_EXT]; 
	_splitpath( fileName.c_str(), nullptr, szDir, szFname, szExt);
//	m_FileName = szFname;
//	m_FileName += szExt;
	std::string filePath = szDir;

	// Dreict3D9を使用して読み込み
	LPD3DXANIMATIONCONTROLLER pAC;
	YsAllocateHierarchy Alloc(filePath);
	// 読み込み
	YsD3DXFRAME_EX* lpFrameRoot;
	HRESULT hr = D3DXLoadMeshHierarchyFromX(fileName.c_str(), D3DXMESH_MANAGED, YsDx.GetD3D9Dev(), &Alloc, nullptr, (LPD3DXFRAME*)&lpFrameRoot, &pAC);

	// 作成失敗
	if(lpFrameRoot == nullptr)return FALSE;

	// ボーン配列作成 & DefWorldMat算出
	//  ※再帰時のラムダは、&でキャプチャする必要あり
	std::function<void(YsD3DXFRAME_EX*, SPtr<YsGameModel::BoneNode>, int)> recCreateFrameTree = [&outBoneTree, &recCreateFrameTree](YsD3DXFRAME_EX* pFrame, SPtr<YsGameModel::BoneNode> pBoneNode, int Level)
	{

		// ツリーに登録
		outBoneTree.push_back(pBoneNode);

		// 情報セット
		pBoneNode->BoneName = pFrame->Name;
		pBoneNode->Level = Level;
		pBoneNode->DefTransMat = pFrame->TransformationMatrix;
		if (pBoneNode->pMother.expired() == false) {
			pBoneNode->DefWorldMat = pBoneNode->DefTransMat * pBoneNode->pMother.lock()->DefWorldMat;
		}

		// 子を再帰
		if (pFrame->pFrameFirstChild != nullptr)
		{
			YsD3DXFRAME_EX *frame = (YsD3DXFRAME_EX*)pFrame->pFrameFirstChild;
			while (frame != nullptr) {
				// 子登録
				SPtr<YsGameModel::BoneNode> bnode = std::make_shared<YsGameModel::BoneNode>();
				bnode->pMother = pBoneNode;
				pBoneNode->Child.push_back(bnode);

				// 再帰
				recCreateFrameTree(frame, bnode, Level + 1);

				// 次へ
				frame = (YsD3DXFRAME_EX*)frame->pFrameSibling;
			}
		}
	};

	recCreateFrameTree(lpFrameRoot, std::make_shared<YsGameModel::BoneNode>(), 0);

	// アニメーションコントローラから独自アニメーション情報作成
	{
		std::vector<std::string> boneNameTbl(outBoneTree.size());
		for (UINT i = 0; i < outBoneTree.size(); i++) {
			boneNameTbl[i] = outBoneTree[i]->BoneName;
		}

		CreateAnimationFromAC(pAC, boneNameTbl, outAnimeTbl);

		YsSafe_Release(pAC);				// アニメーションコントローラはもういらん
	}

	//======================================================
	// メッシュデータ構築
	//======================================================
	for(UINT i=0;i<Alloc.m_TmpMcTbl.size();i++){
		YsD3DXMESHCONTAINER_EX* mc = Alloc.m_TmpMcTbl[i].pMC;
		SPtr<YsSingleModel> pModel = Alloc.m_TmpMcTbl[i].pModel;

		// メッシュデータ作成
		SPtr<YsMesh> pMeshData = std::make_shared<YsMesh>();
		CreateMeshFromD3DXMesh(pMeshData.get(), mc->MeshData.pMesh, pModel->GetMaterials().size(), mc->pSkinInfo, outBoneTree);	// Direct3DのMesh型から、独自のメッシュ(YsMesh型)作成

		// データ登録
		pModel->SetName(mc->Name == nullptr ? "" : mc->Name);
		pModel->SetMesh(pMeshData);
		outModelTbl.push_back(pModel);

		// スキン情報から、Offset行列を取得
		if(mc->pSkinInfo){
			UINT cBones = mc->pSkinInfo->GetNumBones();	// このメッシュに影響を受けるボーン数
			for (UINT iBone = 0; iBone < cBones; iBone++){
				// ボーン内から同じ名前のボーンを検索
				for(UINT Idx = 0; Idx<outBoneTree.size(); Idx++) {
					if(strcmp(mc->pSkinInfo->GetBoneName(iBone), outBoneTree[Idx]->BoneName.c_str()) == 0) {
						// Offset行列
						outBoneTree[Idx]->OffsetMat = *(mc->pSkinInfo->GetBoneOffsetMatrix(iBone));

						break;
					}
				}
			}
		}
	}

	// スキンメッシュとスタティックメッシュの参照を分けたテーブル作成
//	CreateRefMeshTbl();

	// OffsetID & OffsetMatが単位行列の奴は作成
	for(UINT Idx = 0; Idx<outBoneTree.size(); Idx++) {
		outBoneTree[Idx]->OffsetID = Idx;

		if(D3DXMatrixIsIdentity(&outBoneTree[Idx]->OffsetMat)) {
			D3DXMatrixInverse(&outBoneTree[Idx]->OffsetMat, nullptr, &outBoneTree[Idx]->DefWorldMat);
		}
	}

	// 元のデータは削除
	if(outBoneTree.size() > 0) {
		YsAllocateHierarchy Alloc(filePath);
		D3DXFrameDestroy( lpFrameRoot, &Alloc );
	}

	return true;
}




//=============================================================================
// CAllocateHierarchy
//=============================================================================
//=====================================================================
// フレーム又はメッシュに使用する名前用のメモリを確保
//=====================================================================
static HRESULT AllocateName(LPCSTR Name, LPSTR *pNewName)
{
	UINT cbLength;

	if(Name != nullptr) {
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if(*pNewName == nullptr) {
			return E_OUTOFMEMORY;
		}
		memcpy(*pNewName, Name, cbLength*sizeof(CHAR));
	}
	else {
		// *pNewName = nullptr;

		// 最低でも""を入れる
		*pNewName = new CHAR[1];
		if(*pNewName == nullptr) {
			return E_OUTOFMEMORY;
		}
		*pNewName[0] = '\0';
	}

	return S_OK;
}

//=====================================================================
// フレーム(ボーン)を作成
//=====================================================================
HRESULT YsAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	YsD3DXFRAME_EX *pFrame;

	auto ExitProc = [&] {
		YsSafe_Delete(pFrame);
	};

	*ppNewFrame = nullptr;

	pFrame = new YsD3DXFRAME_EX;
	if(pFrame == nullptr) {
		hr = E_OUTOFMEMORY;
		ExitProc();
		return hr;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr)) {
		ExitProc();
		return hr;
	}

	// その他
	//	pFrame->OffsetID = 0xFFFFFFFF;
	//	D3DXMatrixIdentity(&pFrame->OffsetMat);

	// initialize other data members of the frame
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);

	pFrame->pMeshContainer = nullptr;
	pFrame->pFrameSibling = nullptr;
	pFrame->pFrameFirstChild = nullptr;

	*ppNewFrame = pFrame;
	pFrame = nullptr;

	ExitProc();

	return hr;
}

//=====================================================================
// メッシュコンテナ作成
//=====================================================================
HRESULT YsAllocateHierarchy::CreateMeshContainer(
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	HRESULT hr;
	YsD3DXMESHCONTAINER_EX *pMeshContainer = nullptr;
	UINT NumFaces;
	LPDIRECT3DDEVICE9 pd3dDevice = nullptr;

	LPD3DXMESH pMesh = nullptr;
	*ppNewMeshContainer = nullptr;
	std::vector<YsMaterial> mateTbl;

	auto ExitProc = [&]() {
		YsSafe_Release(pd3dDevice);

		// call Destroy function to properly clean up the memory allocated 
		if (pMeshContainer != nullptr) {
			DestroyMeshContainer(pMeshContainer);
		}
	};


	// YsSingleModel作成
	SPtr<YsSingleModel> pModel = std::make_shared<YsSingleModel>();

	// this sample does not handle patch meshes, so fail when one is found
	if(pMeshData->Type != D3DXMESHTYPE_MESH) {
		hr = E_FAIL;
		ExitProc();
		return hr;
	}

	// メッシュデータ構造からpMeshインタフェースポインタを得る
	pMesh = pMeshData->pMesh;

	// this sample does not FVF compatible meshes, so fail when one is found
	if(pMesh->GetFVF() == 0) {
		hr = E_FAIL;
		ExitProc();
		return hr;
	}

	// メッシュコンテナメモリ確保
	pMeshContainer = new YsD3DXMESHCONTAINER_EX;
	if(pMeshContainer == nullptr) {
		hr = E_OUTOFMEMORY;
		ExitProc();
		return hr;
	}

	// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
	hr = AllocateName(Name, &pMeshContainer->Name);
	if(FAILED(hr)) {
		ExitProc();
		return hr;
	}

	pMesh->GetDevice(&pd3dDevice);
	NumFaces = pMesh->GetNumFaces();	// 面数取得

	// メッシュが法線を持たない場合は法線を追加する
	if(!(pMesh->GetFVF() & D3DFVF_NORMAL)) {
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// clone the mesh to make room for the normals
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pMeshContainer->MeshData.pMesh);
		if(FAILED(hr)) {
			ExitProc();
			return hr;
		}

		pMesh = pMeshContainer->MeshData.pMesh;

		D3DXComputeNormals(pMesh, nullptr);
	}
	else {
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	// 隣接面データ作成
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	if(pMeshContainer->pAdjacency == nullptr) {
		hr = E_OUTOFMEMORY;
		ExitProc();
		return hr;
	}
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);

	// 元からあるマテリアルデータは使わん
	pMeshContainer->NumMaterials = 0;
	pMeshContainer->pMaterials = nullptr;

	// メッシュコンテナリストに追加
	{
		TmpMcData tmpMcData;
		tmpMcData.pMC = pMeshContainer;
		tmpMcData.pModel = pModel;
		m_TmpMcTbl.push_back(tmpMcData);
	}

	// マテリアル・テクスチャ
	mateTbl.resize(max(1, NumMaterials));
	for(UINT ma = 0; ma<NumMaterials; ma++) {
		// 材質データ
		mateTbl[ma].Diffuse = (YsVec4&)pMaterials[ma].MatD3D.Diffuse;
		mateTbl[ma].Ambient = (YsVec3&)pMaterials[ma].MatD3D.Diffuse;	// XファイルにはAmbientはないので、Diffuseをそのまま使用
		mateTbl[ma].Specular = (YsVec4&)pMaterials[ma].MatD3D.Specular;
		mateTbl[ma].Power = pMaterials[ma].MatD3D.Power;
		mateTbl[ma].Emissive = (YsVec4&)pMaterials[ma].MatD3D.Emissive;

		// テクスチャ読み込み
		if(pMaterials[ma].pTextureFilename) {
			mateTbl[ma].TextureFileName = pMaterials[ma].pTextureFilename;
		} else {
			mateTbl[ma].TextureFileName = "";
		}
		std::string strFile = m_FilePath + mateTbl[ma].TextureFileName;

		mateTbl[ma].TexSet.LoadTextureSet(strFile);
	}
	// マテリアルが無い場合は適当に作成
	if(NumMaterials == 0) {
		// 材質データ
		mateTbl[0].Diffuse = YsVec4(1, 1, 1, 1);
		mateTbl[0].Ambient = YsVec3(1, 1, 1);
		mateTbl[0].Specular = YsVec4(1, 1, 1, 1);
		mateTbl[0].Power = 100;
		mateTbl[0].Emissive = YsVec4(0, 0, 0, 0);

		mateTbl[0].TexSet.LoadTextureSet("");
	}
	pModel->SetMaterials(mateTbl);

	// スキンメッシュに場合
	if(pSkinInfo != nullptr) {
		pMeshContainer->pSkinInfo = pSkinInfo;	// スキニング情報コピー
		pSkinInfo->AddRef();
	}
	// スタティックメッシュ
	else {
		pMeshContainer->pSkinInfo = nullptr;	// スキニング情報なし
	}

	// ローカルに生成したメッシュコンテナーを呼び出し側にコピーする
	pMeshContainer->pEffects = nullptr;
	pMeshContainer->pNextMeshContainer = nullptr;
	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = nullptr;


	ExitProc();

	return hr;
}

//=====================================================================
// フレーム(ボーン)削除
//=====================================================================
HRESULT YsAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	YsD3DXFRAME_EX* p = (YsD3DXFRAME_EX*)pFrameToFree;

	YsSafe_Delete_Array(p->Name);
	YsSafe_Delete(p);

	return S_OK;
}

//=====================================================================
// メッシュコンテナ削除
//=====================================================================
HRESULT YsAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	YsD3DXMESHCONTAINER_EX *pMeshContainer = (YsD3DXMESHCONTAINER_EX*)pMeshContainerBase;

	YsSafe_Delete_Array(pMeshContainer->Name);
	YsSafe_Delete_Array(pMeshContainer->pAdjacency);
	YsSafe_Delete_Array(pMeshContainer->pMaterials);
	YsSafe_Release(pMeshContainer->MeshData.pMesh);
	YsSafe_Release(pMeshContainer->pSkinInfo);
	YsD3DXMESHCONTAINER_EX* pMC = (YsD3DXMESHCONTAINER_EX*)pMeshContainer;
	YsSafe_Delete(pMC);

	return S_OK;
}

//==================================================
// LPD3DXMESH形式のメッシュから、YsMesh作成。
//==================================================
static bool CreateMeshFromD3DXMesh(YsMesh* outMesh, const LPD3DXMESH pMesh, UINT materialCnt, const LPD3DXSKININFO pSkinInfo, const std::vector<SPtr<YsGameModel::BoneNode>>& boneTree)
{
	outMesh->Release();

	auto SearchBoneIndex = [&boneTree](const std::string& bonename) -> int {
		for (UINT i = 0; i < boneTree.size(); i++) {
			if (boneTree[i]->BoneName == bonename) {
				return i;
			}
		}
		return -1;
	};

	//==============================================
	// アトリビュートテーブルよりサブセット作成
	//==============================================
	std::vector<YsMeshSubset> subset;
	{
		DWORD sz;
		pMesh->GetAttributeTable(nullptr, &sz);
		std::vector<D3DXATTRIBUTERANGE> attrTbl(sz);
		pMesh->GetAttributeTable(&attrTbl[0], &sz);

		// マテリアルの数ぶん、サブセットを作成
		subset.resize(materialCnt);
		memset(&subset[0], 0, sizeof(YsMeshSubset)*materialCnt);
		for(UINT i = 0; i<sz; i++) {
			int attrNo = attrTbl[i].AttribId;
			subset[attrNo].AttribId = attrTbl[i].AttribId;
			subset[attrNo].FaceStart = attrTbl[i].FaceStart;
			subset[attrNo].FaceCount = attrTbl[i].FaceCount;
		}
	}

	//==============================================
	// 頂点データの用意
	//==============================================
	// 頂点の各要素のオフセット算出
	WORD offset_pos = -1;
	WORD offset_tangent = -1;
	WORD offset_binormal = -1;
	WORD offset_normal = -1;
	WORD offset_texcoord = -1;
	WORD offset_blendweight = -1;
	WORD offset_blendindex = -1;
	D3DVERTEXELEMENT9 elem[MAX_FVF_DECL_SIZE];
	pMesh->GetDeclaration(elem);
	for(int el = 0;; el++) {
		if(elem[el].Stream == 0xFF) {
			break;
		}
		else if(elem[el].Usage == D3DDECLUSAGE_POSITION) {
			offset_pos = elem[el].Offset;
		}
		else if(elem[el].Usage == D3DDECLUSAGE_TANGENT) {
			offset_tangent = elem[el].Offset;
		}
		else if(elem[el].Usage == D3DDECLUSAGE_BINORMAL) {
			offset_binormal = elem[el].Offset;
		}
		else if(elem[el].Usage == D3DDECLUSAGE_NORMAL) {
			offset_normal = elem[el].Offset;
		}
		else if(elem[el].Usage == D3DDECLUSAGE_TEXCOORD) {
			offset_texcoord = elem[el].Offset;
		}
		else if(elem[el].Usage == D3DDECLUSAGE_BLENDWEIGHT) {
			offset_blendweight = elem[el].Offset;
		}
		else if(elem[el].Usage == D3DDECLUSAGE_BLENDINDICES) {
			offset_blendindex = elem[el].Offset;
		}
	}

	int numVerts = pMesh->GetNumVertices();
	int numFaces = pMesh->GetNumFaces();
	int verByteSize = pMesh->GetNumBytesPerVertex();// 1頂点のサイズ

	// スキンメッシュ
	if(pSkinInfo) {
		// 頂点形式データ取得
		YsVertexTypeData vtd = YsVertex_Pos_UV_TBN_Skin::GetVertexTypeData();
		std::vector<YsVertex_Pos_UV_TBN_Skin> verTbl(numVerts);

		char*	pVertices;
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
		{
			for(int i = 0; i<numVerts; i++) {
				verTbl[i].Pos = *(YsVec3*)(pVertices + i*verByteSize + offset_pos);
				verTbl[i].UV = *(YsVec2*)(pVertices + i*verByteSize + offset_texcoord);
				verTbl[i].Normal = *(YsVec3*)(pVertices + i*verByteSize + offset_normal);

				// ※Tangent,Binormalはあとで算出する

				for(int k = 0; k<4; k++) {
					verTbl[i].BlendWeight[k] = 0;
					verTbl[i].BlendIndices[k] = 0xFFFF;
				}
			}

			// ボーンウェイト系を作成
			if(pSkinInfo) {
				DWORD boneNum = pSkinInfo->GetNumBones();	// このメッシュで使われるボーン数
				for(UINT bn = 0; bn<boneNum; bn++) {
					DWORD num = pSkinInfo->GetNumBoneInfluences(bn);	// このボーンの影響度数(頂点数) 取得

					const std::string& boneName = pSkinInfo->GetBoneName(bn);	// ボーン名
					int boneIdx = SearchBoneIndex(boneName);
					// 影響頂点、影響度　取得
					std::vector<DWORD> vers(num);
					std::vector<float> weis(num);
					if(num > 0) {
						pSkinInfo->GetBoneInfluence(bn, &vers[0], &weis[0]);
					}

					// 各頂点へ設定
					for(UINT infl = 0; infl<num; infl++) {
						// 影響頂点番号
						int verIdx = vers[infl];
						//
						for(int wloop = 0; wloop<4; wloop++) {	// 空き検索
							// 空きあり
							if(verTbl[verIdx].BlendIndices[wloop] == 0xFFFF) {
								verTbl[verIdx].BlendIndices[wloop] = (unsigned short)boneIdx;	// ???
								verTbl[verIdx].BlendWeight[wloop] = weis[infl];
								break;
							}
						}

					}
				}
			}
		}
		pMesh->UnlockVertexBuffer();

		// ブレンドの重みの合計が１にならないものは、最後の頂点を強制的に１にする。
		for(int i = 0; i<numVerts; i++) {
			float totalWeight = 0;
			int k = 0;
			for(k = 0; k<3; k++) {
				if(verTbl[i].BlendIndices[k + 1] == 0xFFFF) {
					break;
				}
				totalWeight += verTbl[i].BlendWeight[k];
			}
			verTbl[i].BlendWeight[k] = 1 - totalWeight;
		}

		//==============================================
		// インデックスデータの用意
		//==============================================
		std::vector<YsMesh::Face> idxTbl(numFaces);
		{
			WORD* pIndices;
			pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndices);
			for(int i = 0; i<numFaces; i++) {
				idxTbl[i].idx[0] = pIndices[i * 3 + 0];
				idxTbl[i].idx[1] = pIndices[i * 3 + 1];
				idxTbl[i].idx[2] = pIndices[i * 3 + 2];

				int f1 = idxTbl[i].idx[0];
				int f2 = idxTbl[i].idx[1];
				int f3 = idxTbl[i].idx[2];

				// 頂点の接線、従法線算出
				YsVec3 tangent, binormal;
				YsVec3 vN = (verTbl[f1].Normal + verTbl[f2].Normal + verTbl[f3].Normal) / 3;
				/*
				CalcTangent(verTbl[f1].Pos, verTbl[f2].Pos, verTbl[f3].Pos,
				verTbl[f1].UV, verTbl[f2].UV, verTbl[f3].UV, vN,
				&tangent, &binormal);
				*/
				YsCalcTangentAndBinormal(&verTbl[f1].Pos, &verTbl[f1].UV,
					&verTbl[f2].Pos, &verTbl[f2].UV,
					&verTbl[f3].Pos, &verTbl[f3].UV,
					&vN, &tangent, &binormal);
				vN.Normalize();

				// 
				verTbl[f1].Tangent = tangent;
				verTbl[f2].Tangent = tangent;
				verTbl[f3].Tangent = tangent;
				verTbl[f1].Binormal = binormal;
				verTbl[f2].Binormal = binormal;
				verTbl[f3].Binormal = binormal;

			}
			pMesh->UnlockIndexBuffer();

		}

		// メッシュ作成
		outMesh->Create(&verTbl[0], verTbl.size(), &idxTbl[0], idxTbl.size(), subset);
	}
	// スタティックメッシュ
	else {
		std::vector<YsVertex_Pos_UV_TBN> verTbl(numVerts);
		// 頂点形式データ取得
		YsVertexTypeData vtd = YsVertex_Pos_UV_TBN::GetVertexTypeData();

		char*	pVertices;
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
		{
			for(int i = 0; i<numVerts; i++) {
				verTbl[i].Pos = *(YsVec3*)(pVertices + i*verByteSize + offset_pos);
				verTbl[i].UV = *(YsVec2*)(pVertices + i*verByteSize + offset_texcoord);
				verTbl[i].Normal = *(YsVec3*)(pVertices + i*verByteSize + offset_normal);

				// ※Tangent,Binormalはあとで算出する
			}

		}
		pMesh->UnlockVertexBuffer();

		//==============================================
		// インデックスデータの用意
		//==============================================
		std::vector<YsMesh::Face> idxTbl(numFaces);
		{
			WORD* pIndices;
			pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndices);
			for(int i = 0; i<numFaces; i++) {
				idxTbl[i].idx[0] = pIndices[i * 3 + 0];
				idxTbl[i].idx[1] = pIndices[i * 3 + 1];
				idxTbl[i].idx[2] = pIndices[i * 3 + 2];

				int f1 = idxTbl[i].idx[0];
				int f2 = idxTbl[i].idx[1];
				int f3 = idxTbl[i].idx[2];

				// 頂点の接線、従法線算出
				YsVec3 tangent, binormal;
				YsVec3 vN = (verTbl[f1].Normal + verTbl[f2].Normal + verTbl[f3].Normal) / 3;
				vN.Normalize();

				YsCalcTangentAndBinormal(&verTbl[f1].Pos, &verTbl[f1].UV,
					&verTbl[f2].Pos, &verTbl[f2].UV,
					&verTbl[f3].Pos, &verTbl[f3].UV,
					&vN, &tangent, &binormal);
				// 
				verTbl[f1].Tangent = tangent;
				verTbl[f2].Tangent = tangent;
				verTbl[f3].Tangent = tangent;
				verTbl[f1].Binormal = binormal;
				verTbl[f2].Binormal = binormal;
				verTbl[f3].Binormal = binormal;

			}
			pMesh->UnlockIndexBuffer();

		}


		// メッシュ作成
		outMesh->Create(&verTbl[0], verTbl.size(), &idxTbl[0], idxTbl.size(), subset);
	}

	return true;
}

//===================================================
//! @brief アニメーションコントローラからアニメーション作成
//! アニメーションコントローラからアニメデータを取り出し、独自データを作成
//! @param[in] pAC				… 基となるアニメーションコントローラ
//! @param[in] boneNameTbl		… ボーン名の配列　すべてのボーン名を順番に入れて持っていくこと
//! @param[out] outAnimeList	… 作成されたアニメデータは、ここに格納される
//! @ingroup Etc
//===================================================
static void CreateAnimationFromAC(LPD3DXANIMATIONCONTROLLER pAC, const std::vector<std::string>& boneNameTbl, std::vector<SPtr<YsAnimationSet>>& outAnimeList)
{
	if (pAC == nullptr) {
		return;
	}

	outAnimeList.clear();

	// アニメーションセットから独自アニメーション作成
	for (UINT animLoop = 0; animLoop < pAC->GetNumAnimationSets(); animLoop++)
	{
		LPD3DXKEYFRAMEDANIMATIONSET as;
		pAC->GetAnimationSet(animLoop, (LPD3DXANIMATIONSET*)&as);
		int numAnime = as->GetNumAnimations();

		// 先頭が"//"で始まる名前は、アニメーキーは無視
		std::string animeName = as->GetName();				// アニメ名
		if (animeName.substr(0, 2) == "//") {
			continue;
		}

		// 新規アニメーション
		SPtr<YsAnimationSet> anime = std::make_shared<YsAnimationSet>();
		outAnimeList.push_back(anime);

		// 情報設定
		anime->m_AnimeName = animeName;							// アニメ名
		anime->m_TicksPerSecond = as->GetSourceTicksPerSecond();	// TicksPerSecound
		double pr = as->GetPeriod();

		// ボーンの数ぶん確保
		anime->CreateFrameAnimeList(boneNameTbl.size());

		float MaxTime = 0;

		// 全ボーンの回転、拡大、座標ゲット
		for (UINT i = 0; i<boneNameTbl.size(); i++)
		{
			YsKeyframeAnime* AnimeFrame = anime->m_FrameAnimeList[i];

			UINT AniIdx;

			// ボーン名からIndex取得
			if (D3D_OK != as->GetAnimationIndexByName(boneNameTbl[i].c_str(), &AniIdx))continue;

			// 回転
			int KeyNum = as->GetNumRotationKeys(AniIdx);
			if (KeyNum > 0) {
				D3DXKEY_QUATERNION *lpK_Q = new D3DXKEY_QUATERNION[KeyNum];
				as->GetRotationKeys(AniIdx, lpK_Q);							// クォータニオン配列取得

				for (int k = 0; k<KeyNum; k++) {
					lpK_Q[k].Value.w *= -1;
				}
				// 回転キーとして登録
				for (int k = 0; k<KeyNum; k++) {
					YsAnimeKey_Quaternion* keyQ = new YsAnimeKey_Quaternion();
					keyQ->Time = lpK_Q[k].Time;
					keyQ->Value = lpK_Q[k].Value;
					if (MaxTime < keyQ->Time)MaxTime = keyQ->Time;

					// 補間方法
					keyQ->CalcType = 0;

					AnimeFrame->m_Rotate.push_back(keyQ);
				}

				delete[] lpK_Q;
			}

			// 拡大
			KeyNum = as->GetNumScaleKeys(AniIdx);
			if (KeyNum > 0) {
				D3DXKEY_VECTOR3 *lpK_V = new D3DXKEY_VECTOR3[KeyNum];
				as->GetScaleKeys(AniIdx, lpK_V);

				for (int k = 0; k<KeyNum; k++) {
					YsAnimeKey_Vector3* keyV = new YsAnimeKey_Vector3();
					keyV->Time = lpK_V[k].Time;
					keyV->Value = lpK_V[k].Value;
					keyV->CalcType = 0;
					if (MaxTime < keyV->Time)MaxTime = keyV->Time;

					AnimeFrame->m_Scale.push_back(keyV);
				}

				delete[] lpK_V;
			}

			// 座標
			KeyNum = as->GetNumTranslationKeys(AniIdx);
			if (KeyNum > 0) {
				D3DXKEY_VECTOR3 *lpK_V = new D3DXKEY_VECTOR3[KeyNum];
				as->GetTranslationKeys(AniIdx, lpK_V);

				for (int k = 0; k<KeyNum; k++) {
					YsAnimeKey_Vector3* keyV = new YsAnimeKey_Vector3();
					keyV->Time = lpK_V[k].Time;
					keyV->Value = lpK_V[k].Value;
					keyV->CalcType = 0;
					if (MaxTime < keyV->Time)MaxTime = keyV->Time;

					AnimeFrame->m_Pos.push_back(keyV);
				}

				delete[] lpK_V;
			}
		}
		as->Release();

		// 最大フレーム値から、アニメの長を計算
		anime->m_AnimeLen = MaxTime + 1;// / anime->m_TicksPerSecond;
	}
}
