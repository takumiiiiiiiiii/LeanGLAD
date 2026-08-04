#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include "Entity/Player.h"
#include "Objects.h"
#include "Shapes/Plane.h"
#include "Shapes/Cube.h"
#include "Core/Input.h"
#include "Entity/Transform.h"
#include "Entity/Player.h"

// ウィンドウサイズ変更時に呼ばれるコールバック関数（ウィンドウサイズが変わっても描画範囲を追従させる）
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// キーボード入力を処理する関数（特定のキーが押されたかチェックする）
void processInput(GLFWwindow *window);

void cursor_enter_callback(GLFWwindow* window, int entered);

//マウスがどんな入力をされたか
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

// 画面の初期サイズ（幅と高さ）
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//画像のミックス具合
float mix;

//カメラ場所
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//カメラ移動
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//時間
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame



int main()
{
    
    // GLFWライブラリの初期化処理
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // OpenGLのバージョンとプロファイルの設定（ここではOpenGL 3.3 Core Profileを指定）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // macOS向けの互換性設定（Mac環境の場合のみ実行される）
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // ウィンドウの生成（サイズ、タイトル、モニター指定などを設定）
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "LearnOpenGL",
        nullptr,
        nullptr
    );

    // ウィンドウの生成に失敗した場合のエラー処理
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // 初期化したGLFWの終了処理
        return -1;
    }

    // 作成したウィンドウを現在のOpenGL描画対象（コンテキスト）に設定
    glfwMakeContextCurrent(window);
    //ウィンドウのカーソル入退室記録
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    //カーソル座標をコールバック
    glfwSetCursorPosCallback(window, mouse_callback);
    Input::SetWindow(window);
    // GLADの初期化。これを行わないと最初のOpenGL関数呼び出しでクラッシュする
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    
    // ウィンドウサイズが変更されたときに呼び出す関数（コールバック）を登録
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    //シェーダーの作成
    Shader shader("../Shaders/VertexShader.SHADER", "../Shaders/FragmentShader.SHADER");
    //テクスチャの作成
    unsigned int texture1,texture2;

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

    // // //頂点シェーダーの初期化
    // unsigned int vertexShader ;
    // vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,0.0f,0.0f,0.0f,0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f,1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f,1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f,1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,0.0f,0.0f,0.0f,0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,0.0f,0.0f,0.0f,0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,0.0f,0.0f,0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.0f,1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.0f,1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,0.0f,0.0f,0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f,1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,0.0f,0.0f,0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,0.0f,0.0f,0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,0.0f,0.0f,0.0f,0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f,1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f,1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f,1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,0.0f,0.0f,0.0f,0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,0.0f,0.0f,0.0f,0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
    };  
    //VBOは頂点バッファオブジェクト(Vertices Buffer Object)
    //VAOは頂点配列オブジェクト(Vertices Array Object)
    //EBOは要素バッファオブジェクト(Element Buffer Objects)
    unsigned int EBO;
    unsigned int VBO[2],VAO[2];
    //一つ以上の頂点バッファを生成
    glGenVertexArrays(1,&VAO[0]);
    glGenVertexArrays(1,&VAO[1]);
    glGenBuffers(1,&VBO[0]);
    glGenBuffers(1,&VBO[1]);
    glGenBuffers(1,&EBO);
    //テクスチャをバインド


    glBindVertexArray(VAO[0]);
    //VBOをバインド
    glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
    //頂点データを入力
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);

      //VAOをバインド
    glBindVertexArray(VAO[1]);
    //VBOをバインド
    glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
    //頂点データを入力
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 


    shader.use();
    Player player = Player(Transform(), 1.0f);
    // ウィンドウが閉じられる指示が出るまで、メインループを繰り返す
    while (!glfwWindowShouldClose(window))
    {
        //時間の更新
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        // エスケープキーなどの入力入力を監視・処理
        processInput(window);
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
        camera.Follow(player.GetPosition(),glm::vec3(0.0f, 2.0f, 5.0f), deltaTime);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        //シェーダーに反映
        shader.setMat4("projection", projection);
        shader.setMat4("view",view);
        //カメラの移動

        //テクスチャをミックスする度合いを変更
        int mixTexture = glGetUniformLocation(shader.ID,"smileMix");
        glUniform1f(mixTexture,mix);

        //プレイヤーの更新
        glm::mat4 Pmodel = glm::mat4(1.0f);
        Pmodel = glm::translate(Pmodel,player.GetPosition());
        shader.setMat4("model",Pmodel);
        player.Update(deltaTime);
        //床の更新
        Plane plane;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        shader.setMat4("model",model);
        plane.Draw();

        // glm::mat4 trans = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // unsigned int transformLoc = glGetUniformLocation(shader.ID,"transform");
        // glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));

        glBindVertexArray(VAO[0]);
        Cube cube;
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

        // カラーバッファを入れ替えて、描画した内容を実際に画面に表示（ダブルバッファリング）
        glfwSwapBuffers(window);
        // キーボードやマウスの操作などのイベントを検知・処理
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO[0]);
    glDeleteBuffers(1, &VBO[0]);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO[1]);
    glDeleteBuffers(1, &VBO[1]);


    // メインループ終了後、ウィンドウを破棄
    glfwDestroyWindow(window);
    
    // GLFWの残りのリソースを解放して終了
    glfwTerminate();
    return 0;
}

// キーボード入力を処理する関数の本体
void processInput(GLFWwindow *window)
{

    // エスケープキー（ESCAPE）が押された場合、ウィンドウを閉じるフラグを立てる
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window,GLFW_KEY_K) == GLFW_PRESS)
        mix +=0.1f;
    if (glfwGetKey(window,GLFW_KEY_M) == GLFW_PRESS)
        mix -=0.1f;
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
        
}


bool cursorInsideWindow = false;

void cursor_enter_callback(GLFWwindow* window, int entered)
{
    cursorInsideWindow = entered == GLFW_TRUE;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!cursorInsideWindow)
        return;
    std::cout << "Mouse" << std::endl;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



// ウィンドウサイズ変更時に呼ばれる関数の本体
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 新しいウィンドウサイズに合わせて、OpenGLが描画する領域（ビューポート）を更新
    glViewport(0, 0, width, height);
}
