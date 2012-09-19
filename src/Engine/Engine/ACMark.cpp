#include "ACMark.h"
#include "ACRenderDevice.h"
#include "ACContentManager.h"

ACMark::ACMark(ACRenderDevice* gDevice, ACContentManager* cManager)
{
	mpGDevice = gDevice;
	mpCManager = cManager;

	mpVertices = new ACVertexPosition[3];
	mpVertices[0].position = Vector3( 0,0.05f,0);
	mpVertices[1].position = Vector3(-0.05f,-0.05f,0);
	mpVertices[2].position = Vector3( 0.05f,-0.05f,0);
	
	mpGDevice->CreateStaticBuffer(VertexFormat::VF_VertexPosition, 3, 0, mpVertices, 0, nullptr, &mpVB);

	mpVS = mpCManager->LoadVertexShader("MaterialColored.VShlsl4", VertexFormat::VF_VertexPosition);
	mpPS = mpCManager->LoadPixelShader("MaterialColored.PShlsl4");
};

ACMark::~ACMark()
{
	mpCManager->RemoveVertexShader(mpVS);
	mpCManager->RemovePixelShader(mpPS);
};

void ACMark::Render(ACCamera* camera, Matrix& worldMatrix)
{
	mpGDevice->ActiveVS(mpVS);
	mpGDevice->ActivePS(mpPS);

	mpGDevice->SetMaterialDiffuse(Vector4(1,1,1,1));
	mpGDevice->SetWorldMatrix(worldMatrix);
	mpGDevice->ApplyConstants();

	mpGDevice->Render(mpVB);
};
