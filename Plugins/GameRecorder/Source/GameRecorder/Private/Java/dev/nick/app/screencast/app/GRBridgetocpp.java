package dev.nick.app.screencast.app;

import android.app.Activity;
import android.content.Intent;

public class GRBridgetocpp {
    public static void starttask(Activity activity, boolean shouldGenerateThumbnails, int intenttype) {
        Intent intent = new Intent(activity, RecorderActivity.class);
        intent.putExtra("Intent_Type",intenttype);
        activity.startActivity(intent);
    }
}
