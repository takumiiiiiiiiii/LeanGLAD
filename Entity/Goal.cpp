#include "Goal.h"

Goal::Goal(CollisionMesh& groundCollision, GLuint textureID, float size = 1.0f)
:cube(size),
goalSize(size)

{
}

void Goal::Update(float deltaTime){
    glm::vec3 centerPos = cube.GetTransform().GetPosition()+glm::vec3(0.0f,goalSize/2.0f,0.0f);
    aabb.Set(centerPos,transform.GetScale());
    
}
bool Goal::GoalPlayer(AABB player_aabb){
    return aabb.Intersects(player_aabb);
 }

