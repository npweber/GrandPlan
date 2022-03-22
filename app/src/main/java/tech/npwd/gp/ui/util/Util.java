package tech.npwd.gp.ui.util;

import android.content.Context;
import android.view.View;
import android.widget.*;

public class Util {

    //Util Setting of Margins
    public void setLayout(View element, int width, int height, int left, int top, int right, int bottom){
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(width, height);
        layoutParams.setMargins(left, top, right, bottom);
        element.setLayoutParams(layoutParams);
    }

    //Util Creation of Text
    public TextView constructBasicText(Context context, String content, int fontSize){
        //Create the TextView
        TextView textView = new TextView(context);

        //Set FontSize IF specified
        if(fontSize != -1)
            textView.setTextSize(fontSize);

        //Set Content
        textView.setText(content);
        return textView;
    }
}
