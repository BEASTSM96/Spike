#pragma once
#include "LightEngine/Renderer/OrthographicCamera.h"
#include "LightEngine/Core/Timestep.h"
#include "LightEngine/Events/ApplicationEvent.h"
#include "LightEngine/Events/MouseEvent.h"

namespace LightEngine
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRation, bool rotation = false);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }
    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        bool m_Rotation = false;
        OrthographicCamera m_Camera;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;
        float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
    };
}