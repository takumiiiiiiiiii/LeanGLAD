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
    GroundHitInfo ground = CheckGround();
    isGrounded = ground.grounded;
    glm::vec3 CurPos = transform.GetPosition();

    // 接地していて、かつ上昇中(ジャンプ直後)でなければ地面にスナップ
    if (isGrounded && kinematics.velocity.y<0) {
        isJump = false;
        //std::cout << "接地" << std::endl;
        kinematics.velocity.y = 0.0f;
        kinematics.acceleration.y = 0.0f;
        CurPos.y = ground.point.y+size/2;
        transform.SetPosition(CurPos);
    } else {
        //std::cout << "NO接地" << std::endl;
        // velocity.y += gravity * deltaTime;
        //加速度を設定
        kinematics.acceleration.y = gravity;
        //velocity.y = -10;
    }
    Jump();
    // std::cout << kinematics.velocity.y << std::endl;
    // std::cout << kinematics.acceleration.y << std::endl;
    //加速度から速度を算出
    kinematics.Integrate(deltaTime);
    // std::cout << kinematics.velocity.y << std::endl;
    // std::cout << kinematics.acceleration.y << std::endl;
    glm::vec3 Gtrans = transform.GetPosition();
    Gtrans.x += velocity.x * deltaTime;
    Gtrans.z += velocity.z * deltaTime;
    Gtrans.y += kinematics.velocity.y;
    transform.SetPosition(Gtrans);
    //Move(deltaTime);a
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

    transform.Translate(dir * moveSpeed * deltaTime);
}
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

    transform.Translate(movement * moveSpeed * deltaTime);
}

void Player::UpdateGravity(float deltaTime){
    glm::vec3 movement(0.0f);
    //movement.y = -9.81;
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

void Player::Jump()
{
    
    if(Input::IsKeyPressed(GLFW_KEY_SPACE)){
        //std::cout << isGrounded << isJump <<std::endl;
        if (isGrounded&&!isJump) {
            kinematics.velocity.y = jumpForce;
        }
        isJump = true;
    }

}