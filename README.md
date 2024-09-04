# **GrandPlan**

An Android calendar app capable of planning daily events anytime in the years 2019 to 2030.

![](https://github.com/npweber/GrandPlan/raw/main/screenshots/Shot12.png)

## Features

Plan your day by the hour, by placing events into any hour slot of the day.

![](https://github.com/npweber/GrandPlan/raw/main/screenshots/Shot7.png)

Get a heads up from the single week display. It shows a preview of the entire week's plans, letting you know of what's ahead.

![](https://github.com/npweber/GrandPlan/raw/main/screenshots/Shot9.png)

Take advantage of the smart weekly navigation controls. By selecting a month in a year and any 1st-4th week in the month, it will be hard to lose track of what week is being viewed.

![](https://github.com/npweber/GrandPlan/raw/main/screenshots/Shot3.png)

## Specifications

GrandPlan is written in the Android SDK and consists of a front and backend implementation, however the calendar functionality is not implemented using the `java.util.calendar` API. It instead utilizes a C++ backend to load and edit each week's contents selected from the interface. At the time of writing the backend, I personally made this design decision. I had recently learned to program in C++, and, instead of making it easy to navigate time on a calendar, I decided to make the functionality custom. I wrote a C++ API that controlled which week was selected by input and how to edit the week's data stored in cache. This was then JNI compiled to be called by a Java reference, and then a backend could be used in turn to implement the calendar functionality.
