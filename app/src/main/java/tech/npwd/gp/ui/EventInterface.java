package tech.npwd.gp.ui;

import android.app.Dialog;
import android.content.*;
import android.os.Bundle;
import android.widget.*;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.DialogFragment;

import tech.npwd.gp.ui.util.Util;

import static tech.npwd.gp.ui.DayInterface.plannerHasChanged;
import static tech.npwd.gp.ui.MainInterface.frameworks;

//EventInterface: Dialog Interface Displaying a Listed Event

public class EventInterface extends DialogFragment {

    private DayInterface host;

    //Util support
    private final Util util = new Util();

    @Override
    //Create Dialog
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        //Build Dialog
        final AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        //Create the content container
        LinearLayout content = new LinearLayout(getActivity());
        content.setOrientation(LinearLayout.VERTICAL);
        //Fill in its content
        //Add text
        //Get description and show it
        String description = getArguments().getStringArray("eventContent")[1];
        content.addView(util.constructBasicText(getActivity(), "      Details: " + description, 16));
        //Set the content to the dialog
        builder.setView(content);
        //Set its title
        builder.setTitle(getArguments().getStringArray("eventContent")[0]);
        //Set its dismiss() button
        builder.setPositiveButton(R.string.dismiss, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                dismiss();
            }
        });
        builder.setNegativeButton(R.string.remove, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                frameworks.removeEvent(getArguments().getInt("index"));
                plannerHasChanged(true);
                dismiss();
                frameworks.dataRetrieval(getArguments().getString("dateStamp"));
                host.recreate();
            }
        });
        //Build
        return builder.create();
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        host = (DayInterface) context;
    }
}
