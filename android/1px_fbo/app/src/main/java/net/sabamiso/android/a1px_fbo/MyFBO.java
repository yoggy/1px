package net.sabamiso.android.a1px_fbo;

import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Random;

public class MyFBO {
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

    int tex_w;
    int tex_h;

    int shader_program;

    int attr_pos;
    int attr_uv;
    int uniform_texture;

    FloatBuffer buf_pos;
    FloatBuffer buf_uv;

    int vertex_count;

    int fbo_texture_id;
    int fbo_id;
    int fbo_render_buffer_id;

    int content_texture_id;

    byte [][] colors = new byte[][] {
            {(byte)255,   (byte)0,   (byte)0,   (byte)255},
            {(byte)0, (byte)255,   (byte)0,   (byte)255},
            {(byte)0, (byte)0,   (byte)255,   (byte)255},
            {(byte)0, (byte)0,   (byte)0,   (byte)255},
    };
    int count = 0;
    long st = System.currentTimeMillis();

    public MyFBO(int w, int h) {
        this.tex_w = w;
        this.tex_h = h;
        init();
    }

    public int getTextureId() {
        return fbo_texture_id;
    }

    public void init() {
        // シェーダープログラムの作成
        shader_program = GLESUtil.createProgram(vertex_shader_str, fragment_shader_str);

        // シェーダー内の変数を指すハンドルを取得する
        attr_pos = GLES20.glGetAttribLocation(shader_program, "attr_pos");
        GLES20.glEnableVertexAttribArray(attr_pos);
        attr_uv = GLES20.glGetAttribLocation(shader_program, "attr_uv");
        GLES20.glEnableVertexAttribArray(attr_uv);
        uniform_texture = GLES20.glGetUniformLocation(shader_program, "uniform_texture");

        setupVertices();
        setupTexture();
        setupFBO();
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

    public void setupTexture() {
        // GL_TEXTURE0をアクティブに設定
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);

        // texture2dオブジェクトを生成
        int [] textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);
        content_texture_id = textures[0];

        // texture2dオブジェクトをGL_TEXTURE0にバインドする
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, content_texture_id);

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

    public void setupFBO() {
        // FBOを生成
        int [] fbos = new int[1];
        GLES20.glGenFramebuffers(1, fbos, 0);
        fbo_id = fbos[0];
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, fbo_id);

        // Render Bufferを生成
        int [] render_buffers = new int[1];
        GLES20.glGenRenderbuffers(1, render_buffers, 0);
        fbo_render_buffer_id = render_buffers[0];
        GLES20.glBindRenderbuffer(GLES20.GL_RENDERBUFFER, fbo_render_buffer_id);
        GLES20.glRenderbufferStorage(GLES20.GL_RENDERBUFFER, GLES20.GL_DEPTH_COMPONENT16, tex_w, tex_h);
        GLES20.glFramebufferRenderbuffer(GLES20.GL_FRAMEBUFFER, GLES20.GL_DEPTH_ATTACHMENT, GLES20.GL_RENDERBUFFER, fbo_render_buffer_id);

        // FBO用テクスチャを生成
        int [] textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);
        fbo_texture_id = textures[0];

        // texture2dオブジェクトをGL_TEXTURE1にバインドする
        GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, fbo_texture_id);

        // テクスチャの補間方法の指定
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

        // texture2dを生成して, FBOに割り当てる
        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, tex_w, tex_h, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, null);
        GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D, fbo_texture_id, 0);
    }

    public void dispose() {
        GLES20.glDeleteFramebuffers(1, new int[]{fbo_id}, 0);
        GLES20.glDeleteRenderbuffers(1, new int[]{fbo_render_buffer_id}, 0);
        GLES20.glDeleteTextures(1, new int[]{fbo_texture_id}, 0);

        GLES20.glDeleteTextures(1, new int[]{content_texture_id}, 0);
    }

    public void draw() {
        // 描画先をFBOに設定 (FBOはGL_TEXTURE1とする)
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, fbo_id);
        GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, fbo_texture_id);

        // テクスチャにあわせてviewportを設定
        GLES20.glViewport(0, 0, tex_w, tex_h);

        // テクスチャをクリア
        GLES20.glClearColor(1f, 0.0f, 0f, 1);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        // コンパイル済みシェーダの適用
        GLES20.glUseProgram(shader_program);

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0); // コンテンツはGL_TEXTURE0を使用
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, content_texture_id);
        GLES20.glUniform1i(uniform_texture, 0);

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
