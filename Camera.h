#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/Input.h"

// デフォルトのカメラ移動
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// デフォルトの値
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
    
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        float Yaw;
        float Pitch;
        //camera options 
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
        //フォロー用の距離と高さの設定
        float FollowDistance = 10.0f;
        float FollowHeight = 3.0f;
        Camera(glm::vec3 position = glm::vec3(0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),float yaw = YAW,float pitch = PITCH);
        Camera(float posX, float posY, float posZ,float upX, float upY, float upZ,float yaw, float pitch);
        glm::mat4 GetViewMatrix();
        glm::mat3 GetNormalMatrix();
        glm::vec3 GetFrontVector();
        void ProcessKeyboard(Camera_Movement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset,float yoffset);
        void ProcessMouseScroll(float yoffset);
        glm::vec3 CalculateOffset() const;
        void UpdateCameraVectors(const glm::vec3& offset);
        void Follow(const glm::vec3& target, float dt);
        void FollowRotate(const glm::vec3& target,float Rotate_speed,float dt);
        
    private:
        glm::vec3 smoothedTarget = glm::vec3(0.0f); // 遅れを含んだ注視点
        bool isInitialized = false;                 // 初回フレーム判定用
        void updateCameraVectors();
};
