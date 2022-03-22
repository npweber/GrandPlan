package tech.npwd.gp.ui;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.view.View;
import android.widget.*;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import tech.npwd.gp.backend.Frameworks;
import tech.npwd.gp.ui.util.Util;

import java.io.*;
import java.util.*;

//Developed by NPWD
//MainInterface: Main Controller for App

public class MainInterface extends AppCompatActivity implements AdapterView.OnItemSelectedListener {

    //Intent EXTRA
    public static final String EXTRA_DATESTAMP = "tech.npwd.gp.ui.DATESTAMP";
    public static final String EXTRA_OFFSET = "tech.npwd.gp.ui.OFFSET";
    public static final String EXTRA_IS_CURRENTDAY = "tech.npwd.gp.ui.IS_CURRENTDAY";

    //Datestamp boolean
    private boolean isCurrentDatestamp;

    //Datestamp
    private static String dateStamp;
    private String translatedDatestamp;

    //Backend Frameworks
    public static final Frameworks frameworks = new Frameworks();
    private Util util;

    //Interaction control var
    private boolean isInteracting = false;

    @Override
    //Init App Activity
    protected void onCreate(Bundle savedInstanceState) {
        //Init
        super.onCreate(savedInstanceState);
        setContentView(R.layout.interface_main);

        //Init Util
        this.util = new Util();

        //Setup Interface
        setupInterface();
    }

    private void setupInterface(){
        //Setup Planner Content for the current calendar week
        establishPlannerContent();

        //Finish the creating the topBar
        finishTopBar();

        //Finish creating the middleBar
        finishMiddleBar();

        //Create the week display below middleBar
        createWeekDisplay();
    }

    private void finishTopBar() {
        //Find "primaryWeeks" Selection GridLayout
        GridLayout primaryWeeks = findViewById(R.id.primaryWeeks);

        //Add each week in as a button
        //Get the text content for weekButtons 1-4
        String[] weeks = getResources().getStringArray(R.array.weeks);
        for(int i = 0; i < weeks.length; i++) {
            //Create button
            Button weekButton = new Button(this);
            //Set layout
            util.setLayout(weekButton, 160, 160, 0, 0, 0, 0);
            //Set "Week Number" Text
            weekButton.setText(weeks[i]);
            if(frameworks.week() == (i + 1))
                weekButton.setTextColor(-65536);
            //Set Listener
            //Define which week to goto by button text
            final int week = i + 1;
            weekButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    //GOTO Week
                    if(frameworks.week() == week)
                        return;
                    dateStamp = frameworks.fabricateStamp(-1, -1, week);
                    recreate();
                }
            });
            //Add to Grid
            primaryWeeks.addView(weekButton);
        }

        //Create the "5th week" button (ONLY IF there is a fifth week)
        if(frameworks.isFiveWeeks(frameworks.month())){
            //Create Button
            Button fifthWeek = new Button(this);
            //Set "5th Week" text
            fifthWeek.setText(R.string.week5);
            //IF It is the 5th week, Set the Text Color to Red
            if(frameworks.week() == 5)
                fifthWeek.setTextColor(-65536);
            //Set Layout
            util.setLayout(fifthWeek, 160, 160, 0, 80, 0, 80);
            //Set Listener
            fifthWeek.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    //GOTO Fifth Week
                    if(frameworks.week() == 5)
                        return;
                    dateStamp = frameworks.fabricateStamp(-1, -1, 5);
                    recreate();
                }
            });
            //Add to the "weekSelect" group
            LinearLayout weekSelect = findViewById(R.id.weekSelect);
            weekSelect.addView(fifthWeek);
        }

        //Set content AND listeners for Month & Year Select
        ArrayAdapter<CharSequence> yearStringContent = ArrayAdapter.createFromResource(this, R.array.years, R.layout.support_simple_spinner_dropdown_item);
        yearStringContent.setDropDownViewResource(R.layout.support_simple_spinner_dropdown_item);
        Spinner yearDropdown = findViewById(R.id.yearSelect);
        yearDropdown.setAdapter(yearStringContent);
        List<String> years = Arrays.asList(getResources().getStringArray(R.array.years));
        yearDropdown.setOnItemSelectedListener(this);
        yearDropdown.setSelection(years.indexOf(String.valueOf(frameworks.year())));

        ArrayAdapter<CharSequence> monthStringContent = ArrayAdapter.createFromResource(this, R.array.months, R.layout.support_simple_spinner_dropdown_item);
        monthStringContent.setDropDownViewResource(R.layout.support_simple_spinner_dropdown_item);
        Spinner monthsDropdown = findViewById(R.id.monthSelect);
        monthsDropdown.setAdapter(monthStringContent);
        monthsDropdown.setOnItemSelectedListener(this);
        monthsDropdown.setSelection(frameworks.month() - 1);
    }

    private void finishMiddleBar(){
        //Set the Week Identifier
        TextView weekIdentifier = findViewById(R.id.weekIdentifier);
        String weekIdentifierText = translatedDatestamp + " - " + frameworks.translateDatestamp(frameworks.addToDatestamp(frameworks.currentDateMonday(dateStamp), 6)) + " | (8AM to 5PM)";
        weekIdentifier.setText(weekIdentifierText);
        //Add functionality to the "todayButton"
        findViewById(R.id.todayButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //When clicked, reload the dateStamp for the calendar to GOTO "Today", given we are not on the current week
                if(isCurrentDatestamp)
                    return;
                dateStamp = frameworks.currentDate();
                recreate();
            }
        });
    }

    private void createWeekDisplay() {
        //Fill in days of the week into "daysDisplay"
        //Find "daysDisplay"
        LinearLayout daysDisplay = findViewById(R.id.daysDisplay);
        //Get the "days" StringArray
        String[] days = getResources().getStringArray(R.array.days);
        for (int i = 0; i < days.length; i++) {
            //Create "dayIdentifier" Text
            //Get the "day" datestamp (datestamp of the certain day in the week) from add with the native method the amount
            //of days passed in the week in the array
            final String dayDatestamp = frameworks.translateDatestamp(frameworks.addToDatestamp(frameworks.currentDateMonday(dateStamp), i));
            //Shorten the day datestamp to MM/DD
            String shortenDate = dayDatestamp.substring(0, dayDatestamp.lastIndexOf("/"));
            //Use the day of the week & the date to identify a day
            String dayIdentifier = days[i] + "\r" + shortenDate;
            //Create the dayIdentifier Text
            TextView day = util.constructBasicText(this, dayIdentifier, 18);
            //Set control for the view to open the day in DayInterface
            final int offset = i;
            //Show the day IS the current day by Coloring the text Red
            boolean b = false;
            if(isCurrentDatestamp && offset == frameworks.weekStartOffset(frameworks.day(), frameworks.month(), frameworks.year())) {
                day.setTextColor(-65536);
                b = true;
            }
            final boolean isCurrentDay = b;
            day.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    Intent dayInterface = new Intent(getApplicationContext(), DayInterface.class);
                    dayInterface.putExtra(EXTRA_DATESTAMP, dateStamp);
                    dayInterface.putExtra(EXTRA_OFFSET, offset);
                    dayInterface.putExtra(EXTRA_IS_CURRENTDAY, isCurrentDay);
                    startActivity(dayInterface);
                }
            });
            //Add dayIdentifier to "daysDisplay" LinearLayout
            daysDisplay.addView(day, new LinearLayout.LayoutParams(180, 230));
        }

        //Fill in the events' preview for "weekPreview"
        //Find the initialized empty week display
        GridLayout weekDisplay = findViewById(R.id.weekPreview);
        //Setup Rows & Column for the week event display
        weekDisplay.setColumnCount(9);
        weekDisplay.setRowCount(7);

        //Cycle thru the days of the week
        for(int i = 0; i < days.length; i++)
            //For each day, cycle thru "preview hours" (8-5)
            for(int j = 8; j <= 17; j++) {
                //Check an entry at 'j' index
                String event = frameworks.titles(j + (i * 24));
                //Create an "eventSlot" Object to fill that slot, defaulting as empty space
                View eventSlot = util.constructBasicText(this,"", -1);
                //IF the event IS present IN the slot, put an event icon in the slot
                if(!event.isEmpty())
                    eventSlot = previewSlot(i);
                //Record the display index in the interface GridLayout, taking the index to start @ j = 8
                int displayIndex = (j - 8) + (i * 9);
                //Add the blank entry OR event icon to the week display
                weekDisplay.addView(eventSlot, displayIndex, new LinearLayout.LayoutParams(100, 230));
            }
    }

    private void establishDatafile(){
        File dataFile = new File(getCacheDir(), "plannerData.txt");
        try {
            //Create if not created
            if(!dataFile.exists())
                dataFile.createNewFile();
        } catch (IOException e){ e.printStackTrace(); }
    }

    //Grab the "previewSlot" icon from drawables
    private ImageView previewSlot(int dayOffset){
        //Get the string of the day from having the offset
        String dayString = getResources().getStringArray(R.array.days)[dayOffset].toLowerCase();
        //Get the res id for the source drawable
        int previewId = getResources().getIdentifier(dayString, "drawable", getPackageName());
        //Get the source drawable
        Drawable previewResource = getResources().getDrawable(previewId);

        //Create an ImageView with that drawable
        ImageView previewSlot = new ImageView(this);
        if(previewResource != null)
            previewSlot.setImageDrawable(previewResource);

        //Return ImageView
        return previewSlot;
    }

    //Establish the calender timespan to load
    private void establishPlannerContent(){
        //Check if dateStamp should go to the default "currentDate()" value
        if(dateStamp == null)
            dateStamp = frameworks.currentDate();
        //Declare dateStamp Boolean
        isCurrentDatestamp = dateStamp.equalsIgnoreCase(frameworks.currentDate()) || dateStamp.equalsIgnoreCase(frameworks.currentDateMonday(frameworks.currentDate()));
        //Establish data file for backend to load the planner with
        establishDatafile();
        //Load the timespan with the original dateStamp
        //temp();
        frameworks.dataRetrieval(dateStamp);
        //temp2();
        //For Convenience, Create everyday date format of established datestamp
        translatedDatestamp = frameworks.translateDatestamp(frameworks.currentDateMonday(dateStamp));
    }

    //Years & Month Dropdown listener
    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id){
        //On Call, ONLY call the rest of the method IF the user is selecting
        if(!isInteracting)
            return;
        //Handle the selection of months and years
        if(parent.equals(findViewById(R.id.monthSelect))) {
            int month = pos + 1;
            if(month == frameworks.month())
                return;
            //Update to selected month
            dateStamp = frameworks.fabricateStamp(-1, month, 1);
        }
        if(parent.equals(findViewById(R.id.yearSelect))) {
            int year = Integer.parseInt((String)parent.getItemAtPosition(pos));
            if(year == frameworks.year())
                return;
            //Update to selected year
            dateStamp = frameworks.fabricateStamp(year, 1, 1);
        }
        //Reload the activity content to update the planner
        recreate();
    }

    //Implemented listener
    @Override
    public void onNothingSelected(AdapterView<?> parent) {}

    //Check for a user interacting AND keep track of it
    @Override
    public void onUserInteraction() {
        super.onUserInteraction();
        isInteracting = true;
    }

    @Override
    public void recreate(){
        finish();
        startActivity(getIntent());
    }

    @Override
    protected void onRestart(){
        super.onRestart();
        if(DayInterface.hasChanged()) {
            recreate();
            DayInterface.plannerHasChanged(false);
        }
    }
}