#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


// ウィンドウサイズ変更時に呼ばれるコールバック関数（ウィンドウサイズが変わっても描画範囲を追従させる）
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// キーボード入力を処理する関数（特定のキーが押されたかチェックする）
void processInput(GLFWwindow *window);

// 画面の初期サイズ（幅と高さ）
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//シェーダーの作成
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos,1.0);\n"
    "vertexColor= vec4(0.5,0.0,0.0,1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

const char *fragmentShaderSourceYellow = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
    "}\n\0";

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

    // //頂点シェーダーの初期化
    unsigned int vertexShader ;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //フラグメントシェーダーの初期化
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //
    unsigned int fragmentShaderYellow;
    fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
    glCompileShader(fragmentShaderYellow);
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShaderYellow,512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //シェーダープログラムの作成
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
     // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    

    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2,vertexShader);
    glAttachShader(shaderProgram2,fragmentShaderYellow);
    glLinkProgram(shaderProgram2);
     // check for linking errors
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // glDeleteShader(fragmentShaderYellow);

    //四角形定義
    float vertices[] = {
        //first triangle
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
         // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 
    };  
    // float vertices[] = {
    //     0.5f,  0.5f, 0.0f,  // top right
    //     0.5f, -0.5f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f,  // bottom left
    //     -0.5f,  0.5f, 0.0f   // top left 
    // };
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

    //VAOをバインド
    glBindVertexArray(VAO[0]);
    //VBOをバインド
    glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
    //頂点データを入力
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

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
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);
    
    // ウィンドウが閉じられる指示が出るまで、メインループを繰り返す
    while (!glfwWindowShouldClose(window))
    {
        // エスケープキーなどの入力入力を監視・処理
        processInput(window);

        // 画面をクリアするときの背景色を設定（暗い青緑色）
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 設定した色でカラーバッファ（画面）を実際に塗りつぶしてクリア
        glClear(GL_COLOR_BUFFER_BIT);
        //色を変更
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue)/2.0f)+0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        //三角形を描画
        //glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]);
        // glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        // glBindVertexArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // カラーバッファを入れ替えて、描画した内容を実際に画面に表示（ダブルバッファリング）
        glfwSwapBuffers(window);

        // キーボードやマウスの操作などのイベントを検知・処理
        glfwPollEvents();
    }

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
}

// ウィンドウサイズ変更時に呼ばれる関数の本体
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 新しいウィンドウサイズに合わせて、OpenGLが描画する領域（ビューポート）を更新
    glViewport(0, 0, width, height);
}