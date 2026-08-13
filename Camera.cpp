#include "Camera.h"
Camera::Camera(glm::vec3 position,glm::vec3 up,float yaw,float pitch)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;

    updateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ,
               float upX, float upY, float upZ,
               float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;

    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix(){
    glm::mat4 view = glm::lookAt(Position, Position + Front, Up);
    return view;
}
glm::mat3 Camera::GetNormalMatrix()
{
    glm::mat4 view = GetViewMatrix();
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view)));
    return normalMatrix;
}

glm::vec3 Camera::GetFrontVector()
{
    return Front;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;

    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset,float yoffset){
    Yaw   += xoffset;
    Pitch += yoffset;

    if(Pitch > 89.0f)
        Pitch = 89.0f;
    if(Pitch < -89.0f)
        Pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    direction.y = sin(glm::radians(Pitch));
    direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(direction);
}
void Camera::ProcessMouseScroll(float yoffset){
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::Follow(const glm::vec3& target, float dt)
{
    //ターゲットとの距離
    glm::vec3 desiredOffset =
        glm::vec3(
            cos(glm::radians(Yaw)) * FollowDistance,
            FollowHeight,
            sin(glm::radians(Yaw)) * FollowDistance
        );
    //ターゲットにピッタリくっついた時の距離
    glm::vec3 desiredPosition =
        target + desiredOffset;

    float alpha =
        1.0f - std::exp(-10.0f * dt);

    Position =
        glm::mix(Position, desiredPosition, alpha);

    Front =
        glm::normalize(-desiredOffset);

    Right =
        glm::normalize(glm::cross(Front, WorldUp));

    Up =
        glm::normalize(glm::cross(Right, Front));
}

void Camera::FollowRotate(const glm::vec3& target,float Rotate_speed,float dt)
{
    bool rotated = false;
    if (Input::IsKeyPressed(GLFW_KEY_LEFT))
    {
        Yaw -= Rotate_speed * dt;
        rotated = true;
    }

    if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
    {
        Yaw += Rotate_speed * dt;
        rotated = true;
    }

    if (rotated)
    {
        glm::vec3 desiredOffset =
            glm::vec3(
                cos(glm::radians(Yaw)) * FollowDistance,
                FollowHeight,
                sin(glm::radians(Yaw)) * FollowDistance
            );
        //ターゲットにピッタリついて行ってる時のカメラ座標
        glm::vec3 desiredPosition =
        target + desiredOffset;
        float alpha =
        1.0f - std::exp(-10.0f * dt);
        //少しターゲットから遅れているカメラ座標
        glm::vec3 mixdesiredPosition = glm::mix(Position, desiredPosition, alpha);
        //ターゲットから少し遅れた地点
        glm::vec3 desiredTarget = mixdesiredPosition-desiredOffset;

        Position = 
            desiredTarget + desiredOffset;

        Front =
            glm::normalize(desiredTarget - Position);

        Right =
            glm::normalize(glm::cross(Front, WorldUp));

        Up =
            glm::normalize(glm::cross(Right, Front));
    }
}
