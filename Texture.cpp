#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path,int imageType, bool flipVertically)
    : m_rendererID(0), m_width(0), m_height(0), m_bpp(0)
{
    // OpenGLは原点が左下、画像は左上が一般的なので上下反転を設定
    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    // 画像のロード (CPUメモリ確保)
    stbi_uc* localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 0);

    if (!localBuffer) {
            std::cerr << "[Texture Error] Failed to load image: "
              << path << std::endl;
        return;
    }

    // チャンネル数に応じたフォーマット選択
    GLenum internalFormat = 0;
    GLenum dataFormat = 0;
    if (m_bpp == 1) {
        internalFormat = dataFormat = GL_RED;
    } else if (m_bpp == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    } else if (m_bpp == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }

    // テクスチャオブジェクトの生成と設定
    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    // テクスチャパラメータの設定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // GPUへデータ転送
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, dataFormat, GL_UNSIGNED_BYTE, localBuffer);
    //glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width, m_height,0,GL_RGBA,GL_UNSIGNED_BYTE,localBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    // CPU側の画像バッファは転送後すぐに解放
    stbi_image_free(localBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    if (m_rendererID != 0) {
        glDeleteTextures(1, &m_rendererID);
    }
}

// ムーブコンストラクタの実装
Texture::Texture(Texture&& other) noexcept
    : m_rendererID(other.m_rendererID), m_width(other.m_width),
      m_height(other.m_height), m_bpp(other.m_bpp)
{
    other.m_rendererID = 0; // 所有権を奪ったため元のIDを無効化
}

// ムーブ代入演算子の実装
Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (m_rendererID != 0) {
            glDeleteTextures(1, &m_rendererID); // 既存リソース破棄
        }
        m_rendererID = other.m_rendererID;
        m_width = other.m_width;
        m_height = other.m_height;
        m_bpp = other.m_bpp;

        other.m_rendererID = 0;
    }
    return *this;
}

void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

