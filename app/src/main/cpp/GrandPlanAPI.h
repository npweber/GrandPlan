//GrandPlanAPI.h Exporting C++ API to the Native Java Methods
#include <jni.h>
#ifndef _Included_tech_npwd_gp_backend_Frameworks
#define _Included_tech_npwd_gp_backend_Frameworks
#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT void JNICALL Java_tech_npwd_gp_backend_Frameworks_dataRetrieval(JNIEnv *, jobject, jstring dateStamp);
	JNIEXPORT void JNICALL Java_tech_npwd_gp_backend_Frameworks_encodeEvent(JNIEnv *, jobject, jint inputIndex, jstring inputTitle, jstring inputDescription);
	JNIEXPORT void JNICALL Java_tech_npwd_gp_backend_Frameworks_removeEvent(JNIEnv *env, jobject, jint inputIndex);
	JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_titles(JNIEnv*, jobject, jint index);
	JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_descriptions(JNIEnv*, jobject, jint index);
	JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_currentDate(JNIEnv *);
	JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_currentDateMonday(JNIEnv* env, jobject, jstring currentDate);
	JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_fabricateStamp(JNIEnv*, jobject, jint inputYear, jint inputMonth, jint numWeek);
	JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_hour(JNIEnv* env);
	JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_day(JNIEnv* env);
	JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_week(JNIEnv* env);
	JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_month(JNIEnv* env);
	JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_year(JNIEnv* env);
	JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_weekStartOffset(JNIEnv *env, jobject, jint day, jint month, jint year);
	JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_addToDatestamp(JNIEnv* env, jobject, jstring inputDatestamp, jint amount);
	JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_translateDatestamp(JNIEnv* env, jobject, jstring inputDatestamp);
	JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_hourFormat(JNIEnv* env, jobject, jint inputHour);
	JNIEXPORT jboolean JNICALL Java_tech_npwd_gp_backend_Frameworks_isFiveWeeks(JNIEnv* env, jobject, jint inputMonth);
	JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_colorOfDay(JNIEnv* env, jobject, jint offset);
#ifdef __cplusplus
}
#endif
#endif