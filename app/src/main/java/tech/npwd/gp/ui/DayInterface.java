package tech.npwd.gp.ui;

import android.content.Intent;
import android.view.View;
import android.widget.*;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import androidx.fragment.app.DialogFragment;
import tech.npwd.gp.ui.util.Util;

import static tech.npwd.gp.ui.MainInterface.frameworks;

//DayInterface: Controller for the "Calender Day View"

public class DayInterface extends AppCompatActivity {

    //Passed variables about the day:
    //The day in the week
    private int weekDay;
    //The date of the day in regular format
    private String dateStamp;

    //Control var for detecting IF any events have been added
    private static boolean hasChanged;

    //Util support
    private Util util;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //Init
        super.onCreate(savedInstanceState);
        setContentView(R.layout.interface_day);

        //Init Util
        this.util = new Util();

        //Pass values from MainInterface
        Intent intent = getIntent();
        this.weekDay = intent.getIntExtra(MainInterface.EXTRA_OFFSET, 0);
        this.dateStamp = intent.getStringExtra(MainInterface.EXTRA_DATESTAMP);

        //Setup Interface
        setupInterface();
    }

    private void setupInterface(){
        //Set dateIdentifier to passed date
        TextView dateIdentifier = findViewById(R.id.dateIdentifier);
        dateIdentifier.setText(frameworks.translateDatestamp(frameworks.addToDatestamp(frameworks.currentDateMonday(dateStamp), weekDay)));

        //Add functionality to the "back button"
        findViewById(R.id.backButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });

        //Find the dayPreview element
        GridLayout dayPreview = findViewById(R.id.dayPreview);
        //Set Column & Row Count
        dayPreview.setColumnCount(2);
        dayPreview.setRowCount(24);
        //Fill in eventSlots into dayPreview
        for(int i = 0; i < 24; i++){
            //Mark the eventSlot's hour with a TextView
            //IF the hour is the current hour, set the TextColor to Red
            TextView hourText = util.constructBasicText(this, frameworks.hourFormat(i + 1), 20);
            if(getIntent().getBooleanExtra(MainInterface.EXTRA_IS_CURRENTDAY, false) && i == (frameworks.hour() - 1))
                hourText.setTextColor(-65536);
            dayPreview.addView(hourText, (i * 2), new LinearLayout.LayoutParams(200, 200));
            //Create the eventSlot
            LinearLayout eventSlot = new LinearLayout(this);
            //Look for an event to put in the slot
            final String event = frameworks.titles((weekDay * 24) + (i + 1));
            final int hourIndex = i;
            //IF there's an event, fill the slot
            if(!event.isEmpty()){
                //Get the existing event's description
                final String eventDescription = frameworks.descriptions((weekDay * 24) + (i + 1));
                //Color the slot
                eventSlot.setBackgroundColor(frameworks.colorOfDay(weekDay));
                //Give it a control to display itself in dialog
                eventSlot.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        //Create Dialog
                        DialogFragment eventInterface = new EventInterface();
                        //Bundle in eventContent
                        Bundle eventContent = new Bundle();
                        eventContent.putStringArray("eventContent", new String[]{ event, eventDescription });
                        eventContent.putInt("index", (weekDay * 24) + (hourIndex + 1));
                        eventContent.putString("dateStamp", dateStamp);
                        eventInterface.setArguments(eventContent);
                        //Display event
                        eventInterface.show(getSupportFragmentManager(), "eventDisplay");
                    }
                });
                //Show the event's title
                eventSlot.addView(util.constructBasicText(this, event, 22));
            } else {
                eventSlot.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        DialogFragment addInterface = new AddInterface();
                        Bundle passIndex = new Bundle();
                        passIndex.putInt("index", (weekDay * 24) + (hourIndex + 1));
                        passIndex.putString("dateStamp", dateStamp);
                        addInterface.setArguments(passIndex);
                        addInterface.show(getSupportFragmentManager(), "eventAdd");
                    }
                });
            }
            //Add the slot
            dayPreview.addView(eventSlot, (i * 2) + 1, new LinearLayout.LayoutParams(-1, 200));
        }
    }

    public static void plannerHasChanged(boolean b) { hasChanged = b; }

    public static boolean hasChanged(){ return hasChanged; }

    @Override
    public void recreate(){
        finish();
        startActivity(getIntent());
    }
}
