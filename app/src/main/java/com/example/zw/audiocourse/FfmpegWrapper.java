package com.example.zw.audiocourse;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.Surface;

import com.example.zw.audiocourse.listener.OnInitListener;
import com.example.zw.audiocourse.listener.OnLoadListener;
import com.example.zw.audiocourse.listener.OnPauseResumeListener;
import com.example.zw.audiocourse.listener.OnSurfaceCreateListener;
import com.example.zw.audiocourse.listener.OnTimeUpdateListener;
import com.example.zw.audiocourse.opengl.MyGLSurfaceView;
import com.example.zw.audiocourse.opengl.MyRender;
import com.example.zw.audiocourse.util.TimeInfoBean;
import com.example.zw.audiocourse.util.TimeUtil;
import com.example.zw.audiocourse.util.VideoFormatSupportUtil;

import java.nio.ByteBuffer;
import java.util.TreeMap;

public class FfmpegWrapper {
    static {
        System.loadLibrary("native-lib");
    }
    private static FfmpegWrapper ffmpegWrapper;
    private OnInitListener onInitListener;
    private OnLoadListener onLoadListener;
    private OnPauseResumeListener onPauseResumeListener;
    private TimeInfoBean timeInfoBean=null;
    private static String audioSource;//音频数据源
    private static boolean playSwitch=false;
    private MyGLSurfaceView mGLSurfaceView;
    private OnTimeUpdateListener onTimeUpdateListener;

    private MediaFormat mediaFormat;
    private MediaCodec mediaCodec;
    private Surface mSurface;
    private MediaCodec.BufferInfo bufferInfo;

    public static FfmpegWrapper getWrapper(){
        if(ffmpegWrapper==null){
            synchronized (FfmpegWrapper.class){
                ffmpegWrapper=new FfmpegWrapper();
            }
        }
        return ffmpegWrapper;
    }
    public void init(final String path){
        Thread thread=new Thread(new Runnable() {
            @Override
            public void run() {
                audioSource=path;
                cpp_init(audioSource);
            }
        });
        thread.start();
    }
    public void setGLSurfaceView(MyGLSurfaceView mGLSurfaceView) {
        this.mGLSurfaceView = mGLSurfaceView;
        mGLSurfaceView.getRender().setOnSurfaceCreateListener(new OnSurfaceCreateListener() {
            @Override
            public void onSurfaceCreate(Surface surface) {
                if(mSurface==null){
                    mSurface=surface;
                }
            }
        });
    }
    //C++回调
    public void onCallInit(){
        if(this.onInitListener!=null){
            onInitListener.onInited();
        }
    }
    public void onCallLoad(boolean load){
        //Log.d("zw_cal","here");
        if(this.onLoadListener!=null){
            onLoadListener.onLoad(load);
        }
    }
    public void onCallTimeUpdate(int currentTime,int totalTime){
        if(this.onTimeUpdateListener!=null){
            if(timeInfoBean==null){
                timeInfoBean=new TimeInfoBean();
            }
            timeInfoBean.setCurrentTime(currentTime);
            timeInfoBean.setTotalTime(totalTime);
            onTimeUpdateListener.onTimeUpdate(timeInfoBean);
        }
    }
    int getDurationTime(){
        return timeInfoBean.getTotalTime();
    }
    public void onCallSwitch(){
        if(playSwitch){
            playSwitch=false;
            init(audioSource);
        }
    }
    public void onCallRenderYUV(int width,int height,byte[]y,byte[]u,byte[]v){
        if(mGLSurfaceView!=null){
            mGLSurfaceView.getRender().setRenderType(MyRender.RENDER_YUV);
            mGLSurfaceView.setYUVData(width, height, y, u, v);
        }
        Log.d("ZW_CALLBACK","获取到yuv");
    }
    public boolean onCallSupportMediaCodec(String ffcodecName){
        //return false;
       return VideoFormatSupportUtil.isSupportCodec(ffcodecName);
    }
    public void setOnInitListener(OnInitListener onInitListener) {
        this.onInitListener = onInitListener;
    }

    public void setOnLoadListener(OnLoadListener onLoadListener) {
        this.onLoadListener = onLoadListener;
    }

    public void setOnPauseResumeListener(OnPauseResumeListener onPauseResumeListener) {
        this.onPauseResumeListener = onPauseResumeListener;
    }

    public void start(){
        Thread thread=new Thread(new Runnable() {
            @Override
            public void run() {
                cpp_start();
            }
        });
        thread.start();
    }
    public void setOnTimeUpdateListener(OnTimeUpdateListener onTimeUpdateListener) {
        this.onTimeUpdateListener = onTimeUpdateListener;
    }

    public void pause() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                cpp_pause();
                onPauseResumeListener.onPause(true);
            }
        }).start();
    }
    public void resume() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                cpp_resume();
                onPauseResumeListener.onPause(false);
            }
        }).start();
    }
    public native void cpp_init(String path);
    public native void cpp_start();
    public native void cpp_pause();
    public native void cpp_resume();
    public native void cpp_stop();
    public native void cpp_seek(int time_sec);



    public void stop() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                cpp_stop();
            }
        }).start();
    }

    public void seek(int time_sec) {
        cpp_seek(time_sec);
    }

    public void switchAudio(String audioUrl) {
        audioSource=audioUrl;
        playSwitch=true;
        stop();
    }

    public void onCallInitMediaCodec(String codecName,int width,int height,byte[]csd_0,byte[]csd_1){
        if(mSurface!=null){
            try {
                mGLSurfaceView.getRender().setRenderType(MyRender.RENDER_MEDIACODEC);
                String mime=VideoFormatSupportUtil.findVideoCodecName(codecName);
                mediaFormat=MediaFormat.createVideoFormat(mime,width,height);
                mediaFormat.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE,width*height);
                mediaFormat.setByteBuffer("csd-0",ByteBuffer.wrap(csd_0));
                mediaFormat.setByteBuffer("csd-1",ByteBuffer.wrap(csd_1));
                mediaCodec=MediaCodec.createDecoderByType(mime);

                bufferInfo=new MediaCodec.BufferInfo();
                mediaCodec.configure(mediaFormat,mSurface,null,0);
                mediaCodec.start();
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }

    private void releaseMediaCodec(){
        if(mediaCodec!=null){
            mediaCodec.flush();
            mediaCodec.stop();
            mediaCodec.release();
            mediaCodec=null;
            mediaFormat=null;
            bufferInfo=null;
        }
    }
    public void onCallDecodeAVPacket(int dataSize,byte[]data){
        if(mSurface!=null && dataSize>0 && data!=null && mediaCodec!=null){
            int inputBufferIndex=mediaCodec.dequeueInputBuffer(10);
            if(inputBufferIndex>=0){
                ByteBuffer byteBuffer=mediaCodec.getInputBuffers()[inputBufferIndex];
                byteBuffer.clear();
                byteBuffer.put(data);
                mediaCodec.queueInputBuffer(inputBufferIndex,0,dataSize,0,0);
            }
            int outputBufferIndex=mediaCodec.dequeueOutputBuffer(bufferInfo,10);
            while (outputBufferIndex>=0){
                mediaCodec.releaseOutputBuffer(outputBufferIndex,true);
                outputBufferIndex=mediaCodec.dequeueOutputBuffer(bufferInfo,10);
            }
        }
    }
}
