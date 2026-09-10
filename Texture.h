#pragma once 
#include <glad/glad.h>
#include <iostream>
class Texture{
    public:
        // コンストラクタ / デストラクタ
        Texture() : m_rendererID(0), m_width(0), m_height(0), m_bpp(0) {}
        explicit Texture(const std::string& path,int imageType=4, bool flipVertically = true);
        ~Texture();

        // コピーの禁止（二重解放防止）
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        // ムーブの許可（所有権の移動）
        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        // テクスチャのバインド / アンバインド
        void Bind(unsigned int slot = 0) const;
        void Unbind() const;  
        // ゲッター
        GLuint GetID() const { return m_rendererID; }
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

    private:
        GLuint m_rendererID;
        int m_width;
        int m_height;
        int m_bpp; // Bits Per Pixel (チャンネル数)
};