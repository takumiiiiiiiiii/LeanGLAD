#include "Player.h"
#include "../Shader.h"
#include <iostream>

Player::Player(CollisionMesh& groundCollision,const Transform& initialTransform, float size)
    : Object(initialTransform),
      cube(size),
      moveSpeed(1.0f),
      groundCollision(groundCollision)
{
    this->size = size;
    std::cout << kinematics.velocity.y << std::endl;
}

void Player::Update(float deltaTime)
{
    GroundHitInfo ground = CheckGroundCube();
    glm::vec3 curPos = transform.GetPosition();

    //地面に接地しているかどうか
    if (ground.grounded && kinematics.velocity.y<0) {
        isJumpimg = false;
        kinematics.velocity.y = 0.0f;
        kinematics.acceleration.y = 0.0f;
        curPos.y = ground.point.y + size / 2.0f;
    } else {
        kinematics.acceleration.y = gravity;
    }

    Jump();
    kinematics.Integrate(deltaTime);

    glm::vec3 resolved = ResolveHorizontalMovement(curPos, pendingMove);
    curPos.x += resolved.x;
    curPos.z += resolved.z;
    curPos.y += kinematics.velocity.y;

    transform.SetPosition(curPos);
    pendingMove = glm::vec3(0.0f); // 次フレーム用にリセット
}

void Player::Move(float deltaTime)
{
    glm::vec3 dir(0.0f);

    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        std::cout << "Press W" << std::endl;
        dir.z -= 1.0f;
    }

    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        std::cout << "Press S" << std::endl;
        dir.z += 1.0f;
    }

    if (Input::IsKeyPressed(GLFW_KEY_A))
    {
        std::cout << "Press A" << std::endl;
        dir.x -= 1.0f;
    }

    if (Input::IsKeyPressed(GLFW_KEY_D))
    {
        std::cout << "Press D" << std::endl;
        dir.x += 1.0f;
    }

    if (glm::length(dir) > 0.0f)
    {
        dir = glm::normalize(dir);
    }

    pendingMove += dir * moveSpeed * deltaTime; // 移動量を変数に代入
}

//カメラ向きを基準に移動
void Player::MoveWithCameraOrientation(Camera& camera, float deltaTime)
{
    UpdateGravity(deltaTime);
    glm::vec3 forward = camera.Front;
    forward.y = 0.0f;
    forward = glm::normalize(forward);
    glm::vec3 ringt = camera.Right;
    ringt.y = 0.0f;
    ringt = glm::normalize(ringt);

    glm::vec3 movement(0.0f);

    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        std::cout << "Press W" << std::endl;
        movement += forward;
    }

    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        std::cout << "Press S" << std::endl;
        movement -= forward;
    }

    if (Input::IsKeyPressed(GLFW_KEY_A))
    {
        std::cout << "Press A" << std::endl;
        movement -= ringt;
    }

    if (Input::IsKeyPressed(GLFW_KEY_D))
    {
        std::cout << "Press D" << std::endl;
        movement += ringt;
    }
    // 水平方向のみに制限
    movement.y = 0.0f;

    if (glm::length(movement) > 0.0f)
    {
        movement = glm::normalize(movement);
    }
    // 移動量を変数に代入
    pendingMove += movement * moveSpeed * deltaTime; 
}

void Player::UpdateGravity(float deltaTime){
    glm::vec3 movement(0.0f);
    transform.Translate(movement*deltaTime);
}
void Player::MoveWithVector(const glm::vec3& direction, float deltaTime)
{
    transform.Translate(direction * moveSpeed * deltaTime);
}

void Player::SetPosition(const glm::vec3& pos)
{
    transform.SetPosition(pos);
}


void Player::SetMoveSpeed(float speed)
{
    moveSpeed = speed;
}

glm::vec3 Player::GetPosition() const
{
    return transform.GetPosition();
}



void Player::Draw(Shader& shader)
{
    shader.setMat4("model", transform.GetModelMatrix());
    cube.SetTranformPosition(this->transform.GetPosition());
    cube.Draw(shader);
}

GroundHitInfo Player::CheckGround(){
    glm::vec3 origin = transform.GetPosition() + glm::vec3(0.0f, rayOriginOffset-size/2.0f, 0.0f);
    glm::vec3 dir(0.0f, -1.0f, 0.0f);

    GroundHitInfo info;
    float dist;
    glm::vec3 normal;
    if (groundCollision.raycast(origin, dir, dist, normal)) {
        float distFromFeet = dist - rayOriginOffset;
        info.grounded = distFromFeet <= groundedThreshold;
        info.point  = origin + dir * dist; // 地面との交点
        info.normal = normal;
    }
    return info;
}

GroundHitInfo Player::CheckGroundCube(){
    GroundHitInfo info;
    float half = size /2.0f;
    //四角形のため四隅からレイを飛ばす
    glm::vec3 offsets[4] = {
        { half, 0.0f,  half},
        { half, 0.0f, -half},
        {-half, 0.0f,  half},
        {-half, 0.0f, -half}
    };
    for(const auto& offset : offsets){
        glm::vec3 origin = transform.GetPosition() + offset + glm::vec3(0.0f, rayOriginOffset - half, 0.0f);
        glm::vec3 dir(0.0f, -1.0f, 0.0f);
        float dist;
        glm::vec3 normal;
        
        if (groundCollision.raycast(origin, dir, dist, normal)) {
            float distFromFeet = dist - rayOriginOffset;
            if(distFromFeet <= groundedThreshold){
                info.grounded = true;
                info.point  = origin + dir * dist - offset; // 地面との交点
                info.normal = normal;
                return info; // この角の真下に地面が無い = 崖
            }
        }

        // float distFromFeet = dist - rayOriginOffset;
        // if (distFromFeet > edgeCheckThreshold) {
        //     return true; // 地面はあるが大きな落差(崖・段差)がある
        // }
    }
    info.grounded = false;
    return info;
}

void Player::Jump()
{
    
    if(Input::IsKeyPressed(GLFW_KEY_SPACE)){
        //std::cout << isGrounded << isJump <<std::endl;
        if (!isJumpimg) {
            kinematics.velocity.y = jumpForce;

        }
        isJumpimg = true;
    }

}
//地面とのコリジョン
bool Player::AreAllCornersGrounded(const glm::vec3& pos) const{
    float half = size /2.0f;
    //四角形のため四隅からレイを飛ばす
    glm::vec3 offsets[4] = {
        { half, 0.0f,  half},
        { half, 0.0f, -half},
        {-half, 0.0f,  half},
        {-half, 0.0f, -half}
    };
    for(const auto& offset : offsets){
        glm::vec3 origin = pos + offset + glm::vec3(0.0f, rayOriginOffset - half, 0.0f);
        glm::vec3 dir(0.0f, -1.0f, 0.0f);
        float dist;
        glm::vec3 normal;
        
        if (!groundCollision.raycast(origin, dir, dist, normal)) {
            return false; // この角の真下に地面が無い = 崖
        }

        float distFromFeet = dist - rayOriginOffset;
        if (distFromFeet > edgeCheckThreshold) {
            return false; // 地面はあるが大きな落差(崖・段差)がある
        }
    }
    return true;
}
//壁とのコリジョン
//desiredはtransformの移動量
glm::vec3 Player::ResolveWallCollision(const glm::vec3& currentPos, const glm::vec3& desiredDelta) const
{
    glm::vec3 result = desiredDelta;
    float half = size / 2.0f;
    float offset = half-0.1f;
    //四角形のため四隅からレイを飛ばす
    glm::vec3 offsetsX[4] = {
        { 0, offset,  offset},
        { 0, offset, -offset},
        { 0,-offset,  offset},
        { 0,-offset, -offset}
        
    };

    glm::vec3 offsetsZ[4] = {
        { offset, offset,  0},
        { offset,-offset,  0},
        {-offset, offset,  0},
        {-offset,-offset,  0}
    };
    //移動している時壁があったら
    //x移動のコリジョン
    for(const auto& offset : offsetsX){
        glm::vec3 RayOrigin = currentPos + offset;
        if (std::abs(result.x) > 0.0001f) {
            //xがどっちの方向か
            glm::vec3 dir(result.x > 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);
            float dist; glm::vec3 normal;
            if (groundCollision.raycast(RayOrigin, dir, dist, normal)) {
                //壁との距離を求める
                float maxAllowed = dist - half - wallSkin;
                if (maxAllowed < std::abs(result.x)) {
                    result.x = std::max(0.0f, maxAllowed) * (result.x > 0.0f ? 1.0f : -1.0f);
                }
            }
        }
    }

    for(const auto& offset : offsetsZ){
        glm::vec3 RayOrigin = currentPos + offset;
        //z移動のコリジョン
        if (std::abs(result.z) > 0.0001f) {
            glm::vec3 dir(0.0f, 0.0f, result.z > 0.0f ? 1.0f : -1.0f);
            float dist; glm::vec3 normal;
            if (groundCollision.raycast(RayOrigin, dir, dist, normal)) {
                float maxAllowed = dist - half - wallSkin;
                if (maxAllowed < std::abs(result.z)) {
                    result.z = std::max(0.0f, maxAllowed) * (result.z > 0.0f ? 1.0f : -1.0f);
                }
            }
        }
    }

    return result;
}
glm::vec3 Player::ResolveHorizontalMovement(const glm::vec3& currentPos, glm::vec3 desiredDelta) const
{
    desiredDelta = ResolveWallCollision(currentPos, desiredDelta);

    // if (!AreAllCornersGrounded(currentPos + glm::vec3(desiredDelta.x, 0.0f, 0.0f))) {
    //     desiredDelta.x = 0.0f;
    // }
    // if (!AreAllCornersGrounded(currentPos + glm::vec3(0.0f, 0.0f, desiredDelta.z))) {
    //     desiredDelta.z = 0.0f;
    // }

    return desiredDelta;
}