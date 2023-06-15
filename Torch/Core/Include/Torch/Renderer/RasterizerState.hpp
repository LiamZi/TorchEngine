#ifndef __TORCH_CORE_INCLUDE_TORCH_RASTERIZER_STATE_HPP__
#define __TORCH_CORE_INCLUDE_TORCH_RASTERIZER_STATE_HPP__

#pragma once

#include <array>
#include <TML/Color4.hpp>

namespace Torch
{
    enum PolygonMode
    {
        Point,
        Line,
        Fill
    };

    enum ShadeMode
    {
        FLat,
        Gouraud,
    };

    enum CullMode
    {
        None,
        Front,
        Back,
    };

    enum BlendOperation
    {
        ADD = 1,
        SUB = 2,
        REV_SUB = 3,
        MIN = 4,
        MAX = 5
    };

    enum AlphaBlendFactor
    {
        ZERO,
        ONE,
        SRC_APLHA,
        DST_ALPHA,
        INV_SRC_ALPHA,
        INV_DST_ALPHA,
        SRC_COLOR,
        DST_COLOR,
        SRC_ALPHA_SAT,
        BLEND_FACTOR,
        INV_BLEND_FACTOR,
        SRC1_ALPHA,
        INV_SRC1_ALPHA,
        SRC1_COLOR,
        INV_SRC1_COLOR,
    };

    enum CompareFunction
    {
        ALWAYS_FAIL,
        ALWAYS_PASS,
        LESS,
        LESSEQUAL,
        EQUAL,
        NOT_EQUAL,
        GREATER_EQUAL,
        GREATER,
    };

    enum StencilOperation
    {
        SO_KEEP,
        SO_ZERO,
        SO_REPLACE,
        SO_INCR,
        SO_DECR,
        SO_INVERT,
        SO_INCR_WRAP,
        SO_DECR_WRAP,
    };

    enum ColorMask
    {
        RED = 1UL << 0,
        GREEN = 1UL << 1,
        BLUE = 1UL << 2,
        ALPHA = 1UL << 3,
        ALL = RED | GREEN | BLUE | ALPHA,
    };

    enum TextureMode
    {
        WRAP,
        MIRROR,
        CLAMP,
        BORDER,
    };

    enum TextureFilter
    {
        MIP_POINT = 0x0,
        MIP_LINEAR = 0x1,
        MAG_POINT = 0x0,
        MAG_LINEAR = 0x2,
        MIN_POINT = 0x0,
        MIN_LINEAR = 0x4,
        ANISOTROPIC = 0x08,
        COMPARISON = 0x10,

        MIN_MAG_MIP_POINT = MIN_POINT | MAG_POINT | MIP_POINT,
        MIN_MAG_POINT_MIP_LINEAR = MIN_POINT | MAG_POINT | MIP_LINEAR,
        MIN_POINT_MAG_LINEAR_MIP_POINT = MIN_POINT | MAG_LINEAR | MIP_POINT,
        MIN_POINT_MAG_MIP_LINEAR = MIN_POINT | MAG_LINEAR | MIP_LINEAR,
        MIN_LINEAR_MAG_MIP_POINT = MIN_LINEAR | MAG_POINT | MIP_POINT,
        MIN_LINEAR_MAG_POINT_MIP_LINEAR = MIN_LINEAR | MAG_POINT | MIP_LINEAR,
        MIN_MAG_LINEAR_MIP_POINT = MIN_LINEAR | MAG_LINEAR | MIP_POINT,
        MIN_MAG_MIP_LINEAR = MIN_LINEAR | MAG_LINEAR | MIP_LINEAR,
        // ANISOTROPIC = ANISOTROPIC,

        CMP_MIN_MAG_MIP_POINT = COMPARISON | MIN_MAG_MIP_POINT,
        CMP_MIN_MAG_POINT_MIP_LINEAR = COMPARISON | MIN_MAG_POINT_MIP_LINEAR,
        CMP_MIN_POINT_MAG_LINEAR_MIP_POINT = COMPARISON | MIN_POINT_MAG_LINEAR_MIP_POINT,
        CMP_MIN_POINT_MAG_MIP_LINEAR = COMPARISON | MIN_POINT_MAG_MIP_LINEAR,
        CMP_MIN_LINEAR_MAG_MIP_POINT = COMPARISON | MIN_LINEAR_MAG_MIP_POINT,
        CMP_MIN_LINEAR_MAG_POINT_MIP_LINEAR = COMPARISON | MIN_LINEAR_MAG_POINT_MIP_LINEAR,
        CMP_MIN_MAG_LINEAR_MIP_POINT = COMPARISON | MIN_MAG_LINEAR_MIP_POINT,
        CMP_MIN_MAG_MIP_LINEAR = COMPARISON | MIN_MAG_MIP_LINEAR,
        CMP_ANISOTROPIC = COMPARISON | ANISOTROPIC,

    };

    enum LogicOperation
    {
        LO_CLEAR,
        LO_SET,
        LO_COPY,
        LO_COPY_INVERTED,
        LO_NOOP,
        LO_INVERT,
        LO_AND,
        LO_NAND,
        LO_OR,
        LO_NOR,
        LO_XOR,
        LO_EQUIV,
        LO_ANDREVERSE,
        LO_ANDINVERTED,
        LO_ORREVERSE,
        LO_ORINVERTED,
    };

    struct TORCH_CORE_API RasterizerDesc
    {
        PolygonMode _polygonMode;
        ShadeMode _shadeMode;
        CullMode _cullMode;
        bool _frontFaceCCW;
        float _polygonOffsetFactor;
        float _polygonOffsetUnits;
        bool _depthClip;
        bool _scissor;
        bool _multiSample;

        RasterizerDesc();

        friend bool operator< (const RasterizerDesc &lhs, const RasterizerDesc &rhs);        
    };

    struct TORCH_CORE_API DepthStencilDesc
    {
        bool _depthEnabled;
        bool _depthWriteMask;
        CompareFunction _depthFunc;

        bool _frontStencilEnabled;
        CompareFunction _frontStencilFunc;
        uint16_t _frontStencilRef;
        uint16_t _frontStencilReadMask;
        uint16_t _frontStencilWriteMask;
        StencilOperation _frontStencilFail;
        StencilOperation _frontStencilDepthFail;
        StencilOperation _frontStencilPass;

        bool _backStencilEnabled;
        CompareFunction _backStencilFunc;
        uint16_t _backStencilRef;
        uint16_t _backStencilReadMask;
        uint16_t _backStencilWriteMask;
        StencilOperation _backStencilFail;
        StencilOperation _backStencilDepthFail;
        StencilOperation _backStencilPass; 

        DepthStencilDesc();

        friend bool operator< (const DepthStencilDesc &lhs, const DepthStencilDesc &rhs);
    };

    struct TORCH_CORE_API BlendDesc
    {
        Color4 _blendFactor;
        uint32_t _sampleMask;

        bool _alphaToCoverageEnabled;
        bool _independentBlendEnabled;

        std::array<bool, 8> _blendEanbled;
        std::array<bool, 8> _logicOpEnabled;
        std::array<BlendOperation, 8> _blendOp;
        std::array<AlphaBlendFactor, 8> _srcBlend;
        std::array<AlphaBlendFactor, 8> _dstBlend;
        std::array<BlendOperation, 8> _blendOpEnabled;
        std::array<AlphaBlendFactor, 8> _srcBlendAlpha;
        std::array<AlphaBlendFactor, 8> _dstBlendAlpha;
        std::array<LogicOperation, 8> _logicOp;
        std::array<uint8_t, 8> _colorWriteMask;

        BlendDesc();

        friend bool operator< (const BlendDesc &lhs, const BlendDesc &rhs);
    };

    struct TORCH_CORE_API SamplerDesc
    {
        Color4 _borderColor;

        TextureMode _addrModeU;
        TextureMode _addrModeV;
        TextureMode _addrModeW;

        TextureFilter _filter;

        uint8_t _maxAnisotropy;
        float _minLod;
        float _maxLod;
        float _mipMapLodBias;

        CompareFunction _func;

        SamplerDesc();

        friend bool operator< (const SamplerDesc &lhs, const SamplerDesc &rhs);

    };

    class TORCH_CORE_API RasterizerState 
    {
    protected:
        RasterizerDesc _rasterizerDesc;
        DepthStencilDesc _depthDesc;
        BlendDesc _blendDesc;

    public:
        explicit RasterizerState(const RasterizerDesc &rdesc, const DepthStencilDesc &ddesc, const BlendDesc &bdesc);
        virtual ~RasterizerState();

        const RasterizerDesc &GetRasterizerDesc() const 
        {
            return _rasterizerDesc;
        } 

        const DepthStencilDesc &GetDepthStencilDesc() const
        {
            return _depthDesc;
        }

        const BlendDesc &GetBlendDesc() const
        {
            return _blendDesc;
        }


        virtual void Active()  {};
    };

    class TORCH_CORE_API SamplerState
    {
    protected:
        SamplerDesc _desc;

    public:
        explicit SamplerState(const SamplerDesc &desc);

        virtual ~SamplerState() {};

        const SamplerDesc &GetSamplerDesc() const
        {
            return _desc;
        }

    };
};

#endif