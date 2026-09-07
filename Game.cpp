#include "Game.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shapes/Plane.h"
#include "Shapes/Cube.h"

namespace
{
    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game == nullptr || game->state != GameState::Editor ||
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
    {
        firstMouse = true;
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    game->camera.ProcessMouseMovement(
        xoffset * game->camera.MouseSensitivity,
        yoffset * game->camera.MouseSensitivity
    );
}


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
    data = stbi_load("../Textures/wall.png",&width,&height,&nrChannles,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout<<"Failed to load texture"<< std::endl;
    }

    stbi_image_free(data);

    blockWorld.SetCubeTextures(texture1, texture2);
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

    case GameState::Editor:
        UpdateEditor(dt);
        break;
    }
}


void Game::UpdateTitle(float dt){
    if(Input::IsJumpJustPressed()){
        state = GameState::Playing;
    }
    if(Input::IsKeyJustPressed(GLFW_KEY_E)){
        state = GameState::Editor;
    }
}

void Game::UpdatePlaying(float dt){
    if(Input::IsKeyJustPressed(GLFW_KEY_E)){
        state = GameState::Editor;
    }
    if(Input::IsBecomeBlockJustPressed()){
        //ブロックセレクトに入った時
        if(!isBlocksSlectPrev){
            selectCube = player.GetPosition();
        }
        isBlocksSlect = !isBlocksSlect;
    }

    shader.use();
    //カメラの移動

    //テクスチャをミックスする度合いを変更
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
    if(isBlocksSlect){
        if(!isBlocksSlectPrev){
            blockWorld.PlaceBlock(blockWorld.SnapToGrid(selectCube), objectTypes[currentTypeIndex], value, true);
        }
        glm::vec3 front = camera.Front;
        front.y = 0;
        glm::vec3 right = camera.Right;
        glm::vec3 up = camera.Up;
        right = glm::normalize(right);
        glm::vec3 movement(0.0f);
        //ブロックの上か下にブロックがある場合は上下移動を優先する　
        glm::vec3 isUpBlock = blockWorld.SnapToGrid(selectCube+up*blockWorld.Getcubesize());
        glm::vec3 isDownBlock = blockWorld.SnapToGrid(selectCube-up*blockWorld.Getcubesize());
        //ブロックの上か下にブロックがある場合は上下移動を優先する
        if(blockWorld.CheckBlockSelectedPos(isUpBlock)||blockWorld.CheckBlockSelectedPos(isDownBlock)){
            movement += up*Input::GetJustMoveInput().y;
            if(!blockWorld.CheckBlockSelectedPos(isUpBlock)&&movement.y>0){//上にブロックがない場合は上に移動できる
                movement.y = 0;
                movement += front*Input::GetJustMoveInput().y;
            }else if(!blockWorld.CheckBlockSelectedPos(isDownBlock)&&movement.y<0){//下にブロックがない場合は下に移動できる
                movement.y = 0;
                movement += front*Input::GetJustMoveInput().y;
            }
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
        if(isBlocksSlectPrev){
            blockWorld.DeleteBlockSelected(player.GetPosition());
            if(blockWorld.SnapToGrid(player.GetPosition())!=selectCube){
                blockWorld.DeleteBlockSelected(selectCube);
                blockWorld.PlaceBlock(player.GetPosition());
                player.SetPosition(selectCube); 
            }
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
}

void Game::UpdateEditor(float dt){
     // ImGuiのデバッグウィンドウを表示
    ImGui::Begin("Debug");
    ImGui::Combo("Editor State", &currentStateIndex, editorStates, IM_ARRAYSIZE(editorStates));
    ImGui::Combo("Object Type", &currentTypeIndex, objectTypes, IM_ARRAYSIZE(objectTypes));
    ImGui::InputInt("Value", &value);
    ImGui::Text("Selected: %s", objectTypes[currentTypeIndex]);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
    if(Input::IsKeyJustPressed(GLFW_KEY_P)){
        state = GameState::Playing;
    }
    camera.ProcessEditorMovement(dt);
    shader.use();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 設定した色でカラーバッファ（画面）を実際に塗りつぶしてクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //色を変更
    //テクスチャを使用
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texture2);
   
    // ImGui::ShowDemoWindow();
    
    //マウス入力
    RemoveBlockByMouse();
    PlaceBlockByMouse();
     if(Input::IsKeyPressed(GLFW_KEY_Z)){
        blockWorld.SaveToFile("/Users/x23029xx/Documents/GitHub/LeanGLAD/coordinates_new.txt","blocks");
    }
    if(Input::IsKeyPressed(GLFW_KEY_X)){
        blockWorld.LoadCubeStateFromFile("/Users/x23029xx/Documents/GitHub/LeanGLAD/coordinates_new.txt");
    }

    //地形の描画
    shader.setInt("texture1",0);

    blockWorld.DrawAll(shader);

    //カメラ反映
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view",view);

}
void Game::RemoveBlockByMouse(){
    if (ImGui::GetIO().WantCaptureMouse ||
        !Input::IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        return;
    }
            double mx, my;
            Input::GetMousePosition(mx, my);
            glm::vec3 rayOrigin, rayDir;
            //クリック位置のワールド座標取得
            screenPosToWorldRay(mx, my, SCR_WIDTH, SCR_HEIGHT, view,projection, rayOrigin,rayDir);
             float dist;
            glm::vec3 normal;
            if (collisionmesh.raycast(rayOrigin, rayDir, dist, normal))
            {
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

void Game::PlaceBlockByMouse(){
    if (ImGui::GetIO().WantCaptureMouse ||
        !Input::IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        return;
    }
     if (editorStates[currentStateIndex] != std::string("Place"))
    {
        return;
    }

    double mx, my;
    Input::GetMousePosition(mx, my);

    glm::vec3 rayOrigin, rayDir;
    //クリック位置のワールド座標取得
    screenPosToWorldRay(mx, my, SCR_WIDTH, SCR_HEIGHT, view,projection, rayOrigin,rayDir);

    float dist;
    glm::vec3 normal;
    if (collisionmesh.raycast(rayOrigin, rayDir, dist, normal))
    {
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
        blockWorld.PlaceBlock(placePos, objectTypes[currentTypeIndex], value); // ここで実際にオブジェクトを生成・配置する
    }
}

void Game::SelectBlockByMouse(){
    if (ImGui::GetIO().WantCaptureMouse ||
        !Input::IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        return;
    }

        double mx, my;
        Input::GetMousePosition(mx, my);

        glm::vec3 rayOrigin, rayDir;
        //クリック位置のワールド座標取得
        screenPosToWorldRay(mx, my, SCR_WIDTH, SCR_HEIGHT, view,projection, rayOrigin,rayDir);

        float dist;
        glm::vec3 normal;
        if (collisionmesh.raycast(rayOrigin, rayDir, dist, normal))
        {
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
            blockWorld.PlaceBlock(placePos, objectTypes[currentTypeIndex], value); // ここで実際にオブジェクトを生成・配置する
        }
}