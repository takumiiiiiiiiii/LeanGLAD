#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Object.h"
#include "../Collision/CollisionMesh.h"
#include "../Shapes/Cube.h"
#include "../Core/Input.h"
#include "../Camera.h"
#include "../Phythiks/Kinematics.h"

//Shaderクラスの前方宣言
#define G = 9.81;
class Shader;

struct GroundHitInfo
{
    bool grounded = false;
    glm::vec3 point{0.0f};   // 接地点のワールド座標
    glm::vec3 normal{0.0f};  // 地面の法線
};

class Player : public Object
{
public:
    explicit Player(CollisionMesh& groundCollision,const Transform& initialTransform = Transform(), float size = 1.0f);
    void Update(float deltaTime) override;

    void Draw(Shader& shader) override;
    void Move(float deltaTime);
    void MoveWithCameraOrientation(Camera& camera, float deltaTime);
    void UpdateGravity(float deltaTime);
    void MoveWithVector(const glm::vec3& direction, float deltaTime);

    glm::vec3 GetPosition() const;
    void SetPosition(const glm::vec3& pos);
    void SetMoveSpeed(float speed);
    void Jump();
private:
    //地面関連
    glm::vec3 pendingMove{0.0f};
    const float wallSkin = 0.005f;
    const float edgeCheckThreshold = 0.2f;

    bool AreAllCornersGrounded(const glm::vec3& pos) const;
    glm::vec3 ResolveWallCollision(const glm::vec3& currentPos,const glm::vec3& desiredDelta) const;
    glm::vec3 ResolveHorizontalMovement(const glm::vec3& currentPos,glm::vec3 desiredDelta) const;

    GroundHitInfo CheckGround();
    GroundHitInfo CheckGroundCube();
    CollisionMesh& groundCollision; // 地面のCollisionMeshへの参照
    glm::vec3 velocity{0.0f};
    bool isGrounded = false;
    bool isJumpimg = false;
    const float rayOriginOffset   = 0.1f;  // 足元より少し上からRayを飛ばす
    const float groundedThreshold = 0.15f; // この距離以内なら接地とみなす
    //物理
    const float gravity   = -0.28f;
    const float jumpForce =  0.1f;
    //サイズ
    float size = 0;
    //加速度と速度
    Kinematics kinematics;
    //描画に使用するオブジェクト
    Cube cube;
    float moveSpeed;
};
