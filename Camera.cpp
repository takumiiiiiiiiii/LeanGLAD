#include "Camera.h"
Camera::Camera(glm::vec3 position,glm::vec3 up,float yaw,float pitch)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      MousePanSensitivity(PANSITIVITY),
      MouseForwardSensitivity(FORWARDENSITIVITY),
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

void Camera::ProcessEditorMovement(float deltaTime)
{
    MoveInput input = Input::GetMoveInput();

    glm::vec3 forward = glm::normalize(Front);
    glm::vec3 horizontalRight = glm::cross(forward, WorldUp);
    if (glm::length(horizontalRight) > 0.0f)
        horizontalRight = glm::normalize(horizontalRight);

    glm::vec3 movement = forward * input.y + horizontalRight * input.x;
    if (glm::length(movement) > 1.0f)
        movement = glm::normalize(movement);

    Position += movement * MovementSpeed * deltaTime;
}

void Camera::ProcessMouseMovement(float xoffset,float yoffset){
    // FrontがFollow系処理などで更新されていても、現在の視線を起点に回転する。
    Yaw = glm::degrees(std::atan2(Front.z, Front.x));
    Pitch = glm::degrees(std::asin(glm::clamp(Front.y, -1.0f, 1.0f)));

    Yaw += xoffset;
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
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ProcessMousePan(float xoffset, float yoffset)
{
    constexpr float PanSpeed = 0.01f;

    // 横方向：カメラのRight方向
    Position += Right * xoffset * PanSpeed;

    // 縦方向：画面上方向を正とする
    Position -= Up * yoffset * PanSpeed;
}

void Camera::ProcessMouseForward(float yoffset)
{
    constexpr float MoveSpeed = 0.5f;

    // スクロール上で前進、下で後退
    Position += Front * yoffset * MoveSpeed;
}

void Camera::ProcessMouseScroll(float yoffset){
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

//<summary>カメラベクトルのアップデート </summary>
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
// --- ヘルパー関数: オフセット位置の計算 ---
glm::vec3 Camera::CalculateOffset() const
{
    float yawRad = glm::radians(Yaw);
    float pitchRad = glm::radians(Pitch);
    float horizontalDistance = FollowDistance * std::cos(pitchRad);
    float verticalDistance   = FollowDistance * std::sin(pitchRad);
    return glm::vec3(
        horizontalDistance * std::cos(yawRad),
        verticalDistance + FollowHeight, // オフセット基準高さ
        horizontalDistance * std::sin(yawRad)
    );
}

//<summary>カメラの方向ベクトル(Front, Right, Up)の更新 </summary>
//<param name="offset"> オフセット</param>
// --- ヘルパー関数: カメラの方向ベクトル(Front, Right, Up)の更新 ---
void Camera::UpdateCameraVectors(const glm::vec3& offset)
{
    // ターゲットを視点中心にするため、オフセットの逆方向を正面とする
    Front = glm::normalize(-offset);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

// --- メイン処理: ターゲット追従 ---
void Camera::Follow(const glm::vec3& target, float dt)
{
    //ターゲットとのオフセットを考慮し、カメラの位置を決定
    glm::vec3 desiredOffset   = CalculateOffset();
    glm::vec3 desiredPosition = target + desiredOffset;

    // フレームレート依存のない滑らかな移動 (10.0f は追従速度の係数)
    constexpr float Smoothness = 10.0f;
    float alpha = 1.0f - std::exp(-Smoothness * dt);

    // カメラ位置を補間
    Position = glm::mix(Position, desiredPosition, alpha);

    // カメラの向きを更新
    UpdateCameraVectors(desiredOffset);
}

// --- メイン処理: 回転入力と追従 ---
void Camera::FollowRotate(const glm::vec3& target, float rotateSpeed, float dt)
{
    float yawInput = 0.0f;
    float pitchInput = 0.0f;

    if (Input::IsKeyPressed(GLFW_KEY_LEFT))
    {
        yawInput += 1.0f;
    }
    if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
    {
        yawInput -= 1.0f;
    }
    if (Input::IsKeyPressed(GLFW_KEY_UP))
    {
        pitchInput -= 1.0f;
    }
    if (Input::IsKeyPressed(GLFW_KEY_DOWN))
    {
        pitchInput += 1.0f;
    }

    StickInput rightStick = Input::GetRightStickInput();
    if (std::abs(rightStick.x) > 0.15f || std::abs(rightStick.y) > 0.15f)
    {
        yawInput = -rightStick.x;
        pitchInput = -rightStick.y;
    }

    Yaw += yawInput * rotateSpeed * dt;
    Pitch += pitchInput * rotateSpeed * dt;
    Pitch = glm::clamp(Pitch, -80.0f, 80.0f);

    if (!isInitialized)
    {
        smoothedTarget = target;
        isInitialized = true;
    }

    float alpha = 1.0f - std::exp(-10.0f * dt);
    smoothedTarget = glm::mix(smoothedTarget, target, alpha);

    glm::vec3 desiredOffset = CalculateOffset();
    Position = smoothedTarget + desiredOffset;
    UpdateCameraVectors(desiredOffset);
}