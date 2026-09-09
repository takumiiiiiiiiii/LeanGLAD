#include "StageEditor.h"

StageEditor::StageEditor()
    : selectBlock(nullptr),
      currentTypeIndex(0),
      currentStateIndex(0),
      value(1)
    {
    // コンストラクタの実装（必要に応じて）
    currentTypeIndex = 0;
    currentStateIndex = 0;
    editorStates[currentStateIndex];

}
void StageEditor::UpdateEditor(float dt) {
    // ImGuiのデバッグウィンドウを表示
    ImGui::Begin("Debug");
    ImGui::Combo("Editor State", &currentStateIndex, editorStates, IM_ARRAYSIZE(editorStates));
    if(editorStates[currentStateIndex]==std::string("Place")){
         ImGui::Combo("Object Type", &currentTypeIndex, objectTypes, IM_ARRAYSIZE(objectTypes));
    }
    if(editorStates[currentStateIndex]==std::string("Select")){
         if (selectBlock != nullptr) {
            glm::vec3 selectObjPos =  selectBlock->object->GetTransform().GetPosition();
            glm::vec3 selectObjsize = selectBlock->object->GetTransform().GetScale();
            ImGui::InputFloat3("Position", &selectObjPos.x);
            ImGui::InputFloat3("Scale", &selectObjsize.x);
            selectBlock->object->GetTransform().SetPosition(selectObjPos);
            selectBlock->object->GetTransform().SetScale(selectObjsize);
            // std::cout << selectObjPos.x << std::endl;
            // std::cout << selectObjPos.y << std::endl;
            // std::cout << selectObjPos.z << std::endl;
        //     if (ImGui::InputFloat3("Position", &selectBlock->position.x)) {
        //         selectBlock->object->GetTransform().SetPosition(
        //             selectBlock->position
        //         );
        //     }
        } else {
            ImGui::Text("No block selected");
        }
    }
    ImGui::InputFloat3("Value", &value.x);
    ImGui::Text("Selected: %s", objectTypes[currentTypeIndex]);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}


void StageEditor::RemoveBlockByMouse(CollisionMesh& collisionmesh, BlockWorld& blockWorld, const glm::mat4& view, const glm::mat4& projection){
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
                    << blockWorld.DeleteBlockSelected(deletePos,"") << std::endl;
    }
}

void StageEditor::PlaceBlockByMouse(CollisionMesh& collisionmesh, BlockWorld& blockWorld, const glm::mat4& view, const glm::mat4& projection) {
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
        blockWorld.PlaceBlock(placePos, objectTypes[currentTypeIndex], value,true); // ここで実際にオブジェクトを生成・配置する
    }
}

void StageEditor::SelectBlockByMouse(CollisionMesh& collisionmesh, BlockWorld& blockWorld, const glm::mat4& view, const glm::mat4& projection){
    if (ImGui::GetIO().WantCaptureMouse ||
        !Input::IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        return;
    }
     if (editorStates[currentStateIndex] != std::string("Select"))
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
        glm::vec3 hitPos = rayOrigin + rayDir * dist;
        std::cout << "hitPos.y=" << hitPos.y << std::endl;
        // ヒット面の法線方向にキューブ半径(0.5)分ずらす
        // → Planeの上面をクリックすれば真上に、既存キューブの側面をクリックすればその横に置ける
        selectBlock = blockWorld.GetBlockSelected(hitPos);
        if(selectBlock!=nullptr){
            std::cout<<"select"<<std::endl;
            // selectBlock->object->GetTransform().SetPosition(selectBlock->position);
        }else{
            std::cout<<"cantselect"<<std::endl;
        }
    }
}