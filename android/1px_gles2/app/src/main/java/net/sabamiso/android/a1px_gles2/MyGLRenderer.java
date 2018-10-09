package net.sabamiso.android.a1px_gles2;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Random;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MyGLRenderer implements GLSurfaceView.Renderer{

    GLSurfaceView glSurfaceView;

    MyGLRenderer(GLSurfaceView glSurfaceView) {
        this.glSurfaceView = glSurfaceView;
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        initGLES2();
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

    int shader_program;
    int vertex_shader;
    int fragment_shader;
    int vertex_count;
    int attr_pos;
    int attr_uv;
    int uniform_texture;
    int [] textures;


    int tex_w = 200;
    int tex_h = 200;

    long st = System.currentTimeMillis();
    int count = 0;

    byte [][] colors = new byte[][] {
            {(byte)255,   (byte)0,   (byte)0,   (byte)255},
            {(byte)0, (byte)255,   (byte)0,   (byte)255},
            {(byte)0, (byte)0,   (byte)255,   (byte)255},
            {(byte)0, (byte)0,   (byte)0,   (byte)255},
    };

    int loadShader(int type, String shader_code_str){
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shader_code_str);
        GLES20.glCompileShader(shader);

        int [] status = new int[1];
        GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, status, 0);
        if (status[0] == 0) {
            String log_str = GLES20.glGetShaderInfoLog(shader);
            Log.e("MyGLRenderer", "GLES20.glCompileShader() failed...log=" + log_str);
            GLES20.glDeleteProgram(shader);
            return 0;
        }

        return shader;
    }

    int createProgram(String vertex_shader_str, String fragment_shader_str) {
        vertex_shader = loadShader(GLES20.GL_VERTEX_SHADER, vertex_shader_str);
        fragment_shader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragment_shader_str);
        int shader_program = GLES20.glCreateProgram();
        GLES20.glAttachShader(shader_program, vertex_shader);
        GLES20.glAttachShader(shader_program, fragment_shader);

        return shader_program;
    }

    void initGLES2() {
        // シェーダープログラムの作成
        shader_program = createProgram(vertex_shader_str, fragment_shader_str);
        GLES20.glLinkProgram(shader_program);

        // シェーダー内の変数を指すハンドルを取得する
        attr_pos = GLES20.glGetAttribLocation(shader_program, "attr_pos");
        GLES20.glEnableVertexAttribArray(attr_pos);
        attr_uv = GLES20.glGetAttribLocation(shader_program, "attr_uv");
        GLES20.glEnableVertexAttribArray(attr_uv);
        uniform_texture = GLES20.glGetUniformLocation(shader_program, "uniform_texture");

        setupVertices();
        setupTexture();

        GLES20.glViewport(0, 0, glSurfaceView.getWidth(), glSurfaceView.getHeight());
    }

    void setupVertices(){
        //
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
        FloatBuffer buf_pos = bb_pos.asFloatBuffer();
        buf_pos.put(a_pos);
        buf_pos.position(0);
        GLES20.glVertexAttribPointer(
                attr_pos,            // vertex shader内の変数attr_posを指すハンドル
                3,              // 頂点1つはfloatいくつで表現されているか？
                GLES20.GL_FLOAT,     // 数値はfloat
                false,   // 正規化は行わない
                3*4,          // 1つの頂点要素は何バイトか？ stride = float3つでvertex1つ * float(4byte)
                buf_pos);              // 頂点バッファ

        ByteBuffer bb_uv = ByteBuffer.allocateDirect(a_uv.length * 4); // float(32bit=4byte) * length)
        bb_uv.order(ByteOrder.nativeOrder());
        FloatBuffer buf_uv = bb_uv.asFloatBuffer();
        buf_uv.put(a_uv);
        buf_uv.position(0);
        GLES20.glVertexAttribPointer(
                attr_uv,             // vertex shader内の変数attr_uvを指すハンドル
                2,              // 頂点1つはfloatいくつで表現されているか？
                GLES20.GL_FLOAT,     // 数値はfloat
                false,   // 正規化は行わない
                2*4,          // 1つの頂点要素は何バイトか？ stride = float2つでuv1つ * float(4byte)
                buf_uv);
    }

    void setupTexture() {
        // GL_TEXTURE0をアクティブに設定
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);

        // texture2dオブジェクトを生成
        textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);

        // texture2dオブジェクトをGL_TEXTURE0にバインドする
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);

        // テクスチャの補間方法の指定
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

        // テクスチャの作成 (ダミーで適当な内容を入れてる)
        byte [] buf = new byte[tex_w * tex_h * 4];
        Random r = new Random();
        for (int i = 0; i < buf.length; ++i) {
            buf[i] = (byte)r.nextInt(256);
        }
        ByteBuffer bb = ByteBuffer.wrap(buf);
        bb.position(0);
        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, tex_w, tex_h, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, bb);
    }

    @SuppressLint("NewApi")
    void updateTexture() {
        // GL_TEXTURE0をアクティブに設定
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);

        // texture2dオブジェクトをGL_TEXTURE0にバインドする
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);

        // 1px
        byte [] buf = new byte[tex_w * tex_h * 4];
        Random r = new Random();
        int x = tex_w / 2;
        int y = tex_h / 2;
        int p = x * 4 + y * tex_w * 4;

        buf[p + 0] = colors[count%colors.length][0];
        buf[p + 1] = colors[count%colors.length][1];
        buf[p + 2] = colors[count%colors.length][2];
        buf[p + 3] = colors[count%colors.length][3];

        // onDrawFrame()は連続で呼び出されるので要注意
        // (invalidate()した時だけ描画したい場合は、setRenderMode(RENDERMODE_WHEN_DIRTY); を設定する)
        if (System.currentTimeMillis() - st >= 200) {
            count ++;
            st = System.currentTimeMillis();
        }

        ByteBuffer bb = ByteBuffer.wrap(buf);
        bb.position(0);

        // 現在バインドされているtexture2dオブジェクトの内容をアップデートする
        // 内容をアップデートする場合は、glTexSubImage2D()を使用すること
        GLES20.glTexSubImage2D(GLES20.GL_TEXTURE_2D, 0, 0, 0, tex_w, tex_h, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, bb);

        // ピクセルシェーダのuniform_textureにGL_TEXTURE0を設定する
        // (ここに間違えてtextures[0]を指定すると表示されなくなるので要注意…)
        GLES20.glUniform1i(uniform_texture, 0);
    }

    void disposeGLES() {
        GLES20.glDeleteTextures(1, textures, 0);
        GLES20.glDisableVertexAttribArray(attr_pos);
        GLES20.glDisableVertexAttribArray(attr_uv);
        GLES20.glDisable(uniform_texture);
        GLES20.glDeleteProgram(shader_program);
        GLES20.glDeleteShader(vertex_shader);
        GLES20.glDeleteShader(fragment_shader);
    }

    void drawGLES() {
        // 画面クリア
        GLES20.glClearColor(1f, 0.0f, 1f, 1);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        // コンパイル済みシェーダの適用
        GLES20.glUseProgram(shader_program);

        // テクスチャのアップデート
        updateTexture();

        // ポリゴンの描画
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, vertex_count);

        // テクスチャのバインドを解除
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
    }
}
