#include "Game.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shapes/Plane.h"
#include "Shapes/Cube.h"


Game::Game()
    : state(GameState::Title),
      player(collisionmesh,Transform(), 0.8f),

      blockWorld(collisionmesh,1.0),
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
    unsigned char*data = stbi_load("../Textures/NormalBox.png",&width,&height,&nrChannles,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
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
    shader.setInt("texture1",0);
    shader.setInt("texture2",1);
    //床初期化
    // plane.GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    //床のコリジョンを設定

    blockWorld.LoadCubeStateFromFile("/Users/x23029xx/Documents/GitHub/LeanGLAD/coordinates_new.txt");
    
}

void Game::Update(float dt){
    switch (state)
    {
    case GameState::Title:
        UpdateTitle(dt);
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

void Game::UpdateTitle(float dt){
    if(Input::IsJumpJustPressed()){
        state = GameState::Playing;
    }
}

void Game::UpdatePlaying(float dt){
    // ImGui::ShowDemoWindow();
        //マウス入力
        if (Input::IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            double mx, my;
            Input::GetMousePosition(mx, my);

            glm::vec3 rayOrigin, rayDir;
            //クリック位置のワールド座標取得
            screenPosToWorldRay(mx, my, SCR_WIDTH, SCR_HEIGHT, view,projection, rayOrigin,rayDir);

            float dist;
            glm::vec3 normal;
            if (collisionmesh.raycast(rayOrigin, rayDir, dist, normal))
            {
                // glm::vec3 hitPos = rayOrigin + rayDir * dist;
                // std::cout <<"hitPos"<<hitPos.x <<":";
                // std::cout           <<hitPos.y <<":";
                // std::cout           <<hitPos.z << std::endl;
                std::cout << "rayOrigin.y=" << rayOrigin.y
                        << " dist=" << dist
                        << " normal=(" << normal.x << "," << normal.y << "," << normal.z << ")"
                        << std::endl;

                glm::vec3 hitPos = rayOrigin + rayDir * dist;
                std::cout << "hitPos.y=" << hitPos.y << std::endl;
                // ヒット面の法線方向にキューブ半径(0.5)分ずらす
                // → Planeの上面をクリックすれば真上に、既存キューブの側面をクリックすればその横に置ける
                glm::vec3 placePos = hitPos + normal * 0.5f;
                // PlaceObject(hitPos); // ここで実際にオブジェクトを生成・配置する
                blockWorld.PlaceBlock(placePos);
            }

        }
        if (Input::IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_RIGHT)){
            double mx, my;
            Input::GetMousePosition(mx, my);

            glm::vec3 rayOrigin, rayDir;
            //クリック位置のワールド座標取得
            screenPosToWorldRay(mx, my, SCR_WIDTH, SCR_HEIGHT, view,projection, rayOrigin,rayDir);
             float dist;
            glm::vec3 normal;
            if (collisionmesh.raycast(rayOrigin, rayDir, dist, normal))
            {
                // glm::vec3 hitPos = rayOrigin + rayDir * dist;
                // std::cout <<"hitPos"<<hitPos.x <<":";
                // std::cout           <<hitPos.y <<":";
                // std::cout           <<hitPos.z << std::endl;
                std::cout << "rayOrigin.y=" << rayOrigin.y
                        << " dist=" << dist
                        << " normal=(" << normal.x << "," << normal.y << "," << normal.z << ")"
                        << std::endl;

                glm::vec3 hitPos = rayOrigin + rayDir * dist;
                std::cout << "hitPos.y=" << hitPos.y << std::endl;
                // ヒット面の法線方向にキューブ半径(0.5)分ずらす
                // → Planeの上面をクリックすれば真上に、既存キューブの側面をクリックすればその横に置ける
                // PlaceObject(hitPos); // ここで実際にオブジェクトを生成・配置する
                // 表面上の座標はセル境界になるため、少し内側へ戻して対象セルを求める
                const glm::vec3 deletePos = hitPos - normal * 0.001f;
                std::cout << "delete result="
                          << blockWorld.DeleteBlockSelected(deletePos) << std::endl;
            }
        }
        if(Input::IsKeyPressed(GLFW_KEY_Z)){
            blockWorld.SaveToFile("/Users/x23029xx/Documents/GitHub/LeanGLAD/coordinates.txt","blocks");
        }
        if(Input::IsKeyPressed(GLFW_KEY_X)){
            blockWorld.LoadCubeStateFromFile("/Users/x23029xx/Documents/GitHub/LeanGLAD/coordinates.txt");
        }
        if(Input::IsBecomeBlockJustPressed()){
            
            //ブロックセレクトに入った時
            if(!isBlocksSlectPrev){
                selectCube = player.GetPosition();
            }
            isBlocksSlect = !isBlocksSlect;
        }



        shader.use();
        player.SetMoveSpeed(10);
        //カメラの移動

        //テクスチャをミックスする度合いを変更
        mix = 0;
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
        


        //シェーダーに反映ture,mix);

        // Playerの設定
        // shader.setInt("texture1",1);//プレイヤーのテクスチャ
        if(isBlocksSlect){
            glm::vec3 front = camera.Front;
            front.y = 0;
            glm::vec3 right = camera.Right;
            glm::vec3 up = camera.Up;
            right = glm::normalize(right);
            glm::vec3 movement(0.0f);
            //ブロックの上か下にブロックがある場合は上下移動を優先する　
            glm::vec3 isUpBlock = blockWorld.SnapToGrid(selectCube+up*blockWorld.Getcubesize());
            glm::vec3 isDownBlock = blockWorld.SnapToGrid(selectCube-up*blockWorld.Getcubesize());
            if(blockWorld.CheckBlockSelectedPos(isUpBlock)||blockWorld.CheckBlockSelectedPos(isDownBlock)){
                movement += up*Input::GetJustMoveInput().y;
            }else if(player.GetPosition().y==isUpBlock.y||player.GetPosition().y==isDownBlock.y){
                movement += up*Input::GetJustMoveInput().y;
            }else{
                movement += front*Input::GetJustMoveInput().y;
            }

            movement += right*Input::GetJustMoveInput().x;
            //プレイヤーの移動方向の座標
            glm::vec3 movePos=blockWorld.SnapToGrid(selectCube+movement*blockWorld.Getcubesize());
            //移動方向にブロックがあるか
            if(blockWorld.CheckBlockSelectedPos(movePos)||blockWorld.SnapToGrid(player.GetPosition())==movePos){
                selectCube = movePos;
            }
            camera.Follow(selectCube,dt);
            camera.FollowRotate(selectCube, 100.0,dt);
            isBlocksSlectPrev = true;

        }else{
            //自分以外のブロックに移動していた場合
            if(isBlocksSlectPrev&&blockWorld.SnapToGrid(player.GetPosition())!=selectCube){
                blockWorld.DeleteBlockSelected(selectCube);
                blockWorld.PlaceBlock(player.GetPosition());
                player.SetPosition(selectCube); 
            }
            isBlocksSlectPrev = false;
            //カメラ関連
            camera.Follow(player.GetPosition(),dt);
            camera.FollowRotate(player.GetPosition(), 100.0,dt);
            player.MoveWithCameraOrientation(camera,dt);
            player.Update(dt);
        }
        player.Draw(shader);
        player.TrunBlock(isBlocksSlect,blockWorld,dt);

        //地形の描画
        shader.setInt("texture1",0);

        blockWorld.DrawAll(shader);

        //カメラ反映
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view",view);
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
