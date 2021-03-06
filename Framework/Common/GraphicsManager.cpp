#include <iostream>
#include "GraphicsManager.hpp"
#include "SceneManager.hpp"
#include "cbuffer.h"
#include "IApplication.hpp"
#include "SceneManager.hpp"

using namespace My;
using namespace std;

int GraphicsManager::Initialize()
{
    int result = 0;
	InitConstants();
    return result;
}

void GraphicsManager::Finalize()
{
    ClearDebugBuffers();
    ClearBuffers();
    ClearShaders();
}

void GraphicsManager::Tick()
{
    if (g_pSceneManager->IsSceneChanged())
    {
        cout << "[GraphicsManager] Detected Scene Change, reinitialize buffers ..." << endl;
        ClearBuffers();
        ClearShaders();
        const Scene& scene = g_pSceneManager->GetSceneForRendering();
        InitializeShaders();
        InitializeBuffers(scene);
        g_pSceneManager->NotifySceneIsRenderingQueued();
    }

    UpdateConstants();

    Clear();
    Draw();
}

void GraphicsManager::UpdateConstants()
{
    // Generate the view matrix based on the camera's position.
    CalculateCameraMatrix();
    CalculateLights();
}

void GraphicsManager::Clear()
{

}

void GraphicsManager::Draw()
{
    UpdateConstants();

    RenderBuffers();
}

void GraphicsManager::InitConstants()
{
    // Initialize the world/model matrix to the identity matrix.
    BuildIdentityMatrix(m_DrawFrameContext.m_worldMatrix);
}

bool GraphicsManager::InitializeShaders()
{
    cout << "[GraphicsManager] GraphicsManager::InitializeShader()" << endl;
    return true;
}

void GraphicsManager::ClearShaders()
{
    cout << "[GraphicsManager] GraphicsManager::ClearShaders()" << endl;
}

void GraphicsManager::CalculateCameraMatrix()
{
    auto& scene = g_pSceneManager->GetSceneForRendering();
    auto pCameraNode = scene.GetFirstCameraNode();
    if (pCameraNode) {
        m_DrawFrameContext.m_viewMatrix = *pCameraNode->GetCalculatedTransform();
        InverseMatrix4X4f(m_DrawFrameContext.m_viewMatrix);
    }
    else {
        // use default build-in camera
        Vector3f position = { 0, -5, 0 }, lookAt = { 0, 0, 0 }, up = { 0, 0, 1 };
        BuildViewMatrix(m_DrawFrameContext.m_viewMatrix, position, lookAt, up);
    }

    float fieldOfView = PI / 2.0f;
    float nearClipDistance = 1.0f;
    float farClipDistance = 100.0f;

    if (pCameraNode) {
        auto pCamera = scene.GetCamera(pCameraNode->GetSceneObjectRef());
        // Set the field of view and screen aspect ratio.
        fieldOfView = dynamic_pointer_cast<SceneObjectPerspectiveCamera>(pCamera)->GetFov();
        nearClipDistance = pCamera->GetNearClipDistance();
        farClipDistance = pCamera->GetFarClipDistance();
    }

    const GfxConfiguration& conf = g_pApp->GetConfiguration();

    float screenAspect = (float)conf.screenWidth / (float)conf.screenHeight;

    // Build the perspective projection matrix.
    BuildPerspectiveFovRHMatrix(m_DrawFrameContext.m_projectionMatrix, fieldOfView, screenAspect, nearClipDistance, farClipDistance);
}

void GraphicsManager::CalculateLights()
{
    auto& scene = g_pSceneManager->GetSceneForRendering();
    auto pLightNode = scene.GetFirstLightNode();
    if (pLightNode) {
        m_DrawFrameContext.m_lightPosition = { 0.0f, 0.0f, 0.0f };
        TransformCoord(m_DrawFrameContext.m_lightPosition, *pLightNode->GetCalculatedTransform());

        auto pLight = scene.GetLight(pLightNode->GetSceneObjectRef());
        if (pLight) {
            m_DrawFrameContext.m_lightColor = pLight->GetColor().Value;
        }
    }
    else {
        // use default build-in light 
        m_DrawFrameContext.m_lightPosition = { -1.0f, -5.0f, 0.0f};
        m_DrawFrameContext.m_lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    }
}

void GraphicsManager::InitializeBuffers(const Scene& scene)
{
    cout << "[GraphicsManager] GraphicsManager::InitializeBuffers()" << endl;
}

void GraphicsManager::ClearBuffers()
{
    cout << "[GraphicsManager] GraphicsManager::ClearBuffers()" << endl;
}

void GraphicsManager::RenderBuffers()
{
    cout << "[GraphicsManager] GraphicsManager::RenderBuffers()" << endl;
}

#ifdef DEBUG
void GraphicsManager::DrawLine(const Vector3f &from, const Vector3f &to, const Vector3f &color)
{
    cout << "[GraphicsManager] GraphicsManager::DrawLine(" << from << ","
        << to << "," 
        << color << ")" << endl;
}

void GraphicsManager::DrawBox(const Vector3f &bbMin, const Vector3f &bbMax, const Vector3f &color)
{
    cout << "[GraphicsManager] GraphicsManager::DrawBox(" << bbMin << ","
        << bbMax << "," 
        << color << ")" << endl;
}

void GraphicsManager::ClearDebugBuffers()
{
    cout << "[GraphicsManager] GraphicsManager::ClearDebugBuffers(void)" << endl;
}
#endif

