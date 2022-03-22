package tech.npwd.gp.ui;

import android.app.*;
import android.content.*;
import android.os.Bundle;
import android.view.*;
import android.widget.*;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.DialogFragment;

import static tech.npwd.gp.ui.DayInterface.plannerHasChanged;
import static tech.npwd.gp.ui.MainInterface.frameworks;

//AddInterface: Dialog Interface that Adds a New Event

public class AddInterface extends DialogFragment {

    private DayInterface host;

    @Override
    //Create Dialog
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        //Build Dialog
        final AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        //Set the layout
        final View layout = LayoutInflater.from(getContext()).inflate(R.layout.interface_add, null);
        builder.setView(layout);
        //Set its title
        builder.setTitle("Add Event");
        //Set its "Add" button
        builder.setPositiveButton(R.string.add, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                int index = getArguments().getInt("index");
                String title = ((EditText)layout.findViewById(R.id.inputTitle)).getText().toString();
                String description = ((EditText)layout.findViewById(R.id.inputDesc)).getText().toString();
                frameworks.encodeEvent(index, title, description);
                plannerHasChanged(true);
                dismiss();
                frameworks.dataRetrieval(getArguments().getString("dateStamp"));
                host.recreate();
            }
        });
        //Set the "Cancel" button
        builder.setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                getDialog().cancel();
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