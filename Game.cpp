#include "Game.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shapes/Plane.h"
#include "Shapes/Cube.h"

Game::Game()
    : state(GameState::Playing),
      player(collisionmesh,Transform(), 1.0f),
      plane(10.0f),
      cube(1.0f),
    shader(
          "../Shaders/VertexShader.SHADER",
          "../Shaders/FragmentShader.SHADER"
      )
{
}

void Game::Initialize()
{
    player.SetPosition(glm::vec3{0.0,2.0,0.0});
    // //シェーダーの作成
    // shader = Shader("../Shaders/VertexShader.SHADER", "../Shaders/FragmentShader.SHADER");
    //テクスチャの作成
    //texture1
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    //テクスチャ初期設定
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);//STはxとy軸
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //stbによるテクスチャ画像読み込み
    int width,height,nrChannles;
    unsigned char*data = stbi_load("../Textures/wall.jpg",&width,&height,&nrChannles,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout<<"Failed to load texture"<< std::endl;
    }
    stbi_image_free(data);
    //texture2
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    //テクスチャ初期設定
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);//STはxとy軸
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //stbによるテクスチャ画像読み込み
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../Textures/awesomeface.png",&width,&height,&nrChannles,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout<<"Failed to load texture"<< std::endl;
    }

    stbi_image_free(data);

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID,"texture1"),0);
    shader.setInt("texture2",1);
    //床初期化
    plane.GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    //床のコリジョンを設定
    plane.RegisterCollision(collisionmesh);

    //箱を出力
    cube.GetTransform().SetPosition(glm::vec3(2.0f,0.5f,0.0f));
    cube.RegisterCollision(collisionmesh);

    //当たり判定のある頂点を表示
    // collisionmesh.prindDebug();
}

void Game::Update(float dt){
    switch (state)
    {
    case GameState::Title:
        break;

    case GameState::Playing:
        UpdatePlaying(dt);
        break;

    case GameState::Goal:
        break;

    case GameState::Result:
        break;
    }
}

void Game::UpdatePlaying(float dt){
        shader.use();
        player.SetMoveSpeed(10);
        //カメラの移動

        //テクスチャをミックスする度合いを変更
        mix = 1;
        int mixTexture = glGetUniformLocation(shader.ID,"smileMix");
        glUniform1f(mixTexture,mix);


        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // 画面をクリアするときの背景色を設定（暗い青緑色）
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 設定した色でカラーバッファ（画面）を実際に塗りつぶしてクリア
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //色を変更

        //テクスチャを使用
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);
        
        //カメラ関連
        camera.Follow(player.GetPosition(),dt);
        camera.FollowRotate(player.GetPosition(), 100.0,dt);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view",view);
        //シェーダーに反映ture,mix);

        // 各ゲームオブジェクトが自分の Transform からモデル行列を設定して描画する。
        player.MoveWithCameraOrientation(camera,dt);
        player.Update(dt);
        player.Draw(shader);
        plane.Draw(shader);
        cube.Draw(shader);

        // glm::mat4 trans = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // unsigned int transformLoc = glGetUniformLocation(shader.ID,"transform");
        // glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));

        // glBindVertexArray(VAO[0]);
        for(unsigned int i = 0; i < 10; i++)
        {
            // glm::mat4 model = glm::mat4(1.0f);
            // model = glm::translate(model, cubePositions[i]);
            // float angle = 20.0f * i; 
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            // shader.setMat4("model",model);

            // cube.Draw();
            // glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glDrawArrays(GL_TRIANGLES,0,36);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
}
