#include "BlockWrold.h"

BlockWorld::BlockWorld(CollisionMesh& collisionMesh, float cubeSize)
    : collisionMesh(collisionMesh), cubeSize(cubeSize)
{
    
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
bool BlockWorld::DeleteBlockSelected(const glm::vec3& Pos) {
    const glm::vec3 cellPos = SnapToGrid(Pos);

    for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        if (SnapToGrid(it->position) == cellPos)
        {
            // Cubeが保持している三角形インデックスのみを削除
            if (!collisionMesh.removeCubeByIndices(it->cube->GetTriangleIndices()))
                return false;
            blocks.erase(it);
            return true;
        }
    }

    return false;
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

void BlockWorld::PlaceBlock(const glm::vec3& worldPos)
{
    // セルインデックスをfloorで求め、そのセルの中心(index + 0.5)*cubeSizeにスナップする。
    // Cubeは中心基準(-0.5〜+0.5)なので、中心をセル境界の真ん中に合わせる必要がある。
    // truncではなくfloorを使うことで、負の座標側でもズレなく動作する。
    glm::vec3 snapped;
    snapped.x = (std::floor(worldPos.x / cubeSize) + 0.5f) * cubeSize;
    snapped.y = (std::floor(worldPos.y / cubeSize) + 0.5f) * cubeSize;
    snapped.z = (std::floor(worldPos.z / cubeSize) + 0.5f) * cubeSize;

    if (IsOccupied(snapped))
        return;

    auto cube = std::make_unique<Cube>(cubeSize);
    cube->SetTranformPosition(snapped);

    // 新しく置いたキューブも次のRaycastの対象にする(これがないと2段目を積めない)
    // インデックスを記録して、削除時に正確に削除できるようにする
    cube->RegisterCollisionAndStoreIndices(collisionMesh);

    blocks.push_back(PlacedBlock{ snapped, std::move(cube) });
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
 
        for (const auto& p : blocks)
        {
            ofs << p.position.x << ","
                << p.position.y << ","
                << p.position.z << "\n";
        }
        // ofs << "fa" << " "
        //         << "fa" << " "
        //         << "fa" << " "
        //         << "fa" << " ";
 
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
    // std::ofstream file(filename);
    // for (const auto& p : blocks)
    // {
    //     file << p.position.x << " "
    //         << p.position.y << " "
    //         << p.position.z << " "
    //         << p.cube << " ";
    // }
    // file.close();
    // return true;
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
 
    if (values.size() != 3) {
        throw std::runtime_error(
            "列数が不正です (行 " + std::to_string(lineNumber) +
            ", 期待値: 3, 実際: " + std::to_string(values.size()) + ")");
    }
 
    return glm::vec3{ values[0], values[1], values[2] };
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

bool BlockWorld::LoadCubeStateFromFile(const std::string& filepath){
    
    std::vector<glm::vec3> file_data;
    file_data = std::move(readCoordinatesFromFile(filepath));
    glm::vec3 p;
    for (size_t i = 0; i < file_data.size(); i += 3) {
        // 残り要素数に応じて、今回の範囲の終わりを取得
        size_t end = std::min(i + 3, file_data.size());

        std::cout << "グループ: ";
        for (size_t j = i; j < end; ++j) {
            auto cube = std::make_unique<Cube>(cubeSize);
            cube->SetTranformPosition(file_data[j]);

            // 新しく置いたキューブも次のRaycastの対象にする(これがないと2段目を積めない)
            // インデックスを記録して、削除時に正確に削除できるようにする
            cube->RegisterCollisionAndStoreIndices(collisionMesh);

            blocks.push_back(PlacedBlock{file_data[j], std::move(cube) });
        }
        std::cout << "\n";
    }
    return true;
}

void BlockWorld::DrawAll(Shader& shader)
{
    for (auto& block : blocks)
    {
        block.cube->Draw(shader);
    }
}