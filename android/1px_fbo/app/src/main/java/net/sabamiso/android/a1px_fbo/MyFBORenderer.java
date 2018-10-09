package net.sabamiso.android.a1px_fbo;

import android.annotation.SuppressLint;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Random;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MyFBORenderer implements GLSurfaceView.Renderer {
    GLSurfaceView glSurfaceView;

    MyFBORenderer(GLSurfaceView glSurfaceView) {
        this.glSurfaceView = glSurfaceView;
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        setupGLES2();
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        drawGLES();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    final String vertex_shader_str =
            "precision mediump float;" +
                    "attribute vec4 attr_pos;" +
                    "attribute mediump vec2 attr_uv;" +
                    "varying mediump vec2 varying_uv;" +
                    "void main() {" +
                    "    gl_Position = attr_pos;" +
                    "    varying_uv = attr_uv;" +
                    "}";

    final String fragment_shader_str =
            "uniform sampler2D uniform_texture;" +
                    "varying mediump vec2 varying_uv;" +
                    "void main() {" +
                    "   gl_FragColor = texture2D(uniform_texture, varying_uv);" +
                    "}";

    MyFBO myFBO;

    int shader_program;

    int attr_pos;
    int attr_uv;
    int uniform_texture;

    FloatBuffer buf_pos;
    FloatBuffer buf_uv;

    int vertex_count;

    int tex_w = 200;
    int tex_h = 200;

    long st = System.currentTimeMillis();
    int count = 0;

    void setupGLES2() {
        // FBOクラスの作成
        myFBO = new MyFBO(tex_w, tex_h);

        // シェーダープログラムの作成
        shader_program = GLESUtil.createProgram(vertex_shader_str, fragment_shader_str);

        // シェーダー内の変数を指すハンドルを取得する
        attr_pos = GLES20.glGetAttribLocation(shader_program, "attr_pos");
        GLES20.glEnableVertexAttribArray(attr_pos);
        attr_uv = GLES20.glGetAttribLocation(shader_program, "attr_uv");
        GLES20.glEnableVertexAttribArray(attr_uv);
        uniform_texture = GLES20.glGetUniformLocation(shader_program, "uniform_texture");

        setupVertices();

        GLES20.glViewport(0, 0, glSurfaceView.getWidth(), glSurfaceView.getHeight());
    }

    void setupVertices() {
        float a_pos[] = {
                -1.0f, -1.0f, 0.0f,
                1.0f,  -1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                1.0f,  1.0f, 0.0f
        };
        float a_uv[] = {
                0.0f,  0.0f,
                1.0f,  0.0f,
                0.0f,  1.0f,
                1.0f,  1.0f
        };
        vertex_count = a_pos.length / 3;

        ByteBuffer bb_pos = ByteBuffer.allocateDirect(a_pos.length * 4); // float(32bit=4byte) * length)
        bb_pos.order(ByteOrder.nativeOrder());
        buf_pos = bb_pos.asFloatBuffer();
        buf_pos.put(a_pos);
        buf_pos.position(0);

        ByteBuffer bb_uv = ByteBuffer.allocateDirect(a_uv.length * 4); // float(32bit=4byte) * length)
        bb_uv.order(ByteOrder.nativeOrder());
        buf_uv = bb_uv.asFloatBuffer();
        buf_uv.put(a_uv);
        buf_uv.position(0);
    }

    void disposeGLES() {
        myFBO.dispose();

        GLES20.glDisableVertexAttribArray(attr_pos);
        GLES20.glDisableVertexAttribArray(attr_uv);
        GLES20.glDisable(uniform_texture);
        GLES20.glDeleteProgram(shader_program);
    }

    void drawGLES() {
        // FBOを描画
        myFBO.draw();

        // 描画先をスクリーンへ切り替え
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
        GLES20.glViewport(0, 0, glSurfaceView.getWidth(), glSurfaceView.getHeight());

        GLES20.glClearColor(1f, 0.0f, 1f, 1);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        // コンパイル済みシェーダの適用
        GLES20.glUseProgram(shader_program);

        // uniform_textureにGL_TEXTURE0を設定
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, myFBO.getTextureId());
        GLES20.glUniform1i(uniform_texture, 0);

        // 頂点を設定 (頂点はシェーダを走らせる直前に設定する。使いまわすと前の値が残ってしまうので要注意…)
        GLES20.glVertexAttribPointer(
                attr_pos,            // vertex shader内の変数attr_posを指すハンドル
                3,              // 頂点1つはfloatいくつで表現されているか？
                GLES20.GL_FLOAT,     // 数値はfloat
                false,   // 正規化は行わない
                3*4,          // 1つの頂点要素は何バイトか？ stride = float3つでvertex1つ * float(4byte)
                buf_pos);            // 頂点バッファ
        GLES20.glVertexAttribPointer(
                attr_uv,             // vertex shader内の変数attr_uvを指すハンドル
                2,              // 頂点1つはfloatいくつで表現されているか？
                GLES20.GL_FLOAT,     // 数値はfloat
                false,   // 正規化は行わない
                2*4,          // 1つの頂点要素は何バイトか？ stride = float2つでuv1つ * float(4byte)
                buf_uv);

        // テクスチャを張った板ポリゴンの描画
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, vertex_count);

        // テクスチャをアンバインド
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
    }
}
