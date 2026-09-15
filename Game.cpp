#include "Game.h"
#include "Shader.h"
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    static double lastX = xpos;
    static double lastY = ypos;

    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(ypos - lastY);

    lastX = xpos;
    lastY = ypos;

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)){
        game->camera.ProcessMouseMovement(
            xoffset * game->camera.MouseSensitivity,
            -yoffset * game->camera.MouseSensitivity
        );
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)){
        game->camera.ProcessMousePan(
            -xoffset * game->camera.MousePanSensitivity,
            -yoffset * game->camera.MousePanSensitivity
        );
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
       
    }
     game->camera.ProcessMouseForward(
            yoffset*game->camera.MouseForwardSensitivity
        );
}


Game::Game()
    : state(GameState::Title),
      player(collisionmesh,Transform(), 1.0f),

      blockWorld(collisionmesh,1.0),
    shader(
          "../Shaders/VertexShader.SHADER",
          "../Shaders/FragmentShader.SHADER"
      )
{
}

void Game::Initialize()
{
    isGoal = false;
    // ★ アルファブレンディングを有効化
    glEnable(GL_BLEND);
    // 重なり合った際の色の計算方法を指定 (前景色のAlpha値で合成)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    player.SetPosition(glm::vec3{0.0,2.0,0.0});

    // //シェーダーの作成
    // shader = Shader("../Shaders/VertexShader.SHADER", "../Shaders/FragmentShader.SHADER");
    // ★ OpenGL初期化完了後にテクスチャを読み込む
    cubeTexture = std::make_unique<Texture>("../Textures/NormalBox.png");
    wallTexture = std::make_unique<Texture>("../Textures/wall.png");
    logoTexture = std::make_unique<Texture>("../Textures/title.png");

    blockWorld.SetCubeTextures(cubeTexture->GetID(),wallTexture->GetID());
    shader.use();
    shader.setInt("texture1",0);
    shader.setInt("texture2",1);
    player.SetTexture(cubeTexture->GetID());

    blockWorld.LoadCubeStateFromFile("/Users/x23029xx/Documents/GitHub/LeanGLAD/coordinates_new.txt");
}



void Game::Update(float dt){
    // 状態が変更されたかを検知
    if (state != previousState) {
        if (state == GameState::Playing) {
            blockWorld.DeleteAllBlocks(); // すべてのブロックを削除
            collisionmesh.removeTrianglesEverything(); // コリジョンメッシュもクリア
            blockWorld.SetCubeTextures(cubeTexture->GetID(),wallTexture->GetID());
            blockWorld.LoadCubeStateFromFile("/Users/x23029xx/Documents/GitHub/LeanGLAD/coordinates_new.txt");
            player.SetPosition(playerStartPos);
            isGoal = false;
        }
        // 他の状態への遷移時処理もここに書ける
        // if (state == GameState::Title) OnEnterTitle();

        previousState = state; // 現在の状態を保存
    }
    switch (state)
    {
    case GameState::Title:
        UpdateTitle(dt);
        break;
    case GameState::StageSelect:
        UpdateStageSelect(dt);
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
        state = GameState::StageSelect;
        
    }
    if(Input::IsKeyJustPressed(GLFW_KEY_E)){
        state = GameState::Editor;
    }
    shader.use();

    // 画面をクリアするときの背景色を設定（暗い青緑色）
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 設定した色でカラーバッファ（画面）を実際に塗りつぶしてクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Cube title(1.0,logoTexture->GetID());
    // テクスチャの幅と高さを取得
    float imgWidth  = static_cast<float>(logoTexture->GetWidth());
    float imgHeight = static_cast<float>(logoTexture->GetHeight())*2.0;

    // アスペクト比（幅 ÷ 高さ）を計算
    float aspectRatio = imgWidth / imgHeight;

    // 基準となるサイズを決定（例: 高さを 2.0 に固定する場合）
    float targetHeight = 0.9f;
    float targetWidth  = targetHeight * aspectRatio; // 比率に合わせて幅を決定

    // スケールを設定（Z軸は平面描画のため 1.0f）
    title.SetScale({ targetWidth, targetHeight, 1.0f });
    title.SetTranformPosition({0.0,0.5,0.0});
    title.Draw(shader);
    
    shader.setMat4("projection", 1);
    shader.setMat4("view",1);
}

void Game::UpdateStageSelect(float dt){
    shader.use();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 設定した色でカラーバッファ（画面）を実際に塗りつぶしてクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    stageSelector.detectStages("/Users/x23029xx/Documents/GitHub/LeanGLAD/Stages");
    stageSelector.handleInput();
    stageSelector.Draw(shader);
    camera.Follow(stageSelector.GetStageCubePosition(),dt);
    std::cout << stageSelector.GetSelectedStageIndex() << std::endl;

    if(Input::IsJumpJustPressed()){
        state = GameState::Playing;
    }
    // shader.setMat4("projection", 1);
    // shader.setMat4("view",1);
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view",view);
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
    glClearColor(101.0f/255.0f, 187.0f/255.0, 233.0f/255.0, 1.0f);
    // 設定した色でカラーバッファ（画面）を実際に塗りつぶしてクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //色を変更

    if(isBlocksSlect){
        if(!isBlocksSlectPrev){
            blockWorld.PlaceBlock(blockWorld.SnapToGrid(selectCube), "Cube", {1.0,1.0,1.0}, false);
        }
        //カメラの前方向ベクトルを取得し、y成分を0にして水平移動のみを考慮
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
            blockWorld.DeleteBlockSelected(player.GetPosition(),"Cube");
            if(blockWorld.SnapToGrid(player.GetPosition())!=selectCube){
                blockWorld.DeleteBlockSelected(selectCube,"Cube");
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
    
    blockWorld.DrawAll(shader,dt);
    ObjectEvent event = blockWorld.CheckPlayerEnter(player.getAABB());
    if(event == ObjectEvent::GoalReached){
        isGoal = true;
    }

    //カメラ反映
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view",view);

    if(player.GetPosition().y<-10.0f){
        
        player.SetPosition(playerStartPos);
    }
    
    if(isGoal){
        state = GameState::Title;
    }
}



void Game::UpdateEditor(float dt){
    stageEditor.UpdateEditor(dt);
    stageEditor.PlaceBlockByMouse(collisionmesh,blockWorld,view,projection);
    stageEditor.RemoveBlockByMouse(collisionmesh,blockWorld,view,projection);
    stageEditor.SelectBlockByMouse(collisionmesh,blockWorld,view,projection);
    if(Input::IsKeyJustPressed(GLFW_KEY_P)){
        state = GameState::Playing;
    }
    shader.use();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 設定した色でカラーバッファ（画面）を実際に塗りつぶしてクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    //マウス入力
     if(Input::IsKeyPressed(GLFW_KEY_Z)){
        blockWorld.SaveToFile("/Users/x23029xx/Documents/GitHub/LeanGLAD/coordinates_new.txt","blocks");
    }
    if(Input::IsKeyPressed(GLFW_KEY_X)){
        blockWorld.LoadCubeStateFromFile("/Users/x23029xx/Documents/GitHub/LeanGLAD/coordinates_new.txt");
    }

    //地形の描画
    shader.setInt("texture1",0);
    blockWorld.DrawAll(shader,dt);

    //カメラ反映
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view",view);
}

