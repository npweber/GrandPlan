package tech.npwd.gp.backend;

//Frameworks: Entire Native Backend for the App in C++ Linked Library
//(See C++ src for docs)

public class Frameworks {

    public Frameworks() {
        System.loadLibrary("GrandPlanAPI");
    }

    public native void dataRetrieval(String dateStamp);

    public native void encodeEvent(int inputIndex, String title, String description);

    public native void removeEvent(int inputIndex);

    public native String titles(int index);

    public native String descriptions(int index);

    public native String currentDate();

    public native String currentDateMonday(String currentDate);

    public native String fabricateStamp(int inputYear, int inputMonth, int numWeek);

    public native int hour();

    public native int day();

    public native int week();

    public native int month();

    public native int year();

    public native int weekStartOffset(int day, int month, int year);

    public native String addToDatestamp(String dateStamp, int amount);

    public native String translateDatestamp(String inputDatestamp);

    public native String hourFormat(int hour);

    public native boolean isFiveWeeks(int inputMonth);

    public native int colorOfDay(int offset);
}
