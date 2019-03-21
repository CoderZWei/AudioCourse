package com.example.zw.audiocourse.util;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;

import java.util.ArrayList;
import java.util.List;

public class PermissionUtils {
    private static final int REQUEST_PERMISSION_CODE=0x293;
    static String perms[]={
            //Manifest.permission.CAMERA,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE,
            //Manifest.permission.MEDIA_CONTENT_CONTROL,
            //Manifest.permission.MODIFY_AUDIO_SETTINGS,
            //Manifest.permission.RECORD_AUDIO
    };
    public static boolean hasPermission(Context context, String perm){
        if(Build.VERSION.SDK_INT>=23){
            if(context.getApplicationContext().checkSelfPermission(perm)!=PackageManager.PERMISSION_DENIED){
                return true;
            }
            return false;
        }
        return true;
    }
    public static void requestPermissionsIfNeed(Activity activity){
        if(perms.length==0){
            return;
        }
        List<String> needPerms=new ArrayList<>();
        for(String perm:perms){
            if(!hasPermission(activity,perm)){
                needPerms.add(perm);
            }
        }
        if(needPerms.size()==0){
            return;
        }else {
            if(Build.VERSION.SDK_INT>=23){
                activity.requestPermissions(needPerms.toArray(new String[0]),REQUEST_PERMISSION_CODE);
            }
        }
    }

}
