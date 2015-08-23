#ifndef VIX_DXQUAD_H
#define VIX_DXQUAD_H

#include <vix_platform.h>
#include <vix_directx.h>
#include <vix_dxtexture.h>

namespace Vixen {

    class VIX_API DXQuad
    {
    public:
        DXQuad();

        ~DXQuad();

        void Initialize(ID3D11Device* device);

        void Render(ID3D11DeviceContext* context);

        void SetVertexShader(ID3D11VertexShader* vs);
        void SetPixelShader(ID3D11PixelShader* ps);
        void SetConstantBuffer(ID3D11Buffer* cb);
        void SetTexture(DXTexture* texture);
        void SetSampleState(ID3D11SamplerState* ss);
        void SetShaderResourceView(ID3D11ShaderResourceView* rv);

    private:
        ID3D11Buffer* m_vertexBuffer;
        ID3D11Buffer* m_indexBuffer;
        ID3D11VertexShader* m_vShader;
        ID3D11PixelShader*  m_pShader;
        ID3D11Buffer*       m_constantBuffer;
        DXTexture*          m_texture;
        ID3D11ShaderResourceView* m_RV;
        ID3D11SamplerState* m_SS;
        float x;
        float y;
    };

}

#endif