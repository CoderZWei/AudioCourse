package com.example.zw.audiocourse.opengl;


import android.content.Context;
import android.opengl.GLES20;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class MyShaderUtil{
    //static修饰 不需要实例化也可调用
    public static String readRawData(Context context,int rawId)  {
        InputStream inputStream=context.getResources().openRawResource(rawId);
        BufferedReader reader=new BufferedReader(new InputStreamReader(inputStream));
        StringBuffer sb=new StringBuffer();
        String line;
        try {
            while ((line=reader.readLine())!=null){
                sb.append(line).append("\n");
            }
            reader.close();
        }catch (IOException e){
            e.printStackTrace();
        }
        return sb.toString();
    }
    public static int loaderShader(int shaderType,String source){
        int shader=GLES20.glCreateShader(shaderType);
        if(shader!=0){
            GLES20.glShaderSource(shader,source);
            GLES20.glCompileShader(shader);
            int []compile=new int[1];
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compile, 0);
            if(compile[0] != GLES20.GL_TRUE)
            {
               // Log.d("ywl5320", "shader compile error");
                GLES20.glDeleteShader(shader);
                shader = 0;
            }
        }
        return shader;
    }
    public static int createProgram(String vertexSource,String fragmentSource){
        //program初始化程序
        int vertexShader=loaderShader(GLES20.GL_VERTEX_SHADER,vertexSource);
        //加载失败
        if(vertexShader==0){
            return 0;
        }
        int fragmentShader=loaderShader(GLES20.GL_FRAGMENT_SHADER,fragmentSource);
        if(fragmentShader==0){
            return 0;
        }
        int program=GLES20.glCreateProgram();
        if(program!=0){
            GLES20.glAttachShader(program,vertexShader);
            GLES20.glAttachShader(program,fragmentShader);
            GLES20.glLinkProgram(program);
            int[] linkStatus=new int[1];
            GLES20.glGetProgramiv(program,GLES20.GL_LINK_STATUS,linkStatus,0);
            if(linkStatus[0]!=GLES20.GL_TRUE){
                Log.e("zw_log","link program error");
                GLES20.glDeleteProgram(program);
                program=0;
            }
        }
        return program;
    }
}
