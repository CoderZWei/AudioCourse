package com.example.zw.audiocourse;

import android.os.Environment;
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
import com.example.zw.audiocourse.util.PermissionUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private Button mBtn_start,mBtn_pause,mBtn_resume;
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

    }

    private void initView() {
        mBtn_start=(Button)findViewById(R.id.Btn_start);
        mBtn_pause=(Button)findViewById(R.id.Btn_pause);
        mBtn_resume=(Button)findViewById(R.id.Btn_resume);
        mBtn_start.setOnClickListener(this);
        mBtn_pause.setOnClickListener(this);
        mBtn_resume.setOnClickListener(this);
    }

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
        }
    }
}
