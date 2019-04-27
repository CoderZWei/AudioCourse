package com.example.zw.audiocourse.opengl;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.support.v4.app.NavUtils;
import android.view.Surface;

import com.example.zw.audiocourse.R;
import com.example.zw.audiocourse.listener.OnRenderListener;
import com.example.zw.audiocourse.listener.OnSurfaceCreateListener;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.HashMap;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyRender implements GLSurfaceView.Renderer {
    private Context context;
    private final float[] vertexData={
            -1f, -1f,
            1f, -1f,
            -1f, 1f,
            1f, 1f
    };
    private final float[] textureData={
            0f,1f,
            1f, 1f,
            0f, 0f,
            1f, 0f
    };
    private FloatBuffer vertexBuffer;
    private FloatBuffer textureBuffer;
    //yuv
    private int program_yuv;
    private int avPosition_yuv;
    private int afPosition_yuv;
    //private int textureId;
    private int[] textureId_yuv;

    private int sampler_y;
    private int sampler_u;
    private int sampler_v;

    private int width_yuv;
    private int height_yuv;
    private ByteBuffer y;
    private ByteBuffer u;
    private ByteBuffer v;

    public static final int RENDER_YUV=1;
    public static final int RENDER_MEDIACODEC=2;
    private int renderType=RENDER_YUV;
    //mediaCodec
    private int program_mediaCodec;
    private int avPosition_mediaCodec;
    private int afPosition_mediaCodec;
    private int sampler_mediaCodec;
    private int textureId_mediaCodec;
    private SurfaceTexture mSurfaceTexture;
    private Surface mSurface;


    private OnSurfaceCreateListener onSurfaceCreateListener;
    public void setOnSurfaceCreateListener(OnSurfaceCreateListener onSurfaceCreateListener) {
        this.onSurfaceCreateListener = onSurfaceCreateListener;
    }

    private OnRenderListener onRenderListener;
    public void setOnRenderListener(OnRenderListener onRenderListener) {
        this.onRenderListener = onRenderListener;
    }



    public void setRenderType(int renderType) {
        this.renderType = renderType;
    }


    public MyRender(Context context) {
        this.context = context;
        vertexBuffer=ByteBuffer.allocateDirect(vertexData.length*4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(vertexData);
        vertexBuffer.position(0);
        textureBuffer=ByteBuffer.allocateDirect(textureData.length*4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(textureData);
        textureBuffer.position(0);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        initRenderYUV();
        initRenderMediaCodec();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0,0,width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        GLES20.glClearColor(0.0f,0.0f,0.0f,1.0f);
        if(renderType==RENDER_YUV){
            renderYUV();
        }else if(renderType==RENDER_MEDIACODEC){
            renderMediaCodec();
        }
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
    }

    private void renderMediaCodec() {
        mSurfaceTexture.updateTexImage();
        GLES20.glUseProgram(program_mediaCodec);
        GLES20.glEnableVertexAttribArray(avPosition_mediaCodec);
        GLES20.glVertexAttribPointer(avPosition_mediaCodec, 2, GLES20.GL_FLOAT, false, 8, vertexBuffer);

        GLES20.glEnableVertexAttribArray(afPosition_mediaCodec);
        GLES20.glVertexAttribPointer(afPosition_mediaCodec, 2, GLES20.GL_FLOAT, false, 8, textureBuffer);

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textureId_mediaCodec);
        GLES20.glUniform1i(sampler_mediaCodec,0);
    }

    private void renderYUV() {
        if(width_yuv>0 && height_yuv>0 && y!= null && u!=null && v!=null){
            GLES20.glUseProgram(program_yuv);
            GLES20.glEnableVertexAttribArray(avPosition_yuv);
            GLES20.glVertexAttribPointer(avPosition_yuv,2,GLES20.GL_FLOAT,false,8,vertexBuffer);

            GLES20.glEnableVertexAttribArray(afPosition_yuv);
            GLES20.glVertexAttribPointer(afPosition_yuv, 2, GLES20.GL_FLOAT, false, 8, textureBuffer);

            //三个texture
            GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D,textureId_yuv[0]);
            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE, width_yuv, height_yuv, 0, GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, y);

            GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId_yuv[1]);
            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE, width_yuv / 2, height_yuv / 2, 0, GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, u);

            GLES20.glActiveTexture(GLES20.GL_TEXTURE2);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId_yuv[2]);
            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE, width_yuv / 2, height_yuv / 2, 0, GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, v);

            GLES20.glUniform1i(sampler_y, 0);
            GLES20.glUniform1i(sampler_u, 1);
            GLES20.glUniform1i(sampler_v, 2);

            y.clear();
            u.clear();
            v.clear();
            y = null;
            u = null;
            v = null;

        }
    }

    private void initRenderYUV(){
        String vertexSource=MyShaderUtil.readRawData(context, R.raw.vertex_shader);
        String fragmentSource=MyShaderUtil.readRawData(context,R.raw.fragment_yuv_shader);
        program_yuv=MyShaderUtil.createProgram(vertexSource,fragmentSource);
        avPosition_yuv = GLES20.glGetAttribLocation(program_yuv, "av_Position");
        afPosition_yuv = GLES20.glGetAttribLocation(program_yuv, "af_Position");

        sampler_y = GLES20.glGetUniformLocation(program_yuv, "sampler_y");
        sampler_u = GLES20.glGetUniformLocation(program_yuv, "sampler_u");
        sampler_v = GLES20.glGetUniformLocation(program_yuv, "sampler_v");

        //yuv三个分量的三个纹理
        textureId_yuv=new int[3];
        GLES20.glGenTextures(3, textureId_yuv, 0);
        for(int i=0;i<3;i++){
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D,textureId_yuv[i]);
            //设置环绕和过滤方式
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        }
    }

    public void setYUVRenderData(int width, int height, byte[] y, byte[] u, byte[] v) {
        this.width_yuv=width;
        this.height_yuv=height;
        this.y=ByteBuffer.wrap(y);
        this.u=ByteBuffer.wrap(u);
        this.v=ByteBuffer.wrap(v);
    }

    public void initRenderMediaCodec(){
        String vertexSource=MyShaderUtil.readRawData(context, R.raw.vertex_shader);
        String fragmentSource=MyShaderUtil.readRawData(context,R.raw.fragment_mediacodec_shader);
        program_mediaCodec=MyShaderUtil.createProgram(vertexSource,fragmentSource);

        avPosition_mediaCodec=GLES20.glGetAttribLocation(program_mediaCodec,"av_Position");
        afPosition_mediaCodec=GLES20.glGetAttribLocation(program_mediaCodec,"af_Position");
        sampler_mediaCodec=GLES20.glGetUniformLocation(program_mediaCodec,"sTexture");

        int[] textureIds=new int[1];
        GLES20.glGenTextures(1,textureIds,0);
        textureId_mediaCodec=textureIds[0];

        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

        mSurfaceTexture=new SurfaceTexture(textureId_mediaCodec);
        mSurface=new Surface(mSurfaceTexture);
        mSurfaceTexture.setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {
            @Override
            public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                onRenderListener.onRender();
            }
        });
        if(onSurfaceCreateListener!=null){
            onSurfaceCreateListener.onSurfaceCreate(mSurface);
        }
    }
}
