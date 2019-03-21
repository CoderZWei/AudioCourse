package com.example.zw.audiocourse;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.zw.audiocourse.util.PermissionUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class MainActivity extends AppCompatActivity {
    private Button mBtn;
    FfmpegWrapper ffmpegWrapper;
    private static final String audioPath=Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"1.mp3";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        PermissionUtils.requestPermissionsIfNeed(this);
        mBtn=(Button)findViewById(R.id.Btn_start);
        ffmpegWrapper=FfmpegWrapper.getWrapper();
        ffmpegWrapper.init(audioPath);
        Toast.makeText(this,audioPath,Toast.LENGTH_SHORT).show();
        try {
            FileInputStream fileInputStream=new FileInputStream(audioPath);

        } catch (FileNotFoundException e) {
            Log.e("zw:can't open:",audioPath);
            e.printStackTrace();
        }
        mBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ffmpegWrapper.start();
            }
        });

    }

}
