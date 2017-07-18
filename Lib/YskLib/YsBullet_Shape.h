//===============================================================
//! @file YsBullet_RigidBody.h
//! @brief 「Bullet Physics Engine」の「形状」関係のクラス
//!
//! 適当にまとめて書いてます。
//!
//! @author 鎌田
//===============================================================
#ifndef __YsBullet_Shape_h__
#define __YsBullet_Shape_h__

#pragma warning(disable:4316)

namespace YskLib
{
	// YsBulletShape_Base		… 形状の基本クラス
	// YsBulletShape_Box		… 立方体形状
	// YsBulletShape_Sphere		… 球形状
	// YsBulletShape_Capsule	… カプセル形状
	// YsBulletShape_Mesh		… メッシュ形状
	// YsBulletShape_Compound	… 複合形状

	//==========================================================================
	//! @brief 形状基本クラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Base : public YsBulletObj_Base {
	public:
		//! @brief 形状を取得
		virtual SPtr<btCollisionShape>	GetShape() { return nullptr; }

		//! @brief スケール値を取得
		void GetLocalScaling(YsVec3& scale) {
			btCollisionShape* shape = GetShape().get();
			btVector3 v = shape->getLocalScaling();
			scale.Set(v.x(), v.y(), v.z());
		}

		//! @brief スケール設定
		void SetLocalScaling(const YsVec3& localScale){
			GetShape()->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

	private:

	};

	//==========================================================================
	//! @brief BOX形状クラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Box : public YsBulletShape_Base {
	public:
		//! @brief 形状を取得
		virtual SPtr<btCollisionShape>		GetShape() override { return m_Shape; }

		//!@ brief BOX形状取得
		SPtr<btBoxShape>					GetShape_Box(){ return m_Shape; }

		//! @brief 形状を作成
		//! @param[in]	halfSize	… 大きさ(ハーフサイズ)
		//! @param[in]	localScale	… 内部に記憶する形状のスケール GetMatrix()時に影響します
		void Create(const YsVec3& halfSize, const YsVec3& localScale = YsVec3::One){
			// 形状作成
			btVector3 shapeHarfSize(halfSize.x, halfSize.y, halfSize.z);
			m_Shape = SPtr<btBoxShape>(new btBoxShape(shapeHarfSize));	// ※Bulletライブラリはmake_shared使うと失敗する時があったので、通常作成
			m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

		//! @brief 解放
		virtual void Release() override{
			m_Shape = nullptr;
		}

		// 
		YsBulletShape_Box(){
		}
		// 
		~YsBulletShape_Box(){
			Release();
		}

	protected:
		SPtr<btBoxShape>		m_Shape;		//!< 形状

	private:
		// コピー禁止用
		YsBulletShape_Box(const YsBulletShape_Box& src){}
		void operator=(const YsBulletShape_Box& src){}

	};

	//==========================================================================
	//! @brief カプセル形状クラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Capsule : public YsBulletShape_Base {
	public:
		//! @brief 形状を取得
		virtual SPtr<btCollisionShape>	GetShape() override { return m_Shape; }

		//!@ brief カプセル形状取得
		SPtr<btCapsuleShape>			GetShape_Capsule(){ return m_Shape; }

		//! @brief 形状を作成
		//! @param[in]	halfSize	… 大きさ(ハーフサイズ)
		//! @param[in]	localScale	… 内部に記憶する形状のスケール GetMatrix()時に影響します
		void Create(float radius, float height, const YsVec3& localScale = YsVec3::One){
			// 形状作成
			m_Shape = SPtr<btCapsuleShape>(new btCapsuleShape(radius, height));	// ※Bulletライブラリはmake_shared使うと失敗する時があったので、通常作成
			m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

		//! @brief 解放
		virtual void Release() override{
			m_Shape = nullptr;
		}

		// 
		YsBulletShape_Capsule(){
		}
		// 
		~YsBulletShape_Capsule(){
			Release();
		}

	protected:
		SPtr<btCapsuleShape>		m_Shape;		//!< 形状

	private:
		// コピー禁止用
		YsBulletShape_Capsule(const YsBulletShape_Capsule& src){}
		void operator=(const YsBulletShape_Capsule& src){}

	};

	//==========================================================================
	//! @brief 球形状クラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Sphere : public YsBulletShape_Base {
	public:
		//! @brief 形状を取得
		virtual SPtr<btCollisionShape>	GetShape() override { return m_Shape; }

		//!@ brief カプセル形状取得
		SPtr<btSphereShape>				GetShape_Sphere(){ return m_Shape; }

		//! @brief 形状を作成
		//! @param[in]	halfSize	… 大きさ(ハーフサイズ)
		//! @param[in]	localScale	… 内部に記憶する形状のスケール GetMatrix()時に影響します
		void Create(float radius, const YsVec3& localScale = YsVec3::One){
			// 形状作成
			m_Shape = SPtr<btSphereShape>(new btSphereShape(radius));	// ※Bulletライブラリはmake_shared使うと失敗する時があったので、通常作成
			m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

		//! @brief 解放
		virtual void Release() override{
			m_Shape = nullptr;
		}

		// 
		YsBulletShape_Sphere(){
		}
		// 
		~YsBulletShape_Sphere(){
			Release();
		}

	protected:
		SPtr<btSphereShape>		m_Shape;		//!< 形状

	private:
		// コピー禁止用
		YsBulletShape_Sphere(const YsBulletShape_Sphere& src){}
		void operator=(const YsBulletShape_Sphere& src){}

	};

	//==========================================================================
	//! @brief 複合形状クラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Compound : public YsBulletShape_Base {
	public:
		//! @brief 形状を取得
		virtual SPtr<btCollisionShape>	GetShape() override { return m_Shape; }

		//!@ brief カプセル形状取得
		SPtr<btCompoundShape>			GetShape_Compound(){ return m_Shape; }

		//! @brief 形状を作成
		//! @param[in]	localScale	… 内部に記憶する形状のスケール GetMatrix()時に影響します
		void Create(const YsVec3& localScale = YsVec3::One){
			// 形状作成
			m_Shape = SPtr<btCompoundShape>(new btCompoundShape());	// ※Bulletライブラリはmake_shared使うと失敗する時があったので、通常作成
			m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));
		}

		//! @brief Compound形状の子形状として登録
		void AddChildShape(SPtr<YsBulletShape_Base> shape, const YsMatrix& mat);

		//! @brief 登録されてる子形状数を取得
		int GetChildShapeCnt(){ return m_Shape->getNumChildShapes(); }

		//! @brief 解放
		virtual void Release() override;

		// 
		YsBulletShape_Compound(){
		}
		// 
		~YsBulletShape_Compound(){
			Release();
		}

	protected:
		SPtr<btCompoundShape>					m_Shape;		//!< 形状

		std::vector< SPtr<YsBulletShape_Base> >	m_ShapeList;	//!< 子形状リスト(形状を削除する場合は、m_Shape内からの削除すること)

	private:
		// コピー禁止用
		YsBulletShape_Compound(const YsBulletShape_Compound& src){}
		void operator=(const YsBulletShape_Compound& src){}

	};

	//==========================================================================
	//! @brief メッシュ形状クラス
	//!
	//! @ingroup Physics
	//==========================================================================
	class YsBulletShape_Mesh : public YsBulletShape_Base {
	public:
		//! @brief 形状を取得
		virtual SPtr<btCollisionShape>	GetShape() override { return m_Shape; }

		//!@ brief カプセル形状取得
		SPtr<btBvhTriangleMeshShape>			GetShape_Compound(){ return m_Shape; }

		//! @brief 形状を作成
		//! @param[in]	pMesh	… 大きさ(ハーフサイズ)
		//! @param[in]	localScale	… 内部に記憶する形状のスケール GetMatrix()時に影響します
		void CreateFromMesh(const YsMesh& pMesh, const YsVec3& localScale = YsVec3::One);


		//! @brief 解放
		virtual void Release() override{
			m_Shape = nullptr;
		}

		// 
		YsBulletShape_Mesh(){
		}
		// 
		~YsBulletShape_Mesh(){
			Release();
		}

	protected:
		btVector3*							m_Verts;				//!< 頂点配列
		int*								m_Idxs;					//!< インデックス配列
		btTriangleIndexVertexArray*			m_indexVertexArrays;	//!< 
		//	btGImpactMeshShape*				m_Shape;				//!< 形状
		SPtr<btBvhTriangleMeshShape>		m_Shape;				//!< 形状


	private:
		// コピー禁止用
		YsBulletShape_Mesh(const YsBulletShape_Mesh& src){}
		void operator=(const YsBulletShape_Mesh& src){}

	};
}


#endif
