#include "BlockWrold.h"
#include "Shapes/Cube.h"
#include "Shapes/Plane.h"
#include "Entity/Object.h"

BlockWorld::BlockWorld(CollisionMesh& collisionMesh, float cubeSize)
    : collisionMesh(collisionMesh), cubeSize(cubeSize)
{
    
}

void BlockWorld::SetCubeTextures(GLuint cubeTexture, GLuint wallTexture)
{
    this->cubeTexture = cubeTexture;
    this->wallTexture = wallTexture;
}

 //ワールド座標をせる座標に
glm::vec3 BlockWorld::SnapToGrid(const glm::vec3& worldPos)const{
    glm::vec3 cellPos;
    cellPos.x = (std::floor(worldPos.x / cubeSize) + 0.5f) * cubeSize;
    cellPos.y = (std::floor(worldPos.y / cubeSize) + 0.5f) * cubeSize;
    cellPos.z = (std::floor(worldPos.z / cubeSize) + 0.5f) * cubeSize;
    return cellPos;
};
//選択した座標にブロックが存在するか
bool BlockWorld::CheckBlockSelectedPos(const glm::vec3& Pos)const{
    glm::vec3 inputcellPos = SnapToGrid(Pos);
    glm::vec3 blockCellPos;
    for(const auto& block:blocks){
        if(block.objectType != "Cube"){
            continue;
        }
        blockCellPos = SnapToGrid(block.position);
        if(blockCellPos == inputcellPos){
            return true;
        }
    }
    return false;
}
//
bool BlockWorld::GetBlockPosition(const glm::vec3& pos,glm::vec3& blockPosition)const{
    const glm::vec3 cellPos = SnapToGrid(pos);
    for (const auto& block : blocks)
    {
        if (SnapToGrid(block.position) == cellPos)
        {
            blockPosition = block.position;
            return true;
        }
    }
    return false;
}
bool BlockWorld::DeleteBlockSelected(
    const glm::vec3& Pos,
    const std::string& objectName)
{
    const glm::vec3 cellPos = SnapToGrid(Pos);
    bool deleted = false;

    for (auto it = blocks.begin(); it != blocks.end();)
    {
        // 座標が一致しない場合
        if (SnapToGrid(it->position) != cellPos) {
            ++it;
            continue;
        }

        // objectNameが指定されている場合は、その種類だけ削除
        if (!objectName.empty() && it->objectType != objectName) {
            ++it;
            continue;
        }

        if (it->objectType == "Cube" ||
            it->objectType == "Wall" ||
            it->objectType == "wall")
        {
            if (auto* cubePtr = dynamic_cast<Cube*>(it->object.get())) {
                if (!collisionMesh.removeCubeById(cubePtr->GetCubeId())) {
                    std::cerr << "警告: Cube ID による削除に失敗しました\n";
                }
            }
        }
        else if (it->objectType == "Plane")
        {
            collisionMesh.removeTrianglesForObject(it->object.get());
        }

        it = blocks.erase(it);
        deleted = true;

        // 名前指定時は1個だけ削除
        if (!objectName.empty()) {
            break;
        }
    }

    return deleted;
}

 PlacedBlock*  BlockWorld::GetBlockSelected(const glm::vec3& Pos){
    const glm::vec3 cellPos = SnapToGrid(Pos);
    for (auto& block : blocks)
    {
        if (SnapToGrid(block.position) == cellPos)
        {
            return &block;
        }
    }
    return nullptr;
 }

bool BlockWorld::IsOccupied(const glm::vec3& worldPos) const
{
    // キューブサイズの1割程度を許容誤差として、同じセルへの重複配置を防ぐ
    const float epsilon = cubeSize * 0.1f;
    for (const auto& block : blocks)
    {
        if (glm::distance(block.position, worldPos) < epsilon)
            return true;
    }
    return false;
}

// ...existing code...

void BlockWorld::PlaceBlock(
    const glm::vec3& worldPos,
    const std::string& objectType,
    const glm::vec3& objectSize,
    bool isShown)
{
    glm::vec3 snapped = SnapToGrid(worldPos);

    if (IsOccupied(snapped))
        return;

    std::unique_ptr<Object> newObject;

    if (objectType == "Cube") {
        auto cube = std::make_unique<Cube>(1.0, cubeTexture);
        cube->SetTranformPosition(snapped);
        cube->GetTransform().SetScale(objectSize);
        cube->RegisterCollisionAndStoreIndices(collisionMesh);
        newObject = std::move(cube);
    }
    else if (objectType == "Wall" || objectType == "wall") {
        auto cube = std::make_unique<Cube>(1.0, wallTexture);
        cube->SetTranformPosition(snapped);
        cube->GetTransform().SetScale(objectSize);
        cube->RegisterCollisionAndStoreIndices(collisionMesh);
        newObject = std::move(cube);
    }
    else if (objectType == "Plane") {
        auto plane = std::make_unique<Plane>(1.0);
        snapped.y -= 0.5f * objectSize.y;
        plane->GetTransform().SetPosition(snapped);
        plane->GetTransform().SetScale(objectSize);
        plane->RegisterCollision(collisionMesh);
        newObject = std::move(plane);
    }
    else {
        std::cerr << "警告: 未対応のオブジェクト種類です: "
                  << objectType << "\n";
        return;
    }

    blocks.push_back(PlacedBlock{
        snapped,
        objectType,
        objectSize,
        std::move(newObject),
        isShown
    });
}



bool BlockWorld::SaveToFile(const std::string& filepath,const std::string& filename) const
{
   std::ofstream ofs;
 
    // 失敗時に例外を投げるよう設定 (failbit / badbit)
    ofs.exceptions(std::ofstream::failbit | std::ofstream::badbit);
 
    try {
        // trunc: 既存内容を上書き。追記したい場合は std::ios::app を使う
        ofs.open(filepath, std::ios::out | std::ios::trunc);
 
        ofs << std::fixed << std::setprecision(6);
 
        // 拡張フォーマット: x,y,z,objectType,size
        for (const auto& p : blocks)
        {
            const glm::vec3 objPos =
                p.object->GetTransform().GetPosition();
            const glm::vec3 objSize =
                p.object->GetTransform().GetScale();
            ofs << objPos.x << ","
                << objPos.y << ","
                << objPos.z << ","
                << p.objectType << ","
                << objSize.x << ","
                << objSize.y << ","
                << objSize.z << ","
                << (p.isShown ? "true" : "false")
                << "\n";
        }
        ofs.close();
        std::cout << "書き込み成功: " << filepath << " (" << blocks.size() << " 件)\n";
    }
    catch (const std::ios_base::failure& e) {
        // open/write/close いずれかで発生したI/Oエラー
        std::cerr << "ファイル操作エラー: " << e.what()
                  << " (path: " << filepath << ")\n";
        throw; // 呼び出し元にも通知したい場合は再送出。不要ならここで握りつぶしてもよい
    }
    catch (const std::exception& e) {
        std::cerr << "予期しないエラー: " << e.what() << "\n";
        throw;
    }
    return true;
}

// 1行分の "x,y,z" 文字列をパースする
glm::vec3 BlockWorld::parseLine(const std::string& line, std::size_t lineNumber) const{
    std::stringstream ss(line);
    std::string token;
    std::vector<float> values;
 
    while (std::getline(ss, token, ',')) {
        try {
            // stof は変換できる部分だけ読んで残りは無視するため、
            // 余分な文字が混ざっていないか厳密にチェックしたい場合は size_t pos を確認する
            std::size_t pos;
            float v = std::stof(token, &pos);
            if (pos != token.size()) {
                throw std::invalid_argument("余分な文字が含まれています: \"" + token + "\"");
            }
            values.push_back(v);
        }
        catch (const std::invalid_argument&) {
            throw std::runtime_error(
                "数値として解釈できません (行 " + std::to_string(lineNumber) +
                ", 値: \"" + token + "\")");
        }
        catch (const std::out_of_range&) {
            throw std::runtime_error(
                "値がfloatの範囲を超えています (行 " + std::to_string(lineNumber) +
                ", 値: \"" + token + "\")");
        }
    }
 
    if (values.size() != expectedColumnCount) {
        throw std::runtime_error(
            "列数が不正です (行 " + std::to_string(lineNumber) +
            ", 期待値: " + std::to_string(expectedColumnCount) + ", 実際: " + std::to_string(values.size()) + ")");
    }
 
    return glm::vec3{ values[0], values[1], values[2] };
}

// 拡張フォーマット: "x,y,z,objectType,size" をパースする
BlockWorld::ParsedBlockData BlockWorld::parseBlockLine(const std::string& line, std::size_t lineNumber) const{
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;
 
    while (std::getline(ss, token, ',')) {
        // 前後の空白を削除
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        tokens.push_back(token);
    }
 

    if (tokens.size() != expectedColumnCount) {
        throw std::runtime_error(
            "列数が不正です (行 " + std::to_string(lineNumber) +
            ", 期待値: " + std::to_string(expectedColumnCount) +
            ", 実際: " + std::to_string(tokens.size()) + ")");
    }
 
    try {
        // x, y, z をパース
        std::size_t pos;
        float x = std::stof(tokens[0], &pos);
        if (pos != tokens[0].size()) {
            throw std::invalid_argument("余分な文字が含まれています");
        }
        float y = std::stof(tokens[1], &pos);
        if (pos != tokens[1].size()) {
            throw std::invalid_argument("余分な文字が含まれています");
        }
        float z = std::stof(tokens[2], &pos);
        if (pos != tokens[2].size()) {
            throw std::invalid_argument("余分な文字が含まれています");
        }
        
        // objectType（文字列）
        std::string objectType = tokens[3];
        
        // size をパース
        float sizeX = std::stof(tokens[4], &pos);
        if (pos != tokens[4].size()) {
            throw std::invalid_argument("余分な文字が含まれています");
        }

        float sizeY = std::stof(tokens[5], &pos);
        if (pos != tokens[5].size()) {
            throw std::invalid_argument("余分な文字が含まれています");
        }

        float sizeZ = std::stof(tokens[6], &pos);
        if (pos != tokens[6].size()) {
            throw std::invalid_argument("余分な文字が含まれています");
        }
        // isShown をパース
        bool isShown = true; // デフォルト値
        std::string isShownStr = tokens[7];
        if (isShownStr == "true" || isShownStr == "1") {
            isShown = true;
        } else if (isShownStr == "false" || isShownStr == "0") {
            isShown = false;
        } else {
            throw std::invalid_argument("isShown は true/false または 1/0 で指定してください");
        }
        
        return ParsedBlockData{ glm::vec3{x, y, z}, objectType,glm::vec3{sizeX,sizeY,sizeZ}, isShown };
    }
    catch (const std::invalid_argument& e) {
        throw std::runtime_error(
            "数値として解釈できません (行 " + std::to_string(lineNumber) + ": " + std::string(e.what()) + ")");
    }
    catch (const std::out_of_range& e) {
        throw std::runtime_error(
            "値が範囲を超えています (行 " + std::to_string(lineNumber) + ")");
    }
}

//ファイルを読み込んで座標を読み込む
std::vector<glm::vec3> BlockWorld::readCoordinatesFromFile(const std::string& filepath)const {
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        // errno にOS側の詳細理由(権限がない/存在しないなど)が入っている
        throw std::runtime_error(
            "ファイルを開けません: " + filepath + " (" + std::strerror(errno) + ")");
    }
 
    std::vector<glm::vec3> points;
    std::string line;
    std::size_t lineNumber = 0;
 
    while (std::getline(ifs, line)) {
        ++lineNumber;
        if (line.empty()) continue; // 空行はスキップ
 
        // 行単位でパースエラーを catch し、1行の不正データで全体を止めない場合はここで処理
        points.push_back(parseLine(line, lineNumber));
    }
 
    if (ifs.bad()) {
        // eof以外の理由でループを抜けた場合 (読み込み中の物理的なI/Oエラーなど)
        throw std::runtime_error("読み込み中にエラーが発生しました: " + filepath);
    }
 
    return points;
}

// 拡張フォーマット "x,y,z,objectType,size" をファイルから読み込む
std::vector<BlockWorld::ParsedBlockData> BlockWorld::readBlockDataFromFile(const std::string& filepath) const {
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        throw std::runtime_error(
            "ファイルを開けません: " + filepath + " (" + std::strerror(errno) + ")");
    }
 
    std::vector<ParsedBlockData> blockData;
    std::string line;
    std::size_t lineNumber = 0;
 
    while (std::getline(ifs, line)) {
        ++lineNumber;
        if (line.empty()) continue; // 空行はスキップ
 
        // 行単位でパースエラーを catch し、パースに失敗した場合はワーニングを出して続行
        try {
            blockData.push_back(parseBlockLine(line, lineNumber));
        }
        catch (const std::exception& e) {
            std::cerr << "警告: " << e.what() << "\n";
            // パースエラーの場合もデータを追加しないで続行
        }
    }
 
    if (ifs.bad()) {
        throw std::runtime_error("読み込み中にエラーが発生しました: " + filepath);
    }
 
    return blockData;
}

bool BlockWorld::LoadCubeStateFromFile(const std::string& filepath)
{
    try {
        const std::vector<ParsedBlockData> blockDataList =
            readBlockDataFromFile(filepath);

        std::cout << "読み込み成功: "
                  << blockDataList.size()
                  << " 個のオブジェクト\n";

        for (const auto& blockData : blockDataList) {
            std::unique_ptr<Object> newObject;

            if (blockData.objectType == "Cube") {
                auto cube = std::make_unique<Cube>(
                    1.0, cubeTexture);

                cube->SetTranformPosition(blockData.position);
                cube->GetTransform().SetScale(blockData.objectSize);
                cube->RegisterCollisionAndStoreIndices(collisionMesh);
                
                newObject = std::move(cube);
            }
            else if (blockData.objectType == "Wall" ||
                     blockData.objectType == "wall") {
                auto wall = std::make_unique<Cube>(
                    1.0, wallTexture);

                wall->SetTranformPosition(blockData.position);
                wall->GetTransform().SetScale(blockData.objectSize);
                wall->RegisterCollisionAndStoreIndices(collisionMesh);

                newObject = std::move(wall);
            }
            else if (blockData.objectType == "Plane") {
                auto plane = std::make_unique<Plane>(
                    1.0);

                plane->GetTransform().SetPosition(blockData.position);
                plane->GetTransform().SetScale(blockData.objectSize);
                plane->RegisterCollision(collisionMesh);
                newObject = std::move(plane);
            }
            else {
                std::cerr << "警告: 未対応のオブジェクト種類です: "
                          << blockData.objectType << "\n";
                continue;
            }

            blocks.push_back(PlacedBlock{
                blockData.position,
                blockData.objectType,
                blockData.objectSize,
                std::move(newObject),
                blockData.isShown
            });

            std::cout << "  " << blockData.objectType
                      << "配置: (" << blockData.position.x << ", "
                      << blockData.position.y << ", "
                      << blockData.position.z << ")"
                      << " サイズ: (" << blockData.objectSize.x << ", "
                      << blockData.objectSize.y << ", "
                      << blockData.objectSize.z << ")"
                      << " 表示: "
                      << (blockData.isShown ? "true" : "false")
                      << "\n";
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "ファイル読み込みエラー: "
                  << e.what() << "\n";
        return false;
    }
}

void BlockWorld::DrawAll(Shader& shader)
{
    for (auto& block : blocks)
    {
        if (!block.isShown) continue; // 描画フラグがfalseの場合はスキップ
        if (!block.object) continue; // objectがnullptrの場合はスキップ
        block.object->Draw(shader);
    }
}
