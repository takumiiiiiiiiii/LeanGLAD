#pragma once
#include "Object.h"
#include "../Shapes/Cube.h"
#include "Player.h"
#include "../Collision/AABB.h"
// #include "../Game.h"
class Shader;
class Goal : public Object{
    public:
        explicit Goal(float input_size = 1.0f, GLuint textureID = 0);
        virtual ~Goal() = default;
        void Update(float deltaTime) override;
        void Draw(Shader& shader) override;
        ObjectEvent OnPlayerEnter()override;
        bool CheckPlayerEnter(const AABB& playerAABB) override;
    private:
    float goalSize;
    Cube cube;
    AABB aabb;
};