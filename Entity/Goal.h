#pragma once
#include "Object.h"
#include "../Shapes/Cube.h"
#include "Player.h"
#include "../Collision/AABB.h"
class Goal : public Object{
    public:
        explicit Goal(CollisionMesh& groundCollision, GLuint textureID, float size);
        virtual ~Goal() = default;
        void Update(float deltaTime) override;
        // void Draw(Shader& shader) override;
        
        bool GoalPlayer(AABB player_aabb);
    private:
    float goalSize;
    Cube cube;
    AABB aabb;
};