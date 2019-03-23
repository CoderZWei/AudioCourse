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
import com.example.zw.audiocourse.util.PermissionUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class MainActivity extends AppCompatActivity {
    private Button mBtn;
    //FfmpegWrapper ffmpegWrapper;
    private MyPlayer mPlayer=null;
    private static final String audioPath=Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"1.mp3";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        PermissionUtils.requestPermissionsIfNeed(this);
        mBtn=(Button)findViewById(R.id.Btn_start);
        mPlayer=new MyPlayer();
        Log.e("zwLog", audioPath );
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

        mBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPlayer.init(audioPath);
            }
        });

    }

}
