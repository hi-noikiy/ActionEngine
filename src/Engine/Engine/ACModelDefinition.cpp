#include "ACModelDefinition.h"
#include "ACContentManager.h"
#include "ACRenderDevice.h"

ACModelDefinition::ACModelDefinition(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	pVertexBuffer = nullptr;

	mNumVertices = 0;
	mpVPNTCache = nullptr;
	mpVSMCache = nullptr;

	mNumIndices = 0;
	mpIndices = nullptr;
};

ACModelDefinition::~ACModelDefinition()
{
	Release();
};

void ACModelDefinition::Prepare(AMT_MODEL* amtModel)
{
	//cria o minimo bb ao redor do modelo
	Vector3* points = new Vector3[amtModel->Head.NumVertices];
	for(int i=0;i<amtModel->Head.NumVertices;i++)
		points[i] = amtModel->pVertices[i]->Position;

	//cria o bb
	BoundingBox::CreateFromPoints(amtModel->Head.NumVertices, points, &BBOriginal);
	//cria o obb
	OrientedBoundingBox::CreateFromBB(&BBOriginal, &OBBOriginal);

	delete[] points;

	//verifica o tipo de vertices se tiver bones entao � um skinnedmesh senao um mesh normal
	if (amtModel->Head.HasSkeleton)
	{
		VFormat = VertexFormat::VF_VertexSkinnedMesh;
		//TODO: programar o tipo skin
	}
	else
	{
		VFormat = VertexFormat::VF_VertexPositionNormalTextured;
		PrepareVPNT(amtModel);
	}
};

void ACModelDefinition::PrepareVPNT(AMT_MODEL* model)
{
	mNumVertices = model->Head.NumVertices;
	mpVPNTCache = new ACVertexPositionNormalTextured[mNumVertices];

	//seta os vertices para o array
	for (int i = 0; i < mNumVertices; i++)
	{
		mpVPNTCache[i].position = model->pVertices[i]->Position;
		mpVPNTCache[i].normal = model->pVertices[i]->Normal;
		mpVPNTCache[i].texcoord = model->pVertices[i]->TexCoord1;
	}

	//atribui os indices
	mNumIndices = model->pFaces.size() * 3;
	mpIndices = new UINT[mNumIndices];
	int index = 0;
	for (int i = 0; i< model->pFaces.size(); i++)
	{
		mpIndices[index++] = model->pFaces[i]->Indices[0];
		mpIndices[index++] = model->pFaces[i]->Indices[1];
		mpIndices[index++] = model->pFaces[i]->Indices[2];
	}

	ACSkin* pSkin = mpCManager->CreateSkin();
	pSkin->Material.AmbientColor = model->pMaterials[0]->Ambient;
	pSkin->Material.EmissiveColor = model->pMaterials[0]->Emissive;
	pSkin->Material.SpecularColor = model->pMaterials[0]->Specular;
	pSkin->Material.SpecularPower = model->pMaterials[0]->SpecularPower;
	pSkin->Material.DiffuseColor = Vector4(&model->pMaterials[0]->Diffuse, model->pMaterials[0]->Transparency);

	//carrega as texturas
	if (model->pMaterials[0]->DiffuseTexture[0] != '\0')
		pSkin->Textures[0] = mpCManager->LoadTexture(model->pMaterials[0]->DiffuseTexture);
	if (model->pMaterials[0]->SpecularTexture[0]  != '\0')
		pSkin->Textures[1] = mpCManager->LoadTexture(model->pMaterials[0]->SpecularTexture);
	if (model->pMaterials[0]->NormalTexture[0]  != '\0')
		pSkin->Textures[2] = mpCManager->LoadTexture(model->pMaterials[0]->NormalTexture);
	if (model->pMaterials[0]->AnimatedTexture[0]  != '\0')
		pSkin->Textures[3] = mpCManager->LoadTexture(model->pMaterials[0]->AnimatedTexture);

	//ja cria o vertexbuffer na api
	mpGDevice->CreateStaticBuffer(VertexFormat::VF_VertexPositionNormalTextured, 
								  mNumVertices, 
								  mNumIndices, 
								  mpVPNTCache, 
								  mpIndices, 
								  pSkin,
								  &pVertexBuffer);
};

void ACModelDefinition::SetSkin(ACSkin* skin)
{
	//se ja existe um skin entao ele deleta para remover todos os resource e tb referencias depois adiciona outro
	if (pVertexBuffer->Skin != nullptr)
		SAFE_DELETE(pVertexBuffer->Skin);

	pVertexBuffer->Skin = mpCManager->CreateSkin();
	mpCManager->CloneSkin(pVertexBuffer->Skin, skin);
};

const ACSkin const * ACModelDefinition::GetSkin()
{
	return pVertexBuffer->Skin;
};

void ACModelDefinition::PrepareVSM(AMT_MODEL* model)
{

};

void ACModelDefinition::Release()
{
	//remove o skin e tb as texturas e tudo mais
	SAFE_RELEASE(pVertexBuffer->Skin);

	//remove o vb e ib da api
	if (pVertexBuffer!=nullptr)
	{
		mpGDevice->ReleaseBuffer(pVertexBuffer);
		pVertexBuffer = nullptr;
	}
};