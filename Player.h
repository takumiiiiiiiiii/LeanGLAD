#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Player
{
    public:

        // the program ID
        unsigned int ID;
        glm::vec3 Position;
        float MoveSpeed;
        // constructor reads and builds the shader
        Player(glm::vec3 StartPos);
        // use/activate the shader
        void use();
        //shader settings
        void setMat4(const std::string &name,glm::mat4 value);
        // utility uniform functions
        void setBool(const std::string &name, bool value) const;   
        void setInt(const std::string &name, int value) const;   
        void setFloat(const std::string &name, float value) const;

    private:
        void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
