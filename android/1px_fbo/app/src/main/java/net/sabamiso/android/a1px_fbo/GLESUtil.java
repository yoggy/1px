package net.sabamiso.android.a1px_fbo;

import android.opengl.GLES20;
import android.util.Log;

public class GLESUtil {
    static int loadShader(int type, String shader_code_str) {
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shader_code_str);
        GLES20.glCompileShader(shader);

        int[] status = new int[1];
        GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, status, 0);
        if (status[0] == 0) {
            String log_str = GLES20.glGetShaderInfoLog(shader);
            Log.e("MyGLRenderer", "GLES20.glCompileShader() failed...log=" + log_str);
            GLES20.glDeleteProgram(shader);
            return 0;
        }

        return shader;
    }

    static int createProgram(String vertex_shader_str, String fragment_shader_str) {
        int vertex_shader = loadShader(GLES20.GL_VERTEX_SHADER, vertex_shader_str);
        int fragment_shader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragment_shader_str);
        int shader_program = GLES20.glCreateProgram();
        GLES20.glAttachShader(shader_program, vertex_shader);
        GLES20.glAttachShader(shader_program, fragment_shader);

        GLES20.glLinkProgram(shader_program);

        return shader_program;
    }
}
