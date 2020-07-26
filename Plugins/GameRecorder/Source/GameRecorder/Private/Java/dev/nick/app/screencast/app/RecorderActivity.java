package dev.nick.app.screencast.app;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.MediaScannerConnection;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.widget.Toast;

import dev.nick.app.screencast.cast.IScreencaster;
import dev.nick.app.screencast.cast.ScreencastServiceProxy;
import permissions.dispatcher.PermissionUtils;

public class RecorderActivity extends Activity {
    private static final int PERMISSION_CODE = 1;
    private static final int SHARE_CODE = 00111;

    private MediaProjection mMediaProjection;
    private MediaProjectionManager mProjectionManager;
    private static boolean mIsCasting=false;
    private static final int REQUEST_STARTRECORDING = 3;
    private static Activity mactivity;
    public static Handler mhandle = new Handler()
    {
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 1: {
                    String filepath = (String)msg.obj;
                    MediaScannerConnection.scanFile(mactivity, new String[] { filepath }, null,
                            new MediaScannerConnection.OnScanCompletedListener() {
                                @Override
                                public void onScanCompleted(String path, Uri uri) {
                                    Log.i("TAG", uri.toString());
                                    Intent shareintent = new Intent();
                                    shareintent.setAction(Intent.ACTION_SEND);
                                    shareintent.putExtra(Intent.EXTRA_STREAM,uri);
                                    shareintent.setType("video/*");
                                    mactivity.startActivityForResult(Intent.createChooser(shareintent,"share"),SHARE_CODE);
                                    Toast.makeText(mactivity, uri.toString(), Toast.LENGTH_SHORT).show();
                                }
                            });
                    break;
                }
                default:
                    break;
            }
        }
    };
    private static final String[] PERMISSION_STARTRECORDING = new String[] {"android.permission.RECORD_AUDIO","android.permission.WRITE_EXTERNAL_STORAGE"};
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(mProjectionManager==null)
        {
            mProjectionManager = (MediaProjectionManager) getSystemService(Context.MEDIA_PROJECTION_SERVICE);
        }
        int intenttype = getIntent().getIntExtra("Intent_Type",0);
        if(intenttype==1)
        {
            if(mIsCasting)
            {
                finish();
                return;
            }
            if (PermissionUtils.hasSelfPermissions(RecorderActivity.this, PERMISSION_STARTRECORDING)) {
                startRecording();
            } else {
                ActivityCompat.requestPermissions(RecorderActivity.this, PERMISSION_STARTRECORDING, REQUEST_STARTRECORDING);
            }
        }
        else if(intenttype==2)
        {
            if(mIsCasting)
            {
                stopRecording();
            }
            else
            {
                finish();
            }

        }
    }
    @Override
    public void onRequestPermissionsResult(int requestCode,  String[] permissions,  int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case REQUEST_STARTRECORDING:
                if (PermissionUtils.getTargetSdkVersion(this) < 23 && !PermissionUtils.hasSelfPermissions(this, PERMISSION_STARTRECORDING)) {
                    Toast.makeText(this,
                            "Give me permission", Toast.LENGTH_SHORT).show();
                    finish();
                    return;
                }
                if (PermissionUtils.verifyPermissions(grantResults)) {
                    startRecording();
                }
                else
                {
                    Toast.makeText(this,
                            "Give me permission", Toast.LENGTH_SHORT).show();
                    finish();
                }
                break;
            default:
                break;
        }
    }
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == PERMISSION_CODE) {
            if (resultCode != RESULT_OK) {
                Toast.makeText(this,
                        "User denied screen sharing permission", Toast.LENGTH_SHORT).show();
                finish();
                return;
            }
            mMediaProjection = mProjectionManager.getMediaProjection(resultCode, data);
            mMediaProjection.registerCallback(new MediaProjection.Callback() {
                @Override
                public void onStop() {
                    super.onStop();
                    onProjectionStop();
                }
            }, null);
            startRecording();
        }
        else if(requestCode == SHARE_CODE)
        {
            finish();
        }

    }


    private void onProjectionStop() {
        mMediaProjection = null;
        ScreencastServiceProxy.stop(getApplicationContext());
    }

    void startRecording() {
        if (mMediaProjection == null) {
            startActivityForResult(mProjectionManager.createScreenCaptureIntent(),
                    PERMISSION_CODE);
            return;
        }

        ScreencastServiceProxy.start(getApplicationContext(), mMediaProjection,true);// SettingsProvider.get().withAudio());
        finish();
    }
    private void stopRecording() {

        ScreencastServiceProxy.stop(getApplicationContext());
    }
    @Override
    protected void onStart() {
        super.onStart();
        mactivity = this;

        ScreencastServiceProxy.watch(getApplicationContext(), new IScreencaster.ICastWatcher() {
            @Override
            public void onStartCasting() {

                refreshState(true);
            }

            @Override
            public void onStopCasting() {

            }
        });
    }
    private void refreshState(final boolean isCasting) {
        mIsCasting = isCasting;
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (isCasting) {
                } else {
                }
            }
        });
    }
}