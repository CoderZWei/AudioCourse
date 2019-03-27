package com.example.zw.audiocourse;

import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.zw.audiocourse.listener.OnInitListener;
import com.example.zw.audiocourse.listener.OnLoadListener;
import com.example.zw.audiocourse.listener.OnPauseResumeListener;
import com.example.zw.audiocourse.listener.OnTimeUpdateListener;
import com.example.zw.audiocourse.util.PermissionUtils;
import com.example.zw.audiocourse.util.TimeInfoBean;
import com.example.zw.audiocourse.util.TimeUtil;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private Button mBtn_start,mBtn_pause,mBtn_resume,mBtn_stop,mBtn_seek,mBtn_switch;
    private TextView mTextView_time;
    //FfmpegWrapper ffmpegWrapper;
    private MyPlayer mPlayer=null;
    private static final String audioPath=Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"1.mp3";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        PermissionUtils.requestPermissionsIfNeed(this);
        initView();
        mPlayer=new MyPlayer();
        mPlayer.setOnInitListener(new OnInitListener() {
            @Override
            public void onInited() {
               /* new Thread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this,"init finished",Toast.LENGTH_SHORT).show();
                    }
                }).start();
                */
                mPlayer.start();
            }
        });
        mPlayer.setOnLoadListener(new OnLoadListener() {
            @Override
            public void onLoad(boolean load) {
                if(load==true){
                    Log.d("zw_log:","加载中");
                }else {
                    Log.d("zw_log:","播放中1");
                }
            }
        });
        mPlayer.setOnPauseResumeListener(new OnPauseResumeListener() {
            @Override
            public void onPause(boolean pause) {
                if(pause==true){
                    Log.d("zw_log:","暂停中");
                }else {
                    Log.d("zw_log:","播放中");
                }
            }
        });
        mPlayer.setOnTimeUpdateListener(new OnTimeUpdateListener() {
            @Override
            public void onTimeUpdate(TimeInfoBean timeInfoBean) {
                Message message=Message.obtain();
                message.what=1;
                message.obj=timeInfoBean;
                handler.sendMessage(message);
            }
        });
    }

    private void initView() {
        mTextView_time=(TextView)findViewById(R.id.TextView_time);
        mBtn_start=(Button)findViewById(R.id.Btn_start);
        mBtn_pause=(Button)findViewById(R.id.Btn_pause);
        mBtn_resume=(Button)findViewById(R.id.Btn_resume);
        mBtn_seek=(Button)findViewById(R.id.Btn_seek);
        mBtn_switch=(Button)findViewById(R.id.Btn_switch);
        mBtn_stop=(Button)findViewById(R.id.Btn_stop);
        mBtn_start.setOnClickListener(this);
        mBtn_pause.setOnClickListener(this);
        mBtn_resume.setOnClickListener(this);
        mBtn_seek.setOnClickListener(this);
        mBtn_switch.setOnClickListener(this);
        mBtn_stop.setOnClickListener(this);
    }
    Handler handler=new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if(msg.what==1){
                TimeInfoBean timeInfoBean=(TimeInfoBean)msg.obj;
                mTextView_time.setText(TimeUtil.secdsToDateFormat(timeInfoBean.getTotalTime(), timeInfoBean.getTotalTime())
                        + "/" + TimeUtil.secdsToDateFormat(timeInfoBean.getCurrentTime(), timeInfoBean.getTotalTime()));
            }
        }
    };
    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.Btn_start:
                mPlayer.init(audioPath);
                break;
            case R.id.Btn_pause:
                mPlayer.pause();
                break;
            case R.id.Btn_resume:
                mPlayer.resume();
                break;
            case R.id.Btn_seek:
                int seek_time=215;
                mPlayer.seek(seek_time);
                break;
            case R.id.Btn_switch:
                mPlayer.switchAudio("http://ngcdn004.cnr.cn/live/dszs/index.m3u8");
                break;
            case R.id.Btn_stop:
                mPlayer.stop();
                break;
        }
    }
}
