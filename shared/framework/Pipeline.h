#pragma once
#include <glad/glad.h>
#include <array>

namespace gpupro
{
    enum class CullFace
    {
        FRONT = GL_FRONT,
        BACK = GL_BACK,
        FRONT_AND_BACK = GL_FRONT_AND_BACK
    };

    enum class FrontFace
    {
        CW = GL_CW,
        CCW = GL_CCW
    };

    enum class CompFunc
    {
        LESS = GL_LESS,
        LESS_EQUAL = GL_LEQUAL,
        GREATER = GL_GREATER,
        GREATER_EQUAL = GL_GEQUAL,
        EQUAL = GL_EQUAL,
        NOT_EQUAL = GL_NOTEQUAL,
        NEVER = GL_NEVER,
        ALWAYS = GL_ALWAYS,
    };

    enum class StencilOp
    {
        KEEP = GL_KEEP,
        ZERO = GL_ZERO,
        REPLACE = GL_REPLACE,
        INCREMENT = GL_INCR_WRAP,
        // Increment. Restart from zero on overflow
        DECREMENT = GL_DECR_WRAP,
        // Decrement. Restart from maximum on underflow
        INC_SAT = GL_INCR,
        // Increment. Clamp to maximum on overflow
        DEC_SAT = GL_DECR,
        // Decrement. Clamp to 0 on underflow
        INVERT = GL_INVERT,
        // Flip all bits
    };

    enum class BlendEquation
    {
        ADD = GL_FUNC_ADD,
        SUBTRACT = GL_FUNC_SUBTRACT,
        REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
        MIN = GL_MIN,
        MAX = GL_MAX
    };

    enum class BlendFactor
    {
        ZERO = GL_ZERO,
        ONE = GL_ONE,
        SRC_ALPHA = GL_SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        DST_ALPHA = GL_DST_ALPHA,
        ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
        SRC_COLOR = GL_SRC_COLOR,
        ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
        DST_COLOR = GL_DST_COLOR,
        ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
    };

    enum class LogicOp
    {
        CLEAR = GL_CLEAR,
        SET_ONE = GL_SET,
        COPY = GL_COPY,
        COPY_INVERTED = GL_COPY_INVERTED,
        NOOP = GL_NOOP,
        INVERT = GL_INVERT,
        AND = GL_AND,
        NAND = GL_NAND,
        OR = GL_OR,
        NOR = GL_NOR,
        XOR = GL_XOR,
        EQUIV = GL_EQUIV,
        AND_REVERSE = GL_AND_REVERSE,
        AND_INVERTED = GL_AND_INVERTED,
        OR_REVERSE = GL_OR_REVERSE,
        OR_INVERTED = GL_OR_INVERTED
    };

    /// The pipeline contains the most important opengl state settings that we
    /// are going to use in the exercise.
    /// Values passed to glEnable/glDisable are prefixed with Enable (i.e. EnableDepthTest will be used for glEnable(GL_DEPTH_TEST)/glDisable(GL_DEPTH_TEST))
    /// The pipeline is initialized with the default values from opengl.
    /// Call apply() to apply the settings of the pipeline to the global opengl state
    struct Pipeline
    {
        // face culling (disabled by default): https://www.khronos.org/opengl/wiki/Face_Culling
        bool EnableCullFace = false; // glEnable/glDisable(GL_CULL_FACE)
        FrontFace FrontFace = FrontFace::CCW;
        // glFrontFace() https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glFrontFace.xhtml
        CullFace CullFace = CullFace::BACK;
        // glCullFace() https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCullFace.xhtml

        // depth testing (disabled by default) https://www.khronos.org/opengl/wiki/Depth_Test
        bool EnableDepthTest = false; // glEnable/glDisable(GL_DEPTH_TEST)
        CompFunc DepthFunc = CompFunc::LESS;
        // glDepthFunc() https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDepthFunc.xhtml
        bool DepthMask = true;
        // glDepthMask() https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDepthMask.xhtml

        // stencil testing (disabled by default) https://www.khronos.org/opengl/wiki/Stencil_Test
        bool EnableStencilTest = false; // glEnable/glDisable(GL_STENCIL_TEST)
        // glStencilOp() arguments https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glStencilOp.xhtml
        StencilOp StencilOpSFail = StencilOp::KEEP; // action when the stencil test fails
        StencilOp StencilOpDpFail = StencilOp::KEEP; // action when the stencil test passes, but the depth test fails
        StencilOp StencilOpDpPass = StencilOp::KEEP; // action when stencil and depth test pass
        // glStencilFunc() arguments https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glStencilFunc.xhtml
        CompFunc StencilFunc = CompFunc::ALWAYS; // test function of glStencilFunc()
        GLint StencilRef = 0; // stencil ref of glStencilFunc()
        // GLuint StencilMask = 0xFF; // not used

        // blending https://www.khronos.org/opengl/wiki/Blending
        bool EnableBlend = false; // glEnable/glDisable(GL_BLEND)
        // glBlendFunc() arguments https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml
        BlendFactor BlendFuncSFactor = BlendFactor::ONE; // contribution of source fragment (from the fragment shader)
        BlendFactor BlendFuncDFactor = BlendFactor::ZERO; // contrinution of destination fragment (from the framebuffer)
        BlendEquation BlendEquation = BlendEquation::ADD;
        // glBlendEquation() https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBlendEquation.xhtml
        bool EnableSampleAlphaToCoverage = false; // glEnable/glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE)

        // logic op https://www.khronos.org/opengl/wiki/Logical_Operation
        bool EnableColorLogicOp = false; // glEnable/glDisable(GL_COLOR_LOGIC_OP)
        LogicOp LogicOp = LogicOp::COPY;
        // glLogicOp() https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLogicOp.xhtml

        // rasterizer and color writes https://www.khronos.org/opengl/wiki/Primitive_Assembly#Rasterizer_discard and https://www.khronos.org/opengl/wiki/Write_Mask#Color_Mask
        bool EnableRasterizerDiscard = false; // glEnable/glDisable(GL_RASTERIZER_DISCARD) 
        std::array<bool, 4> ColorMask = {true, true, true, true};
        // glColorMask https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glColorMask.xhtml
        bool EnableFramebufferSrgb = false;
        // glEnable/glDisable(GL_FRAMEBUFFER_SRGB) https://www.khronos.org/opengl/wiki/Framebuffer#Colorspace

        /// applies all set values to opengl functions
        void apply()
        {
            // culling
            setGlMode(GL_CULL_FACE, EnableCullFace);
            glFrontFace(GLenum(FrontFace));
            glCullFace(GLenum(CullFace));

            // depth testing
            setGlMode(GL_DEPTH_TEST, EnableDepthTest);
            glDepthFunc(GLenum(DepthFunc));
            glDepthMask(GLboolean(DepthMask));

            // stencil testing
            setGlMode(GL_STENCIL_TEST, EnableStencilTest);
            glStencilOp(GLenum(StencilOpSFail), GLenum(StencilOpDpFail), GLenum(StencilOpDpPass));
            glStencilFunc(GLenum(StencilFunc), StencilRef, 0xFF);

            // blending
            setGlMode(GL_BLEND, EnableBlend);
            glBlendFunc(GLenum(BlendFuncSFactor), GLenum(BlendFuncDFactor));
            glBlendEquation(GLenum(BlendEquation));
            setGlMode(GL_SAMPLE_ALPHA_TO_COVERAGE, EnableSampleAlphaToCoverage);

            // logic op
            setGlMode(GL_COLOR_LOGIC_OP, EnableColorLogicOp);
            glLogicOp(GLenum(LogicOp));

            // rasterizer and color writes
            setGlMode(GL_RASTERIZER_DISCARD, EnableRasterizerDiscard);
            glColorMask(GLboolean(ColorMask[0]), GLboolean(ColorMask[1]), GLboolean(ColorMask[1]),
                        GLboolean(ColorMask[3]));
            setGlMode(GL_FRAMEBUFFER_SRGB, EnableFramebufferSrgb);
        }

    private:
        static void setGlMode(GLenum mode, bool enable)
        {
            if (enable)
                glEnable(mode);
            else
                glDisable(mode);
        }
    };
}
