#include "Goal.h"
#include "../Shader.h"
Goal::Goal(float input_size, GLuint textureID)
:cube(input_size),
goalSize(input_size)

{
}

void Goal::Update(float deltaTime){
    glm::vec3 centerPos = cube.GetTransform().GetPosition()+glm::vec3(0.0f,goalSize/2.0f,0.0f);
    aabb.Set(centerPos,transform.GetScale());
    
}
bool Goal::CheckPlayerEnter(const AABB& playerAABB){
    return aabb.Intersects(playerAABB);
};



void Goal::Draw(Shader& shader)
{
    cube.SetTranformPosition(transform.GetPosition());
    cube.Draw(shader);
}

ObjectEvent  Goal::OnPlayerEnter(){
    return ObjectEvent::GoalReached;
}

