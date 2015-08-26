#include <vix_dxquad.h>

namespace Vixen {

    DXQuad::DXQuad()
    {
        m_vertexBuffer = nullptr;
        m_indexBuffer = nullptr;
        m_vShader = nullptr;
        m_pShader = nullptr;
        m_constantBuffer = nullptr;
        m_texture = nullptr;
        m_RV = nullptr;
        m_SS = nullptr;
        
    }

    DXQuad::~DXQuad()
    {
        ReleaseCOM(m_vertexBuffer);
        ReleaseCOM(m_indexBuffer);
    }

   
    void DXQuad::Initialize(ID3D11Device* device)
    {

        if (!m_texture)
            return;

        VertexTex _verts[4] =
        {
            { DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
            { DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
            { DirectX::XMFLOAT3(0.0, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
            { DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }
        };

        D3D11_BUFFER_DESC vbd;
        ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.ByteWidth = sizeof(VertexTex) * 4;
        vbd.CPUAccessFlags = 0;
        vbd.Usage = D3D11_USAGE_DEFAULT;
        D3D11_SUBRESOURCE_DATA initVBD;
        ZeroMemory(&initVBD, sizeof(D3D11_SUBRESOURCE_DATA));
        initVBD.pSysMem = _verts;
        device->CreateBuffer(&vbd, &initVBD, &m_vertexBuffer);
        

        USHORT  _indices[6] = 
        {
            0, 1, 2,
            2, 1, 3
        };

        D3D11_BUFFER_DESC ibd;
        ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.ByteWidth = sizeof(USHORT) * 6;
        ibd.CPUAccessFlags = 0;
        ibd.Usage = D3D11_USAGE_DEFAULT;
        D3D11_SUBRESOURCE_DATA initIBD;
        ZeroMemory(&initIBD, sizeof(D3D11_SUBRESOURCE_DATA));
        initIBD.pSysMem = _indices;
        device->CreateBuffer(&ibd, &initIBD, &m_indexBuffer);
    }


    void DXQuad::Render(ID3D11DeviceContext* context)
    {
        UINT stride = sizeof(VertexTex);
        UINT offset = 0;
        if (m_vertexBuffer)
            context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
        if (m_indexBuffer)
            context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

        // Set primitive topology
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        if (m_vShader)
            context->VSSetShader(m_vShader, nullptr, 0);
        if (m_constantBuffer)
            context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
        if (m_pShader)
            context->PSSetShader(m_pShader, nullptr, 0);
        
        ID3D11ShaderResourceView* RV = m_texture->ResourceView();
        context->PSSetShaderResources(0, 1, &m_RV);
        ID3D11SamplerState* SS = m_texture->SampleState();
        context->PSSetSamplers(0, 1, &m_SS);

        context->DrawIndexed(6, 0, 0);

    }


    void DXQuad::SetVertexShader(ID3D11VertexShader* vs)
    {
        m_vShader = vs;
    }

    void DXQuad::SetPixelShader(ID3D11PixelShader* ps)
    {
        m_pShader = ps;
    }

    void DXQuad::SetConstantBuffer(ID3D11Buffer* cb)
    {
        m_constantBuffer = cb;
    }

    void DXQuad::SetTexture(DXTexture* texture)
    {
        m_texture = texture;
    }

    void DXQuad::SetShaderResourceView(ID3D11ShaderResourceView* rv)
    {
        m_RV = rv;
    }

    void DXQuad::SetSampleState(ID3D11SamplerState* ss)
    {
        m_SS = ss;
    }
}